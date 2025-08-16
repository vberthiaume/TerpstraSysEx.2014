# Lumatone Editor Development Guide

## Getting started

### JUCE
The Lumatone Editor primarily relies on the JUCE framework. Please visit the JUCE website if you need help setting that up. Version 1.0.X used JUCE 6, and 1.X in development is on JUCE 8, but we will need to move to get a license for the next release.

### CMake
This project initially used the Projucer for setup and some design. This was a bit limited to work with so JUCE's CMake integration was chosen as a replacement, which also has very nice support in some IDEs. Please visit the CMake website if you need help setting this up. For now, the .jucer project file is still updated if you prefer using that, however its build routines will not be updated as the CMake script is extended.

### Third-Party Libraries
In order to support the current firmware update routine, we also need to libssh2, which is pre-built and included in this repository for macOS and Windows. 

If you are building on Linux, you can install libssh2 with your package manager and CMake should be able to find it via pkg-config.

Both the Projucer and CMake project files should take care of installing these properly for development builds on macOS and Windows. They are accessed dynamically, so on macOS they must be signed and notarized for release, and on Windows they must be in the same folder as the binary. Using static linking on Windows is planned to be revisited.

### Installers
The third-party applications used to make installers are Packages for macOS and InnoSetup for Windows. These will be necessary if you want to make distribution packages.

### Fonts in v1.0.X
Lumatone Editor uses a licensed font, which weren't able to be uploaded to our public repository. This required some workarounds, particularly using a flag USE_LICENSED_FONTS. By default it would just be 0 and JUCE should choose default system fonts. 

Turning this on should be easy with the CMake setup, where you simply set the flag in your CMakeLists.txt to 1. 
With the Projucer project configuration, you need add the fonts to .jucer project, re-save the project, and build with the flag set to 1. 

This is going to be simplified in this private repository where we can store the licensed fonts.

## Building

### CMake
 - Create an environment variable called `juce_DIR` that defines a path to your `JUCE` folder (TODO: use "find_package" CMake command)
 - Make a copy of the CMakeLists.txt.template file without the ".template" suffix.
 - Optionally edit any property you need to, such as version number or build flags
 - Use your IDE to configure the project, or use `cmake -B build`
 - Use your IDE to setup your build configuration and build, or use `cmake --build build`
   - NOTE: you should be able to specify a config with `cmake --build build --config CONFIG` but it wasn't working for me


### Projucer
 - Build the Projucer (in the JUCE repo - extras/Projucer)
 - Open LumatoneEditor.jucer
 - Optionally edit any properties you need to
 - If building with licensed fonts:
   - Draw the "Fonts" folder into the "Sources" folder in Projucer
   - Go to project settings and set "USE_LICENSED_FONTS" to 1
 - Add your Exporter if it's not there already
 - Re-save the project
 - Select and launch your exporter/IDE
 - Build the project in your IDE

## Installers

### macOS
- Create a Release build and move it to `Releases/macOS/Latest`
- Run a codesign command on the entire app directory (TODO add script)
- Open up the Packages file `Installers/Packages/Lumatone Editor/Lumatone Editor.pkgproj`
- Optionally update the "version" property in the "Lumatone Editor" section under "Packages" column on the left
- Run the "Build" command
- Run a codesign command on the generated `Lumatone Editor.pkg` file
- Submit the package to Apple notarization (xcrun --notarize-app)
- Staple the successful receipt to the pkg file
- Zip the pkg file with our guide (TODO add to project) and License

### Windows
- Create a Release build with Visual Studio 2019 (or fix the path in InnoSetup!)
- Open up the InnoSetup file in `Installers/InnoSetup/LumatoneEditorWinInstallerScript.iss"
- Run the Build command
- Zip the generated installer exe file with our guide (TODO add to project) and License

## Project structure

TODO
