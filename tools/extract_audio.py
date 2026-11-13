# Pulls the sound out of the test-day videos in E:\sound into data\sound\raw\*.wav (mono, 48 kHz). E:\sound is only read.
#   blender -b --factory-startup --python tools\extract_audio.py
# Blender's own audio library reads MP4, so no video tool has to be installed. Its file writer fails on these clips, so the samples are written here.
import glob
import os
import wave

import aud
import numpy as np

SOURCE = r'E:\sound'
OUT = os.path.join(os.path.dirname(os.path.dirname(os.path.abspath(__file__))), 'data', 'sound', 'raw')
os.makedirs(OUT, exist_ok=True)

for path in sorted(glob.glob(os.path.join(SOURCE, '*.mp4'))):
    target = os.path.join(OUT, os.path.splitext(os.path.basename(path))[0] + '.wav')
    samples = aud.Sound(path).rechannel(1).resample(48000).data()[:, 0]
    with wave.open(target, 'wb') as out:
        out.setnchannels(1)
        out.setsampwidth(2)
        out.setframerate(48000)
        out.writeframes((np.clip(samples, -1, 1) * 32767).astype('<i2').tobytes())
    print('FSLAP_AUDIO', os.path.basename(target), round(len(samples) / 48000, 1), 's', flush=True)
print('FSLAP_AUDIO_DONE', flush=True)
