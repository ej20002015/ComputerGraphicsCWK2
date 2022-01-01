# Computer Graphics Coursework 2

## Requirements

- C++17 compiler
- Make or Visual Studio 2019 (depending on the platform being used)

## Build Instructions

### Windows

- Generate VS2019 project files by performing the following command: \
```call scripts/GenerateWindowsFiles.bat```
- Open the CWK2.sln VS2019 solution file
- Build all the projects

### Linux
- Generate CMake project files by performing the following command: \
```sh scripts/GenerateLinuxFiles.bash```
- Build the projects by running the following command: \
```make```

## Run instructions

- Executable can be found in the directories below the application/bin/ directory
- **Run the excecutable from the application directory since that's the working directory all the filepaths in the code are rooted in**
- It's recommended to use the Release build as this will be much quicker (both GLM and STB_image run much faster in release)

### Windows run
- Run from within Visual Studio
- *You could run the executable directly but the current working directory will need to be set properly before doing this (could use a shortcut)*

### Linux run
- from the project root folder run: \
```cd application```
- and depending on the type of build that was done, either: \
```./bin/Debug-linux-x86_64/application/application``` \
or \
```./bin/Release-linux-x86_64/application/application```