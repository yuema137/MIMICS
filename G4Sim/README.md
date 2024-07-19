## GEANT4 Simulations for Medical Imaging Systems

### Prerequisite
- Make sure you already have C++ complier installed , either gcc or g++.
- Also, you need [CMake](https://cmake.org/) installed.
- Install `GEANT4 11.2` or above following the [Installtion Guide](https://geant4-userdoc.web.cern.ch/UsersGuides/InstallationGuide/html/index.html)

### Usage on Unix Platforms (Linux/MaxOS)

#### Run the Examples directly
- Go into a project folder. For example: `cd SPECT`
- Make a folder to store the built files and executables: `mkdir build`, then go into the build folder: `cd build`
- Inside the build folder, generate the CMake files with `cmake ..` . Those files are the instructions and configurations telling the complier how to do the compling.
- Complie by running the command `make`
- If the compling is done successfully, you will see an executable named `run` in your build folder. 
- Run the command `./run --help` to see which arguments are needed for the executable, and what are the meaning of each of them.
