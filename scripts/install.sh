#!/bin/bash

#################################################
# btnx-config install script			#
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
ICON_DIR=data/icons
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

function prompt_yn {
	while [ 1 ]; do
	  echo -n " (y or n)? : "
	  read CONFIRM
	  case $CONFIRM in
	    y|Y|YES|yes|Yes) return 1 ;;
	    n|N|no|NO|No) return 0 ;;
	    *) echo "Invalid option"
	  esac
	done
}


echo -ne "Installing..."


# Check for config dir. Create if nonexistent.
if [ -d $CONFIG_DIR  ]; then
	echo -ne "."
else
	#echo -ne "Making config dir... "
	mkdir $CONFIG_DIR
	if [ $? -ne 0 ]; then
		echo "Error: could not make directory: $CONFIG_DIR. \rAre you root?"
		exit 1
	fi
	echo -ne "."
fi

# Check for data dir. Create if nonexistent.
if [ -d $DEST_DATA_DIR  ]; then
	echo -ne "."
else
	#echo -ne "Making data dir... "
	mkdir $DEST_DATA_DIR
	if [ $? -ne 0 ]; then
		echo "Error: could not make directory: $DEST_DATA_DIR.?"
		exit 1
	fi
	echo -ne "."
fi

# Copy the binary file to the binary directory.
#echo -ne "Installing binary files... "
cp $NAME $BIN_DIR
if [ $? -ne 0 ]; then
	echo "Error: could not copy $NAME to $BIN_DIR. Did you run make yet?"
	exit 1
fi
echo -ne "."

# Make sure only root has execution privs for program
chown root $PROGRAM
chmod 0744 $PROGRAM

echo -ne "."

# Copy glade file.
cp $GLADE_FILE $DEST_DATA_DIR
if [ $? -ne 0 ]; then
	echo "Error: could not copy $GLADE_FILE to $DEST_DATA_DIR."
	exit 1
fi
echo -ne "."

# Copy desktop file
cp $DATA_DIR/$DESKTOP_FILE $DESKTOP_DIR
if [ $? -ne 0 ]; then
	echo "Error: could not copy $DATA_DIR/$DESKTOP_FILE to $DESKTOP_DIR."
	exit 1
fi
echo -ne "."

cd $ICON_DIR

# Copy 72x72 png to pixmap dir
cp $ICON_72 $PIXMAP_DIR/$ICON_NAME
if [ $? -ne 0 ]; then
	echo "Error: could not copy $ICON_72 to $PIXMAP_DIR."
	#exit 1
fi
echo -ne "."

# Copy 128x128 png to hicolor theme
cp $ICON_128 $HICOLOR_DIR/128x128/apps/$ICON_NAME
if [ $? -ne 0 ]; then
	echo "Error: could not copy $ICON_128 to $HICOLOR_DIR."
	#exit 1
fi
echo -ne "."

# Copy 72x72 png to hicolor theme
cp $ICON_72 $HICOLOR_DIR/72x72/apps/$ICON_NAME
if [ $? -ne 0 ]; then
	echo "Error: could not copy $ICON_72 to $HICOLOR_DIR."
	#exit 1
fi
echo -ne "."

# Copy 48x48 png to hicolor theme
cp $ICON_48 $HICOLOR_DIR/48x48/apps/$ICON_NAME
if [ $? -ne 0 ]; then
	echo "Error: could not copy $ICON_48 to $HICOLOR_DIR."
	#exit 1
fi
echo -ne "."

# Copy 32x32 png to hicolor theme
cp $ICON_32 $HICOLOR_DIR/32x32/apps/$ICON_NAME
if [ $? -ne 0 ]; then
	echo "Error: could not copy $ICON_32 to $HICOLOR_DIR."
	#exit 1
fi
echo -ne "."

# Copy 24x24 png to hicolor theme
cp $ICON_24 $HICOLOR_DIR/24x24/apps/$ICON_NAME
if [ $? -ne 0 ]; then
	echo "Error: could not copy $ICON_24 to $HICOLOR_DIR."
	#exit 1
fi
echo -ne "."

# Copy 16x16 png to hicolor theme
cp $ICON_16 $HICOLOR_DIR/16x16/apps/$ICON_NAME
if [ $? -ne 0 ]; then
	echo "Error: could not copy $ICON_16 to $HICOLOR_DIR."
	#exit 1
fi
echo -ne "."

cd -

# Copy locale dir.
cp -r $LOCALE_DIR $DEST_DATA_DIR
if [ $? -ne 0 ]; then
	echo "Error: could not copy $LOCALE_DIR to $DEST_DATA_DIR."
	exit 1
fi
echo -ne "."

echo "."


echo "$NAME successfully installed."



