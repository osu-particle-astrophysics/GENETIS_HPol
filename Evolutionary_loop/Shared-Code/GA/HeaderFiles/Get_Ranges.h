#pragma once

#include <vector>
extern string design;
extern int genes;


void Get_Ranges(float& variable_max, float& variable_min, int gene)
{
  // Establish max and min range vectors for each design
  vector<float> max_value;
  vector<float> min_value;
  if (design == "ARA")
  {
    // ARA ranges
    max_value = {7.5, 140.0, 1.0, 1.0};
    min_value = {0, 10.0, -1.0, -1.0};
  }
  else if (design == "PUEO")
  {
    // PUEO ranges
    max_value = {50.0, 175, 50.0, 50.0, 175.0, 175.0, 12.25};
    min_value = {0.0, 75.0, 0.0, 0.0, 0.0, 0.0, 0.0};
  }
  else if (design == "AREA")
  {
    // AREA ranges
    max_value = {5.0, 5.0, 5.0, 5.0, 5.0, 5.0, 5.0, 5.0, 5.0, 5.0, 5.0, 5.0, 5.0, 5.0 };
    min_value = {-5.0, -5.0, -5.0, -5.0, -5.0, -5.0, -5.0, -5.0, -5.0, -5.0, -5.0, -5.0, -5.0, -5.0};
  }
  else if (design == "Symmetric Dipole" || design == "Asymmetric Dipole")
  {
    // Dipole ranges
    max_value = { 10.0, 10000.0 };
    min_value = { 0.25, 200.0 };
  }

  // Find value for the gene that is being mutated. 
  for (int i = 0; i < genes; i++)
  {
    if (i == gene)
    {
      variable_max = max_value[i];
      variable_min = min_value[i];
    }
  }
}
