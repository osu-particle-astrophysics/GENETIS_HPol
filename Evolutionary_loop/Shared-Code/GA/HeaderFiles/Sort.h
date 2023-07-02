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


void Sort(vector<float>& fitness, vector<vector<vector<float> > >& dna_input,
                                  vector<int>& parent_location)
{
  // Sort individuals by fitness score

  // Establish variables
  int i, j, x, y;
  vector<int> original_location;
  for (int z = 0; z < fitness.size(); z++)
  {
    original_location.push_back(z);
  }

  for (i = 0; i < fitness.size(); i++)
  {
    double temp = fitness[i];
    int t = original_location[i];

    // Create location vector
    vector<vector<vector<float>>> location(1, vector<vector<float> >
                                          (sections, vector <float>
                                          (genes, 0.0f)));

    // Populate the locations
    for (int a = 0; a < sections; a++)
    {
      for (int b = 0; b < genes; b++)
      {
        location[0][a][b] = dna_input[i][a][b];

      }
    }

    // Put dna_input in order
    for (j = i; j > 0 && fitness[j - 1] < temp; j--)
    {
      fitness[j] = fitness[j - 1];
      parent_location[j] = parent_location[j - 1];
      for (x = 0; x < sections; x++)
      {
        for (y = 0; y < genes; y++)
        {
          dna_input[j][x][y] = dna_input[j - 1][x][y];
        }
      }
    }
    fitness[j] = temp;
    parent_location[j] = t;

    // Fill in last individual
    for (int a = 0; a < sections; a++)
    {
      for (int b = 0; b < genes; b++)
      {
        dna_input[j][a][b] = location[0][a][b];
      }
    }
  }
}
