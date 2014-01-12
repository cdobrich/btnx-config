
 /* 
  * Copyright (C) 2007  Olli Salonen <oasalonen@gmail.com>
  * see btnx-config.c for detailed license information
  */

/*
 * This file provides support for i18n.
 * It contains many, but not all strings that have translation support.
 */

#ifndef TEXT_H_
#define TEXT_H_

#include <libintl.h>
#include <locale.h>

#define _(String) gettext(String)
#define gettext_noop(String) String
#define N_(String) gettext_noop(String)

#define TEXT_ERROR					_("Error")
#define TEXT_WARNING				_("Warning")
#define TEXT_YES					_("Yes")
#define TEXT_NO						_("No")
#define TEXT_DETECTED_DEVICES		_("Detected devices:")
#define TEXT_DETECTED_BUTTONS		_("Detected buttons:")
/* TRANSLATORS: this text shows the user the number of input device handlers
 * that have been detected by the program. A handler is a file (for
 * example, /dev/input/event1) that can be read. The information read from
 * that file is data coming from an input device (mouse, keyboard, etc.).*/
#define TEXT_DETECTION_STATUS		_("Detected handlers: %d")
/* TRANSLATORS: Vendor ID is an identification number that a device has. Every
 * manufacturer has their own "Vendor ID" that they can use for their devices */
#define TEXT_VENDOR_ID				_("Vendor ID:")
/* TRANSLATORS: Product ID is an identification number that a device has.
 * Manufacturers can designate unique Product IDs for different products. */
#define TEXT_PRODUCT_ID				_("Product ID:")
#define TEXT_INSTRUCTIONS			_("<b>Instructions</b>")
/* TRANSLATORS: Make this short. Abbreviate if necessary. It refers to a mouse
 * button. A number is appended to it (ex. Button1, Button2...). */
#define TEXT_BUTTON					_("Button")
/* TRANSLATORS: this is a status text. It informs the user that the program is
 * in a detection state. */
#define TEXT_DETECTING				_("Detecting...")
/* TRANSLATORS: Button refers to a mouse button */
#define TEXT_ALREADY_DETECTED		_("Button already detected. Try another one.")
/* TRANSLATORS: Button refers to a mouse button, and it has successfully been
 * detected. */
#define TEXT_DETECTED				_("Button detected!")
/* TRANSLATORS: this is the label of a button which starts the detection
 * process when pressed. Try to keep it short. */
#define TEXT_START_BUTTON_DETECTION _("Press to start button detection")
/* TRANSLATORS: this is the label of a button which stops the detection
 * process when pressed. Try to keep it short. */
#define TEXT_STOP_BUTTON_DETECTION 	_("Press to stop button detection")
/* TRANSLATORS: this is the label of a button which starts the detection
 * process when pressed. Try to keep it short. */
#define TEXT_START_MOUSE_DETECTION	_("Press to start mouse detection")
/* TRANSLATORS: this is the label of a button which stops the detection
 * process when pressed. Try to keep it short. */
#define TEXT_STOP_MOUSE_DETECTION	_("Press to stop mouse detection")
/* TRANSLATORS: this is a status text, informing the user that the program is
 * waiting for the user to start the detection process */
#define TEXT_WAIT_DETECTION			_("waiting for the start of detection")
/* TRANSLATORS: a modifier key is a keyboard key that can be combined with
 * other buttons. Examples: Ctrl, Shift, Alt. */
#define TEXT_MODIFIER				_("Modifier key")
#define TEXT_HELP_TOOLTIP			_("Help documentation")
#define TEXT_CANCEL_DETECTION		_("Cancel the detection process")
/* TRANSLATORS: this text informs the user that they tried to input some value
 * that is incorrect. Example: using letters in a number */
#define TEXT_INVALID_VALUE			_("Invalid value")
/* TRANSLATORS: this is the title of a table column. Keep it short. Abbreviate
 * if necessary. The user doesn't necessarily need to understand what "rawcode"
 * means. It is a 32-bit integer value in hexadecimal form, intercepted from an
 * input stream. It is called "raw" because it has not been modified to make
 * it more readable or understandable. */
