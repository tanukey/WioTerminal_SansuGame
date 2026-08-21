#!/usr/bin/env bash
# Build the firmware and stage a .uf2 file for manual drag-and-drop flashing.
#
# Background: on this WSL2 setup, flashing the Wio Terminal directly from WSL
# (usbipd passthrough + bossac/sam-ba over the CDC serial port) is unreliable
# once the board is in its UF2 bootloader (frequent USB resets, bossac hangs).
# The robust path is: build in WSL, convert firmware.bin to firmware.uf2,
# copy it to the Windows side, then double-tap the board's reset button and
# drag the .uf2 file onto the "Arduino" mass-storage drive from Windows
# Explorer. The board flashes and reboots automatically on drop.
set -euo pipefail
cd "$(dirname "$0")/.."

export PATH="$HOME/.local/bin:$PATH"

BUILD_DIR=".pio/build/seeed_wio_terminal"
OFFSET="0x4000" # seeed_wio_terminal upload.offset_address (bootloader size)
WIN_USER="${WIN_USER:-tanukey}"
DEST="/mnt/c/Users/${WIN_USER}/Desktop/sansugame_firmware.uf2"

pio run

python3 tools/bin2uf2.py "${BUILD_DIR}/firmware.bin" "${BUILD_DIR}/firmware.uf2" "${OFFSET}"

cp "${BUILD_DIR}/firmware.uf2" "${DEST}"

echo ""
echo "firmware.uf2 copied to: ${DEST}"
echo "Next steps:"
echo "  1. Double-tap the Wio Terminal's reset button (bottom of the device)."
echo "  2. Wait for the 'Arduino' removable drive to appear in Windows Explorer."
echo "  3. Drag/copy sansugame_firmware.uf2 onto that drive."
echo "  The board flashes and reboots automatically."
