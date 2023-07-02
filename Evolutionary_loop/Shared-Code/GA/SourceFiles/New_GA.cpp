// New_GA.cpp
// Author: Ryan Debolt, Dylan Wells
// Organization: GENETIS
// Date: 12/5/2022
// Info:
// This version of the GA is designed to house all functions as standalone functions
// This allows us to more quickly and efficiently add new designs and features

// Compile using:
// g++ -std=c++11 New_GA.cpp -o GA.exe

// Call using
// ./GA.exe "design", generation, population, rank, roulette, tournament, reproduction, crossover, mutation, sigma
// Example:
// ./GA.exe "ARA" 1 100 60 20 20 10 70 15 10


// Delcare the namespace (needs to happen here)
using namespace std;

// Libraries
#include <time.h>
#include <math.h>
#include <random>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <vector>
#include <chrono>
#include <thread>
#include <ctime>

// User functions
#include "../HeaderFiles/ConstraintARA.h"
#include "../HeaderFiles/ConstraintPUEO.h"
#include "../HeaderFiles/ConstraintAREA.h"
#include "../HeaderFiles/ConstraintDipole.h"
#include "../HeaderFiles/ConstraintHPol.h"
#include "../HeaderFiles/Crossover.h"
#include "../HeaderFiles/DataRead.h"
#include "../HeaderFiles/DataWrite.h"
#include "../HeaderFiles/GenerateARA.h"
#include "../HeaderFiles/GeneratePUEO.h"
#include "../HeaderFiles/GenerateAREA.h"
#include "../HeaderFiles/GenerateDipole.h"
#include "../HeaderFiles/GenerateHPol.h"
#include "../HeaderFiles/Immigration.h"
#include "../HeaderFiles/Initialize.h"
#include "../HeaderFiles/Mutation.h"
#include "../HeaderFiles/Rank.h"
#include "../HeaderFiles/Reproduction.h"
#include "../HeaderFiles/Roulette.h"
#include "../HeaderFiles/Selection.h"
#include "../HeaderFiles/Sort.h"
#include "../HeaderFiles/Tournament.h"
#include "../HeaderFiles/ParameterCheck.h"
#include "../HeaderFiles/Get_Ranges.h"

// Set global variables
int seed = time(NULL);
default_random_engine generator(seed);
string design;
int generation;
int population;
int sections;
int genes;
int reproduction_no;
int crossover_no;
int mutation_no;
int sigma;
int rank_no;
int roulette_no;
int tournament_no;
bool termination = false;
string message = "";


int main(int argc, char const* argv[])
{
  // Start Flag
  cout << "Genetic Algorithm initialized" << endl;
  cout << endl;

  // Read in all arguments that determine what functions get run
  design = string(argv[1]);
  generation = atoi(argv[2]);
  population = atoi(argv[3]);
  rank_no = atoi(argv[4]);
  roulette_no = atoi(argv[5]);
  tournament_no = atoi(argv[6]);
  reproduction_no = atoi(argv[7]);
  crossover_no = atoi(argv[8]);
  mutation_no = atoi(argv[9]);
  sigma = atoi(argv[10]);

  // Check the read in arguments
  ParameterCheck(argc);

  // If the Arguments are incorret, exit the code with a message
  if (termination == true)
  {
    cout << message << endl;
    cout << "Proper call format is :" << endl;
    cout << "./Ga.exe <design>, generation, population, rank, roulette," 
            "tournament, reproduction, crossover, mutation_no, sigma" << endl;
    exit(0);
  }

  // Vectors
  vector<int> p_loc(population);
  vector<float> fitness(population, 0.0f);
  vector<int> selected;

  // Check the design and prepare input/output vectors
  if (design == "ARA")
  {
    // Determine sections and genes for ara
    sections = 2;
    genes = 4;

  }

  // if PUEO, create PUEO antennas
  else if (design == "PUEO")
  {
    // Determine sections and genes for PUEO
    sections = 1;
    genes = 7;
  }

  else if (design == "AREA")
  {
    // Determine sections and genes for AREA
    sections = 2;
    genes = 14;
  }

  else if (design == "Symmetric Dipole")
  {
    // Determine sections and genes for symmetric dipole
    sections = 1;
    genes = 2;
  }

  else if (design == "Asymmetric Dipole")
  {
    // Determine sections and genes for Asymmetric dipole
    sections = 2;
    genes = 2;
  }

  else if (design == "HPol")
  {
    // Determine sections and genes for Hpol
    sections = 1; // TBD; 
    genes = 1;    // TBD; 
  }

  // create input/output dna vectors based on parameters
  vector<vector<vector<float>>> dna_input(population,
                                          vector<vector<float> >(sections,
                                          vector <float>(genes, 0.0f)));
  vector<vector<vector<float>>> dna_output(population,
                                           vector<vector<float> >(sections,
                                           vector <float>(genes, 0.0f)));


  // FUNCTION CALLS

  // Generation zero functions
  if (generation == 0)
  {
    // run initialization
    Initialize(dna_output);
  }

  // Generation 1+ functions
  if (generation != 0)
  {
    // Read in data from previous generation
    DataRead(dna_input, fitness);

    // Sort vectors by fitness scores
    Sort(fitness, dna_input, p_loc);

    // Pass individuals from the previous generation into the current one
    Reproduction(dna_input, dna_output, fitness, p_loc, selected);

    // Create new individuals via sexual reproduction 
    Crossover(dna_input, dna_output, fitness, p_loc, selected);

    // Mutate selected individuals
    Mutation(dna_input, dna_output, fitness, p_loc, selected);

    // Introduce new individuals into the population by random generation
    Immigration(dna_output);
  }

  // Write information to data files
  DataWrite(dna_output, selected);

  // End Flag
  cout << endl;
  cout << "Genetic Algorithm Completed" << endl;

  return 0;
}
