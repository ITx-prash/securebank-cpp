<div align="center">

# 🏦 SecureBank-CPP

<img src="https://img.shields.io/static/v1?style=for-the-badge&label=Platform&message=Linux%20%7C%20Windows&colorA=1e1e2e&colorB=f9e2af&logo=linux&logoColor=f9e2af">
<img src="https://img.shields.io/static/v1?style=for-the-badge&label=C%2B%2B&message=17&colorA=1e1e2e&colorB=89dceb&logo=cplusplus&logoColor=89dceb">
<a href="https://github.com/ITx-prash/securebank-cpp/releases"><img src="https://img.shields.io/github/v/release/ITx-prash/securebank-cpp?style=for-the-badge&label=Release&colorA=1e1e2e&colorB=a6e3a1&logo=githubactions&logoColor=a6e3a1"></a>
<a href="https://github.com/ITx-prash/securebank-cpp/actions/workflows/build.yml"><img src="https://img.shields.io/github/actions/workflow/status/ITx-prash/securebank-cpp/build.yml?style=for-the-badge&label=Build&colorA=1e1e2e&colorB=a6e3a1&logo=githubactions&logoColor=a6e3a1"></a>
<a href="https://github.com/ITx-prash/securebank-cpp/issues"><img src="https://img.shields.io/static/v1?style=for-the-badge&label=&message=Report%20Issue&colorA=1e1e2e&colorB=f38ba8&logo=gitbook&logoColor=f38ba8"></a>

</div>

---

**SecureBank-CPP** is a modern C++ banking application featuring both a CLI and a responsive web interface, built to demonstrate practical C++ development and web integration patterns using the Crow microframework.

> [!WARNING]
> **This project is for educational and demonstration purposes only.**
> It is **not production-ready** and may contain security vulnerabilities. **Do not use it with real or sensitive financial data.**

## ✨ Features

- **Web Interface:** Responsive HTML/CSS/JS frontend served via the Crow C++ microframework
- **CLI Interface:** Core banking operations accessible directly from the command line
- **Cross-Platform:** Supports building and running on both Linux and Windows (via MinGW)
- **Automated CI/CD:** GitHub Actions pipeline for continuous builds and automated releases

## 🚀 Quick Start

### Pre-built Releases

Download the latest release from the [Releases](https://github.com/ITx-prash/securebank-cpp/releases) page:

| Platform   | Archive                   |
| :--------- | :------------------------ |
| 🐧 Linux   | `securebank-linux.tar.gz` |
| 🪟 Windows | `securebank-windows.zip`  |

```sh
# Linux
tar -xzf securebank-linux.tar.gz
cd securebank-linux/
./crow_bank_app

# Windows — extract securebank-windows.zip, then run:
crow_bank_app.exe
```

Then open your browser to [http://localhost:8080](http://localhost:8080).

## 🛠️ Build from Source

**Requirements:**

- Linux: GCC 8+, CMake 3.15+, Make
- Windows: MinGW-w64, CMake 3.15+

1. **Clone the repository:**

   ```bash
   git clone https://github.com/ITx-prash/securebank-cpp.git
   cd securebank-cpp
   ```

2. **Run the build script:**

   ```sh
   cd crow-bank-app/
   ./build.sh linux    # For Linux
   ./build.sh windows  # For Windows (cross-compilation)
   ```

> [!IMPORTANT]
> Ensure all required build dependencies are installed before proceeding.

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

<p align="center">
	<img src="https://raw.githubusercontent.com/ITx-prash/floweave/main/assets/coder.png" height="150" alt="Coder illustration"/>
	<br/>
	<em>Crafted with 💚 on GNU/Linux</em>
	<br/>
	Copyright &copy; 2025-present <a href="https://github.com/ITx-prash" target="_blank">Prashant Adhikari</a>
	<br/><br/>
	<a href="https://github.com/ITx-prash/securebank-cpp/blob/main/LICENSE"><img src="https://img.shields.io/static/v1.svg?style=for-the-badge&label=License&message=MIT&logoColor=a6e3a1&colorA=1e1e2e&colorB=a6e3a1"/></a>
</p>
