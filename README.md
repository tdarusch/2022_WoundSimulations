# Installation & Setup Instructions (WINDOWS)

[Chaste with Docker](https://github.com/Chaste/chaste-docker#mounting-host-directories)

[Chaste getting started](https://chaste.cs.ox.ac.uk/trac/wiki/GettingStarted)

### From [chaste.cs.ox.ac.uk:](https://chaste.cs.ox.ac.uk/trac/wiki/GettingStarted)
> Chaste is primarily developed on ​Ubuntu Linux (recommended), and is regularly tested on all Long Term Support (LTS) versions of Ubuntu, but should also work on other Linux/Unix systems.
> 
> Chaste is supported and tested on Mac OS X.
> 
> While there is partial/experimental support for Windows, it is not currently recommended to use Chaste on Windows. ***The recommended route for unsupported systems is to install Chaste via Docker or virtual machine running Ubuntu Linux.***
### This guide will walk you through running this project on a windows machine using Chaste, Docker, and WSL2 (No VM).
***
### REQUIRED SOFTWARE (DOWNLOAD BEFORE PROCEEDING)
- [Docker](https://www.docker.com/products/docker-desktop/)
- [VSCode](https://code.visualstudio.com/download)
- [Ubuntu Terminal](https://apps.microsoft.com/store/detail/ubuntu/9PDXGNCFSCZV)
***
### Installing WSL2
1. Open Command Prompt as an admin and run `wsl --install`
2. To update the WSL2 kernel, run `wsl --update`
***
### Setting up Ubuntu Terminal
As of ***01/30/2023*** the linked Ubuntu Terminal is running ***Ubuntu 22.04.1 LTS***.

The Ubuntu terminal will launch and install both WSL 2 (if not already installed) and the Ubuntu 22.04. To complete the setup, you will be prompted to restart your machine. **Error 0x80004002** presents when attempting to run the terminal without a restart.

1. Open the Ubuntu Terminal after restarting & finishing the install. Create a username & password (this does not have to match your Windows login).
2. Run `sudo apt update` to install the latest updates. Enter your password when prompted.

To verify your Ubuntu version, with the terminal open run `lsb_release -a`

*** 
### Setting up remote development (VSCode) & Docker
1. Download, install, and configure Docker by following the accompanying prompts. Select **Use WSL 2 instead of Hyper-V** (Default)
2. After Docker is properly configured and running (program will indicate 'Engine Running' when properly set up) open the Ubuntu Terminal
3. ***The easiest way to run the latest Chaste release is by running*** `docker run -it --init --rm -v chaste_data:/home/chaste chaste/release`

This will generate an image (if this is the first run) with an independent Docker container with all dependencies and code precompiled. You can edit the image name [(options here)](https://hub.docker.com/repository/docker/chaste/release) to download versions other than the latest release, but this will not be required for this project. 

If you don't have a project, use `new_project.sh` to generate a template, which can be found in `/home/chaste/src/projects`.

For a detailed guide on how to build your own image/branch, see [here](https://github.com/Chaste/chaste-docker#mounting-host-directories)

4. Open VSCode and download the [remote development plugin](https://code.visualstudio.com/docs/remote/remote-overview) for significantly improved QoL.
5. After the plugin has been added, pair VSCode with the container via `Remote Explorer > Dev Containers > chaste/release [...]`
***
### Copying data & files i/o of Containers

In order to add this repository to your build, you will need to copy the files into your container via the terminal and vice versa to extract data. Docker is intended to allow for a consistent, homogenous computational environment through virtualization. However, this means we cannot copy data directly in/out of our containers.

#### Copying in
1. Open a separate Ubuntu Terminal. ***DO NOT*** close the existing terminal. This will close your container.
2. Run `docker container ls`
3. The first column `CONTAINER ID` contains the ID of every active docker container on your system. Identify the container running the Chaste image.
4. Copying in is done using the following format `docker cp /mnt/c/Users/User/... [CONTAINER ID]:/home/chaste/projects`

#### Copying out
1. Open a separate Ubuntu Terminal. ***DO NOT*** close the existing terminal. This will close your container.
2. Run `docker container ls`
3. The first column `CONTAINER ID` contains the ID of every active docker container on your system. Identify the container running the Chaste image.
4. Copying in is done using the following format `docker cp [CONTAINER ID]:/home/chaste/projects /mnt/c/Users/User/...`

***

### Cmake Guide

For a supplemental CMake guide, see [here](https://chaste.cs.ox.ac.uk/trac/wiki/ChasteGuides/CmakeFirstRun).

#### Assuming first time setup:
1. Normally, the path to the Chaste source code is `/home/chaste/src`, but if not use the same format to access it (`/path/to/Chaste_source`)
2. `/home/chaste/lib` can be used as your build directory, but if you wish to use a different directory you can create a new build folder by `mkdir /path/to/chaste_build`. This ***MUST*** be outside of the Chaste source directory.
3. Change directory to your new build folder by running `cd /path/to/chaste_build`
4. ***CRUCIAL:*** Run CMake (generates build files) with the following command `cmake /path/to/Chaste_source` (Unless directory structure has been changed, it should be `/home/chaste/src`)
5. To configure Chaste with a gui, run `ccmake /home/chaste/src` or `ccmake /path/to/Chaste_source`

After adjusting settings, press `C` to configure and `G` to generate makefiles.

6. To build, we will use the `make` command. To compile the global directory and all dependencies, run `make global`. To build specific targets, see [here](https://chaste.cs.ox.ac.uk/trac/wiki/ChasteGuides/CmakeBuildGuide).
7. If you wish to build a specific library without components, the syntax is `make chaste_${component}`
8. If you wish to build a specific test & its dependencies in your project, add the file name to a valid testpack file and run `make [TEST NAME]`
9. After building targets, tests can be run using the `ctest` command. To run a single test, use the `$` notation - `ctest Test1$`

###Configuring a project
From [chaste.cs.ox.ac.uk](https://chaste.cs.ox.ac.uk/trac/wiki/ChasteGuides/CmakeBuildGuide):

You can build projects by adding a subdirectory to the `projects/` folder and adding a `projects/project/CMakeLists.txt` file with

`find_package(Chaste COMPONENTS ${components})`

`chaste_do_project(myproject)`

where `${components}` are the components the project depends on. For this project, the only major dependency is the `cell_based` library.

***

### Some source method in my build have been modified for our tests to work properly, I will revise and indicate which methods/classes in the Chaste src need to be modified for proper compilation.
