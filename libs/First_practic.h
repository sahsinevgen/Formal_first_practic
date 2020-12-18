#ifndef First_practic
#define First_practic

#include"Reg_expr_to_state_machine.h"
#include"Make_one_letter_moves.h"
#include"Determinization.h"
#include"Minimization.h"
#include<stack>
#include<stdexcept>
#include<queue>

std::string true_reg_expr_from_input(std::string input);

int get_ans(state_machine &machine, std::string str);

#endif //First_practic
