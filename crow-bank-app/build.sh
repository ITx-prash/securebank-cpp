#!/bin/bash
# Comprehensive build and deploy script

usage() {
    echo "Usage: $0 [linux|windows|both] [options]"
    echo ""
    echo "Platforms:"
    echo "  linux    - Build for Linux (quick development testing)"
    echo "  windows  - Build for Windows and deploy to shared folder"
    echo "  both     - Build for both platforms"
    echo ""
    echo "Options:"
    echo "  --run-vm    - Start Windows VM after Windows build"
    echo "  --clean     - Clean build directories before building"
    echo "  --help      - Show this help message"
    echo ""
    echo "Examples:"
    echo "  $0 linux                    # Quick Linux build and test"
    echo "  $0 windows --run-vm         # Build for Windows and start VM"
    echo "  $0 both --clean             # Clean build both platforms"
}

# Parse arguments
PLATFORM=""
RUN_VM=false
CLEAN=false

while [[ $# -gt 0 ]]; do
    case $1 in
        linux|windows|both)
            PLATFORM="$1"
            shift
            ;;
        --run-vm)
            RUN_VM=true
            shift
            ;;
        --clean)
            CLEAN=true
            shift
            ;;
        --help)
            usage
            exit 0
            ;;
        *)
            echo "Unknown option: $1"
            usage
            exit 1
            ;;
    esac
done

if [ -z "$PLATFORM" ]; then
    echo "Error: Platform not specified"
    usage
    exit 1
fi

# Clean build directories if requested
if [ "$CLEAN" = true ]; then
    echo "🧹 Cleaning build directories..."
    rm -rf build-linux win-build
fi

# Build for Linux
if [ "$PLATFORM" = "linux" ] || [ "$PLATFORM" = "both" ]; then
    echo "🐧 Building for Linux..."
    ./build_linux.sh
    if [ $? -ne 0 ]; then
        echo "❌ Linux build failed!"
        exit 1
    fi
    
    # If only Linux, exit here (the script will run the server)
    if [ "$PLATFORM" = "linux" ]; then
        exit 0
    fi
    
    echo "✅ Linux build completed"
    echo ""
fi

# Build for Windows
if [ "$PLATFORM" = "windows" ] || [ "$PLATFORM" = "both" ]; then
    echo "🪟 Building for Windows..."
    
    # Modify build_windows.sh to handle VM starting
    if [ "$RUN_VM" = true ]; then
        # This will be handled in the Windows build script
        export START_VM=true
    fi
    
    ./build_windows.sh
    if [ $? -ne 0 ]; then
        echo "❌ Windows build failed!"
        exit 1
    fi
fi

echo ""
echo "🎉 All builds completed successfully!"
