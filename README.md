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
--> ls NewThing/
NEWNAME.jucer		NEWNAMEParameters.cpp	NEWNAMEProcessor.h
NEWNAMEAudioPlugin.cpp	NEWNAMEParameters.h	UiContainer.cpp
NEWNAMEAudioPlugin.h	NEWNAMEProcessor.cpp	UiContainer.h
```