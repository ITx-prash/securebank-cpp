# 🏦 SecureBank-CPP

Modern C++ banking application with CLI and web interfaces. Demonstrates OOP concepts, REST API design, and cross-platform development.

## Features

- **CLI Interface**: Complete banking operations with cross-platform support
- **Web Interface**: REST API with Crow framework + modern frontend
- **Cross-Platform**: Windows/Linux builds via CMake
- **Real-time Updates**: Live transaction stats and history
- **Security**: Session management, input validation, encrypted storage

## Tech Stack

- **Backend**: C++17, Crow micro web framework
- **Frontend**: HTML5/CSS3/Vanilla JS
- **Build**: CMake
- **Storage**: File-based with XOR encryption

## Quick Start

### Simple Build

```bash
cd crow-bank-app/
./build.sh linux    # For Linux
./build.sh windows  # For Windows cross-compilation
```

### Manual Build (CMake)

```bash
cd crow-bank-app/
mkdir build && cd build
cmake ..
make
./crow_bank_app
```

### Build Options

```bash
./build.sh --help    # See all build options
```

## Development

### Project Structure

```
crow-bank-app/
├── main.cpp              # Entry point + Crow server setup
├── crow_all.h            # Crow framework header
├── public/               # Web interface files
│   ├── index.html        # Login page
│   ├── register.html     # Registration
│   ├── dashboard.html    # Main banking interface
│   ├── style.css         # Styles
│   └── js/               # JavaScript modules
└── build.sh              # Build script
```

### Testing Environment

- Tested on Linux (native & KVM VMs)
- Windows cross-compilation support
- Lightweight - works well in resource-constrained environments

### Web Interface

Start server and visit `http://localhost:8080`

### API Endpoints

- `POST /api/register` - User registration
- `POST /api/login` - Authentication
- `GET /api/user` - User data
- `POST /api/deposit` - Deposit funds
- `POST /api/withdraw` - Withdraw funds
- `POST /api/transfer` - Transfer money
- `GET /api/transactions` - Transaction history

---

**Educational project showcasing practical C++ development and modern web integration.**

**Made with ❤️ by ITx-prash**