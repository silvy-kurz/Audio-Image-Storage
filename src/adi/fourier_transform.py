from pickle import load, dump
from numpy import fft
from scipy.io.wavfile import read


def get_fft_input_data(file_path):
    return read(file_path)[1]  # Source:


def apply_fft(input_data):
    if input_data.ndim == 2:
        input_data = input_data[:, 0]

    transformed_data = fft.fft(input_data)  # Source:
    return transformed_data


def write_out_fft(fft_data, pickle_path):
    with open(pickle_path, "wb") as file:
        dump(fft_data, file)


def load_fft(pickle_path):
    with open(pickle_path, "rb") as file:
        return load(file)
