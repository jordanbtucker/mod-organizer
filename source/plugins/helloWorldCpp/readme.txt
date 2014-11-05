This is a minimal sample of a plugin written in c++.

The code should be more or less self explanatory but a few words on how to use
this package:

In QtCreator, open the helloworld.pro project.
On the first start qt creator will ask you to choose a directory to place output files.
Don't set this directly to your ModOrganizer installation.

Now the project is ready to build, there are no dependencies apart from Qt (which you probably
already installed when you're running qt creator)

There are 6 files:
- readme.txt                     - the file you're reading right now
- helloworld.h, helloworld.cpp   - source code
- helloworld.pro                 - project file
- helloworld.json                - empty. This file is required for plugins in Qt5. Right now this is not needed
- uibase.lib                     - This is a stub library to interface with uibase.dll

The lib file is there for your convenience so you don't have to compile it yourself.

After you have compiled your the plugin copy the helloworld.dll from the directory you set up
as output directory before to your ModOrganizer-installation (plugins subdirectory) and
(re-)start MO. You should now have a new "Hello World" entry in your Tools menu.