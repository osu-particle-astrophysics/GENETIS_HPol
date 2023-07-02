#pragma once

#include <vector>
#include <random>
extern int genes;
extern int sections;
extern int seed;
extern std::default_random_engine generator;

std::vector<std::vector<float> > GenerateDipole()
{
  // Create a Dipole antenna

  // Initialize an intersect condition
  bool intersect = true;
  std::vector<std::vector<float> > antenna(sections,
                                           std::vector <float>(genes, 0.0f));

  // Distances in cm
  float max_radius = 10.0;
  float min_radius = 0.25;
  float max_length = 10000.0;
  float min_length = 200.0;
  float radius, length;
  for (int i = 0; i < sections; i++)
  {
    // While the intersect condition is true, generate the side of an individual
    intersect = true;
    while (intersect == true)
    {
      // Generate gene for the radius
      std::uniform_real_distribution <float> distribution_radius(min_radius,
                                                                 max_radius);
      radius = distribution_radius(generator);

      // Generate gene for the length
      std::uniform_real_distribution <float> distribution_length(min_length, 
                                                                 max_length);
      length = distribution_length(generator);

      // Pass individual to the constraint function and update the interesct condition
      intersect = ConstraintDipole(radius, length);
    }
    // Store the variables into the antenna std::vector
    antenna[i][0] = radius;
    antenna[i][1] = length;
  }
  return(antenna);
}
