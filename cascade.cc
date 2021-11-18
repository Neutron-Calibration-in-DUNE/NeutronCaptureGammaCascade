/**
 * @file    cascade.cc
 * @brief   Code for generating a gamma cascade
 * @ingroup cascade
 * @author  Nicholas Carrara [nmcarrara@ucdavis.edu]
 *          Luca Pagani      [lpagani@ucdavis.edu]
 */ 
#include <stdio.h>
#include <stdlib.h>
#include <cstring>

#include <string>
#include <iostream>
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

        Gamma(){}
        ~Gamma(){}
        Gamma(double energy, double rel_intensity, 
              double tot_intensity, double conv_coefficient)
        : energy(energy), rel_intensity(rel_intensity)
        , tot_intensity(tot_intensity), conv_coefficient(conv_coefficient) 
        {}
    };

    struct Level
    {
        double energy;
        std::string spin_parity;
        std::string half_life;
        std::vector<Gamma> gammas;

        void addGamma(double energy, double rel_intensity, 
              double tot_intensity, double conv_coefficient)
        {
            gammas.emplace_back(Gamma(energy, rel_intensity, tot_intensity, conv_coefficient));
        }
        Level(){}
        ~Level(){}
        Level(double energy, std::string spin_parity,
              std::string half_life)
        : energy(energy), spin_parity(spin_parity)
        , half_life(half_life)
        {}
    };

    struct Event
    {
        std::vector<double> energies;
        Event(){}
        ~Event(){}
        Event(std::vector<double> energies)
        : energies(std::move(energies))
        {}
    };

    class ENDFParser
    {
    public:
        ENDFParser(){}
        ~ENDFParser(){}
        ENDFParser(std::string inputFile)
        : fInputFile(inputFile)
        {
        }

        const Level operator[](size_t index)
        {
            if (index < fLevels.size()) {
                return fLevels[index];
            }
            else {
                return fLevels[0];
            }
        }
        const Level max() {
            return fLevels[fLevels.size() - 1];
        }
        
        void parse();

    private:
        std::string fInputFile;
        FILE *fFile;
        // container of levels
        std::vector<Level> fLevels;
        
    };

    class Cascade
    {
    public:
        Cascade()
        : fGenerator(fDevice())
        {}
        ~Cascade(){}
        Cascade(std::string inputFile,
            size_t numberOfEvents,
            std::string outputFile)
        : fParser(ENDFParser(inputFile))
        , fNumberOfEvents(numberOfEvents)
        , fOutputFile(outputFile)
        , fGenerator(fDevice())
        {}

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

    void ENDFParser::parse()
    {

    }

    void Cascade::generate(size_t numberOfEvents)
    {
        for (auto i = 0; i < numberOfEvents; i++) {
            fEvents.emplace_back(generate());
        }
    }

    Event Cascade::generate()
    {
        Level topLevel = fParser.max();
        
    }

    void Cascade::save()
    {

    }
}




int main(int argc, char** argv)
{
    cascade::Cascade cas("dummy_input.txt", 10, "dummy_output.txt");
    return 0;
}