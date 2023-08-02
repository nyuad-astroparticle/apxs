# APXS 
Alpha Particle X-ray Spectrometer Simulation

Development of a Geant4 simulation for choosing a radiactive source for the particles.
Analysis scripts as well as the complete geant4 simulation are provided.

## Project Management

This project is managed through github right [here](https://github.com/orgs/nyuad-astroparticle/projects/5)
You can view and update milestones, etc. [here](https://github.com/nyuad-astroparticle/apxs/milestones?with_issues=no)

## Help

Here is how to do certain things:

1. [Guide to Compile](#compiling)\
	The Guide will help you compile on the your computer, on the `arneodolab HPC`, or `Jubail`.
2. Guide to Run
	- With Visualization
	- Batch Mode
3. Guide to Analyse

## Compiling

This section can help you compile the simulation on whatever system you use. If you are on the `arneodolab HPC` you will have a great time because a lot of these stuff are installed there by default.

### I JUST WANNA COMPILE THIS PLEASE DON'T TEACH ME ANYTHING
<details>
<summary>These are the the commands to run. If it doesn't work for you look below</summary>

```console
git clone git@github.com:nyuad-astroparticle/apxs.git
cd apxs
mkdir build
cd build
source /path/to/geant4/installation/bin/geant4.sh
cmake -DGDML_DETECTOR=ON ..
make -j10
```
</details>

### Preparation
Before you compile make sure you have done the following:

- [ ] Have a working [Geant4 Installation](https://geant4-userdoc.web.cern.ch/UsersGuides/InstallationGuide/html/). *If you are running this in the Lab's computer you're good!* In `Jubail` you will need to [install it by yourself :)](SYKE!)
- [ ] Have downloaded the simulation code from here. 
  - [ ] To do this you must have [git installed](https://git-scm.com/book/en/v2/Getting-Started-Installing-Git) and 
  - [ ] Added your [ssh key](https://docs.github.com/en/authentication/connecting-to-github-with-ssh/adding-a-new-ssh-key-to-your-github-account) to your github account. 

If you have done this, you can clone this repository by running this on your terminal:
```console
$ cd /The/Directory/You/Want/To/Download/This/Code
$ git clone git@github.com:nyuad-astroparticle/apxs.git
```
This will create a folder called `apxs` in the location you chose. It contains all the code you need in order to run this!

- [ ] Have `cmake` installed in your computer. (If you have Geant4 you probably already do unless you did something weird in which case you know what you are doing so stop reasing this.)

### Choose Your Options

The Simulation can be compiled in multiple ways that enable some of its features. Currently there are two options.

1. **MPI Support |** MPI is a standard that allows an application to run parallel in parallel which increases performance a crap ton! **You will probably only need to enable this option when running on the `arneodolab HPC`  or `Jubail`**.
2. **GDML Support |** GDML is a format for Geant4 to load geometry. **This option is enabled by default**. This will load an accurate model of the detector in your simulation. Disable this only if you get errors *(in wich case you would need to reinstall geant4 with the GDML option)*.

You can enable these options using `cmake` as shown below.

### Actually compiling for real this time

To actually compile you need to follow these steps. 

First enter the `apxs` directory you just downloaded, create a folder there called `build` and enter it. This folder will contain all the files required for the simulation ro un **including the output files.** If somehow this is messed up, feel free to delete the `build` directory and recompile :).

```console
$ cd apxs
$ mkdir build
$ cd build
```

While you are there **load Geant4**. This is different depending on the computer you are on
1.  If you are on the `arneodolab HPC` you can simply type
	```console
	$ geant4
	```
	or if you want to enable the **MPI** option do
	```console
	$ geant4-mpi
	```
2. If you are using `Jubail` or your personal geant4 installation do
   ```console
   $ source /path/to/geant4/installation/bin/geant4.sh
   ```
   If you have a version of geant4 that supports MPI and you want to enable the MPI option you will have to include that too by running
   ```console
   $ export G4MPI_DIR="/path/to/g4mpi/lib/G4mpi-11.1.1"
   ```

Now inside `build` you can configure the installation by running
```console
$ ccmake ..
```

This will open an interface that looks likes this. This will allow you to configure the compilation of the simulation. This includes loading the geant4 libary, visualization drivers, and setting the [options](#choose-your-options) you want to enable.

```console 
EMPTY CACHE



Keys: [enter] Edit an entry [d] Delete an entry                                        CMake Version 3.26.3
      [l] Show log output   [c] Configure
      [h] Help              [q] Quit without generating
      [t] Toggle advanced mode (currently off)
```

To continue press `c`. The interface should show something like this. *(If it doesn't, you either got an error or cmake is simply showing you some output. Press `e` to exit in that case.)*

```console
CMAKE_BUILD_TYPE                *
CMAKE_INSTALL_PREFIX            */usr/local
ENABLE_MPI                      *OFF
GDML_DETECTOR                   *ON
Geant4_DIR                      */path-to/geant4/....
....
```

In particular look at the options `ENABLE_MPI` and `GDML_DETECTOR` these are the default values, however, if you want to change them you simply navigate in that line using your arrow keys and press enter to select the options you want. *The default configuration should be pretty god for most things*

Once you are done press `c` a couple more times untill an option to `press [g] to generate` becomes available. Then press `g` and you should be back to your regular terminal.

Finally build the simulation by running in the build directory
```console
$ make
```

**AND YOU ARE DONE!**
