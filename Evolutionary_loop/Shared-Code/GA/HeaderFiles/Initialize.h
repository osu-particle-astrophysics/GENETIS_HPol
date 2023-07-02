#pragma once

extern int population;
extern string design;

void Initialize(std::vector<std::vector<std::vector<float> > >& dna_output)
{
  // Create the inital population of antennas

  for (int i = 0; i < population; i++)
  {
    // If the experiment is ARA, then call GenerateARA 
    if (design == "ARA")
    {
      dna_output[i] = GenerateARA();
    }

    // If the experiment is PUEO, then call GeneratePUEO 
    if (design == "PUEO")
    {
      dna_output[i] = GeneratePUEO();
    }

    // If the experiment is AREA, then call GenerateAREA 
    if (design == "AREA")
    {
      dna_output[i] = GenerateAREA();
    }

    // If the experiment is a Dipole call GenerateDipole
    if (design == "Symmetric Dipole" || design == "Asymmetric Dipole")
    {
      dna_output[i] = GenerateDipole();
    }

    // If the experiment is an Hpol call GenerateHpol
    if (design == "Hpol")
    {
      dna_output[i] = GenerateHPol();
    }
  }
}
