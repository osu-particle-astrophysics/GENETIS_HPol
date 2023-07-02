#pragma once

#include <algorithm>
#include <random>

// User Functions
#include "ConstraintARA.h"
#include "ConstraintPUEO.h"
#include "ConstraintAREA.h"
#include "Selection.h"
#include "Mutation.h"

// Global Variables
extern int seed;
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


void Crossover(vector<vector<vector<float> > >& dna_input, 
               vector<vector<vector<float> > >& dna_output, 
               vector<float> fitness, vector<int> p_loc, 
               vector<int>& selected)
{
  // Generate a pair of children from a pair of selected parents

  // Start Flag
  cout << "Crossover Started" << endl;

  // Define Variables
  vector<int> locations;
  vector<int> spare_locations;
  double exchange;
  uniform_int_distribution<int> choice(0, 1);

  // call selection methods to populate locations
  Selection(crossover_no, fitness, locations);

  // call selection to populate spare locations
  Selection(population, fitness, spare_locations);

  uniform_int_distribution<int> grab(0, spare_locations.size()-1);

  // shuffle vector
  auto rng = default_random_engine{};
  shuffle(begin(locations), end(locations), rng);
  shuffle(begin(spare_locations), end(spare_locations), rng);


  // Check the size of location vector
  if (locations.size() != crossover_no)
  {
    cout << "error: parent vector is not proper length" << endl;
  }

  // Crossover two antennas to make 2 children
  for (int i = 0; i < crossover_no; i = i + 2)
  {
    // Force parents to be different
    while (dna_input[locations[i]] == dna_input[locations[i + 1]])
    {
      int new_parent = grab(generator);
      swap(locations[i + 1], spare_locations[new_parent]);

    }
    // If the children are identical to their parent, find new parent
    bool identical = true;
    while (identical == true)
    {
      for (int j = 0; j < sections; j++)
      {
        // If the design self-intersects, find a new design
        bool intersect = true;
        while (intersect == true)
        {
          for (int k = 0; k < genes; k++)
          {
            // Swap genes between parents to create the children
            exchange = choice(generator);
            if (exchange == 1)
            {
              dna_output[i + reproduction_no][j][k]
                = dna_input[locations[i]][j][k];

              dna_output[i + 1 + reproduction_no][j][k]
                = dna_input[locations[i + 1]][j][k];
            }
            else if (exchange == 0)
            {
              dna_output[i + reproduction_no][j][k]
                = dna_input[locations[i + 1]][j][k];

              dna_output[i + 1 + reproduction_no][j][k]
                = dna_input[locations[i]][j][k];
            }
          }

          // Call constraint functions to make sure the designs are applicable
          if (design == "ARA")
          {
            bool intersect_a = true;

            // define output vectors and check intersects
            vector<float> output_a = dna_output[i + reproduction_no][j];
            intersect_a = ConstraintARA(output_a[0], output_a[1],
              output_a[2], output_a[3]);


            bool intersect_b = true;
            vector<float> output_b = dna_output[i + 1 + reproduction_no][j];
            intersect_b = ConstraintARA(output_b[0], output_b[1],
              output_b[2], output_b[3]);

            if (intersect_a == false && intersect_b == false)
            {
              intersect = false;
            }
          }

          else if (design == "PUEO")
          {
            // Call constraint PUEO for variables
            bool intersect_a = true;
            vector<float> output_a = dna_output[i + reproduction_no][j];
            intersect_a = ConstraintPUEO(output_a[0], output_a[1], output_a[2],
              output_a[3], output_a[4], output_a[5],
              output_a[6]);

            bool intersect_b = true;
            vector<float> output_b = dna_output[i + 1 + reproduction_no][j];
            intersect_b = ConstraintPUEO(output_b[0], output_b[1], output_b[2],
              output_b[3], output_b[4], output_b[5],
              output_b[6]);

            if (intersect_a == false && intersect_b == false)
            {
              intersect = false;

            }
          }

          else if (design == "AREA")
          {
            // Call constraint AREA for variables
            bool intersect_a = true;
            intersect_a = ConstraintAREA(dna_output[i + reproduction_no]);

            bool intersect_b = true;
            intersect_b = ConstraintAREA(dna_output[i + 1 + reproduction_no]);

            if (intersect_a == false && intersect_b == false)
            {
              intersect = false;
            }

          }

          else if (design == "Symmetric Dipole" || design == "Asymmetric Dipole")
          {
            // Call constraint Dipole for variables
            bool intersect_a = true;
            vector<float> output_a = dna_output[i + reproduction_no][j];
            intersect_a = ConstraintDipole(output_a[0], output_a[1]);

            bool intersect_b = true;
            vector<float> output_b = dna_output[i + 1 + reproduction_no][j];
            intersect_b = ConstraintDipole(output_b[0], output_b[1]);

            if (intersect_a == false && intersect_b == false)
            {
              intersect = false;

            }
          }
        }
      }
      // If a child is identical to a parent, 
      // select a new parent
      if (( dna_output[i + reproduction_no] == dna_input[locations[i]] )
          || (dna_output[i + 1 + reproduction_no] == dna_input[locations[i + 1]] )
          || (dna_output[i + reproduction_no] == dna_input[locations[i + 1]] )
          || (dna_output[i + 1 + reproduction_no] == dna_input[locations[i]] ))
      {
        identical = true;
      }
      else
      {
        identical = false;
      }

      if (identical == true)
      {
        int new_parent = grab(generator);
        swap(locations[i + 1], spare_locations[new_parent]);
      }
    }
    // Save location of the parent antennas
    selected.push_back(p_loc[locations[i]]);
    selected.push_back(p_loc[locations[1 + i]]);
  }

  // End Flag
  cout << "Crossover Complete" << endl;
}
