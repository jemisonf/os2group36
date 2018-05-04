1. Group 36: Scott Russell

1. LINK TO no-op schedular that was adjusted for our implementation
1. https://elixir.bootlin.com/linux/latest/source/block/noop-iosched.c

1. // current files
1.	sstf-iosched.c
1.	readme
1. // files that need to be added
1.	group36.patch
1.	latex file
1. 	python test file.


	

1. ## Builds Steps

1.	Open two terminal windows. One for gdb, one for virutal machine.
1.	log into CS 444 os-class
1.	make way to our group `cd /scratch/spring2018/group36`
1.	Clone the linux kernel: `git clone git://git.yoctoproject.org/linux-yocto-3.19`
1.	Navigate into the kernel: `cd linux-yocto-3.19`
1.  Checkout to a previous version: `git checkout v3.19.2`
1.	Source the file that will configure bash settings to utilize the qemu environment: `source /scratch/files/environment-setup-i586-poky-linux`
1.	Copy qemu config: `cp /scratch/files/config-3.19.2-yocto-standard .config`
1.	Run the menuconfig file: `make menuconfig` (a window should pop up)
1.	In this window, press `/` and type in `LOCALVERSION` then press enter. 
1. // I was having a problem deleting the value of the kernel name so I just appended our group name to the end
1.	Hit `1`, press enter and then edit the value to be `-yocto-standardG36`. This will be appended to the kernel name
1.	`make -j4 all` (your kernel instance will be built with 4 threads)


1. // TODO* Add Patch File
1. // Would be patched into the linux-yocto-3.19/block folder
1.   cp patch file into linux-yocto-3.19/block
1.  run `patch < assn2.patch`
1. // remade with sstf schedular
1.  run `make -j4 all` and select sstf scheduler



1. // start gdb
1.	`cd ..` and then run gdb. (source directory)
1.	gdb -tui 5536
1. Copy over a pre-built version of the kernel to test booting into the VM with: `cp /scratch/files/bzImage-qemux86.bin .`
1.	To run a virtual drive for the VM in the local directory: `/scratch/files/core-image-lsb-sdk-qemux86.ext4 .`
1.	Run the command to start up the standard linux kernel: `qemu-system-i586 -gdb tcp::5536 -S -nographic -kernel linux-yocto-3.19/arch/x86/boot/bzImage -drive file=core-image-lsb-sdk-qemux86.ext4 -enable-kvm -net none -usb -localtime --no-reboot --append "root=/dev/hda rwconsole=ttyS0 debug"`
1.	In terminal running gdb: `target remote :5536` to connect the qemu.
1.	Type `continue`. This will start the boot on the virtual machine.
1. 	Login with root, no password
1. 	to check the name of the current running VM 'uname -a' 
1.	if it's not -yocto-standardG36 than you did not properly change the menuconfig.

1. 	// Todo: Create a file to test the sstf-iosched.c scheduler.
1. create python file to test with: `cat > 'group36_test.py'` ctrl-d to save
1. run python file `python group36_test.py`
