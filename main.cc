#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <cstring>

#include <string>
#include <iostream>
#include <vector>
#include <random>

#include "arxiv/Stephen/simulate_gamma_cascade/geant4-simple/try/include/Level.hh"

class ENSDparser {

  public:

    void Parse(const std::string& input_filename) {
      FILE *inputFile;
      char reader[80];
      char levelEnergy[10];
      char gammaEnergy[10];
      char spinParity[18];
      char halfLife[10];
      char relativePhotonIntensity[8];
      char conversionCoefficient[7];
      char totalTransitionIntensity[10];

      inputFile = fopen( input_filename.c_str(), "r");

      if (!inputFile){
        // TODO: handle this error
      }

      for (int i = 0; i < 8; i++){
        relativePhotonIntensity[i] = 0;
      }
      for (int i = 0; i < 7; i++){
        conversionCoefficient[i] = 0;
      }
      for (int i = 0; i < 10; i++){
        levelEnergy[i] = 0;
        gammaEnergy[i] = 0;
        halfLife[i] = 0;
        totalTransitionIntensity[i] = 0;
      }
      for (int i = 0; i < 18; i++){
        spinParity[i] = 0;
      }
      while (fgets(reader, 80, inputFile)!=NULL){

        if (gammaEnergy[0] !=0){
          double lev_energy = strtod(levelEnergy, NULL);

          size_t l = 0;
          bool found_match = false;
          for (; l < fLevels.size(); ++l) {
            const Level& lev = fLevels.at(l);
            if ( lev_energy == lev.GetEnergy() ) {
              found_match = true;
              break;
            }
          }

          if (found_match) {
            // Add a new Gamma to the existing level
            fLevels.at(l).AddGamma( strtod(gammaEnergy, NULL),
              strtod(relativePhotonIntensity, NULL),
              strtod(totalTransitionIntensity, NULL),
              strtod(conversionCoefficient, NULL)
            );
          }
          else {
            // Create a new Level and add the first Gamma to it
            fLevels.push_back( Level(strtod(levelEnergy, NULL),
              std::string(spinParity, 18),
              std::string(halfLife, 10)) );

            fLevels.back().AddGamma( strtod(gammaEnergy, NULL),
              strtod(relativePhotonIntensity, NULL),
              strtod(totalTransitionIntensity, NULL),
              strtod(conversionCoefficient, NULL)
            );
          }

          for (int i = 0; i < 10; i++){
            gammaEnergy[i] = 0;
          }
        }else{
          if ((reader[5] == ' ')&&(reader[6] == ' ')&&(reader[7] == 'L')){
            strncpy(levelEnergy, &reader[9], 9);
            strncpy(spinParity, &reader[21], 17);
            strncpy(halfLife, &reader[39], 9);
          }
          if ((reader[5] == ' ')&&(reader[6] == ' ')&&(reader[7] == 'G')){
            strncpy(gammaEnergy, &reader[9], 9);
            strncpy(relativePhotonIntensity, &reader[21], 7);
            strncpy(totalTransitionIntensity, &reader[64], 9);

            if(reader[55] == ' '){
              for(int i = 1; i < 7; i++){
                conversionCoefficient[0] = '0';
                conversionCoefficient[i] = 0;
              }
            }else{
              strncpy(conversionCoefficient, &reader[55], 6);
            }
          }
        }
      }

      fclose (inputFile);
    }

    ENSDparser(const std::string& input_filename) {
      Parse(input_filename);
    }

    const Level& GetLevel(size_t index) const {
      return fLevels.at(index);
    }

    size_t NumLevels() const { return fLevels.size(); }

  private:

    std::vector<Level> fLevels;
};

int main()
{

  std::random_device rd;
  std::mt19937 mt(rd());
  std::uniform_real_distribution<double> dist(0.0, 1.0);
  
  ENSDparser parser("ENDF/endf.txt");
  Level level = parser.GetLevel(parser.NumLevels()-1);

  while(level.GetEnergy()) {
    double sumRelIntensity = 0.;

    //adds all of the relative intensities from each gamma in the level to get the total
    for (unsigned int j = 0; j < level.NumGammas(); j++) {
      const Gamma& gramma = level.GetGamma(j);
      sumRelIntensity = sumRelIntensity + gramma.GetRelIntensity();
    }

    double relProbabilities[level.NumGammas()];

    //reinitialize for every level's gammas
    for (unsigned int init = 0; init < level.NumGammas(); init++) {
      relProbabilities[init] = 0.;
    }

    //store the relative prob for each gamma in that level
    for (unsigned int k = 0; k < level.NumGammas(); k++) {
      const Gamma& gramma = level.GetGamma(k);
      relProbabilities[k] = gramma.GetRelIntensity() / sumRelIntensity;
    }

    double cumulativeProb[level.NumGammas()];
    cumulativeProb[0] = relProbabilities[0];

    for (unsigned int l = 1; l < level.NumGammas(); l++) {
      cumulativeProb[l] = relProbabilities[l] + cumulativeProb[l-1];
    }

    // pick a number according to the relintensities and then choose associated gamma
    double r = dist(mt);

    unsigned int n = 0;
    for(; n < level.NumGammas(); n++) {
      if (r <= cumulativeProb[n]) break;
    }

    // Creates a gamma at that index
    const Gamma& gamma = level.GetGamma(n);
    double k_e = gamma.GetEnergy() * 1000;

    //prints the gamma created
    std::cout <<  "Gamma energy " << gamma.GetEnergy() << " from Energy level " << level.GetEnergy() <<" was created." << std::endl;

    //HistogramData << gamma.GetEnergy() << std::endl;

    //to find the next energy level and its index
    size_t match_index = 0;
    double match_diff = std::numeric_limits<double>::max();
    for (size_t j = 0; j < parser.NumLevels(); ++j) {

      const Level& newLevel = parser.GetLevel(j);
      double diff = fabs( level.GetEnergy() - gamma.GetEnergy() - newLevel.GetEnergy() );

      if (diff < match_diff) {
        match_diff = diff;
        match_index = j;
      }

      //if this is within some tolerance of 0, we have reached the end and can add the event
      if (level.GetEnergy() - gamma.GetEnergy() < 1) {
        // int num_gamma = vertex->GetNumberOfParticle();
        // HistogramData << num_gamma << '\n';
        // for (int qq = 0; qq < num_gamma; ++qq) {
        //         double KE = vertex->GetPrimary(qq)->GetKineticEnergy();
        //         HistogramData << KE << ' ';
        // }
        // HistogramData << '\n';
        // anEvent->AddPrimaryVertex(vertex);
        // lnumEvents= lnumEvents + 1;
        // std::cout << "num Events " << lnumEvents << " numGammasTotal " << numGammasTotal << std::endl;
        return 0;
      }

    }

    level = parser.GetLevel(match_index);
}

  return 0;
}
