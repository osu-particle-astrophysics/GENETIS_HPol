#pragma once

#include <vector>
extern int seed;
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

void DataWrite(std::vector<std::vector<std::vector<float> > >& dna_output,
	             std::vector<int> selected)
{
  // This function writes out data to various files.

  // Write the header
  ofstream generationDNA;
  generationDNA.open("generationDNA.csv");
  generationDNA << "Hybrid of Roulette and Tournament -- "
                   "Thanks to Cal Poly / Jordan Potter" << "\n";
  generationDNA << "Author was David Liu" << "\n";
  generationDNA << "Notable contributors: Julie Rolla, Hannah Hasan, "
                   "and Adam Blenk" << "\n";
  generationDNA << "Done at The Ohio State University" << "\n";
  generationDNA << "Working on behalf of Dr. Amy Connolly" << "\n";
  generationDNA << "And the ANITA project" << "\n";
  generationDNA << "Revision date: 21 March 2018 1800 EST" << "\n";
  generationDNA << "Frequencies go here:\n";
  generationDNA << "Matrices for this Generation:\n";

  // Write DNA
  for (int i = 0; i < population; i++)
  {
    for (int j = 0; j < sections; j++)
    {
      for (int k = 0; k < genes; k++)
      {
        if (k == (genes - 1))
        {
          generationDNA << dna_output[i][j][k] << "\n";
        }
        else
        {
          generationDNA << dna_output[i][j][k] << ",";
        }
      }
    }
  }
  generationDNA.close();


  // Write parent file
  ofstream Parents;
  Parents.open("parents.csv");
  Parents << "Location of individuals used to make this generation:" << endl;
  Parents << "Seed: " << seed << endl;
  Parents << "\n" << endl;
  Parents << "Current Gen, Parent 1, Parent 2, Operator" << endl;
  int j = 0;

  if (generation != 0)
  {
    for (int i = 0; i < population; i++)
    {
      if (i < reproduction_no)
      {
        Parents << setw(2) << i + 1 << ", " << setw(2) 
                << selected[i] + 1 << setw(2) << ", NA, Reproduction" << endl;
      }
      else if (i >= reproduction_no && i < crossover_no + reproduction_no)
      {
        if (j % 2 == 0)
        {
          Parents << setw(2) << i + 1 << ", " << setw(2) 
                  << selected[i] + 1 << ", " << setw(2) 
                  << selected[i + 1] + 1 << ", Crossover" << endl;
        }
        else if (j % 2 != 0)
        {
          Parents << setw(2) << i + 1 << ", " << setw(2) 
                  << selected[i - 1] + 1 << ", " << setw(2) 
                  << selected[i] + 1 << ", Crossover" << endl;
        }
        j = j + 1;

      }
      else if (i >= crossover_no + reproduction_no && i < crossover_no + reproduction_no + mutation_no)
      {
        Parents << setw(2) << i + 1 << ", " << setw(2)
                << selected[i] + 1 << setw(2) << ", NA, Mutation" << endl;
      }
      else
      {
        Parents << setw(2) << i + 1 << ", NA, NA, Immigration" << endl;
      }
    }
    Parents.close();
  }
}
