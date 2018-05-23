#Discussion of Linux Device Drivers: https://lwn.net/Kernel/LDD3/
#Link to Block Driver Source: (Used in project) http://blog.superpat.com/2010/05/04/a-simple-block-driver-for-linux-kernel-2-6-31/
#This link also includes: the Makefile and Module examples to compare with CryptoLoop modules
#This above Code was combined with the CrytoLoop enabling Module Found here: https://elixir.bootlin.com/linux/v3.14.26/source/drivers/block/cryptoloop.c
#SOURCE FOR KERNEL MODULE COMMANDS: https://wiki.archlinux.org/index.php/Kernel_module
##List of commands and HOW TO BUILD
#Group 36
In order to create Multiple Sessions to run concurrently software like tmux can be used 
(or multiple instance of putty)
tmux creation:  https://robots.thoughtbot.com/a-tmux-crash-course)
Note: `prefix` is defined by default as `control-b`

1. Make sure you have a kernel and the associated .ext3 and .bin files
1. `cd linux-yocto-3.19/drivers/block`
1. get a sbd example
    * [sbd.c](http://blog.superpat.com/2010/05/04/a-simple-block-driver-for-linux-kernel-2-6-31/)
1. Give your new driver a name, I will be using sbd.c in this example
1. In `drivers/block` there is a Makefile. Edit it and
        add the line `obj-$(CONFIG_BLK_DEV_SBD) += sbd.o`
        (put it in a place where it looks like it might belong.
        Pretty much anywhere is fine)
1. Save and close the Makefile
1. Open up the Kconfig file in the same directory and add a new config entry for your new driver.
    You can use other entries in the file as an example. You don't need to include any dependencies
    or anything like that. All you need is a:
    1. config (which in this case would be BLK_DEV_SBD)
    1. tristate (the name that the driver will be listed under in the menuconfig)
    1. help (the description of the driver)
1. Save and close the Kconfig file
1. `cd ../../`
1. `make menuconfig`
1. Select `Device Drivers`
1. Select `Block devices`
1. Select this NEW Block Device. Press "M" to set it to be modular.
1. Save and exit the configuration
1. `cd ../`
1. `tmux` (opens up a screen session) // this is optinal based on what you want
1.  Source the file that will configure bash settings to utilize the qemu environment: `source /scratch/opt/environment-setup-i586-poky-linux`
1. `(ctr-b) + c` (creates a new screen) //tmux commant (use ctr-b + w) to view all tmux session
1. `gdb`
1. `ctr-b + n` (takes you to the next screen, which should be the first screen you made)
1. Compile the kernel and then start it up in the vm with `qemu-system-i386 -gdb tcp::5622 -S -nographic -kernel linux-yocto-3.19/arch/x86/boot/bzImage -drive file=core-image-lsb-sdk-qemux86.ext3 -enable-kvm -usb -localtime --no-reboot --append "root=/dev/hda rwconsole=ttyS0 debug"`
1. `prefix + n` (takes you to the next screen, which should be your gdb screen)
1. `target remote :5622`
1. 	Type `continue`. This will initate the boot sequence of the VM from the port that GDB has connected to.
1. `ctr-b + n` (you get the idea)
1. Log in as root


//////////////// !!!!havn't tested below yet!!!!/////////////
1 commands modified from https://wiki.archlinux.org/index.php/Kernel_module
1. While in the VM, run: `scp $(username)@os-class.engr.oregonstate.edu:/scratch/spring2018/group36/linux-yocto-3.19/drivers/block/$(driver).ko .` This will transfer the driver into the VM. (both $() fields should be replaced with your associated values)

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