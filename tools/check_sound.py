# Checks the sound the game really played, because nobody can listen in an automatic test. After: tools\slice_test.ps1 -Sound
#   python tools\check_sound.py [folder with audio.wav and audio_track.csv]
# The upper gear tone in the recording of the game's output has to sit on the value the game asked for, all along the lap.
import csv
import os
import sys

import numpy as np
from scipy.io import wavfile

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
FOLDER = sys.argv[1] if len(sys.argv) > 1 else os.path.join(ROOT, 'FSLapUE', 'Saved', 'SliceTest', 'sim')
WINDOW = 8192

fs, sound = wavfile.read(os.path.join(FOLDER, 'audio.wav'))
sound = (sound.mean(axis=1) if sound.ndim > 1 else sound.astype('f8')) / 32768
with open(os.path.join(FOLDER, 'audio_track.csv'), newline='') as f:
    track = np.array([[float(v) for v in row] for row in csv.reader(f) if row])

print('recording', round(len(sound) / fs, 1), 's, peak', round(float(np.abs(sound).max()), 3), 'rms', round(float(np.sqrt(np.mean(sound ** 2))), 4))
times = np.arange(1.0, min(len(sound) / fs, track[-1, 0]) - 1.0, 0.25)
errors, heard = [], []
for t in times:
    start = int(t * fs - WINDOW / 2)
    wanted = float(np.interp(t, track[:, 0], track[:, 1]))
    # the tone must have been steady in this window, otherwise a peak says nothing
    span = track[(track[:, 0] >= t - WINDOW / fs / 2) & (track[:, 0] <= t + WINDOW / fs / 2), 1]
    if wanted < 400 or len(span) < 2 or span.max() > span.min() * 1.02:
        continue
    spectrum = np.abs(np.fft.rfft(sound[start:start + WINDOW] * np.hanning(WINDOW), n=4 * WINDOW))
    f = np.fft.rfftfreq(4 * WINDOW, 1.0 / fs)
    near = (f > wanted * 0.9) & (f < wanted * 1.1)
    around = (f > wanted * 0.7) & (f < wanted * 1.3)
    peak = f[near][np.argmax(spectrum[near])]
    errors.append(abs(peak - wanted) / wanted)
    heard.append(20 * np.log10(spectrum[near].max() / (np.median(spectrum[around]) + 1e-12)))

errors, heard = np.array(errors), np.array(heard)
print('steady moments checked', len(errors), 'of', len(times))
print('tone error: median', round(float(np.median(errors)) * 100, 2), '%, 90th percentile', round(float(np.percentile(errors, 90)) * 100, 2), '%')
print('tone above its surroundings: median', round(float(np.median(heard)), 1), 'dB')
good = len(errors) >= 20 and np.median(errors) < 0.02 and np.median(heard) > 12 and 0.15 < np.abs(sound).max() < 1   # neither too quiet to notice nor clipping
print('SOUND CHECK', 'PASS' if good else 'FAIL')
sys.exit(0 if good else 1)
