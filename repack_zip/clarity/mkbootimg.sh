#!/sbin/sh
#
# mkboot script helper
# Base of Anykernel by osm0sis (https://github.com/osm0sis).
# rainforce279 @ 17-12-2017

cmdline=`cat /tmp/clarity/boot.img-cmdline`;
board=`cat /tmp/clarity/boot.img-board`;
base=`cat /tmp/clarity/boot.img-base`;
pagesize=`cat /tmp/clarity/boot.img-pagesize`;
kerneloff=`cat /tmp/clarity/boot.img-kerneloff`;
ramdiskoff=`cat /tmp/clarity/boot.img-ramdiskoff`;
tagsoff=`cat /tmp/clarity/boot.img-tagsoff`;
secondoff=`cat /tmp/clarity/boot.img-secondoff`;

if [ -f /tmp/clarity/boot.img-hash ]; then
	hashfck=`cat /tmp/clarity/boot.img-hash`;
else
	hashfck=sha1;
fi;

if [ -f /tmp/clarity/boot.img-osversion ]; then
	osver=`cat /tmp/clarity/boot.img-osversion`;
else
	osver=7.1.2;
fi;

if [ -f /tmp/clarity/boot.img-oslevel ]; then
	osvel=`cat /tmp/clarity/boot.img-oslevel`;
else
	osvel=2018-05;
fi;

/tmp/clarity/mkbootimg --kernel /tmp/clarity/zImage --ramdisk /tmp/clarity/boot.img-ramdisk.gz --cmdline "$cmdline" --board "$board" --base $base --pagesize $pagesize --kernel_offset $kerneloff --ramdisk_offset $ramdiskoff --second_offset $secondoff --tags_offset "$tagsoff" --os_version "$osver" --os_patch_level "$osvel" --hash "$hashfck" --output /tmp/clarity/newboot.img;

