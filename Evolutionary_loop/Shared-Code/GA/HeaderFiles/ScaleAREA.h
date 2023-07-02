#pragma once

std::vector<std::vector<float> > ScaleAREA(vector<vector<float> > & antenna,
                                    vector<vector<vector<float> > > & gains)
{
  // Establish Gains vector length variables
  float min = 0.0;

  // Find Min Vlaue
  min = FindMin(gains);

  // Check if Min is negative
  if (min < 0.0)
  {
    // Scale the individual to conserve energy
    // Loop over all non-constant gain coefficients
    // Currently not adjusting extrapolating coefficient
    for (int i = 1; i < 13; i++)
    {
      antenna[0][i] = (1.0 / (1.0 - min)) * antenna[0][i];
    }
  }

  // Return scaled individual
  return antenna;
}
