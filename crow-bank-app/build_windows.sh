#!/bin/bash
# Build and deploy script for Windows

# Configuration
SHARED_FOLDER="/home/prash/Desktop/Windows_Share"
VM_NAME="win10"
DEPLOY_DIR="$SHARED_FOLDER/crow-bank-app"

echo "🔨 Building for Windows..."

# Create Windows build directory if it doesn't exist
mkdir -p win-build
cd win-build

# Configure for Windows cross-compilation
cmake .. -DCMAKE_TOOLCHAIN_FILE=../win-toolchain.cmake -DCMAKE_BUILD_TYPE=Release
make -j$(nproc)

if [ $? -eq 0 ]; then
    echo "✅ Windows build successful!"
    
    # Create deployment directory
    echo "📦 Deploying to shared folder..."
    mkdir -p "$DEPLOY_DIR"
    
    # Copy executable and public folder
    cp crow_bank_app.exe "$DEPLOY_DIR/"
    cp -r ../public "$DEPLOY_DIR/"
    
    echo "📁 Deployed to: $DEPLOY_DIR"
    echo "   Files:"
    ls -la "$DEPLOY_DIR"
    
    # Optional: Start VM
    if [ "$START_VM" = true ]; then
        echo "🚀 Starting Windows VM: $VM_NAME"
        virsh start "$VM_NAME"
        
        if [ $? -eq 0 ]; then
            echo "✅ VM started successfully."
            echo "� Waiting 10s for VM to begin boot..."
            sleep 8
            
            # Open the VM display
            echo "🖥️ Launching viewer window..."
            virt-viewer "$VM_NAME" &
            
            echo ""
            echo "🎯 VM Instructions:"
            echo "   1. Wait for Windows to fully boot"
            echo "   2. Navigate to the shared folder in Windows"
            echo "   3. Run crow_bank_app.exe"
            echo "   4. Open browser to http://localhost:8080"
        else
            echo "❌ Failed to start VM."
        fi
    fi
    
    echo ""
    echo "✅ Deployment complete!"
    echo "   Windows executable: $DEPLOY_DIR/crow_bank_app.exe"
    echo "   Web files: $DEPLOY_DIR/public/"
    echo ""
    echo "In Windows VM:"
    echo "   1. Navigate to the shared folder"
    echo "   2. Run crow_bank_app.exe"
    echo "   3. Open browser to http://localhost:8080"
    
else
    echo "❌ Windows build failed!"
    exit 1
fi
