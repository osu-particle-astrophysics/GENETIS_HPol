#pragma once

extern int reproduction_no;
extern int crossover_no;
extern int population;
extern string design;

void Immigration(vector<vector<vector<float> > >& dna_output)
{
  // Generate new individuals for remaining population

  // Start Flag
  cout << "Immigration Started" << endl;

  
  for (int i = reproduction_no + crossover_no + mutation_no; i < population; i++)
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
  }

  // End Flag
  cout << "Immigration Complete" << endl;
}
