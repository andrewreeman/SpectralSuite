#!/bin/bash
# The plugins are ad-hoc signed, not notarized: clear the download quarantine
# flag so hosts load them without a Gatekeeper prompt, then restart the AU
# registrar so hosts pick up new or updated components.

PLUGINS=("Bin Scrambler" "Frequency Magnet" "Frequency Shifter" "Morph" "Phase Lock" "Sinusoidal Shaped Filter" "Spectral Gate")

for name in "${PLUGINS[@]}"; do
    [ -e "$DSTROOT/$name.component" ] && /usr/bin/xattr -dr com.apple.quarantine "$DSTROOT/$name.component"
done
/usr/bin/killall -9 AudioComponentRegistrar 2>/dev/null
exit 0
