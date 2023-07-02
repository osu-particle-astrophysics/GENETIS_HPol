#pragma once


bool ConstraintARA(float radius, float length, 
                   float quadratic_coeff, float linear_coeff)
{
  // This function checks if the passed in antenna design is viable

  // Establish variables
  bool intersect = true;
  float max_radius = 7.5;
  float min_length = 10.0;
  float max_length = 140.0;
  float min_coeff = -1.0;
  float max_coeff = 1.0;
  float vertex = 0.0f;
  float end_point = quadratic_coeff * pow(length, 2) 
                      + linear_coeff * length + radius;
    
  // Solve for the vertex
  if(quadratic_coeff != 0.0)
  {
    vertex = (radius - pow(linear_coeff, 2) / (4 * quadratic_coeff));
  }
  else
  {
    vertex = end_point;
  }

  // Perform checks
  if(quadratic_coeff == 0.0 && max_radius > end_point && end_point >= 0.0)
  {
    if(radius < 0.0 || length < min_length || length > max_length 
       || quadratic_coeff < min_coeff || quadratic_coeff > max_coeff 
       || linear_coeff < min_coeff || linear_coeff > max_coeff)
    {
      intersect = true;
    }
    else
    {
      intersect = false;
    }
  }
  else if (quadratic_coeff != 0.0 && max_radius > end_point 
           && end_point >= 0.0 && max_radius > vertex && vertex >= 0.0)
  {
    if(radius < 0.0 || length < min_length || length > max_length 
       || quadratic_coeff < min_coeff || quadratic_coeff > max_coeff 
       || linear_coeff < min_coeff || linear_coeff > max_coeff)
    {
      intersect = true;
    }
    else
    {
      intersect = false;
    }
  }
  else
  {
    intersect = true;
  }
  
  // Return the condition
  return intersect;
}
