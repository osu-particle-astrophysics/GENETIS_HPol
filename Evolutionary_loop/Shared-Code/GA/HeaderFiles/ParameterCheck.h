#pragma once

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
extern bool termination;
extern string message;

void ParameterCheck(int argc)
{
  // Check to see if any arguments are not viable
  // If they arent update the termination clause

  if (argc != 11)
  {
    termination = true;
    message = "ERROR: Improper number of arguments";
  }

  else if (reproduction_no < 0 || crossover_no < 0 || mutation_no < 0
           || mutation_no < 0 || sigma < 0 || rank_no < 0 || roulette_no < 0
           || tournament_no < 0 || crossover_no % 2 != 0)
  {
    termination = true;
    message = "ERROR: One or more arguments are of an improper value. "
              "For example, values may not be less than zero.";
  }

  else if (design != "PUEO" && design != "ARA" && design != "AREA" &&
           design != "Symmetric Dipole" && design != "Asymmetric Dipole" && 
           design != "HPol")
  {
    termination = true;
    message = "ERROR: Unknown design. Known designs are PUEO, " 
              "AREA, ARA, Symmetric Dipole, Asymmetric Dipole, and HPol.";
  }

  else if (roulette_no + tournament_no + rank_no > population 
           || reproduction_no + crossover_no + mutation_no > population 
           || roulette_no + tournament_no + rank_no 
           < reproduction_no + crossover_no + mutation_no)
  {
    termination = true;
    message = "ERROR: Sum of selection or opperator parameters is improper.";
  }
}
