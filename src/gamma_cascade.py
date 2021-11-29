"""
Python implementation of gamma cascade ()

Nicholas Carrara [nmcarrara@ucdavis.edu]
Luca Pagani      [lpagani@ucdavis.edu]
"""
import numpy as np
import matplotlib.pyplot as plt
from dataclasses import dataclass, field
from typing import List
import csv

@dataclass
class Gamma:
    """A class used to represent a single gamma

    Attributes
    ----------
    energy: float
        A float for this gammas energy
    rel_intensity: float
        The relative intensity of this gamma with respect to its level
    tot_intensity: float
        The total intensity
    conv_coefficient: float
        Conversion coefficient for this gamma

    """
    energy: float
    rel_intensity: float
    tot_intensity: float
    conv_coefficient: float

@dataclass
class Level:
    """A class representing a level in a gamma cascade model
    
    Attributes
    ----------
    energy: float
        Energy for this level
    spin_parity: str
        The spin parity for this level
    half_life: str
        The half-life for this level
    gammas: List[Gamma]
        The list of gammas that are emitted from this level
    probabilities: List[float]
        The probabilities for each gamma
    cumulative_probabilities: List[float]
        The cumulative probability for each gamma
    next_level: List[int]
        
    """
    energy: float
    spin_parity: str
    half_life: str
    gammas: list = field(default_factory=list)
    probabilities: list = field(default_factory=list)
    cumulative_probabilities: list = field(default_factory=list)
    next_level: list = field(default_factory=list)

    def add_gamma(self,
        energy, rel_intensity, tot_intensity, conv_coefficient
    ) -> None:
        """Method to add a new gamma to the list"""
        self.gammas.append(
            Gamma(energy,rel_intensity,tot_intensity,conv_coefficient)
        )

    def construct_probabilities(self) -> None:
        """Method to construct probabilities and 
        cumulative probabilities for the list of gammas
        
        """
        level_sum = sum([gamma.rel_intensity for gamma in self.gammas])
        for idx, gamma in enumerate(self.gammas):
            self.probabilities.append(gamma.rel_intensity/level_sum)
            if idx == 0:
                self.cumulative_probabilities.append(self.probabilities[0])
            else:
                self.cumulative_probabilities.append(
                    self.probabilities[idx] + self.cumulative_probabilities[idx-1])


class ENDFParser:
    """Class for parsing ENDF txt files and generating gamma cascade models.
    
    """
    def __init__(self,
        input_file: str,
    ) -> None:
        """Init function for ENDF parser.
        
        """
        self.input_file = input_file
        self.levels = []
        self.parse()
    
    def parse(self) -> None:
        """Function for parsing ENDF files"""
        with open(self.input_file,"r") as file:
            reader = csv.reader(file, delimiter=",")
            current_level = -1
            for row in reader:
                level = int(row[0])
                energy = float(row[1])
                gamma = int(row[2])
                gamma_energy = float(row[3])
                rel_intensity = float(row[4])
                probability = float(row[5])
                cum_probability = float(row[6])
                next_level = int(row[7])
                if current_level != level:
                    current_level = level
                    self.levels.append(Level(energy,"",""))
                self.levels[-1].add_gamma(gamma_energy,rel_intensity,0,0)
                self.levels[-1].probabilities.append(probability)
                self.levels[-1].cumulative_probabilities.append(cum_probability)
                self.levels[-1].next_level.append(next_level)

    def __getitem__(self, idx):
        """Overload of [] operator"""
        if idx < len(self.levels):
            return self.levels[idx]
        else:
            return self.levels[0]
    
    def max(self):
        """Returns the last level"""
        return self.levels[-1]
    
    def min(self):
        """Returns the first level"""
        return self.levels[0]
    
    def print(self):
        """Print out the levels and gamma information"""
        for idx, level in enumerate(self.levels):
            print("Level {} - {} MeV".format(idx, level.energy))
            for jdx, gamma in enumerate(level.gammas):
                output =  f"  - {jdx}) "
                output += f"{gamma.energy} --> level "
                output += f"{level.next_level[jdx]} - p(l-"
                output += f"{level.next_level[jdx]}|g-{jdx}) = "
                output += f"{level.probabilities[jdx]}"
                print(output)

@dataclass
class Event:
    """Class for holding cascade event information"""
    energies: list = field(default_factory=list)

class Cascade:
    """Gamma cascade generator"""
    def __init__(self,
        input_file: str,
        output_file: str="../results/output.txt"
    ) -> None:
        self.input_file = input_file
        self.output_file = output_file
        self.parser = ENDFParser(input_file)
        self.events = []

    def generate_event(self):
        """Generate a single event"""
        energies = []
        starting_energy = self.parser.max().energy
        mc_level_idx = len(self.parser.levels) - 1

        while starting_energy >= self.parser.min().energy:
            mc_random = np.random.uniform(0,1,1)
            for mc_gamma_idx in range(len(self.parser[mc_level_idx].gammas)):
                if (mc_random <= self.parser[mc_level_idx].cumulative_probabilities[mc_gamma_idx]):
                    energies.append(self.parser[mc_level_idx].gammas[mc_gamma_idx].energy)
                    mc_level_idx = self.parser[mc_level_idx].next_level[mc_gamma_idx]
                    starting_energy -= energies[-1]
                    break
        return Event(energies)

    def generate(self,
        num_events: int,
    ) -> None:
        """Generate 'num_events'"""
        for i in range(num_events):
            self.events.append(self.generate_event())
    
    def save(self):
        """Save events to file"""
        with open(self.output_file,"w") as file:
            writer = csv.writer(file, delimiter=",")
            for event in self.events:
                writer.writerow(event.energies)

if __name__ == "__main__":
    # try import cascade
    input_file = "../ENDF/ar40_levels.txt"
    cascade = Cascade(input_file, output_file="../results/ar40_results_python.txt")
    cascade.parser.print()
    cascade.generate(num_events=10000)
    cascade.save()
    



