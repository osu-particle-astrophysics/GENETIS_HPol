#pragma once

#include <vector>

vector<vector<vector<float> > > SolveGains(vector<vector<float> > & antenna)
{
  // Solve for the gain pattern of an AREA antenna

  // Declare variables and value vectors
  long double pi = 3.14159265359;
  int theta_index = 37;
  vector<float> theta = {};
  int phi_index = 72;
  vector<float> phi = {};
  int frequency_index = 60;
  vector<float> frequency = {};

  // Declare vectors to store important values
  vector<vector<vector<float>>> gains(theta_index, vector<vector<float> >
                                     (phi_index,
                                      vector <float>(frequency_index, 0.0f)));

  // populate Theta, Phi, and Frequency vector
  float theta_start = 0;
  for (int i = 0; i < theta_index; i++)
  {
    theta.push_back(theta_start);
    theta_start = theta_start + 5.0;
  }

  float phi_start = 0;
  for (int i = 0; i < phi_index; i++)
  {
    phi.push_back(phi_start);
    phi_start = phi_start + 5.0;
  }

  float frequency_start = pow(83.33 * 10.0, 6.0);
  for (int i = 0; i < frequency_index; i++)
  {
    frequency.push_back(frequency_start);
    frequency_start = frequency_start + pow(16.67 * 10.0, 6.0);
  }

  // Loop over Theta, Phi, and Frequency and calculate gain for each point
  // Gain should be symetrical for phi and frequency
  for (int i = 0; i < theta_index; i++)
  {
    double cosangle = cos(theta[i] * pi / 180.0f);

    long double theta_gain = (antenna[0][0] * (1 / 2.0) * (1 / sqrt(pi))
                              + antenna[0][1] * (1 / 2.0) * sqrt(3 / pi) * cosangle
                              + antenna[0][2] * (1 / 4.0) * sqrt(5 / pi) * (3 * pow(cosangle, 2) - 1)
                              + antenna[0][3] * (1 / 4.0) * sqrt(7 / pi) * (5 * pow(cosangle, 3) - 3 * cosangle)
                              + antenna[0][4] * (3 / 16.0) * sqrt(1 / pi) * (35 * pow(cosangle, 4) - 30 * pow(cosangle, 2) + 3)
                              + antenna[0][5] * (1 / 16.0) * sqrt(11 / pi) * (15 * cosangle - 70 * pow(cosangle, 3) + 63 * pow(cosangle, 5))
                              + antenna[0][6] * (1 / 32.0) * sqrt(13 / pi) * (-5 + 105 * pow(cosangle, 2) - 315 * pow(cosangle, 4) + 231 * pow(cosangle, 6))
                              + antenna[0][7] * (1 / 32.0) * sqrt(15 / pi) * (-35 * cosangle + 315 * pow(cosangle, 3) - 693 * pow(cosangle, 5) + 429 * pow(cosangle, 7))
                              + antenna[0][8] * (1 / 256.0) * sqrt(17 / pi) * (35 - 1260 * pow(cosangle, 2) + 6930 * pow(cosangle, 4) - 12012 * pow(cosangle, 6) + 6435 * pow((cosangle), 8))
                              + antenna[0][9] * (1 / 256.0) * sqrt(19 / pi) * (315 * cosangle - 4620 * pow(cosangle, 3) + 18018 * pow(cosangle, 5) - 25740 * pow(cosangle, 7) + 12155 * pow((cosangle), 9))
                              + antenna[0][10] * (1 / 512.0) * sqrt(21 / pi) * (-63 + 3465 * pow(cosangle, 2) - 30030 * pow(cosangle, 4) + 90090 * pow(cosangle, 6) - 109395 * pow((cosangle), 8) + 46189 * pow(cosangle, 10))
                              + antenna[0][11] * (1 / 512.0) * sqrt(23 / pi) * (-693 * pow(cosangle, 1) + 15015 * pow(cosangle, 3) - 90090 * pow(cosangle, 5) + 218790 * pow((cosangle), 7) - 230945 * pow(cosangle, 9) + 88179 * pow(cosangle, 11))
                              + antenna[0][12] * (1 / 2048.0) * sqrt(25 / pi) * (231 - 18018 * pow(cosangle, 2) + 225225 * pow(cosangle, 4) - 1021020 * pow(cosangle, 6) + 2078505 * pow((cosangle), 8) - 1939938 * pow(cosangle, 10) + 676039 * pow(cosangle, 12)));
                               /*(1+(genes[13]*(16.67*freq)/83.33))**/
    for (int j = 0; j < phi_index; j++)
    {
      for (int k = 0; k < frequency_index; k++)
      {
        // Calculate the spherical harmonics at this point
        gains[i][j][k] = theta_gain;
      }
    }
  }

  // Return the Gain vector
  return(gains);
}
