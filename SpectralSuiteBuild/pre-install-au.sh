#!/bin/bash
# Backs up any Spectral Suite AUs already in the install location before they
# are overwritten. The installer runs this as root, so $HOME is root's home;
# the backup goes to the logged-in user's home instead.

PLUGINS=("Bin Scrambler" "Frequency Magnet" "Frequency Shifter" "Morph" "Phase Lock" "Sinusoidal Shaped Filter" "Spectral Gate")

user=$(/usr/bin/stat -f%Su /dev/console)
if [ -z "$user" ] || [ "$user" = "root" ] || [ "$user" = "loginwindow" ]; then
    user=""
    backup_root="/Users/Shared/SpectralSuite-Backup"
else
    home=$(/usr/bin/dscl . -read "/Users/$user" NFSHomeDirectory | /usr/bin/awk '{print $2}')
    backup_root="$home/SpectralSuite-Backup"
fi
dir="$backup_root/AU-$(date +%F-%H-%M-%S)"

/bin/mkdir -p "$dir"
for name in "${PLUGINS[@]}"; do
    [ -e "$DSTROOT/$name.component" ] && /bin/cp -Rp "$DSTROOT/$name.component" "$dir/"
done

# Remove the folders again when there was nothing to back up.
/bin/rmdir "$dir" "$backup_root" 2>/dev/null
[ -d "$dir" ] && [ -n "$user" ] && /usr/sbin/chown -R "$user" "$backup_root"
exit 0
