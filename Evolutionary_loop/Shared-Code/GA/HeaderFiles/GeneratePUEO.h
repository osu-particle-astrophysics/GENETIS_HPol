#pragma once

#include <vector>
#include <random>
#include "ConstraintPUEO.h"
extern int genes;
extern int sections;
extern int seed;
extern std::default_random_engine generator;

std::vector<std::vector<float> > GeneratePUEO()
{
  // Create a PUEO antenna
 
  //initialize variables
  std::vector<std::vector<float> > outputVector(sections,
                                                std::vector<float>(genes, 
                                                                  0.0f));
  float max_s = 50.0;
  float max_h = 175.0;
  float min_h = 75.0;
  bool intersect = true;
  float s, h, x_0, y_0, z_f, y_f, beta;


  for (int i = 0; i < sections; i++) {
    while (intersect == true) 
    { 
      //while the antenna intersects, generate new values for antena 

      std::uniform_real_distribution <float> distribution_s(0, max_s);
      s = distribution_s(generator);

      std::uniform_real_distribution <float> distribution_h(min_h, max_h);
      h = distribution_h(generator);

      std::uniform_real_distribution <float> distribution_x_0(0, s); // s
      x_0 = distribution_x_0(generator);

      std::uniform_real_distribution <float> distribution_y_0(0, x_0); // x_0
      y_0 = distribution_y_0(generator);

      std::uniform_real_distribution <float> distribution_z_f(0, h); // h
      z_f = distribution_z_f(generator);

      std::uniform_real_distribution <float> distribution_y_f(0, z_f); // z_f
      y_f = distribution_y_f(generator);

      std::uniform_real_distribution <float> distribution_beta((4.0 / 30.0) 
                                                               * z_f, 7 * z_f); // z_f
      beta = distribution_beta(generator) / 100;

      //float tau = 0.26;  tau must be 0.26, not evolving
      //float m = 1;  we are only evolving m = 1 for now.
      //flaot Z0 = 0; not evolving
      //outputVector[i][7] = m;
      //outputVector[i][8] = tau;
      intersect = ConstraintPUEO(s, h, x_0, y_0, y_f, z_f, beta);
    }
    outputVector[i][0] = s;
    outputVector[i][1] = h;
    outputVector[i][2] = x_0;
    outputVector[i][3] = y_0;
    outputVector[i][4] = y_f;
    outputVector[i][5] = z_f;
    outputVector[i][6] = beta;
  }
  return outputVector;

}
