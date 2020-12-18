#include"libs/First_practic.h"

int main() {
    std::string input;
    std::cin >> input;

    state_machine machine;
    machine = reg_expr_to_state_machine(true_reg_expr_from_input(input));
    int n = machine.n;
    machine.add_vertex();
    machine.add_edge(n - 1, n, "abc");
    machine = minimization(determinization(make_one_letter_moves(machine)));
    
    std::string str;
    std::cin >> str;

    std::cout << get_ans(machine, str) << std::endl;


}
