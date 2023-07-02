#pragma once

// User Functions
#include "ConstraintARA.h"
#include "ConstraintPUEO.h"
#include "ConstraintAREA.h"
#include "ConstraintDipole.h"
#include "Get_Ranges.h"
#include <random>


// Global Variables
extern int seed;
extern std::default_random_engine generator;
extern string design;
extern int generation;
extern int population;
extern int sections;
extern int genes;
extern int reproduction_no;
extern int crossover_no;
extern int mutation_no;
extern int sigma;
extern int rank_no;
extern int roulette_no;
extern int tournament_no;

void Mutation(vector<vector<vector<float> > >& dna_input,
              vector<vector<vector<float> > >& dna_output,
              vector<float> fitness, vector<int> p_loc,
              vector<int>& selected)
{
  // Start Flag
  cout << "Mutation Started" << endl;

  // define storage vector	
  vector<int> locations;

  // Call selection methods
  Selection(mutation_no, fitness, locations);

  // Place individuals into the output arrays
  int individual = 0;
  for (int i = reproduction_no + crossover_no; i < reproduction_no + crossover_no + mutation_no; i++)
  {
    selected.push_back(p_loc[locations[individual]]);
    for (int j = 0; j < sections; j++)
    {
      for (int k = 0; k < genes; k++)
      {
        dna_output[i][j][k] = dna_input[locations[individual]][j][k];
      }
    }
    individual = individual + 1;
  }


  // Create random number generators
  uniform_int_distribution<int> select_section(0, sections-1);
  uniform_int_distribution<int> select_gene(0, genes-1);

  // itterate over individuals and genes to determine for mutations
  individual = 0;
  for (int i = reproduction_no + crossover_no; i < reproduction_no + crossover_no + mutation_no; i++)
  {
    // Select section and gene to mutate
    int mutate_section = select_section(generator);
    int mutate_gene = select_gene(generator);

    // Initialize vector to store temporary values as to not lose data
    vector<vector<float> > temp(sections, vector <float> (genes, 0.0f));

    // Save input genes into temp vector
    for (int x = 0; x < sections; x++)
    {
      for (int y = 0; y < genes; y++)
      {
        temp[x][y] = dna_output[i][x][y];
      }
    }


    // Set the intersect condition
    bool intersect = true;

    // Attempt mutation and check if it is viable
    while (intersect == true)
    {
      // set max and min for the RNG (method doesnt work well)
      //float variable_max = 0.0;
      //float variable_min= 0.0;
      //Get_Ranges(variable_max, variable_min, mutate_gene);

      // Set distribution based on current gene
      // uniform_real_distribution <float> mutate(variable_max, variable_min);
      normal_distribution<float> mutate(dna_output[i][mutate_section][mutate_gene],
                                        (sigma / 100.0)
                                        * dna_output[i][mutate_section][mutate_gene]);

      // Save the mutated value into temp
      temp[mutate_section][mutate_gene] = mutate(generator);

      // Check to see if the antenna is viable
      if (design == "ARA")
      {
        intersect = ConstraintARA(temp[mutate_section][0],
                                  temp[mutate_section][1],
                                  temp[mutate_section][2],
                                  temp[mutate_section][3]);
      }
      else if (design == "PUEO")
      {
        intersect = ConstraintPUEO(temp[mutate_section][0], 
                                   temp[mutate_section][1],
                                   temp[mutate_section][2],
                                   temp[mutate_section][3],
                                   temp[mutate_section][4],
                                   temp[mutate_section][5],
                                   temp[mutate_section][6]);
      }
      else if (design == "AREA")
      {
        // rescale and replace antenna with gain corrected version
        intersect = ConstraintAREA(temp);
      }
      else if (design == "Symmetric Dipole" 
               || design == "Asymmetric Dipole")
      {
        intersect = ConstraintDipole(temp[mutate_section][0],
                                     temp[mutate_section][1]);
      }
    }


    // Save temp values back to the output vector
    for (int x = 0; x < sections; x++)
    {
      for (int y = 0; y < genes; y++)
      {
        dna_output[i][x][y] = temp[x][y];
      }
    }


    // Make sure temp is cleared between loops
    temp.clear();

    // Save location of the parent antenna
    selected.push_back(p_loc[locations[individual]]);

    // Update individual
    individual = individual + 1;
  }
  // End Flag
  cout << "Mutation Complete" << endl;
}
