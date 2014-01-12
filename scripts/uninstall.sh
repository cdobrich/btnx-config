#!/bin/bash

#################################################
# btnx uninstall script				#
# ----------------------------------------------#
# Author: Olli Salonen <oasalonen@gmail.com>	#
#################################################

SCRIPT_DIR=scripts
DATA_DIR=data
DEST_DATA_DIR=/usr/share/btnx-config
LOCALE_DIR=po/locale
NAME=btnx-config
BIN_DIR=/usr/sbin
PROGRAM=$BIN_DIR/$NAME
CONFIG_DIR=/etc/btnx
PIXMAP_DIR=/usr/share/pixmaps
HICOLOR_DIR=/usr/share/icons/hicolor
DESKTOP_DIR=/usr/share/applications
CONFIG=btnx_config
EVENTS=events

DESKTOP_FILE=$NAME.desktop
GLADE_FILE=$DATA_DIR/$NAME.glade
ICON_16=btnx-16x16.png
ICON_24=btnx-24x24.png
ICON_32=btnx-32x32.png
ICON_48=btnx-48x48.png
ICON_72=btnx-72x72.png
ICON_128=btnx-128x128.png
ICON_NAME=btnx.png

echo " "
echo -ne "Uninstalling $NAME..."
# Remove config dir
if [ -d $CONFIG_DIR ]; then
	rm -rf $CONFIG_DIR
	if [ $? -ne 0 ]; then
		echo "Error: could not remove $CONFIG_DIR. Did you forget sudo?"
		#exit 1
	fi
fi
echo -ne "."

# Remove the binary file
if [ -f $BIN_DIR/$NAME ]; then
	rm $BIN_DIR/$NAME 
	if [ $? -ne 0 ]; then
		echo "Error: could not remove $BIN_DIR/$NAME."
		#exit 1
	fi
fi
echo -ne "."

# Remove data dir
if [ -f $DEST_DATA_DIR ]; then
	rm -rf $DEST_DATA_DIR
	if [ $? -ne 0 ]; then
		echo "Error: could not remove $DEST_DATA_DIR."
		#exit 1
	fi
fi
echo "."

# Remove desktop file
if [ -f $DESKTOP_DIR/$DESKTOP_FILE ]; then
	rm $DESKTOP_DIR/$DESKTOP_FILE
	if [ $? -ne 0 ]; then
		echo "Error: could not remove $DESKTOP_DIR/$DESKTOP_FILE."
		#exit 1
	fi
fi
echo -ne "."

# Remove pixmap icon
if [ -f $PIXMAP_DIR/$ICON_NAME ]; then
	rm $PIXMAP_DIR/$ICON_NAME
	if [ $? -ne 0 ]; then
		echo "Error: could not remove $PIXMAP_DIR/$ICON_NAME."
		#exit 1
	fi
fi
echo -ne "."

# Remove 128 icon
if [ -f $HICOLOR_DIR/128x128/apps/$ICON_NAME ]; then
	rm $HICOLOR_DIR/128x128/apps/$ICON_NAME
	if [ $? -ne 0 ]; then
		echo "Error: could not remove $HICOLOR_DIR/128x128/apps/$ICON_NAME."
		#exit 1
	fi
fi
echo -ne "."

# Remove 72 icon
if [ -f $HICOLOR_DIR/72x72/apps/$ICON_NAME ]; then
	rm $HICOLOR_DIR/72x72/apps/$ICON_NAME
	if [ $? -ne 0 ]; then
		echo "Error: could not remove $HICOLOR_DIR/72x72/apps/$ICON_NAME."
		#exit 1
	fi
fi
echo -ne "."

# Remove 48 icon
if [ -f $HICOLOR_DIR/48x48/apps/$ICON_NAME ]; then
	rm $HICOLOR_DIR/48x48/apps/$ICON_NAME
	if [ $? -ne 0 ]; then
		echo "Error: could not remove $HICOLOR_DIR/48x48/apps/$ICON_NAME."
		#exit 1
	fi
fi
echo -ne "."

# Remove 32 icon
if [ -f $HICOLOR_DIR/32x32/apps/$ICON_NAME ]; then
	rm $HICOLOR_DIR/32x32/apps/$ICON_NAME
	if [ $? -ne 0 ]; then
		echo "Error: could not remove $HICOLOR_DIR/32x32/apps/$ICON_NAME."
		#exit 1
	fi
fi
echo -ne "."

# Remove 24 icon
if [ -f $HICOLOR_DIR/24x24/apps/$ICON_NAME ]; then
	rm $HICOLOR_DIR/24x24/apps/$ICON_NAME
	if [ $? -ne 0 ]; then
		echo "Error: could not remove $HICOLOR_DIR/24x24/apps/$ICON_NAME."
		#exit 1
	fi
fi
echo -ne "."

# Remove 16 icon
if [ -f $HICOLOR_DIR/16x16/apps/$ICON_NAME ]; then
	rm $HICOLOR_DIR/16x16/apps/$ICON_NAME
	if [ $? -ne 0 ]; then
		echo "Error: could not remove $HICOLOR_DIR/16x16/apps/$ICON_NAME."
		#exit 1
	fi
fi
echo -ne "."

echo "Done. $NAME successfully uninstalled."


