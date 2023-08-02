# APXS 
Alpha Particle X-ray Spectrometer Simulation

Development of a Geant4 simulation for choosing a radiactive source for the particles.
Analysis scripts as well as the complete geant4 simulation are provided.

## Project Management

This project is managed through github right [here](https://github.com/orgs/nyuad-astroparticle/projects/5)
You can view and update milestones, etc. [here](https://github.com/nyuad-astroparticle/apxs/milestones?with_issues=no)

## Help

Here is how to do certain things:

1. [Guide to Compile](#compiling)
	- On Arneodolab HPC
	- On Jubail
	- On Your Computer
2. Guide to Run
	- With Visualization
	- Batch Mode
3. Guide to Analyse

## Compiling

This section can help you compile the simulation on whatever system you use. If you are on the `arneodolab HPC` you will have a great time because a lot of these stuff are installed there by default.

### Preparation
Before you compile make sure you have done the following:

- [ ] Have a working [Geant4 Installation](https://geant4-userdoc.web.cern.ch/UsersGuides/InstallationGuide/html/). *If you are running this in the Lab's computer you're good!* In Jubail you will need to [install it by yourself :)](SYKE!)
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



### Choose Your Option

The Simulation can be compiled in multiple ways that enable some of its features. Currently there are two options.

1. **MPI Support |** MPI is a standard that allows an application to run parallel in parallel which increases performance a crap ton! ==You will probably only need to enable this option when running on the `arneodolab HPC`  or `Jubail` ==
2. **GDML Support |** GDML is a format for Geant4 to load geometry. **This option is enabled by default**. This will load an accurate model of the detector in your simulation. Disable this only if you get errors *(in wich case you would need to reinstall geant4 with the GDML option)*.
