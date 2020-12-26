#ifndef Make_one_letter_moves
#define Make_one_letter_moves

#include"State_machine.h"
#include<algorithm>

void dfs(const state_machine &machine, 
         std::vector<int> &used,
         int used_colour,
         int root, 
         int vertex,
         state_machine &new_machine);

state_machine make_one_letter_moves(const state_machine &machine); // + removing multiple edges

#endif //Make_one_letter_moves
