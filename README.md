# Gcode-SimulatorV2

## What is it?
It's the improved version of my [Gcode Simulator](https://github.com/XEDGit/Gcode-Simulator), so a full C Gcode file renderer 
which works in terminal or powershell rendering the model with characters

## What changed?
This time [hebekat](https://github.com/hebekat) helped me and we gained some basic C experience
from our @42college admission exam, but we decided to go crazy with features and decided to try
to render a rotating 3d model instead of the 2d top-down view of the building process as the V1

## Warnings
This project is abandoned (waiting for V3:pray:) and his final state is not completed, there's 
some memory management issues in particular while handling big Gcode files, yet these are 
managed by the operative system in all known cases.
The dimensions of the scene are not properly displaying if the model isn't cubic in it's extreme far from center points.
Multiple features are present but not working, but when rateo doesn't get modified the program works mostly as expected.
Windows version as expected is slower than Linux or Unix one, it's also not tested with latest
features

## Installation
### Dependencies
  * gcc

Just simply clone the repo or download the folder, then go to your terminal and type:
### Terminal:
    git clone git@github.com:XEDGit/Gcode-SimulatorV2.git
    cd Gcode-SimulatorV2
    make
    ./a.out
or
### Powershell:
    git clone git@github.com:XEDGit/Gcode-SimulatorV2.git
    cd Gcode-SimulatorV2
    gcc matrixManager.c readGcode.c helperFunctions.c -lm -Ofast
    .\a.exe
and you're ready to go!

## Arguments
The executable receives zero or two parameters, which are
 * rateo
 * source_file
where
### rateo
Is the part of the display proportion rateo:1, with latest feature add this feature broke so leave at 1, or go over if you want to have fun :)
### source_file
the file which you choose to display, files which are more than 50MB will probably be pretty slow to load. Insert the full path or the relative path from the executable folder

## Example:
    ./a.out 1 hrt.gcode
    ./a.out 2 /home/user/Gcode-SimulatorV2/hrt.gcode
    ./a.out 1 /home/user/Desktop/myfile.gcode
