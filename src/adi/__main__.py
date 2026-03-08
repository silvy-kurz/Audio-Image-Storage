import os


from adi import INPUT_DIR
from adi.fourier_transform import apply_fft, get_fft_input_data


def main():
    file_path = os.path.join(INPUT_DIR, "dingus.wav")
    input_frequencies = get_fft_input_data(file_path)
    transformed_data = apply_fft(input_frequencies)
    complex_wave_data = list(transformed_data)
    print(complex_wave_data[0])
    print(len(complex_wave_data))


if __name__ == "__main__":
    main()
