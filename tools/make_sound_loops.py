# Cuts the car's sound out of the test-day recordings (data\sound\raw, from tools\extract_audio.py) into seamless loops in data\sound\loops.
#   python tools\make_sound_loops.py
# Only clip 20260927_124341 is used: the car runs at two steady speeds with nobody talking. The motor's tones are followed,
# their small speed wobble is taken out, and only the tones plus a little of the surrounding noise are kept, so no voices or wind get in.
# The tones are the two gearbox meshes, 3.07 apart. ToneB below is the upper one, and the game drives it with the same numbers (FSLapVehiclePawn.cpp).
import os

import numpy as np
from scipy import signal
from scipy.io import wavfile

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
RAW = os.path.join(ROOT, 'data', 'sound', 'raw', '20260927_124341.wav')
OUT = os.path.join(ROOT, 'data', 'sound', 'loops')
FS = 48000
RATIO = 3.07
SECONDS = 3.0
FADE = 0.15
#        name    from   to    start: time   ToneB near   ToneB after flattening
CUTS = [('Low',  1.5,  5.6,  3.0,          855,         800.0),
        ('Mid',  14.0, 17.6, 16.0,         1930,        1930.0)]


def tone_score(x, seconds):
    """Spectrum of 85 ms windows, in dB above the local noise floor, so a tone scores high whatever its level."""
    window = 4096
    hop = 256
    pad = 16384
    starts = np.arange(int(seconds[0] * FS), int(seconds[1] * FS) - window, hop)
    spectrum = np.abs(np.fft.rfft(np.stack([x[s:s + window] * np.hanning(window) for s in starts]), n=pad, axis=1))
    level = 20 * np.log10(spectrum + 1e-9)
    floor = signal.medfilt2d(level[::8].astype('f4'), (1, 201))
    floor = np.repeat(floor, 8, axis=0)[:len(level)]
    return (starts + window / 2) / FS, FS / pad, np.clip(level - floor, 0, 40)


def follow(x, cut):
    """ToneB through the cut, from the sum of its three lines: B, B / 3.07 and 2 B, frame to frame."""
    _, t0, t1, start, near, _ = cut
    times, df, score = tone_score(x, (t0, t1))
    rows = np.arange(len(times))

    def at(row, f):
        return score[row, np.clip(np.rint(f / df).astype(int), 0, score.shape[1] - 1)]

    def best(row, centre, width):
        grid = centre * np.linspace(1 - width, 1 + width, 121)
        total = at(row, grid) + at(row, grid / RATIO) + at(row, grid * 2)
        return grid[np.argmax(total)]

    middle = int(np.searchsorted(times, start))
    track = np.zeros(len(times))
    track[middle] = best(middle, near, 0.06)
    for row in rows[middle + 1:]:
        track[row] = best(row, track[row - 1], 0.012)
    for row in rows[:middle][::-1]:
        track[row] = best(row, track[row + 1], 0.012)
    return times, signal.savgol_filter(track, 41, 2)


def flatten(x, times, track, target):
    """Resamples so that ToneB stays exactly on target. The motor's wobble goes, the sound does not change otherwise."""
    fine = signal.resample_poly(x, 4, 1)
    t = np.arange(times[0], times[-1], 1.0 / FS)
    rate = np.interp(t, times, track) / target
    warped = np.cumsum(rate) / FS                         # how far along the flat sound each input moment is
    wanted = np.arange(0, warped[-1], 1.0 / FS)
    source = np.interp(wanted, warped, t)
    return np.interp(source * FS * 4, np.arange(len(fine)), fine)


