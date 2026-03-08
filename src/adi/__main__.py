import os


from adi import INPUT_DIR
from adi.fourier_transform import apply_fft, get_fft_input_data, load_fft, write_out_fft


def create_temp_fft_data():
    file_path = os.path.join(INPUT_DIR, "dingus.wav")
    input_frequencies = get_fft_input_data(file_path)
    transformed_data = apply_fft(input_frequencies)
    write_out_fft(transformed_data, "temps/data.pkl")


def main():
    fft_data = load_fft("temps/data.pkl")
    complex_wave_data = [list(fft_data) for _ in range(0, 100)]
    print(complex_wave_data[0][0])


if __name__ == "__main__":
    main()
