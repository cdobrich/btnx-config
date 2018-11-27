btnx-config
===========

btnx-config is a configuration tool for btnx (Button Extension). It allows the user to send keyboard combinations or execute commands with mouse buttons. btnx-config provides mouse and button detection, and an easy way to configure btnx's behavior. See https://github.com/cdobrich/btnx/ for btnx.

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

## Depenencies

To build on **Ubuntu** or **Debian**, you need the following dependencies:

* `build-essential`
* `libdaemon-dev`
* `libglade2-dev`
* `libgtk2.0-dev`

On most Ubuntu machines, these are available without needing additional sources being enabled on your apt-package respositories. To install these dependencies, run the following commands on the commandline:

`sudo apt-get install libdaemon-dev libglade2-dev libgtk2.0-dev`

To build on **Fedora** or **Redhat**, you need the following dependencies:

* `libdaemon-devel`
* `libglade2-devel`

On Fedora, you may need more if you don't have GNU-Make or the C/C++ compilers (gcc/g++) installed.

To install these dependencies, run the following commands on the commandline:

`sudo dnf install libdaemon-devel libglade2-devel `

## Compiling

btnx and btnx-config follow the simple GNU-Make conventions for easy compiling, after you have the dependencies installed. Run these commands from commandline:

`./configure
make`

## Installing

btnx and btnx-config follow the simple GNU-Make conventions for installing, after you have successfully run the compiling step. Run this commands from commandline:

`sudo make install`

## Uninstalling

btnx and btnx-config follow the simple GNU-Make conventions for uninstalling:

`sudo make install`

Using btnx-config
=======

Assuming you have successfully compiled and installed btnx and btnx-config, you can now configure your mouse-buttons.

## Starting btnx-config

Older versions of Ubuntu can use the KDE application menu button to invoke the btnx-config graphical program. It requires the program `gksu`. However Debian and Ubuntu have both decided to move away from using the `gksu`, `gksudo`, `kdesu`, and `kdesudo` programs for security concerns. You can try to install `gksu` program IF it is available for your Linux system. But these programs are being gradually phased out, in favor of using the simple `sudo` command. Instead you should simply invoke the program from a commandline window using the more common `sudo` command:

`sudo btnx-config`

When the older versions of Ubuntu Linux 16.04 LTS is no longer supported by April 2021, the KDE menu button generation will be removed.

## Troubleshooting

If receive the following error message when trying to use the KDE application menu button to invoke the btnx-config graphical program:

`Could not find the program 'gksu'`

This means the helper program 'gksu' is not installed on your system. Debian and Ubuntu have both decided to move away from using the `gksu`, `gksudo`, `kdesu`, and `kdesudo` programs for security concerns. You can try to install this program IF it is available for your Linux system. But these programs are being gradually phased out, in favor of using the simple `sudo` command. Instead you should simply invoke the program from a commandline window using the more common `sudo` command:

`sudo btnx-config`
