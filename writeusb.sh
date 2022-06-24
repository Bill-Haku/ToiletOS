version=0.3.1
diskutil unmountDisk /dev/disk2
cd ~/toiletos-release
dd if=./disk-${version}.img of=/dev/disk2
diskutil eject /dev/disk2