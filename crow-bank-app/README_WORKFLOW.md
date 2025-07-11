# Crow Bank App - Development Workflow

## Quick Reference

### For Development (Linux Testing)

```bash
./build_linux.sh
```

- Builds for Linux in `build-linux/` directory
- Automatically starts the development server
- Access at: http://localhost:8080
- Press Ctrl+C to stop

### For Windows Deployment

```bash
./build_windows.sh
```

- Builds for Windows in `win-build/` directory
- Deploys to `/home/prash/Desktop/Windows_Share/crow-bank-app/`
- Includes both executable and public folder

### For Windows Deployment + VM Launch

```bash
./build.sh windows --run-vm
```

- Builds for Windows
- Deploys to shared folder
- Automatically starts Windows VM
- Opens VM viewer

### Start VM Only

```bash
./start_vm.sh
```

- Just starts the Windows VM without building

## Development Workflow

### 1. Daily Development Cycle

```bash
# Make code changes
./build_linux.sh        # Quick test on Linux
# Continue development...
```

### 2. Windows Testing Cycle

```bash
# After major changes
./build.sh windows --run-vm    # Build, deploy, and start VM
```

### 3. Full Testing

```bash
./build.sh both --clean        # Clean build both platforms
```

## Directory Structure

```
crow-bank-app/
├── main.cpp                   # Your source code
├── public/                    # Web assets (HTML, CSS, JS)
├── build-linux/              # Linux builds (development)
├── win-build/                 # Windows builds (cross-compiled)
├── build_linux.sh            # Linux build script
├── build_windows.sh          # Windows build + deploy script
├── build.sh                  # Unified build script
├── start_vm.sh               # VM launcher
└── CMakeLists.txt            # Build configuration
```

## Shared Folder Setup

- **Linux Path**: `/home/prash/Desktop/Windows_Share/crow-bank-app/`
- **Contains**: `crow_bank_app.exe` + `public/` folder
- **In Windows VM**: Navigate to your shared folder location

## Platform Configuration Details

The CMakeLists.txt now uses `CMAKE_SYSTEM_NAME STREQUAL "Windows"` which correctly detects cross-compilation target:

- **Cross-compiling to Windows**: Uses static linking (-static-libgcc -static-libstdc++)
- **Building for Linux**: Uses dynamic linking with pthread

This ensures no DLL dependencies when running the Windows executable.

## Tips

1. Use `./build_linux.sh` for rapid development iterations
2. Use `./build_windows.sh` when you want to test on Windows
3. The `--run-vm` flag automatically starts your VM after Windows build
4. Both builds automatically copy the `public/` folder to their respective build directories
