#!/sbin/sh
#
# FT5336 Touchscreen files.
#

if [ ! -f /system/etc/ft5336_biel_1080p.ini ];
then
	cp /tmp/clarity/ft5336_biel_1080p.ini /system/etc/ft5336_biel_1080p.ini
	if [ -f /system/etc/ft5336_biel_1080p.ini ];
	then
		chmod 0644 /system/etc/ft5336_biel_1080p.ini
		chown root:root /system/etc/ft5336_biel_1080p.ini
	fi
fi

if [ ! -f /system/etc/ft5336_oufei_720p.ini ];
then
	cp /tmp/clarity/ft5336_oufei_720p.ini /system/etc/ft5336_oufei_720p.ini
	if [ -f /system/etc/ft5336_oufei_720p.ini ];
	then
		chmod 0644 /system/etc/ft5336_oufei_720p.ini
		chown root:root /system/etc/ft5336_oufei_720p.ini
	fi
fi

