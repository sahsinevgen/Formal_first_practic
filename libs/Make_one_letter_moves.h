#ifndef Make_one_letter_moves
#define Make_one_letter_moves

#include"State_machine.h"
#include<algorithm>

void dfs(const state_machine &g, 
         std::vector<int> &used,
         int used_colour,
         int root, 
         int u,
         state_machine &new_g);

state_machine make_one_letter_moves(const state_machine &g); // + removing multiple edges

#endif //Make_one_letter_moves