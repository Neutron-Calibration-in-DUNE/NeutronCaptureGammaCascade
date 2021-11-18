// int main()
// {

//   std::random_device rd;
//   std::mt19937 mt(rd());
//   std::uniform_real_distribution<double> dist(0.0, 1.0);
  
//   ENSDparser parser("ENDF/endf.txt");
//   Level level = parser.GetLevel(parser.NumLevels()-1);

//   while(level.GetEnergy()) {
//     double sumRelIntensity = 0.;

//     //adds all of the relative intensities from each gamma in the level to get the total
//     for (unsigned int j = 0; j < level.NumGammas(); j++) {
//       const Gamma& gramma = level.GetGamma(j);
//       sumRelIntensity = sumRelIntensity + gramma.GetRelIntensity();
//     }

//     double relProbabilities[level.NumGammas()];

//     //reinitialize for every level's gammas
//     for (unsigned int init = 0; init < level.NumGammas(); init++) {
//       relProbabilities[init] = 0.;
//     }

//     //store the relative prob for each gamma in that level
//     for (unsigned int k = 0; k < level.NumGammas(); k++) {
//       const Gamma& gramma = level.GetGamma(k);
//       relProbabilities[k] = gramma.GetRelIntensity() / sumRelIntensity;
//     }

//     double cumulativeProb[level.NumGammas()];
//     cumulativeProb[0] = relProbabilities[0];

//     for (unsigned int l = 1; l < level.NumGammas(); l++) {
//       cumulativeProb[l] = relProbabilities[l] + cumulativeProb[l-1];
//     }

//     // pick a number according to the relintensities and then choose associated gamma
//     double r = dist(mt);

//     unsigned int n = 0;
//     for(; n < level.NumGammas(); n++) {
//       if (r <= cumulativeProb[n]) break;
//     }

//     // Creates a gamma at that index
//     const Gamma& gamma = level.GetGamma(n);
//     double k_e = gamma.GetEnergy() * 1000;

//     //prints the gamma created
//     std::cout <<  "Gamma energy " << gamma.GetEnergy() << " from Energy level " << level.GetEnergy() <<" was created." << std::endl;

//     //HistogramData << gamma.GetEnergy() << std::endl;

//     //to find the next energy level and its index
//     size_t match_index = 0;
//     double match_diff = std::numeric_limits<double>::max();
//     for (size_t j = 0; j < parser.NumLevels(); ++j) {

//       const Level& newLevel = parser.GetLevel(j);
//       double diff = fabs( level.GetEnergy() - gamma.GetEnergy() - newLevel.GetEnergy() );

//       if (diff < match_diff) {
//         match_diff = diff;
//         match_index = j;
//       }

//       //if this is within some tolerance of 0, we have reached the end and can add the event
//       if (level.GetEnergy() - gamma.GetEnergy() < 1) {
//         // int num_gamma = vertex->GetNumberOfParticle();
//         // HistogramData << num_gamma << '\n';
//         // for (int qq = 0; qq < num_gamma; ++qq) {
//         //         double KE = vertex->GetPrimary(qq)->GetKineticEnergy();
//         //         HistogramData << KE << ' ';
//         // }
//         // HistogramData << '\n';
//         // anEvent->AddPrimaryVertex(vertex);
//         // lnumEvents= lnumEvents + 1;
//         // std::cout << "num Events " << lnumEvents << " numGammasTotal " << numGammasTotal << std::endl;
//         return 0;
//       }

//     }

//     level = parser.GetLevel(match_index);
// }

//   return 0;
// }