#define TEXT_RAWCODE				_("Rawcode")
/* TRANSLATORS: this refers to a combination of keyboard keys. Example:
 * Ctrl+Alt+B. It is an option chosen by the user, meaning that the program
 * will send that type of an event. Try to keep it short (under 30 characters)
 * */
#define EVTYPE_KEY_STR				_("Key combination")

#define EVTYPE_CONFIG_STR			_("Configuration switch")
/* TRANSLATORS: this refers to the execution of a shell command. Example:
 * "nano /etc/X11/xorg.conf".
 * It is an option chosen by the user, meaning that the program
 * will send that type of an event. Try to keep it short (under 30 characters)
 * */
#define EVTYPE_COMMAND_STR			_("Command execution")
/* TRANSLATORS: this refers to an action that causes the next program
 * configuration to be loaded. */
#define CONFIG_SWITCH_NEXT_STR		_("Next config.")
/* TRANSLATORS: this refers to an action that causes the previous program
 * configuration to be loaded. */
#define CONFIG_SWITCH_PREV_STR		_("Previous config.")
/* TRANSLATORS: this refers to an action that causes a specific configuration
 * to be loaded. It "switches" to another configuration */
#define CONFIG_SWITCH_TO_STR		_("Switch to")
/* TRANSLATORS: this is a label of a tab. Try to keep it short. */
#define TEXT_BUTTONS				_("Buttons")
/* TRANSLATORS: refers to advanced settings, something a novice user should
 * not change */
#define TEXT_ADVANCED				_("Advanced")
#define TEXT_CONFIGURATIONS			_("Configurations")
/* TRANSLATORS: do not translate btnx. It is the name of a program. */
#define TEXT_RESTART_BTNX			_("Restart btnx")
/* TRANSLATORS: this refers to a setting a user can set, which forces
 * the program to send a mouse or keyboard button release signal right after
 * a pressed signal is sent. This means the button cannot be held down. */
#define TEXT_FORCE_RELEASE			_("Force immediate button release")

/* TRANSLATORS: this is a label, try to keep it short. revoco refers to a
 * program, do not translate it. */
#define TEXT_REVOCO_SETTINGS		_("<b>revoco settings</b>")
/* TRANSLATORS: do not translate revoco. It is the name of a program. The
 * text refers to enabling the functionality offered by this program. */
#define TEXT_REVOCO_ENABLE			_("Enable revoco")
/* TRANSLATORS: free-scroll mode refers to MX Revolution mouse's wheel mode,
 * when it scrolls without friction. */
#define TEXT_REVOCO_FREE_MODE		_("Free-scroll mode")
/* TRANSLATORS: click-to-click mode refers to MX Revolution mouse's wheel mode,
 * when it scrolls like normal mouse wheels. */
#define TEXT_REVOCO_CLICK_MODE		_("Click-to-click mode")
#define TEXT_REVOCO_MANUAL_MODE		_("Manual mode")
#define TEXT_REVOCO_AUTO_MODE		_("Automatic mode")
#define TEXT_REVOCO_BUTTON_NUMBER	_("Button number:")
/* TRANSLATORS: this refers to the speed that a mouse wheel is scrolled up. */
#define TEXT_REVOCO_UP_SCROLL		_("Up scroll speed:")
/* TRANSLATORS: this refers to the speed that a mouse wheel is scrolled down. */
#define TEXT_REVOCO_DOWN_SCROLL		_("Down scroll speed:")
/* TRANSLATORS: do not translate MX Revolution or revoco. */
#define TEXT_REVOCO_STATUS_DISABLED \
_("<b>An MX Revolution mouse has not been detected. revoco is disabled.</b>")
/* TRANSLATORS: do not translate MX Revolution or revoco. */
#define TEXT_REVOCO_STATUS_ENABLED \
_("<b>MX Revolution mouse detected. You may enable revoco.</b>")
/* TRANSLATORS: do not translate revoco, btnx, or MX Revolution. */
#define TEXT_REVOCO_INFO \
_("revoco is a program which allows changing the behavior of MX Revolution's \
mouse wheel.\n\
It has been integrated into btnx with the blessing of its author.\n\
\n\
The settings below will take effect when btnx is restarted.")

