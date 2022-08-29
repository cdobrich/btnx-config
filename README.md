btnx-config
===========

btnx-config & [btnx](https://github.com/cdobrich/btnx) are mouse button mapper tools for mouse hardware typically from 2000s to 2015 (for newer mice support, we recommend [Piper](https://github.com/libratbag/piper)). btnx-config is the configuration graphical user-interface (GUI) tool for btnx (Button Extension). It allows the user to send keyboard combinations or execute commands with mouse buttons. btnx-config provides mouse and button detection, and an easy way to configure the software's behavior.

IMPORTANT: btnx-config *REQUIRES* the other package btnx, which is the graphical configuration tool for btnx. See [btnx](https://github.com/cdobrich/btnx) for more details.

Modern Mice Support
----
btnx works best with older mice, typically between 2005 and 2015 (but may still work). For newer mice support, we recommend [Piper](https://github.com/libratbag/piper).


Screenshots
----
![Main window](https://raw.githubusercontent.com/cdobrich/btnx-config/master/doc/images/btnx-config-main-window.png)

![Button detection window](https://raw.githubusercontent.com/cdobrich/btnx-config/master/doc/images/btnx-config-button-detection-window.png)

![Button configuration window](https://raw.githubusercontent.com/cdobrich/btnx-config/master/doc/images/btnx-config-button-configuration-window.png)

Warning
----
To avoid possibly problems, Configuration Names should contain no space (and probably no special symbols). The use of underscores is fine. This used to not be a problem, but recently I've noticed (in Fedora 21) btnx-config barfs if you try to feed it a file that has spaces in the name. Will look into fixing this in the long term.

History
----
btnx and btnx-config was originally written by [Olli Salonen](https://launchpad.net/~daou). However he has ceased development of the code and one of the repositories is lost. Afraid the code might be totally lost if it is not hosted somewhere, I acquired his blessing to move the project here and maintain it. (Meaning this isn't a fork.) It is now being maintained (albeit limitedly at the moment), particularly for major bugs.

Licences
----
GNU GPL v2

Programming Languages
----
C

Building
=======

## Dependencies

To build on **Ubuntu** or **Debian**, you need the following dependencies:

* `build-essential`
* `libdaemon-dev`
* `libglade2-dev` For Manjaro, install libglade instead.
* `libgtk2.0-dev`

On most Ubuntu machines, these are available without needing additional sources being enabled on your apt-package respositories. To install these dependencies, run the following commands on the commandline:

`sudo apt install libdaemon-dev libglade2-dev libgtk2.0-dev`

To build on **Fedora** or **Redhat**, you need the following dependencies:

* `libdaemon-devel`
* `libglade2-devel`

To build on **Manjaro** you need the following dependencies:

* `libglade`

On Fedora, you may need more if you don't have GNU-Make or the C/C++ compilers (gcc/g++) installed.

To install these dependencies, run the following commands on the commandline:

`sudo dnf install libdaemon-devel libglade2-devel `

## Compiling

btnx and btnx-config follow the simple GNU-Make conventions for easy compiling, after you have the dependencies installed. Run these commands from commandline:

  `./configure`

Then run:

  `make`

## Installing

btnx and btnx-config follow the simple GNU-Make conventions for installing, after you have successfully run the compiling step. Run this commands from commandline:

`sudo make install`

## Uninstalling

btnx and btnx-config follow the simple GNU-Make conventions for uninstalling:

`sudo make uninstall`

Using btnx-config
=======

Assuming you have successfully compiled and installed btnx and btnx-config, you can now configure your mouse-buttons.

## Starting btnx-config

The latest version of BTNX-Config should use the `pkexec` command. The KDE menu should full support users clicking on it, which will prompt for a password for elevated privileges.

If for some reason your system does not support 'pkexec', instead you should invoke the program from a commandline window using the more common `sudo` command:

`sudo btnx-config`

## Troubleshooting

If receive the following error message when trying to use the KDE application menu button to invoke the btnx-config graphical program:

`Could not find the program 'gksu'`

This means you are running an older version of BTNX-Config and the helper program 'gksu' is not installed on your system. Please update to the newer version of BTNX-Config which supports modern Linux `pkexec` commands instead.

Debian and Ubuntu have both decided to move away from using the `gksu`, `gksudo`, `kdesu`, and `kdesudo` programs for security concerns. You can try to install this program IF it is available for your Linux system. But these programs are being gradually phased out, in favor of using the simple `sudo` command. Instead you should simply invoke the program from a commandline window using the more common `sudo` command:

`sudo btnx-config`
