#pragma once


bool ConstraintPUEO(float s, float h, float x_0,
                    float y_0, float y_f, float z_f, float beta)
{
  // Check to see if PUEO antenna conforms

  // Variables
  float max_s = 50.0;
  float max_h = 175.0;
  float min_h = 75.0;
  bool intersect = true;
  float x_f = s;

  // Run checks
  if (s < 0.0 || s > max_s || h > max_h || h < min_h)
  {
    intersect = true;
  }
  else if (x_0 < 0 || x_0 > x_f) 
  {
    intersect = true;
  }
  else if (y_0 < 0 || y_0 > z_f || y_0 > x_0) 
  {
    intersect = true;
  }
  else if (y_f < 0 || y_f > z_f) 
  {
    intersect = true;
  }
  else if (z_f < 0 || z_f > h) 
  {
    intersect = true;
  }
  else if ((4.0 / 30.0) * z_f > (beta * 100) 
           || (7 * z_f) < (beta * 100) || 2 < beta) 
  {
    intersect = true;
  }
  else 
  {
    intersect = false;
  }

  return intersect;
}
