# 📦 AFI — File ↔ Image Preservation

> *Encode Files into Images, with Byte Level Perfect Reconstruction*

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
- ⚠️ Robust error handling via explicit C error codes (no silent failures)
---

## ℹ️ Overview

AFI (originally *Audio-File-Image*, now evolving toward *Any-File-Image*) is an experiment in **reversible data representation**.

Instead of treating images as mere containers (like traditional steganography), AFI treats them as a **structured encoding space**:
- Every byte of the original file is mapped intentionally to pixel data
- The mapping aims to **maximize visible patterns** whenever possible 
- File metadata is embedded in a recoverable format at the start of the image

The result is something in between:
- 📷 an image  
- 💾 a file container  
- 🧠 a visualization of raw data  

---
## 📦 AFVF Format (Internal Representation)

AFI converts files into an intermediate binary format before mapping them to image pixels.

This format — **AFVF (AFI Visual Format)** — is designed for:
- 🔁 **Perfect reconstruction**
- 🧩 **Structured metadata storage**
- 🎞️ **Support for multi-frame / multi-section data**

At a high level, AFVF separates:
- metadata (file info, structure)
- frame data (the actual encoded content)

This allows AFI to reconstruct the original file *exactly*, while still enabling flexible encoding strategies.



---
###  Why?

Most existing data-into-image techniques are either completely visually incomprehensible, or lossy. For example, converting audios to images usually involves breaking down the audio using a Short-Fast-Fourier-Transform, and mapping the resultant complex numbers to Pixels. This process, when reconstructed into audio, creates sounds that are audibly identical, but on a byte level completely different. Alternatively, programs in cryptography will dump bytes into pixels without structure. 

AFI takes an approach somewhere in the middle:
> *Pixel Mappings with byte level accuracy, that reflect the structure of the data*

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
