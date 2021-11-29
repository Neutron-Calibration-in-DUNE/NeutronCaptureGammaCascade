"""
    Script for plotting cascade spectrums which come out of the main program
"""
import numpy as np
import matplotlib.pyplot as plt
import csv


def plot_spectrum(
    input_file: str,
    title:      str='',
    save_plot:  str='',
    show_plot:  bool=True,
):
    energies = []
    cascades = []
    with open(input_file, "r") as file:
        reader = csv.reader(file, delimiter=",")
        for row in reader:
            cascades.append([float(row[i]) for i in range(len(row))])
            for i in range(len(row)):
                energies.append(float(row[i]))
    
    fig, axs = plt.subplots()
    axs.hist(energies, bins=500, label="{} events".format(len(cascades)))
    axs.set_xlabel("Energy (MeV)")
    if title != '':
        axs.set_title("Gamma cascade energy (MeV)\n"+title)
    else:
        axs.set_title("Gamma cascade energy (MeV)")
    plt.legend()
    plt.tight_layout()
    if save_plot != '':
        plt.savefig(save_plot)
    if show_plot:
        plt.show()

if __name__ == "__main__":
    plot_spectrum(
        "results/ar40_results_python.txt",
        title="Ar40 Neutron Capture",
        save_plot="plots/ar40_neutron_capture_python.png"
    )