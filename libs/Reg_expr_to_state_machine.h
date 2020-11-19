#ifndef Reg_expr_to_state_machine
#define Reg_expr_to_state_machine

#include"State_machine.h"

void add_number_to_all_edges(state_machine &g, int inc);

state_machine state_machine_with_one_move(std::string move);

state_machine add_kleene_closure(state_machine a);

state_machine concatenation(state_machine a, state_machine b);

state_machine sum(state_machine a, state_machine b);

state_machine term(const std::string &s, int &i);

state_machine expr(const std::string &s, int &i);

state_machine reg_expr_to_state_machine(const std::string &s);

#endif //Reg_expr_to_state_machine