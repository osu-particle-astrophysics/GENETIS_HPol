#pragma once

#include <vector>
#include <random>
extern int genes;
extern int sections;
extern int seed;
extern std::default_random_engine generator;

std::vector<std::vector<float> > GenerateARA()
{
  // Create an ARA antenna

  // initialize an interesect condidtion
  bool intersect = true;
  std::vector<std::vector<float> > antenna(sections, std::vector <float>(genes, 0.0f));

  // variables
  float max_outer_radius = 7.5;
  float max_radius = max_outer_radius;
  float min_length = 10.0; // in cm
  float max_length = 140;  // in cm
  float max_theta = atan(max_outer_radius / min_length);
  float min_coeff = -1.0;
  float max_coeff = 1.0;
  float radius, length, quadratic_coeff, linear_coeff;
  for (int i = 0; i < sections; i++)
  {
    // while the intersect condition is true, generate the side of an individual
    intersect = true;
    while (intersect == true)
    {
      std::uniform_real_distribution <float> distribution_radius(0, 
                                                                 max_radius);
      radius = distribution_radius(generator);

      std::uniform_real_distribution <float> distribution_length(min_length,
                                                                 max_length);
      length = distribution_length(generator);

      std::uniform_real_distribution <float> distribution_quadratic(min_coeff, 
                                                                    max_coeff);
      quadratic_coeff = distribution_quadratic(generator);

      std::uniform_real_distribution <float> distribution_linear(min_coeff,
                                                                 max_coeff);
      linear_coeff = distribution_linear(generator);

      // Pass antenna to constraint function and update the intersect condition
      intersect = ConstraintARA(radius, length, quadratic_coeff, linear_coeff);
    }
    // store the variables into the antenna vector
    antenna[i][0] = radius;
    antenna[i][1] = length;
    antenna[i][2] = quadratic_coeff;
    antenna[i][3] = linear_coeff;
  }
  return(antenna);
}
