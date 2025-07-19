# 🏦 SecureBank-CPP

![Build Status](https://github.com/ITx-prash/securebank-cpp/workflows/Build%20and%20Release/badge.svg)
![Release](https://img.shields.io/github/v/release/ITx-prash/securebank-cpp)
![Platform](https://img.shields.io/badge/platform-Linux%20%7C%20Windows-blue)
![C++](https://img.shields.io/badge/C%2B%2B-17-blue.svg)

**SecureBank-CPP** is a modern C++ banking application with CLI and web interfaces, designed for educational purposes and to demonstrate practical C++ and web integration.

---

## 🚀 Features

- **Web Interface:** Responsive HTML/CSS/JS frontend served by Crow C++ microframework
- **CLI Interface:** Basic banking operations via command line
- **Cross-Platform:** Build and run on Linux and Windows (via MinGW)
- **Automated CI/CD:** GitHub Actions for builds and releases

---

## ⚠️ Disclaimer

> **This project is for educational and demonstration purposes only.**
>
> It is **not production-ready** and may contain security vulnerabilities.  
> **Not recommended for practical application with  sensitive data.**

---

## 📦 Quick Start

### Pre-built Releases

Download the latest release from [Releases](https://github.com/ITx-prash/securebank-cpp/releases):

- **🐧 Linux:** `securebank-linux.tar.gz`
- **🪟 Windows:** `securebank-windows.zip`

```sh
# Linux
tar -xzf securebank-linux.tar.gz
cd securebank-linux/
./crow_bank_app

# Windows
# Extract securebank-windows.zip
crow_bank_app.exe
```

Then open your browser to [http://localhost:8080](http://localhost:8080)

---

## 🛠️ Build from Source

**Requirements:**

- Linux: GCC 8+, CMake 3.15+, Make
- Windows: MinGW-w64, CMake 3.15+

```sh
cd crow-bank-app/
./build.sh linux    # For Linux
./build.sh windows  # For Windows cross-compilation
```

---

## 📁 Project Structure

```
securebank-cpp/
├── crow-bank-app/
│   ├── main.cpp
│   ├── crow_all.h
│   ├── CMakeLists.txt
│   ├── public/
│   └── build.sh
├── .github/
│   └── workflows/
└── README.md
```

---

## 📄 License

MIT License – see [LICENSE](LICENSE) for details.

---

*Made for learning and experimentation. Not for production use.*

**Made with ❤️ by ITx-prash**
