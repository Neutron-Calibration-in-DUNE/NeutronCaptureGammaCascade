#include <stdio.h>
#include <stdlib.h>
#include <cstring>

#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <random>

#include "cascade.hh"

int main(int argc, char** argv)
{
    std::string inputFile;
    size_t numEvents;
    std::string outputFile = "dummy_output.txt";

    if (argc == 1) {
        std::cout << "ERROR! No input file specified!" << std::endl;
        exit(0);
    }
    else if (argc == 2)
    {
        inputFile = argv[1];
        numEvents = 10; 
    }
    else if (argc == 3)
    {
        inputFile = argv[1];
        numEvents = atoi(argv[2]);
    }
    else if (argc == 4)
    {
        inputFile = argv[1];
        numEvents = atoi(argv[2]);
        outputFile = argv[3];
    }
    else
    {
        std::cout << "ERROR! Number of inputs must be < 3!" << std::endl;
        exit(0);
    }

    std::cout << "Using input file:        " << inputFile << "\n";
    std::cout << "Using number of samples: " << numEvents << "\n";
    std::cout << "Using output file:       " << outputFile << "\n";

    cascade::Cascade cas(inputFile, numEvents, outputFile);
    cas.save();
    cas.save_levels("ar40_levels.txt");
    return 0;
}