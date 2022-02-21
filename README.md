# Gcode-SimulatorV2

## What is it?
It's the improved version of my [Gcode Simulator](https://github.com/XEDGit/Gcode-Simulator),
so a full C Gcode file renderer which works in terminal or powershell rendering the model
with characters

## What changed?
This time [hebekat](https://github.com/hebekat) helped me and we gained some basic C experience
from our @42college admission exam, but we decided to go crazy with features and decided to try
to render a rotating 3d model instead of the 2d top-down view of the building process as the V1

## Warnings
This project is abandoned (waiting for V3:pray:) and his final state is not completed, there's 
some memory management issues in particular while handling big Gcode files, yet these are 
managed by the operative system in all cases
Windows version as expected is slower than Linux or Unix one, it's also not tested with latest
features

## Installation
### Dependencies
  * gcc

Just simply clone the repo or download the folder, then go to your terminal and type:
### Terminal:
    cd /.../path_to_the_repo/
    make
    ./a.out
or
### Powershell:
    cd \...\path_to_the_repo\
    gcc matrixManager.c readGcode.c helperFunctions.c -lm -Ofast
    .\a.exe
and you're ready to go!
