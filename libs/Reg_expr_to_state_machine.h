#ifndef Reg_expr_to_state_machine
#define Reg_expr_to_state_machine

#include"State_machine.h"

void add_number_to_all_edges(state_machine &machine, int diff);

state_machine state_machine_with_one_move(std::string move);

state_machine add_kleene_closure(state_machine machine);

state_machine concatenation(state_machine first_machine, 
                            state_machine second_machine);

state_machine sum(state_machine first_machine, 
                  state_machine second_machine);

state_machine process_term(const std::string &reg_expr, int &symbol_id);

state_machine process_summ(const std::string &reg_expr, int &symbol_id);

state_machine reg_expr_to_state_machine(const std::string &reg_expr);

#endif //Reg_expr_to_state_machine
