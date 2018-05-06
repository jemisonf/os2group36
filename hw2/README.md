# HW 2
Group 36: Scott Russell, Fischer Jemison, Arya Asgari

LINK TO no-op schedular that was adjusted for our implementation
https://elixir.bootlin.com/linux/latest/source/block/noop-iosched.c

## Files included:
* sstf-iosched.c
* readme
* group36.patch
* latex file
* c test file.




## Builds Steps

1.	Open two terminal windows. One for gdb, one for virutal machine.
1.	log into CS 444 os-class
1.	make way to our group `cd /scratch/spring2018/group36`
1.	Clone the linux kernel: `git clone git://git.yoctoproject.org/linux-yocto-3.19`
1.	Navigate into the kernel: `cd linux-yocto-3.19`
1.  	Checkout to a previous version: `git checkout v3.19.2`
1.	Source the file that will configure bash settings to utilize the qemu environment: `source /scratch/files/environment-setup-i586-poky-linux`
1.  Copy our block.patch file to base of the linux-yocto directory and run `patch -i -p0 block.patch`. This will copy our configuration settings to the blocks folder.
1.	Copy qemu config: `cp /scratch/files/config-3.19.2-yocto-standard .config`
1.	Run the menuconfig file: `make menuconfig` (a window should pop up)
1.	In this window, press `/` and type in `LOCALVERSION` then press enter. 
1.	Hit `1`, press enter and then edit the value to be `-yocto-standardG36`. This will be appended to the kernel name
1.  Press `/` and type sstf. Press `2`. This will allow you to select the default scheduler, which you should make sure is set to sstf.
1.  	run `make -j4 all` and select sstf scheduler
1.	`make -j4 all` (your kernel instance will be built with 4 threads)
1.	To run a virtual drive for the VM in the local directory: `/scratch/files/core-image-lsb-sdk-qemux86.ext4 .`
1.	Start the kernel with: `qemu-system-i586 -gdb tcp::5536 -S -nographic -kernel linux-yocto-3.19/arch/x86/boot/bzImage -drive file=core-image-lsb-sdk-qemux86.ext4 -enable-kvm -net none -usb -localtime --no-reboot --append "root=/dev/hda rwconsole=ttyS0 debug"`
1.  Login as root. There is no password.
1. 	to check the name of the current running VM 'uname -a'. It should end it G36.
1.  Run `dmesg | grep SSTF`. You should see a message reading "SSTF starting" that was outputted as the kernel was starting.
1.  If you are using our hard drive image, check the `test` directory. You should find a script called sstf-test.c and a Makefile.
1.  *If you are not using our hard drive image*, you need to create these files yourself. You should copy the text from [sstf\_test.c](https://raw.githubusercontent.com/jemisonf/os2group36/master/hw2/test/sstf_test.c) and [Makefile](https://github.com/jemisonf/os2group36/blob/master/hw2/test/Makefile) by following the previous links to the raw text of those files. For each file, copy the text of the file to your clipboard. Then, in the qemu vm, use vi or your preferred text editor to paste the text of the file into a file of the same name in the qemu vm. You can do this where you want as long as there are no conflicting makefiles. We used a directory called `test` when running our tests.
1.  Run `make` to compile the test. You can also use `make more_files` if you want to test with 200 files instead of 100.
1.  You should have a binary called `sstf_test` or `sstf_test_many` if you used the `more_files` target. When run, it will concurrently attempt to write 100 or 200 files with 1000 random characters each to the hard drive. If this occurs in a reasonable amount of time, then our solution has succeeded.