#define TEXT_BACKUP_ERROR \
_("Error: Creating a configuration backup failed.")

#define TEXT_RESTORE_ERROR \
_("Error: Restoring configuration backup failed")

/* TRANSLATORS: do not translate "btnx". It is the name of a program */
#define TEXT_BTNX_RESTART_ERROR		_("Error: could not restart btnx")
/* TRANSLATORS: do not translate "btnx". It is the name of a program. Leave
 * the command as it is. */
#define TEXT_BTNX_UNSPECIFIED_ERROR \
_("Failed to execute command: \"" INIT_SCRIPT_FILE " restart\". Make sure btnx \
is installed correctly.")
#define TEXT_BTNX_NO_CONFIG_ERROR \
_("btnx failed to start. No suitable configuration was found. \
Make sure to fully configure btnx before starting it.")
#define TEXT_BTNX_BAD_CONFIG_ERROR \
_("btnx failed to start. The configuration file was corrupt or invalid.")
#define TEXT_BTNX_OPEN_HANDLER_ERROR \
_("btnx failed to start. It could not locate a suitable input event handler \
for the configured device.")
#define TEXT_BTNX_OPEN_UINPUT_ERROR \
_("btnx failed to start. uinput handlers could not be opened. Make sure \
your system has uinput kernel modules.")

/* TRANSLATORS: do not edit the <b> formatting tags */
#define TEXT_ADVANCED_SETTINGS		_("<b>Advanced settings</b>")
/* TRANSLATORS: do not edit the <b> formatting tags.
 * Hot-plugging refers to device detection and configuration when it is
 * plugged into the system while the system is running. */
#define TEXT_HOTPLUGGING			_("<b>Hot-plugging</b>")
/* TRANSLATORS:
 * Hot-plugging refers to device detection and configuration when it is
 * plugged into the system while the system is running. */
#define TEXT_ENABLE_HOTPLUGGING		_("Enable hot-plugging")
/* TRANSLATORS: do not translate WAIT_FOR_SYSFS or udev */
#define TEXT_WAIT_FOR_SYSFS			_("WAIT_FOR_SYSFS udev option")

/* TRANSLATORS: this describes to the user what a modifier key is. The modifier
 * key is a key such as shift, alt, ctrl. The keycode is a keyboard or button
 * name. Example: KEY_PAGEUP for a "Page Up" key. The main key refers to the
 * keycode (the key that is sent with a modifier key. For example, in
 * Ctrl+Alt+B the main key would be "B". */
#define TEXT_MODIFIER_TOOLTIP \
_("A modifier key for the keycode. This key is sent with the main key to \
modify its behavior. Examples: shift, alt, ctrl.")
/* TRANSLATORS: do not translate btnx. It is the name of a program. */
#define TEXT_EVTYPE_TOOLTIP \
_("Select the type of event sent by btnx when pressing this button. You can \
either send a key combination or execute a command.")

/* TRANSLATORS: button refers to a mouse button. */
#define TEXT_TITLE_BTND_FORK_FAIL \
_("Error: button detection start error")
/* TRANSLATORS: button refers to a mouse button. */
#define TEXT_BODY_BTND_FORK_FAIL \
_("Button detection startup failed. Try again. If the problem persists, \
file a bug report.")

#define TEXT_HELP_BTND_RUN	\
_("Press a single button on your mouse until the detection progress bar is \
full.\n\n\
Press Cancel to discard all changes.\n")
/* TRANSLATORS: button refers to a mouse button. "Add" is the label of a button.
 * The "Add" label is automatically translated by the system and your
 * translation should be the same. If you don't know what name your system
 * uses for the "Add" button, try running some programs that use it. */
#define TEXT_HELP_BTND_ADD \
_("A button has been detected. Give it a name that you will recognize in the \
future.\n\
Then press Add to add it to the configuration.")

#define TEXT_TITLE_DIALOG_BTND _("Button detection problem")
/* TRANSLATORS: do not translate btnx. It is the name of a program. Cancel
 * is the name of a button. Make sure to translate it to the same name that
 * your system uses on all "Cancel" buttons. Rawcode is used elsewhere in the
 * program. Make sure to use the same translation here.
 * The user doesn't necessarily need to understand what "rawcode"
 * means. It is a 32-bit integer value in hexadecimal form, intercepted from an
 * input stream. It is called "raw" because it has not been modified to make
 * it more readable or understandable. */
