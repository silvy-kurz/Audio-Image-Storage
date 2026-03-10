import numpy as np
from scipy.io.wavfile import write


def generate_mono_sine(filename, freq=440, duration=2.0, sr=44100):
    t = np.linspace(0, duration, int(sr * duration), endpoint=False)
    # Single column of data
    audio = (np.sin(2 * np.pi * freq * t) * 32767).astype(np.int16)
    write(filename, sr, audio)
    print(f"Saved Mono: {filename}")


def generate_stereo_sine(filename, freq_l=440, freq_r=880, duration=2.0, sr=44100):
    t = np.linspace(0, duration, int(sr * duration), endpoint=False)

    # Generate two separate channels
    left_channel = np.sin(2 * np.pi * freq_l * t) * 32767
    right_channel = np.sin(2 * np.pi * freq_r * t) * 32767

    # Stack them into a 2D array and transpose to get [L, R] pairs
    # Shape becomes (num_samples, 2)
    audio_stereo = np.vstack((left_channel, right_channel)).T.astype(np.int16)

    write(filename, sr, audio_stereo)
    print(f"Saved Stereo: {filename}")


# Generate test files
generate_mono_sine("inputs/test_mono.wav")
generate_stereo_sine("inputs/test_stereo.wav", freq_l=440, freq_r=880)
