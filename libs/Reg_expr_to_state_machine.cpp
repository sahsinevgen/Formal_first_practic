#include"Reg_expr_to_state_machine.h"

void add_number_to_all_edges(state_machine &machine, int diff) {
    for (int u = 0; u < machine.n; u++) {
        for (int i = 0; i < machine.graph[u].size(); i++) {
            machine.graph[u][i].to += diff;
        }
    }
}

state_machine state_machine_with_one_move(std::string move) {
    state_machine ans_machine;
    ans_machine.add_vertex(2);
    ans_machine.start = 0;
    ans_machine.terminals[1] = 1;
    ans_machine.add_edge(0, 1, move);
    return ans_machine;
}

state_machine add_kleene_closure(state_machine machine) {
    machine.add_vertex(2);
    int new_start = machine.n - 2;
    int new_terminal = machine.n - 1;
    machine.add_edge(new_start, machine.start, "");
    machine.add_edge(machine.start, new_terminal, "");
    for (int u = 0; u < machine.n; u++) {
        if (machine.terminals[u]) {
            machine.add_edge(u, machine.start, "");
            machine.terminals[u] = 0;
        }
    }

    machine.start = new_start;
    machine.terminals[new_terminal] = 1;
    
    return machine;
}

state_machine concatenation(state_machine first_machine, 
                            state_machine second_machine) {
    int n_last = first_machine.n;
    first_machine.add_vertex(second_machine.n);
    add_number_to_all_edges(second_machine, n_last);
    for (int u = 0; u < n_last; u++) {
        if (first_machine.terminals[u]) {
            first_machine.add_edge(u, second_machine.start + n_last, "");
            first_machine.terminals[u] = 0;
        }
    }
    for (int u = 0; u < second_machine.n; u++) {
        first_machine.graph[n_last + u] = second_machine.graph[u];
        first_machine.terminals[n_last + u] = second_machine.terminals[u];
    }
    return first_machine;
}

state_machine sum(state_machine first_machine, 
                  state_machine second_machine) {

    int n_last = first_machine.n;
    first_machine.add_vertex(second_machine.n + 2);
    int new_start = first_machine.n - 2;
    int new_terminal = first_machine.n - 1;
    add_number_to_all_edges(second_machine, n_last);
    for (int u = 0; u < second_machine.n; u++) {
        first_machine.graph[n_last + u] = second_machine.graph[u];
        first_machine.terminals[n_last + u] = second_machine.terminals[u];
    }
    first_machine.add_edge(new_start, first_machine.start, "");
    first_machine.add_edge(new_start, second_machine.start + n_last, "");
    for (int u = 0; u < first_machine.n; u++) {
        if (first_machine.terminals[u]) {
            first_machine.add_edge(u, new_terminal, "");
            first_machine.terminals[u] = 0;
        }
    }
    first_machine.start = new_start;
    first_machine.terminals[new_terminal] = 1;
    return first_machine;
}

state_machine process_summ(const std::string &reg_expr, int &i);

state_machine process_term(const std::string &reg_expr, int &i) {
    state_machine ans_machine;
    bool is_first_term = true;
    while (i != reg_expr.size() && reg_expr[i] != '+' && reg_expr[i] != ')') {
        //std::cout << i << std::endl;
        state_machine last_term;
        if (reg_expr[i] == '(') {
            i++;
            last_term = process_summ(reg_expr, i);
            i++;
        } else {
            std::string move = "";
            if (reg_expr[i] != '#') {
                move += reg_expr[i];
            }
            last_term = state_machine_with_one_move(move);
            i++;
        }
        if (i != reg_expr.size() && reg_expr[i] == '*') {
            last_term = add_kleene_closure(last_term);
            i++;
        }
        if (is_first_term) {
            ans_machine = last_term;
            is_first_term = false;
        } else {
            ans_machine = concatenation(ans_machine, last_term);
        }
    }
    return ans_machine;
}

state_machine process_summ(const std::string &reg_expr, int &i) {
    state_machine ans_machine;
    ans_machine = process_term(reg_expr, i);
    while (i != reg_expr.size() && reg_expr[i] == '+') {
        i++;
        ans_machine = sum(ans_machine, process_term(reg_expr, i));
    }
    return ans_machine;
}

state_machine reg_expr_to_state_machine(const std::string &reg_expr) {
    int i = 0;
    return process_summ(reg_expr, i);
}
