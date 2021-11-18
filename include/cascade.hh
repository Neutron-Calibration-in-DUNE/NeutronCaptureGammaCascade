/**
 * @file    cascade.hh
 * @brief   Code for generating a gamma cascade
 * @ingroup cascade
 * @author  Nicholas Carrara [nmcarrara@ucdavis.edu]
 *          Luca Pagani      [lpagani@ucdavis.edu]
 */ 
#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <cstring>

#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <random>


namespace cascade
{
    struct Gamma
    {
        double energy;
        double rel_intensity;
        double tot_intensity;
        double conv_coefficient;

        Gamma();
        ~Gamma();
        Gamma(double energy, double rel_intensity, 
              double tot_intensity, double conv_coefficient);
    };

    struct Level
    {
        double energy;
        std::string spin_parity;
        std::string half_life;
        std::vector<Gamma> gammas;
        std::vector<double> probabilities;
        std::vector<double> cumulative_probabilities;
        std::vector<size_t> next_level;

        void addGamma(double energy, double rel_intensity, 
              double tot_intensity, double conv_coefficient);
        Level();
        ~Level();
        Level(double energy, std::string spin_parity,
              std::string half_life);
        
        void constructProbabilities();
    };

    struct Event
    {
        std::vector<double> energies;
        Event();
        ~Event();
        Event(std::vector<double> energies);
    };

    class ENDFParser
    {
    public:
        ENDFParser();
        ~ENDFParser();
        ENDFParser(std::string inputFile);

        const Level operator[](size_t index);
        const Level max();
        const Level min();
        
        const size_t numLevels() { return fLevels.size(); }

        void constructProbabilities();
        void constructNextLevels();
        void parse();
        void print();

    private:
        std::string fInputFile;
        FILE *fFile;
        // container of levels
        std::vector<Level> fLevels;
    };

    class Cascade
    {
    public:
        Cascade();
        ~Cascade();
        Cascade(std::string inputFile,
            size_t numberOfEvents,
            std::string outputFile);

        void generate(size_t numberOfEvents);
        Event generate();

        void save();

    private:
        ENDFParser fParser;
        FILE *fFile;
        size_t fNumberOfEvents = {1};
        std::string fOutputFile = {"dummy_cascade.txt"};
        // container of event
        std::vector<Event> fEvents;
        // random number classes
        std::random_device fDevice;
        std::mt19937 fGenerator;
        std::uniform_real_distribution<double> fUniform = {std::uniform_real_distribution<double>(0.0,1.0)};
    };
}