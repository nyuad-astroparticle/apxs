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
2. [Guide to Run](#running-the-simulation)
	- [With Visualization](#interactive-mode)
	- [Batch Mode](#batch-mode)
3. Guide to Analyse

## Compiling

This section can help you compile the simulation on whatever system you use. If you are on the `arneodolab HPC` you will have a great time because a lot of these stuff are installed there by default.

#### I JUST WANNA COMPILE THIS PLEASE DON'T TEACH ME ANYTHING
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
- [ ] In `Jubail` you can load all of these modules if you run
	```console
	$ source hpc/load_modules.sh
	```
	from inside the `apxs` directory.

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

## Running the simulation
There are 2 ways to run the simulation, both of which create output files

1. **In Interactive Mode |** A window opens up with a visualization of the tracks and the geometry so that you can see what the hell is happening with those stupid particles.
2. **In Batch Mode |** No window, no visualization, it only runs a macro and saves data in `/build/output`. It runs much much faster though.

*If you are on `Jubail` you can only run in **Batch Node**.*

### Interactive Mode

To run in interactive mode go to the build directory and run the executable by using
```console
$ cd build
$ ./apxs
```

This is literally it. Then a bunch of output will come and a window will pop up. There is a command input on the bottom. You can use it to enter simulation commands.

To make it do something you can **select the material for the source** (by default is curium) by typing the following command
```
/apxs/setSourceMaterial G4_Cm
```

The supported options are:
1. `G4_Cm`: Curium-244
2. `G4_Fe`: Iron-55
3. `G4_Cd`: Cadmium-109
4. `G4_Co`: Cobalt-60
5. `G4_Am`: Americium-241

Then you can create a number of nuclei in the substrate that then the simulation will automatically decay and track by running this command *(change 10 with the number of nuclei you want)*
```
/run/beamOn 10
```

This will generate a bunch of stuff that could be hard to see, therefore there are three macros at your disposal that you can run **prior** to the `/run/beamOn` command to filter the tracks that are visible on the screen. *Note: The rest of the tracks will still be simulated they will just not be shown.*

1. `filter.mac`: Only shows the tracks that hit the target
2. `filter-detector.mac`: Only shows the tracks that hit the detector
3. `filter-full.mac`: Only shows the tracks that hit the detector or the target

You can load any of these macros by typing the following command
```
/control/execute ./macros/filter.mac
```

### Batch Mode

Running in batch mode depends on your compiler options. If you have compiled the simulation using MPI you will need to run with `mpiexec` as shown later. However, in all cases you will need a macro file. 

A macro file is a text file with the extension `.mac` that contains commands that are executed by the simulation line-by-line. There are already some examples of macro files in the simulation. For example the one in `macros/run_hpc.mac` is shown below

```
/apxs/setSourceMaterial G4_Cm
/run/beamOn 10000000
/apxs/setSourceMaterial G4_Fe
/run/beamOn 10000000
/apxs/setSourceMaterial G4_Cd
/run/beamOn 10000000
/apxs/setSourceMaterial G4_Co
/run/beamOn 10000000
/apxs/setSourceMaterial G4_Am
/run/beamOn 10000000
```

This macro sets the source material, then simulates a bunch of decays, then sets the source to a different material and so on. 

In compilation all macros that are in the `apxs/macros` directory are automatically copied to `apxs/build/macros` so make sure if you edit them to edit the copies on the build directory.

To start the simulation in batch mode you need to do the following

1. If you have **not** enabled **MPI**. Go to the build directory and run
   ```console
   $ ./apxs ./macros/macroname.mac
   ```
2. If you have enabled **MPI** go to the build directory and run
   ```console
   $ mpiexec -n 5 ./apxs ./macros/macroname.mac
   ```
   chanve the `-n 5` to the number of parallel processes you want to start (it should be less than number of cores you have available).

3. If you are in `Jubail` you need to use slurm to run the simulation. in that case the `apxs/hpc` folder is your friend. It contains a sample script you can use to submit a job that will run the `apxs/build/macros/run_hpc.mac`. To do this you just need to run (from the `apxs` directory)
   ```console
   $ cd hpc
   $ sbatch run_start.sh
   ```

## Analysis Guide

The simulation by default outputs particle interactions in the detector using `.csv` files. There are python scripts that can run in parallel that read out these files and make pretty plots.

### Output file Structure

Every time a track interacts in the sensitive detector the data is stored in the directory `apxs/build/output`. 

If you are running in **Multithreaded Mode** (default) or in **Multiprocessing Mode** (MPI Option) one file will be created per thread. The files are **not** merged by default. Rather the analysis scripts use them to read the large output in parallel (It's not a bug, it's a feature! lol).

If you open a file it looks something like this

```
#class tools::wcsv::ntuple
#title APXS Detector Hits
#separator 44
#vector_separator 59
#column int EventID
#column int TrackID
#column string Particle
#column double InitialEnergy
#column double DepositedEnergy
#column double XPosition
#column double YPosition
#column double ZPosition
#column double LocalTime
#column string Volume
2002,3,alpha,3.73318e+06,5804.88,199.186,269.861,5.22229,1.30652,worldPhysical
2012,367,alpha,3.7325e+06,5123.58,103.168,346.675,-45.9375,3860.8,worldPhysical
....
```

The likes with `#` are the header and then there are the comma separated values we recorded during the run. Each line corresponds to particle stepping through the detector region.

The first 4 lines are information about the file. The next 10 lines starting with `#` are the column headers. So the first value is the `EventID` the second is the `TrackID` and so on. These are automatically in the analysis scripts.


### Doing analysis in Python

I don't know how to use `Root` so I did the analysis in python. The scripts are in the `apxs/analysis` folder. 

They are designed so that you copy the output into a new folder called `runsomething` in `apxs/analysis/data/runsomething`. That said, of course you can use a different folder. 

There are 2 files in `apxs/analysis`

1. `accellerate.py`: Contains functions that can run in paralle that laod and process the data based on the file using multiple threads. Functions in there are suuuuper fast so feel free to use them.
2. `analysis.ipynb`: This jupyter notebook contains an example of an analysis for the apxs. It loads the data, groups the hits by `TrackID` (or by their `MigrantID` which is the ID of the particle that they were created as a result of, e.g. all the ionization electrons from a signle particle will have the same `MigrantID`) and the gets the energy deposited to the detector, or the kinetic energy, and plots histograms for the various sources. It can also do some rudementary peak finding id you load the composition of the target material. The script will output pictures in the analsysis directory that are automatically ignored by `.git`.