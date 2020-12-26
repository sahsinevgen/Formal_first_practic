#include"Reg_expr_to_state_machine.h"

void add_number_to_all_edges(state_machine &machine, int diff) {
    for (int vertex = 0; vertex < machine.size; vertex++) {
        for (auto &edge : machine.graph[vertex]) {
            edge.to += diff;
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
    int new_start = machine.size - 2;
    int new_terminal = machine.size - 1;
    machine.add_edge(new_start, machine.start, "");
    machine.add_edge(machine.start, new_terminal, "");
    for (int vertex = 0; vertex < machine.size; vertex++) {
        if (machine.terminals[vertex]) {
            machine.add_edge(vertex, machine.start, "");
            machine.terminals[vertex] = 0;
        }
    }

    machine.start = new_start;
    machine.terminals[new_terminal] = 1;
    
    return machine;
}

state_machine concatenation(state_machine first_machine, 
                            state_machine second_machine) {
    int prev_size = first_machine.size;
    first_machine.add_vertex(second_machine.size);
    add_number_to_all_edges(second_machine, prev_size);
    for (int vertex = 0; vertex < prev_size; vertex++) {
        if (first_machine.terminals[vertex]) {
            first_machine.add_edge(vertex, second_machine.start + prev_size, "");
            first_machine.terminals[vertex] = 0;
        }
    }
    for (int vertex = 0; vertex < second_machine.size; vertex++) {
        first_machine.graph[prev_size + vertex] = second_machine.graph[vertex];
        first_machine.terminals[prev_size + vertex] = 
                                            second_machine.terminals[vertex];
    }
    return first_machine;
}

state_machine sum(state_machine first_machine, 
                  state_machine second_machine) {

    int prev_size = first_machine.size;
    first_machine.add_vertex(second_machine.size + 2);
    int new_start = first_machine.size - 2;
    int new_terminal = first_machine.size - 1;
    add_number_to_all_edges(second_machine, prev_size);
    for (int vertex = 0; vertex < second_machine.size; vertex++) {
        first_machine.graph[prev_size + vertex] = 
                                            second_machine.graph[vertex];
        first_machine.terminals[prev_size + vertex] = 
                                            second_machine.terminals[vertex];
    }
    first_machine.add_edge(new_start, first_machine.start, "");
    first_machine.add_edge(new_start, second_machine.start + prev_size, "");
    for (int vertex = 0; vertex < first_machine.size; vertex++) {
        if (first_machine.terminals[vertex]) {
            first_machine.add_edge(vertex, new_terminal, "");
            first_machine.terminals[vertex] = 0;
        }
    }
    first_machine.start = new_start;
    first_machine.terminals[new_terminal] = 1;
    return first_machine;
}

state_machine process_summ(const std::string &reg_expr, int &symbol_id);

state_machine process_term(const std::string &reg_expr, int &symbol_id) {
    state_machine ans_machine;
    bool is_first_term = true;
    while (symbol_id != reg_expr.size() && 
           reg_expr[symbol_id] != '+' && reg_expr[symbol_id] != ')') {
        state_machine last_term;
        if (reg_expr[symbol_id] == '(') {
            symbol_id++;
            last_term = process_summ(reg_expr, symbol_id);
            symbol_id++;
        } else {
            std::string move = "";
            if (reg_expr[symbol_id] != '#') {
                move += reg_expr[symbol_id];
            }
            last_term = state_machine_with_one_move(move);
            symbol_id++;
        }
        if (symbol_id != reg_expr.size() && reg_expr[symbol_id] == '*') {
            last_term = add_kleene_closure(last_term);
            symbol_id++;
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

state_machine process_summ(const std::string &reg_expr, int &symbol_id) {
    state_machine ans_machine;
    ans_machine = process_term(reg_expr, symbol_id);
    while (symbol_id != reg_expr.size() && reg_expr[symbol_id] == '+') {
        symbol_id++;
        ans_machine = sum(ans_machine, process_term(reg_expr, symbol_id));
    }
    return ans_machine;
}

state_machine reg_expr_to_state_machine(const std::string &reg_expr) {
    int symbol_id = 0;
    return process_summ(reg_expr, symbol_id);
}
