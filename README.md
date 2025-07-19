# 🏦 SecureBank-CPP

![Build Status](https://github.com/ITx-prash/securebank-cpp/workflows/Build%20and%20Release/badge.svg)
![Release](https://img.shields.io/github/v/release/ITx-prash/securebank-cpp)
![Platform](https://img.shields.io/badge/platform-Linux%20%7C%20Windows-blue)
![C++](https://img.shields.io/badge/C%2B%2B-17-blue.svg)

Modern C++ banking application with CLI and web interfaces. Demonstrates OOP concepts, REST API design, and cross-platform development.

## 🚀 Features

- **Modern Web Interface**: Responsive HTML/CSS/JavaScript frontend
- **CLI Interface**: Complete banking operations with cross-platform support
- **Web Interface**: REST API with Crow framework + modern frontend
- **Cross-Platform**: Windows/Linux builds via CMake with automated releases
- **Real-time Updates**: Live transaction stats and history
- **Security First**: Session management, input validation, encrypted storage
- **Account Management**: User registration, login, and account operations
- **Real-time Transactions**: Handle deposits, withdrawals, and transfers

## Tech Stack

- **Backend**: C++17, Crow micro web framework
- **Frontend**: HTML5/CSS3/Vanilla JS
- **Build**: CMake
- **Storage**: File-based with XOR encryption

## 📦 Quick Start

### 🚀 Pre-built Releases (Recommended)

Download the latest release for your platform from [Releases](https://github.com/ITx-prash/securebank-cpp/releases):

- **🐧 Linux**: Download `securebank-linux.tar.gz`
- **🪟 Windows**: Download `securebank-windows.zip`

```bash
# Linux
tar -xzf securebank-linux.tar.gz
cd securebank-linux/
./crow_bank_app

# Windows
# Extract securebank-windows.zip
crow_bank_app.exe
```

Then open your browser to `http://localhost:8080`

### 🛠️ Build from Source

#### Simple Build

```bash
cd crow-bank-app/
./build.sh linux    # For Linux
./build.sh windows  # For Windows cross-compilation
```

#### Manual Build (CMake)

```bash
cd crow-bank-app/
mkdir build && cd build
cmake ..
make
./crow_bank_app
```

#### Build Options

```bash
./build.sh --help    # See all build options
```

## 🏗️ Development

### Automated CI/CD

This project uses GitHub Actions for professional development workflow:

- **🔄 Continuous Integration**: Every push to `main` triggers automated builds
- **📦 Automated Releases**: Cross-platform builds automatically published
- **🔍 Quality Checks**: Code analysis and security scanning
- **📝 Release Notes**: Auto-generated changelogs and version management

### Project Structure

```
securebank-cpp/
├── crow-bank-app/
│   ├── main.cpp              # Entry point + Crow server setup
│   ├── crow_all.h            # Crow framework header
│   ├── CMakeLists.txt        # Build configuration
│   ├── public/               # Web interface files
│   │   ├── index.html        # Login page
│   │   ├── register.html     # Registration
│   │   ├── dashboard.html    # Main banking interface
│   │   ├── style.css         # Styles
│   │   └── js/               # JavaScript modules
│   └── build.sh              # Build script
├── .github/
│   └── workflows/            # CI/CD pipelines
│       ├── release.yml       # Automated builds and releases
│       ├── quality-check.yml # Code quality checks
│       └── version-bump.yml  # Version management
└── README.md                 # This file
```

### Prerequisites

- **Linux**: GCC 8.0+, CMake 3.15+, Make
- **Windows**: MinGW-w64, CMake 3.15+
- **Memory**: 512MB RAM minimum, 1GB+ recommended
- **Storage**: 50MB free space

### Testing Environment

- Tested on Linux (native & KVM VMs)
- Windows cross-compilation support
- Lightweight - works well in resource-constrained environments

## 🌐 Usage

### Web Interface

Start server and visit `http://localhost:8080`

### API Endpoints

| Method | Endpoint            | Description         |
| ------ | ------------------- | ------------------- |
| `POST` | `/api/register`     | User registration   |
| `POST` | `/api/login`        | Authentication      |
| `GET`  | `/api/user`         | User data           |
| `POST` | `/api/deposit`      | Deposit funds       |
| `POST` | `/api/withdraw`     | Withdraw funds      |
| `POST` | `/api/transfer`     | Transfer money      |
| `GET`  | `/api/transactions` | Transaction history |

## 🐛 Troubleshooting

### Common Issues

1. **Port already in use**: Application uses port 8080 - ensure it's available
2. **Missing dependencies**: Install C++ development tools and check CMake version
3. **Web assets not loading**: Ensure `public` folder is with the executable
4. **Permission errors**: Check file permissions on Linux/macOS

## 📄 License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.

---

**Educational project showcasing practical C++ development and modern web integration.**

**Made with ❤️ by ITx-prash**
