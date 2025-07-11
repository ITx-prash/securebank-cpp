#!/bin/bash
# Build script for Linux development/testing

echo "🔨 Building for Linux (development)..."

# Create Linux build directory if it doesn't exist
mkdir -p build-linux
cd build-linux

# Configure and build for Linux
cmake .. -DCMAKE_BUILD_TYPE=Debug
make -j$(nproc)

if [ $? -eq 0 ]; then
    echo "✅ Linux build successful!"
    echo "🚀 Starting development server..."
    echo "   Access at: http://localhost:8080"
    echo "   Press Ctrl+C to stop"
    echo ""
    # Copy public folder for Linux testing
    cp -r ../public .
    # Run the Linux executable
    ./crow_bank_app
else
    echo "❌ Linux build failed!"
    exit 1
fi
