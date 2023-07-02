#pragma once

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


void DataRead(vector<vector<vector<float> > >& dna_input,
              vector<float>& fitness)
{
  // This function reads in data from various files

  // Establish variables
  int dna_garbage_end = 9;
  ifstream generationDNA;
  generationDNA.open("generationDNA.csv");
  int csv_file_size = dna_garbage_end + (population * sections);
  string csv_content[csv_file_size];
  string str_to_dbl;

  // This loop reads through the .csv file line by line
  // If we're in data (past line 9), it reads in the line

  for (int i = 0; i < csv_file_size; i++)
  {
    getline(generationDNA, csv_content[i]);
    if (i >= dna_garbage_end)
    {
      double j = floor((i - dna_garbage_end) / sections);
      int p = i - dna_garbage_end - sections * j;
      istringstream stream(csv_content[i]);
      for (int k = 0; k < genes; k++)
      {
        getline(stream, str_to_dbl, ',');
        dna_input[j][p][k] = atof(str_to_dbl.c_str());
      }
    }
  }

  generationDNA.close();

  // Now we need to read fitness scores:

  ifstream fitnessScores;
  fitnessScores.open("fitnessScores.csv");
  string fitness_read[population + 2];

  for (int i = 0; i < (population + 2); i++)
  {
    getline(fitnessScores, fitness_read[i]);
    if (i >= 2)
    {
      fitness[i - 2] = atof(fitness_read[i].c_str());
      if (fitness[i - 2] < 0)
      {
        fitness[i - 2] = 0;
      }
    }
  }

  fitnessScores.close();

}
