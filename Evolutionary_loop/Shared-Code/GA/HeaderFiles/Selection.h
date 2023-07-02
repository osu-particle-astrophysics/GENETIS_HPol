#pragma once

// User Functions
#include "Rank.h"
#include "Roulette.h"
#include "Tournament.h"

// Gloabal Variables
extern int seed;
extern int generation;
extern int population;
extern int sections;
extern int genes;
extern int reproduction_no;
extern int crossover_no;
extern int mutation_rate;
extern int sigma;
extern int rank_no;
extern int roulette_no;
extern int tournament_no;

void Selection(int opp_no, vector<float> fitness, vector<int> & locations)
{
  // Calculate and call selection methods

  // Start Flag
  cout << "Begining Selection" << endl;

  // Initialize Values
  int roul_select = (double)roulette_no / population * 1.0 * opp_no;
  int rank_select = (double)rank_no / population * 1.0 * opp_no;
  int tour_select = (double)tournament_no / population * 1.0 * opp_no;

  // Check to make sure values sum to correct value
  while (roul_select + rank_select + tour_select < opp_no)
  {

    float roul_cut = roulette_no / population * 1.0 
                     * opp_no - 1.0 * roul_select;
    float rank_cut = rank_no / population * 1.0 
                     * opp_no - 1.0 * rank_select;
    float tour_cut = tournament_no / population * 1.0 
                     * opp_no - 1.0 * tour_select;

    if (roul_cut > rank_cut && roul_cut > tour_cut)
    {
      roul_select = roul_select + 1;
    }

    else if (rank_cut > roul_cut && rank_cut > tour_cut)
    {
      rank_select = rank_select + 1;
    }

    else if (tour_cut > roul_cut && tour_cut > rank_cut)
    {
      tour_select = tour_select + 1;
    }

    else
    {
      if (rank_select > roul_select && rank_select > tour_select)
      {
        rank_select = rank_select + 1;
      }

      else if (roul_select > rank_select && roul_select > tour_select)
      {
        roul_select = roul_select + 1;
      }

      else if (tour_select > roul_select && tour_select > rank_select)
      {
        tour_select = tour_select + 1;
      }

      else
      {
        rank_select = rank_select + 1;
      }
    }
  }

  // Call each selection method
  for (int i = 0; i < roul_select; i++)
  {
    locations.push_back(Roulette(fitness));
  }
  for (int i = 0; i < rank_select; i++)
  {
    locations.push_back(Rank(fitness));
  }
  for (int i = 0; i < tour_select; i++)
  {
    locations.push_back(Tournament(fitness));
  }

  // End Flag
  cout << "Selection Finished" << endl;
}
