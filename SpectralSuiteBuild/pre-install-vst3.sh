#!/bin/bash
# Backs up the installed Spectral Suite VST3 folder before it is overwritten.
# The installer runs this as root, so $HOME is root's home; the backup goes to
# the logged-in user's home instead.

user=$(/usr/bin/stat -f%Su /dev/console)
if [ -z "$user" ] || [ "$user" = "root" ] || [ "$user" = "loginwindow" ]; then
    user=""
    backup_root="/Users/Shared/SpectralSuite-Backup"
else
    home=$(/usr/bin/dscl . -read "/Users/$user" NFSHomeDirectory | /usr/bin/awk '{print $2}')
    backup_root="$home/SpectralSuite-Backup"
fi
dir="$backup_root/VST3-$(date +%F-%H-%M-%S)"

if [ -d "$DSTROOT" ] && [ -n "$(/bin/ls -A "$DSTROOT")" ]; then
    /bin/mkdir -p "$dir"
    /bin/cp -Rp "$DSTROOT/" "$dir/"
    [ -n "$user" ] && /usr/sbin/chown -R "$user" "$backup_root"
fi
exit 0