def seamless(flat):
    """Makes SECONDS of sound whose end runs into its start, then keeps the tones and turns the rest down by 12 dB."""
    n = int(SECONDS * FS)
    fade = int(FADE * FS)
    middle = (len(flat) - n - fade) // 2
    piece = flat[middle:middle + n + fade].copy()
    gain = np.sqrt(np.linspace(0, 1, fade))
    piece[:fade] = piece[:fade] * gain + piece[n:n + fade] * gain[::-1]
    piece = piece[:n]

    spectrum = np.fft.rfft(piece)
    f = np.fft.rfftfreq(n, 1.0 / FS)
    hz = int(round(1 / (f[1] - f[0])))
    size = np.abs(spectrum)
    smooth = np.convolve(size, np.ones(4 * hz) / (4 * hz), 'same')
    floor = signal.medfilt(smooth[::hz], 81)
    floor = np.interp(f, f[::hz], floor)
    tone = smooth > 2.5 * floor
    tone = np.convolve(tone, np.ones(16 * hz + 1), 'same') > 0   # keep the sidebands of each line
    keep = np.where(tone, 1.0, 0.25)
    keep *= np.clip((f - 130) / 60, 0, 1) * np.clip((14000 - f) / 3000, 0, 1)   # wind thumps below, hiss above
    loop = np.fft.irfft(spectrum * keep, n)
    share = np.sum(np.abs(spectrum[tone]) ** 2 * keep[tone] ** 2) / np.sum(np.abs(spectrum * keep) ** 2)
    return loop * 10 ** (-16 / 20) / np.sqrt(np.mean(loop ** 2)), share


def noise_loop(band):
    """Two seconds of noise with this spectrum. The recordings have no clean tyre, wind or grass sound to cut."""
    n = int(2.0 * FS)
    rng = np.random.default_rng(7)
    f = np.fft.rfftfreq(n, 1.0 / FS)
    loop = np.fft.irfft(band(f) * np.exp(2j * np.pi * rng.random(len(f))), n)
    return loop * 10 ** (-16 / 20) / np.sqrt(np.mean(loop ** 2))


def cone_hit():
    """The web version's cone sound: a square wave falling from 320 to 110 Hz."""
    t = np.arange(int(0.12 * FS)) / FS
    phase = 2 * np.pi * np.cumsum(320 * (110 / 320) ** (t / 0.08)) / FS
    return 0.35 * np.sign(np.sin(phase)) * np.exp(-t / 0.035) * np.clip((0.12 - t) / 0.01, 0, 1)


def beep():
    t = np.arange(int(0.18 * FS)) / FS
    return 0.4 * np.sin(2 * np.pi * 880 * t) * np.clip(np.minimum(t / 0.005, (0.18 - t) / 0.02), 0, 1)


def write(name, sound):
    wavfile.write(os.path.join(OUT, 'A_' + name + '.wav'), FS, (sound * 32767).astype('<i2'))


def save(name, loop):
    assert np.abs(loop).max() < 1, name
    seam = abs(loop[0] - loop[-1])
    step = np.percentile(np.abs(np.diff(loop)), 99.9)
    assert seam < step, (name, seam, step)   # the join must be no bigger than the sound's own steps, or it clicks
    write(name, loop)
    print('FSLAP_SOUND', name, round(len(loop) / FS, 2), 's peak', round(float(np.abs(loop).max()), 2), 'seam', round(float(seam), 4), 'step', round(float(step), 4), flush=True)


def main():
    os.makedirs(OUT, exist_ok=True)
    rate, x = wavfile.read(RAW)
    assert rate == FS
    x = x.astype('f8') / 32768
    loops = {}
    for cut in CUTS:
        times, track = follow(x, cut)
        print('FSLAP_TONE', cut[0], 'B from', round(track.min()), 'to', round(track.max()), 'Hz', flush=True)
        loops[cut[0]], share = seamless(flatten(x, times, track, cut[5]))
        print('FSLAP_TONE', cut[0], 'tones hold', round(share * 100), '% of the kept sound', flush=True)
        save('Motor' + cut[0], loops[cut[0]])
    # orders stay put when the motor speeds up, so an octave up is the same sound at twice the motor speed
    high = signal.resample_poly(np.tile(loops['Mid'], 3), 1, 2)
    half = len(loops['Mid']) // 2
    save('MotorHigh', high[half:2 * half])
    save('Tyre', noise_loop(lambda f: np.exp(-0.5 * ((f - 1250) / 260) ** 2) + 0.5 * np.exp(-0.5 * ((f - 2500) / 340) ** 2)))
    save('Wind', noise_loop(lambda f: np.clip((f - 40) / 60, 0, 1) / (1 + (f / 450) ** 2)))
    save('Grass', noise_loop(lambda f: np.exp(-0.5 * ((f - 130) / 60) ** 2) + 0.4 * np.exp(-0.5 * ((f - 420) / 120) ** 2)))
    write('ConeHit', cone_hit())
    write('Beep', beep())


if __name__ == '__main__':
    main()
