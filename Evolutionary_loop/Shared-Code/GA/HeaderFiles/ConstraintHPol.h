#ifndef CONSTRAINTHPOL_H
#define CONSTRAINTHPOL_H

bool ConstraintHPol(float radius, float length)
{
  // Set variables
  bool intersect = true;

  // distances in cm
  float max_radius = 10.0;
  float min_radius = 0.25;
  float max_length = 10000.0;
  float min_length = 200.0;

  // Check to see if antenna meets requirements
  if (radius > max_radius || radius < min_radius || length > max_length || length < min_length)
  {
    intersect = true;
  }
  else
  {
    intersect = false;
  }

  // Return interesect condition
  return intersect;
}

#endif