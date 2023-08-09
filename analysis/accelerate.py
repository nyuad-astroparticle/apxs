# A set of methods and decorators to be used to accellerate processes
import os
import pandas as pd
from pandas import concat as CONCUT
from multiprocessing import Pool
from functools import partial
from tqdm.auto import tqdm
from numpy.random import normal, lognormal, rand
import numpy as np
import matplotlib.pyplot as plt

# SOME CONSTANTS
BASALT = {
    'name' : 'Basalt',
    'peaks' : {
        'Fe\nKa' : 6.40,
        'Fe\nKb' : 7.06,
        'Ti\nKa' : 4.51,
        'Ti\nKb' : 4.93,
        'Si\nKa' : 1.74,
        'Al\nKa' : 1.49,
        'Mg\nKa' : 1.25,
        'O\nKa'  : 0.52,
    }
}

# Parallelizes any function that is given to it
def parallel(function):
    """Decorator to parallelize a function

    Args:
        function (_type_): the function to parallelize. The first argument must be the one to iterate
    
    Returns:
        A function that when called is running in parallel the input function on the input_array
    """
    def inner(input_array,*args,**kwargs):
        # Create the arguments
        # input_extended = [(element, *args) for element in input_array] if len(args) != 0 else input_array

        func = partial(function,*args,**kwargs)

        # Parallelize excecution them
        with Pool(os.cpu_count()-1) as pool:
            output = list(tqdm(pool.imap(func, input_array, chunksize=100),total=len(input_array),desc=function.__name__,leave=False))
        return output 

    return inner

def concat(function):
    """Decorator for concatenating a list of pandas dataframes

    Args:
        function (_type_): The function that returns said list

    Returns:
        A function that concats it
    """
    def inner(*args,**kwargs):
        return CONCUT(function(*args,**kwargs))
    
    return inner

def loadFile(filename):
    """Load an output file from the simulation

    Args:
        filename (str): The filename to open

    Returns:
        pd.dataframe: A pandas dataframe of the data in the file using the header described there
    """
    file = open(filename,'r')
    
    header  = []
    line    = '#'
    while '#' in line:
        line = file.readline()
        if 'column' in line:
            header.append(line.split()[2])

    file.close()

    return pd.read_csv(filename,skiprows=len(header)+4,names=header)

# Function to extract the energy deposited per track
def getEnergy(data,columnName='DepositedEnergy',groupBy='TrackID'):
    """Given a python data frame it groups particles by their volume and returns an energy value

    Args:
        data (pd.dataframe): The original data

    Returns:
        pd.dataframe: The dataframe with the energies summed by trackid
    """
    data[groupBy] = data[groupBy].replace({-1: (-data.index[data[groupBy] == -1]).tolist()})

    return data.groupby(['EventID',groupBy,'Particle'])[columnName].sum().reset_index().rename(columns={columnName:'Energy'})


# Define some smear function
def smear_gaussian(energy,std=0.005):
    return normal(energy,scale=std)

def smear_lognormal(energy,std=0.005):
    return lognormal(energy,sigma=std)

# Apply it to the dataframe
def smearEnergy(data,smearfunc=smear_gaussian,columnName='Energy',*args,**kwargs):
    smear = partial(smearfunc,*args,**kwargs)
    data[columnName] = data[columnName].apply(smear)
    return data

# PARALLEL VERSIONS OF FUNCTIONS
loadFiles           = parallel(loadFile)
getEnergies         = concat(parallel(getEnergy))
getEnergyBatches    = parallel(getEnergy)
smearEnergies       = concat(parallel(smearEnergy))


# Put in one function
def getParticleEnergies(files,columnName='DepositedEnergy',groupBy='TrackID'):
    # Get the deposited energies
    energy = getEnergies(files,columnName=columnName,groupBy=groupBy)

    # Split into the energy per particle
    particleEnergies = {particle:energy.loc[energy.Particle == particle] for particle in energy.Particle.unique()}

    return particleEnergies

# We will sample a lognormal distribution with a set standard deviation for each energy
def getParticleEnergiesSmeared(files,columnName='DepositedEnergy',groupBy='TrackID',smearfunc=smear_gaussian,size=0.005):
    # Run it
    energyData      = getEnergyBatches(files,columnName=columnName,groupBy=groupBy)
    energySmeared   = smearEnergies(energyData,smearfunc=smearfunc,std=size)

    # Split into the energy per particle
    return {particle:energySmeared.loc[energySmeared.Particle.str.contains(particle)] for particle in energySmeared.Particle.unique()}

def adjustByEfficiency(histogram, bins, efficiency):
    return histogram*np.interp((bins[1:]+bins[:-1])/2, efficiency['Energy'], efficiency['Efficiency'])

# Plot a histogram
def plotParticleHistogram(particleEnergies,
                          PARTICLE      = 'gamma',
                          MATERIAL      = BASALT,
                          source        = 'Cm244',
                          MAX_PEAKS     = 15,
                          NBINS         = 1000,
                          LIMS          = (0,10),
                          SHOW_MATERIAL = True,
                          SAVE          = True,
                          yscale        = 'log',
                          columnName    = 'Energy',
                          efficiency    = None):
    
    fig = plt.figure(figsize=(10,5))
    ax  = fig.add_subplot(111)

    BINS        = np.linspace(*LIMS,NBINS+1)
    histogram   = np.histogram(particleEnergies[particleEnergies[columnName]!=0][columnName],bins=BINS)[0]
    if efficiency is not None: histogram = adjustByEfficiency(histogram,BINS,efficiency)

    color       = np.round(np.random.rand(1,3),1)
    ax.step(BINS[:-1],histogram,label=PARTICLE,color=color[0])

    # Get the peaks
    if MAX_PEAKS>0:
        peak_index  = np.argsort(histogram)[-MAX_PEAKS:][::-1]
        peaks       = (BINS[peak_index+1] + BINS[peak_index])/2
        idx_unique  = np.sort(np.unique((peaks*10).astype('int'),return_index=True)[1])
        peak_index  = peak_index[idx_unique]
        peak_height = histogram[peak_index]
        peaks       = peaks[idx_unique] 

        ax.scatter(peaks,peak_height*1.5,c='k',marker = '1')
        for peak,height in zip(peaks,peak_height*1.55):
            ax.annotate("%.2f"%peak,(peak,height),ha='center', va='bottom')

    # Plot the Material peaks
    if SHOW_MATERIAL:
        for peak in MATERIAL['peaks']:
            ax.axvline(MATERIAL['peaks'][peak],ls='--',lw=0.5,c = 'grey')
            ax.annotate(f"{peak}",(MATERIAL['peaks'][peak],2*ax.get_ylim()[1]),ha='left', va='top',fontsize=10)

    ax.set_xlim(BINS[1],BINS[-1])
    ax.set_xlabel(f'Energy of {PARTICLE} particles through the detector [keV]')
    ax.set_ylabel('Density')
    ax.legend(frameon=False)
    ax.set_yscale(yscale)
    ax.set_ylim(None,ax.get_ylim()[1]*2)
    ax.set_title(f'{MATERIAL["name"]} spectrum by {source} source')

    if SAVE: fig.savefig(source+'.png')

    return fig,ax