#pragma once

// Libraries
#include <vector>

// User Functions
#include "Selection.h"

// Global variables
extern int seed;
extern int generation;
extern int population;
extern int sections;
extern int genes;
extern int reproduction_no;
extern int crossover_no;
extern int mutation_rate;
extern int sigma;
extern int rank_no;
extern int roulette_no;
extern int tournament_no;

void Reproduction(vector<vector<vector<float> > >& dna_input, 
                  vector<vector<vector<float> > >& dna_output, 
                  vector<float> fitness, vector<int> p_loc, 
                  vector<int>& selected)
{
  // Copy selected individuals into the new generation

  // Start Flag
  cout << "Starting Reproduction" << endl;

  // define storage vector	
  vector<int> locations;

  // Call selection methods
  Selection(reproduction_no, fitness, locations);

  // Place individuals into the output arrays
  for (int i = 0; i < reproduction_no; i++)
  {
    selected.push_back(p_loc[locations[i]]);
    for (int j = 0; j < sections; j++)
    {
      for (int k = 0; k < genes; k++)
      {
        dna_output[i][j][k] = dna_input[locations[i]][j][k];
      }
    }
  }
  // End Flag
  cout << "Reproduction Finished" << endl;
}