#define TEXT_BODY_DIALOG_BTND \
_("A problem has occurred during button detection.\n\
Multiple mouse buttons have been detected. There are three possible causes:\n\
\n\
1. You pressed different buttons too many times. Press Cancel and try again.\n\
2. You pressed a previously detected button too many times. Press Cancel and \
try again\n\
3. Your mouse button sends more than one event when pressing that button. \
btnx cannot handle more than one event during a button press. You must choose \
one rawcode below to associate with that button.\n\
(Hint: choose the one with the most clicks. If there are multiple rawcodes \
with the same number of clicks, try one and see if it works. If it doesn't, \
try another one next time.")

/* TRANSLATORS: keycode is used elsewhere in the program. Use the same
 * translation here. The keycode is a keyboard or button
 * name. Example: KEY_PAGEUP for a "Page Up" key. */
#define TEXT_TITLE_EVENT_FILE_ERROR	_("Error: no keycode file")
/* TRANSLATORS: Do not translate btnx-config
 * or btnx, they are the names of programs. */
#define TEXT_BODY_EVENT_FILE_ERROR \
_("btnx-config needs the file /etc/btnx/events to run. Make sure you have \
installed btnx first, and that the file exists.")

/* TRANSLATORS: glade is the name of a program and a file extension type.
 * Do not translate it. */
#define TEXT_TITLE_GLADE_FILE_ERROR _("Error: no glade file")
/* TRANSLATORS: btnx-config is the name of a program. Do not translate it */
#define TEXT_BODY_GLADE_FILE_ERROR \
_("btnx-config could not find the necessary btnx-config.glade file. \
This file is necessary for btnx-config to run. Try reinstalling the program \
to fix the problem.")

/* TRANSLATORS: device ID is an identification number for a device, in this
 * case, an input device. The text informs the user that the program has
 * detected more than one different device ID, which is a problem. */
#define TEXT_MOUSED_ID_CONFLICT_TITLE _("Device ID conflict detected")
/* TRANSLATORS: device ID is an identification number for a device. btnx is a
 * program name. Do not translate it. The phrase "btnx will not handle that
 * button at all" means that the program will not do anything when that button
 * is pressed. It will ignore the button. */
#define TEXT_MOUSED_ID_CONFLICT_BODY \
_("Multiple device IDs were detected during mouse detection. btnx cannot \
simultaneously handle multiple devices. \n\n\
If you accidentally did something \
with another input device, try detecting again.\n\n\
If this is a reoccurring problem, \
it's possible that your device will not work with btnx. If this happens when \
pressing some specific button, do not press it: this means btnx will not handle \
that button at all.")

/* TRANSLATORS: Cancel, Forward, and Undo are button names. Make sure these
 * names are translated exactly as the buttons are in your system language.
 * If you are unsure, try to find programs that use these buttons. */
#define TEXT_HELP_MOUSED_START \
_("OK, move your mouse now and press each button on it.\n\n\
You can stop the detection by pressing the button above.\n\
Press Cancel to discard all changes.\n\
Press Forward to proceed to the button detection phase.\n\n\
If you think you did something wrong (like pressed a \n\
keyboard button), press Undo to restart the test.")

#define TEXT_HELP_MOUSED_INIT \
_("Press the button above to start the detection process. \
Once the test has started, DO NOT move your mouse or press its \
buttons until told to do so.")

#define TEXT_HELP_MOUSED_WAIT \
_("DO NOT move your mouse or\n\
press its buttons for 5 seconds.")

/* TRANSLATORS: Cancel and OK are button names. Make sure these
 * names are translated exactly as the buttons are in your system language.
 * If you are unsure, try to find programs that use these buttons. */
#define TEXT_HELP_BTND_INIT	\
_("Press the button above to start the button detection. \n\n \
Press Cancel to discard all changes.\n \
Press OK to to finish the button detection.")


#endif /*TEXT_H_*/
