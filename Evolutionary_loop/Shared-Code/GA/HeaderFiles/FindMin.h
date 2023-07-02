#pragma once

#include <vector>

float FindMin(vector<vector<vector<float> > > & gains)
{
  // Find the minimum value of our gain pattern

  // Establish gains vector length variables
  int theta_index = 37;
  int phi_index = 72;
  int frequency_index = 60;
  float min = gains[0][0][0];

  // Find the minimum value of the gains vector
  for (int i = 0; i < theta_index; i++)
  {
    for (int j = 0; j < phi_index; j++)
    {
      for (int k = 0; k < frequency_index; k++)
      {
        if (gains[i][j][k] < min)
        {
          min = gains[i][j][k];
        }
      }
    }
  }
  return(min);
}
