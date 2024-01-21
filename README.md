# SpectralSuite

To build ensure the 'public.sdk', 'plugininterfaces' and 'vstgui' 
folders from the 
Steinburg VST SDK are in the 'shared' folder

On Mac set an alias in ~/.zshrc for 'projucer' pointing to the projucer executable.
E.g: `alias projucer=~/Documents/Personal/JUCE/Projucer.app/Contents/MacOS/Projucer`


### Making a new plugin
The boilerplate code for a new plugin is handled for you with a python script.
```bash
--> py makeSpectral.py -h
usage: makeSpectral.py [-h] -p PATH -n NAME [-d DESCRIPTION] [-v {True,False}]

Automate SpectralSuite boilerplate for new plugin.

options:
  -h, --help            show this help message and exit
  -p PATH, --path PATH  The relative or absolute path to the directory containing new plugin
  -n NAME, --name NAME  The name of the new plugin
  -d DESCRIPTION, --description DESCRIPTION
                        The description of the new plugin
  -v {True,False}, --verbose {True,False}
```
```bash
--> py makeSpectral.py -p NewThing/ -n "NEWNAME" -d "NEWDESC" -v True
Populated NewThing/ with boilerplate files
Pushing provided values into <__main__.UiContainer_Cpp object at 0x109af54f0> and renaming file
/Users/jonathanwatson/C++/SpectralSuite/NewThing/UiContainer.cpp
Pushing provided values into <__main__.AudioPluggin_Cpp object at 0x109af5640> and renaming file
/Users/jonathanwatson/C++/SpectralSuite/NewThing/BaseAudioPlugin.cpp
Pushing provided values into <__main__.Parameters_H object at 0x109a56510> and renaming file
/Users/jonathanwatson/C++/SpectralSuite/NewThing/BaseParameters.h
Pushing provided values into <__main__.AudioPluggin_H object at 0x109af5610> and renaming file
/Users/jonathanwatson/C++/SpectralSuite/NewThing/BaseAudioPlugin.h
Pushing provided values into <__main__.Base object at 0x109af54c0> and renaming file
/Users/jonathanwatson/C++/SpectralSuite/NewThing/Base.jucer
Pushing provided values into <__main__.UiContainer_H object at 0x109af5490> and renaming file
/Users/jonathanwatson/C++/SpectralSuite/NewThing/UiContainer.h
Pushing provided values into <__main__.Parameters_Cpp object at 0x109af55b0> and renaming file
/Users/jonathanwatson/C++/SpectralSuite/NewThing/BaseParameters.cpp
Pushing provided values into <__main__.Processor_H object at 0x109a88740> and renaming file
/Users/jonathanwatson/C++/SpectralSuite/NewThing/BaseProcessor.h
Pushing provided values into <__main__.Processor_Cpp object at 0x109af5580> and renaming file
/Users/jonathanwatson/C++/SpectralSuite/NewThing/BaseProcessor.cpp
```
```bash
--> ls NewThing/
NEWNAME.jucer		NEWNAMEParameters.cpp	NEWNAMEProcessor.h
NEWNAMEAudioPlugin.cpp	NEWNAMEParameters.h	UiContainer.cpp
NEWNAMEAudioPlugin.h	NEWNAMEProcessor.cpp	UiContainer.h
```