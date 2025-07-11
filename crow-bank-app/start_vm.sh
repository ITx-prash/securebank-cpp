#!/bin/bash
# Quick VM launcher script

VM_NAME="win10"

echo "🚀 Starting Windows VM: $VM_NAME"
virsh start "$VM_NAME"

if [ $? -ne 0 ]; then
    echo "❌ Failed to start VM."
    exit 1
fi

echo "✅ VM started successfully."
echo "💤 Waiting 10s for VM to begin boot..."
sleep 8

# Open the VM display
echo "🖥️ Launching viewer window..."
virt-viewer "$VM_NAME" &

echo ""
echo "🎯 VM is starting up!"
echo "   Shared folder location in Windows: (check your VM shared folder configuration)"
echo "   App will be at: crow-bank-app/crow_bank_app.exe"
