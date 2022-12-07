# ELFStudio
Static analysis tool for analyzing Linux ELF files.

This has the framework to build a database of malicious Linux functions that are comparable to Windows malicous functions.

## Installation Instructions

### Dependencies
The following packages are needed:
>sudo apt-get install binutils-dev  

>sudo apt-get install libelf-dev  

>sudo apt-get install libssl-dev  

>sudo apt-get install build-essential libgl1-mesa-dev  

### Static Build
The latest version of Qt, cmake, and the Qt Creator IDE can be installed at this link: https://www.qt.io/download-qt-installer.

After the installation wizard is finished, run Qt creator and open the CMakeLists.txt file. Build all project files then in the build folder created, enter the following command:
>./ELFStudio -lbfd
