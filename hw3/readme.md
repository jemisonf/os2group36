# CS 444 Homework 3

## Links to Resources Used (Template for Block Driver and Crypto Stuff)
### Discussion of Linux Device Drivers: https://lwn.net/Kernel/LDD3/
### Link to Block Driver Source: (Used in project) http://blog.superpat.com/2010/05/04/a-simple-block-driver-for-linux-kernel-2-6-31/
### This link also includes: the Makefile and Module examples to compare with CryptoLoop modules
### This above Code was combined with the CrytoLoop enabling 
### Module Found here: https://elixir.bootlin.com/linux/v3.14.26/source/drivers/block/cryptoloop.c
### SOURCE FOR KERNEL MODULE COMMANDS: https://wiki.archlinux.org/index.php/Kernel_module
### Group 36: Arya Asgari, Scott Russell, Fischer Jemison

## How to build and test the block driver
1. Apply our patch file by running `patch -p0 -i hw3.patch` from the base of the linux-yocto directory.

1. Run `make menuconfig`
1. Select `Device Drivers`
1. Select `Block devices`
1. Select SBD and press the spacebar. An "M" should appear between the angle brackets next to the name.
1. Save and exit the configuration
1. Run `make -j4 all` to build the kernel and all modules. You may see a warning from sbd.c but it should compile successfully.
1. Make sure you have a second terminal session open. We recommond using `tmux new` and then `<CTRL+B><%>` to open two vertical terminal sessions.
1.  In each terminal session, source the file that will configure bash settings to utilize the qemu environment with `source /scratch/opt/environment-setup-i586-poky-linux`
1. Start the vm with `qemu-system-i386 -gdb tcp::5536 -nographic -kernel arch/x86/boot/bzImage -drive file=core-image-lsb-sdk-qemux86.ext4,if=virtio -enable-kvm -usb -localtime -net user,hostfwd=tcp::55360-:22 -net nic --no-reboot --append "root=/dev/vda rw console=ttyS0 debug"`. This will start with networking enabled and forward host port 55360 to port 22 on the virtual machine, allowing you to connect ssh and scp via port 55360.
1. Enter `root` as the username.
1. On the second terminal, from the base linux-yocto directory, run `scp -P 55360 drivers/block/sbd.ko root@localhost:/home/root` to transfer the kernel module to the virtual machine.
1. In the terminal hosting the vm, run `ls` in the base directory for the root user. You should have a file called `sbd.ko`.
1. to load the module: `insmod sbd.ko`
1. `lsblk` will show that it is loaded
1. `mkfs -t ext2 /dev/sbd0` creates a file system for the module
1. `mkdir /mtn` creates a new folder location for interaction with the module
1. `mount -t ext2 /dev/sbd0 /mtn` mount the module in /mtn with the ext2 file system
1. `lsblk -f` should show the mounted module
1. `echo "sample string" > /mtn/$(folder name)` writes to the module
1. `cat /mtn/$(folder name)` reads from the module
1. A grep for the sample string should return nothing if it is encrypted properly
1. When you are done with the module, unmount it with `umount /mtn`
1. Remove the module with `rmmod sbd.ko`
