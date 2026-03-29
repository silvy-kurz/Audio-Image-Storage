# 📦 AFI — File ↔ Image Preservation

> *Encode files into images. Not approximately — **perfectly**.*

AFI is a low-level C library (with a Python interface) for converting files into images and back again with **byte-perfect reconstruction**.  
It doesn’t just hide data — it **restructures it visually**, mapping raw bytes into pixels in a way that preserves information while maximizing human-recognizable structure.

---

## 🌟 Highlights

- 🔁 **Lossless, byte-perfect roundtrip** — reconstruct the exact original file
- 🧠 **Smart pixel mapping** — organizes data to reveal visual structure where possible
- 📎 **Metadata preservation** — stores file metadata directly in the image header
- 🎧 **Audio → image support (complete)** — first fully implemented pipeline
- 🧩 **Extensible design** — built to support *any* file type
- ⚙️ **C core + Python wrapper** — performance + usability
- 🧪 **Experimental / novelty project** — explore data in a completely new way

---

## ℹ️ Overview

AFI (originally *Audio-File-Image*, now evolving toward *Any-File-Image*) is an experiment in **reversible data representation**.

Instead of treating images as mere containers (like traditional steganography), AFI treats them as a **structured encoding space**:
- Every byte of the original file is mapped intentionally to pixel data
- The mapping aims to **maximize visible patterns**, not randomness
- File metadata is embedded in a recoverable format at the start of the image

The result is something in between:
- 📷 an image  
- 💾 a file container  
- 🧠 a visualization of raw data  

---

### 🤔 Why?

Most data-into-image techniques fall into one of two categories:
- **Steganography** → hide data invisibly  
- **Naive encoding** → dump bytes into pixels without structure  

AFI explores a third direction:
> *What if the image itself reflects the structure of the data — while still being perfectly reversible?*

This makes it interesting for:
- 🎧 audio engineers (visualizing waveform structure differently)
- 🔐 niche cryptography / encoding experiments
- 🧪 general tech curiosity

---

### 👤 Author

Created by **Silvy** — a student with interests in systems programming, data representation, and experimental software design.

---

## 🚀 Usage

> *Early-stage project — API is still evolving*

### Python example

```py
from afi import encode_audio_to_image, decode_image_to_audio

encode_audio_to_image("input.wav", "output.png")
decode_image_to_audio("output.png", "reconstructed.wav")
