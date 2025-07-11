#!/bin/bash
# Unified build script for Crow Bank App

# Configuration
SHARED_FOLDER="/home/prash/Desktop/Windows_Share"
VM_NAME="win10"
DEPLOY_DIR="$SHARED_FOLDER/crow-bank-app"

usage() {
    echo "Usage: $0 [linux|windows|both] [options]"
    echo ""
    echo "Platforms:"
    echo "  linux    - Build and run for Linux (development)"
    echo "  windows  - Build for Windows, deploy to shared folder, and start VM"
    echo "  both     - Build for both platforms"
    echo ""
    echo "Options:"
    echo "  --no-vm       - Skip starting Windows VM after Windows build"
    echo "  --no-clean    - Skip cleaning build directories (faster, but less reliable)"
    echo "  --help        - Show this help message"
    echo ""
    echo "Examples:"
    echo "  $0 linux                      # Clean Linux build (recommended)"
    echo "  $0 linux --no-clean          # Quick incremental Linux build"
    echo "  $0 windows                    # Clean Windows build and start VM (default)"
    echo "  $0 windows --no-vm            # Clean Windows build without starting VM"
    echo "  $0 both --no-clean            # Quick incremental build both platforms"
    echo ""
    echo "Note: Clean builds and VM start are DEFAULT for reliability and convenience."
}

# Parse arguments
PLATFORM=""
RUN_VM=true   # Default to starting VM for Windows builds
CLEAN=true    # Default to clean builds for reliability

while [[ $# -gt 0 ]]; do
    case $1 in
        linux|windows|both)
            PLATFORM="$1"
            shift
            ;;
        --no-vm)
            RUN_VM=false
            shift
            ;;
        --no-clean)
            CLEAN=false
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

# Clean build directories by default (for reliability)
if [ "$CLEAN" = true ]; then
    echo "🧹 Cleaning build directories for reliable build..."
    rm -rf build-linux win-build
else
    echo "⚡ Using incremental build (faster but potentially less reliable)..."
fi

# Function to build for Linux
build_linux() {
    echo "🐧 Building for Linux..."
    
    mkdir -p build-linux
    cd build-linux
    
    cmake .. -DCMAKE_BUILD_TYPE=Debug
    make -j$(nproc)
    
    if [ $? -eq 0 ]; then
        echo "✅ Linux build successful!"
        
        # If only building for Linux, run the server
        if [ "$PLATFORM" = "linux" ]; then
            echo "🚀 Starting development server..."
            echo "   Access at: http://localhost:8080"
            echo "   Press Ctrl+C to stop"
            echo ""
            ./crow_bank_app
            exit 0
        fi
        
        cd ..
        echo "✅ Linux build completed"
    else
        echo "❌ Linux build failed!"
        exit 1
    fi
}

# Function to build for Windows
build_windows() {
    echo "🪟 Building for Windows..."
    
    mkdir -p win-build
    cd win-build
    
    cmake .. -DCMAKE_TOOLCHAIN_FILE=../win-toolchain.cmake -DCMAKE_BUILD_TYPE=Release
    make -j$(nproc)
    
    if [ $? -eq 0 ]; then
        echo "✅ Windows build successful!"
        
        # Deploy to shared folder
        echo "📦 Deploying to shared folder..."
        mkdir -p "$DEPLOY_DIR"
        
        cp crow_bank_app.exe "$DEPLOY_DIR/"
        cp -r ../public "$DEPLOY_DIR/"
        
        echo "📁 Deployed to: $DEPLOY_DIR"
        echo "   Files: crow_bank_app.exe, public/"
        
        # Start VM if requested
        if [ "$RUN_VM" = true ]; then
            echo ""
            echo "🚀 Starting Windows VM: $VM_NAME"
            virsh start "$VM_NAME"
            
            if [ $? -eq 0 ]; then
                echo "✅ VM started successfully."
                echo "💤 Waiting 10s for VM to begin boot..."
                sleep 8
                
                echo "🖥️ Launching viewer window..."
                virt-viewer "$VM_NAME" &
                
                echo ""
                echo "🎯 VM Instructions:"
                echo "   1. Wait for Windows to fully boot"
                echo "   2. Navigate to shared folder"
                echo "   3. Run crow_bank_app.exe"
                echo "   4. Open browser to http://localhost:8080"
            else
                echo "❌ Failed to start VM."
            fi
        fi
        
        cd ..
        echo "✅ Windows build and deployment completed"
    else
        echo "❌ Windows build failed!"
        exit 1
    fi
}

# Function to start VM only
start_vm() {
    echo "🚀 Starting Windows VM: $VM_NAME"
    virsh start "$VM_NAME"
    
    if [ $? -ne 0 ]; then
        echo "❌ Failed to start VM."
        exit 1
    fi
    
    echo "✅ VM started successfully."
    echo "💤 Waiting 10s for VM to begin boot..."
    sleep 8
    
    echo "🖥️ Launching viewer window..."
    virt-viewer "$VM_NAME" &
    
    echo ""
    echo "🎯 App location: $DEPLOY_DIR/crow_bank_app.exe"
}

# Main execution
case $PLATFORM in
    linux)
        build_linux
        ;;
    windows)
        build_windows
        ;;
    both)
        build_linux
        echo ""
        build_windows
        ;;
esac

echo ""
echo "🎉 All builds completed successfully!"

# If this script is called with 'vm' as first argument, just start VM
if [ "$1" = "vm" ]; then
    start_vm
    exit 0
fi
