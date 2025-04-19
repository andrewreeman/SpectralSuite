# SpectralSuite

To build ensure the 'public.sdk', 'plugininterfaces' and 'vstgui' 
folders from the 
Steinburg VST SDK are in the 'shared' folder

On Mac set an alias in ~/.zshrc for 'projucer' pointing to the projucer executable.
E.g: `alias projucer=~/Documents/Personal/JUCE/Projucer.app/Contents/MacOS/Projucer`


### Making a new plugin
The boilerplate code for a new plugin is handled for you with a python script. This uses python 3.12 or higher.
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

### Develop plugins
The plugins are in a CMake project so a single project containing all plugins
can be created. `mkdir build && cd build && cmake ..`. Use `-G` and select a
[generator](https://cmake.org/cmake/help/latest/variable/CMAKE_GENERATOR.html) for a particular generator. 

### Building plugins for release 
This section was written before the projects were migrated to CMake so can most
likely be rewritten. Building a release for all plugins should be as simple as
`cd build && cmake --build . --config Release`
Build must be performed on MacOS and Windows.

#### MacOS
##### Prerequisites
Matching the version numbers may not be required. 
Ruby (2.7.4) to run the build scripts
XCode (15.4) 
MacOS intel (14.6.1)
Projucer (7)
[Packages](http://s.sudre.free.fr/Software/Packages/about.html) (1.2.9) to build the installer.

##### Steps
Update the project version in `SpectralSuiteBuild` `Base.jucer` file. 
In the preprocessor definitions increment the `VersionCode`.
Update `CHANGES.md` with the latest changes.

Then on the terminal naviate the `SpectralSuiteBuild` and run:
`ruby ssutil.rb`

This should update all projucer files with the version number and code. 
Start a build using xcode command line tools.
Copy the AU and VST3 files to a `release` folder in top level repository dir. 
Build a package that can be used for installing. This will be located in
`SpectralSuiteBuild/package/MacOS-SpectralSuite-vX.X.X.pkg`

#### Windows 
##### Prerequisites
Visual Studio 2022
Powershell

##### Steps
Open Developer Powershell
Navigate to `SpectralSuiteBuild`.
Then run `ssutil.ps1`

#### Linux
##### Prerequisites
Docker

Can be run on any system with Docker. Tested on Mac.
On terminal, navigate to `SpectralSuiteBuild`.
Then run `buildLinux.sh`
The release files will then be in top level `release/linux-x86_64` folder

