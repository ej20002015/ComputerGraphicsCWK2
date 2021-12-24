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
- It's recommended to use the Release build as this will be much quicker (both GLM and STB_image run much faster in release)
- Run the excecutable from the application directory since that's the working directory all the filepaths in the code are rooted in