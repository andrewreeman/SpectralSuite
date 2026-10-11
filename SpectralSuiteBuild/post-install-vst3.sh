#!/bin/bash
# The plugins are ad-hoc signed, not notarized: clear the download quarantine
# flag so hosts load them without a Gatekeeper prompt.

[ -d "$DSTROOT" ] && /usr/bin/xattr -dr com.apple.quarantine "$DSTROOT"
exit 0
