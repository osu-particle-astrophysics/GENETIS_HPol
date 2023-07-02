#pragma once

// Includes
#include "SolveGains.h"
#include "FindMin.h"
#include "ScaleAREA.h"
#include <vector>

bool ConstraintAREA(vector<vector<float> > & antenna)
{
  // This function checks if the passed in antenna design is viable

  // Set the condition
  bool intersect = true;

  // Fetch Gains of this antenna design
  vector<vector<vector<float> > > gains = SolveGains(antenna);

  // Use gains to check energy conservation
  // Check if min gain value is negative, if it is, rescale gain coefficents 
  float min = FindMin(gains);
  while (intersect == true)
  {
    if (min >= 0.0)
    {
      intersect = false;
    }
    else
    {
      // Scale the antenna's coefficients
      antenna = ScaleAREA(antenna, gains);

      // Re-solve Gains and Min
      vector<vector<vector<float>>> gains = SolveGains(antenna);
      min = FindMin(gains);
    }
  }

  // Return the interesect constraint
  return intersect;
}
