#include"First_practic.h"

std::string true_reg_expr_from_input(std::string input) {
    std::stack<std::string> reg_exprs;
    for (const auto &symbol : input) {
        if ('a' <= symbol && symbol <= 'c') {
            std::string word = "";
            word += symbol;
            reg_exprs.push(word);
        } else if (symbol == '*') {
            if (reg_exprs.size() < 1) {
                throw std::invalid_argument(input);
            }
            std::string arg;
            arg = reg_exprs.top();
            reg_exprs.pop();
            arg = "(" + arg + ")*";
            reg_exprs.push(arg);
        } else if (symbol == '+' || symbol == '.') {
            if (reg_exprs.size() < 2) {
                throw std::invalid_argument(input);
            }
            std::string first_arg, second_arg;
            second_arg = reg_exprs.top();
            reg_exprs.pop();
            first_arg = reg_exprs.top();
            reg_exprs.pop();
            if (symbol == '.') {
                first_arg = "(" + first_arg + ")(" + second_arg + ")";
            } else {
                first_arg = first_arg + "+" + second_arg;
            }
            reg_exprs.push(first_arg);
        } else {
            throw std::invalid_argument(input);
        }
    }
    if (reg_exprs.size() != 1) {
        throw std::invalid_argument(input);
    }
    return reg_exprs.top();
}

int get_ans(state_machine &machine, std::string str) {
    int size = machine.size;
    std::vector<int> can_be_pref(size, 0);
    std::vector<std::vector<int>> rev_graph(size);
    for (int vertex = 0; vertex < size; vertex++) {
        for (const auto &edge : machine.graph[vertex]) {
            int vertex_to = edge.to;
            rev_graph[vertex].push_back(vertex_to);
        }
    }
    std::queue<int> not_processed_vertex;
    for (int vertex = 0; vertex < size; vertex++) {
        if (machine.terminals[vertex]) {
            not_processed_vertex.push(vertex);
            can_be_pref[vertex] = 1;
        }
    }
    while (!not_processed_vertex.empty()) {
        int vertex = not_processed_vertex.front();
        not_processed_vertex.pop();
        for (auto next_vertex : rev_graph[vertex]) {
            if (can_be_pref[next_vertex] == 0) {
                can_be_pref[next_vertex] = 1;
                not_processed_vertex.push(next_vertex);
            }
        }
    }
    int ans = 0;
    int cur_vertex = machine.start;
    for (int symbol_id = 0; symbol_id < str.size(); symbol_id++) {
        for (const auto &edge : machine.graph[cur_vertex]) {
            if (edge.word == str.substr(symbol_id, 1)) {
                cur_vertex = machine.graph[cur_vertex][str[symbol_id] - 'a'].to;
                break;
            }
        }
        if (can_be_pref[cur_vertex]) {
            ans = symbol_id + 1;
        }
    }
    return ans;
}
