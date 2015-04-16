btnx-config
===========

btnx-config is a configuration tool for btnx (Button Extension). It allows the user to send keyboard combinations or execute commands with mouse buttons. btnx-config provides mouse and button detection, and an easy way to configure btnx's behavior. See https://github.com/cdobrich/btnx/ for btnx.

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
