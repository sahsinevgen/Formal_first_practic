#include"First_practic.h"

std::string true_reg_expr_from_input(std::string input) {
    std::stack<std::string> reg_exprs;
    for (int i = 0; i < input.size(); i++) {
        if ('a' <= input[i] && input[i] <= 'c') {
            reg_exprs.push(input.substr(i, 1));
        } else if (input[i] == '*') {
            if (reg_exprs.size() < 1) {
                throw std::invalid_argument(input);
            }
            std::string arg;
            arg = reg_exprs.top();
            reg_exprs.pop();
            arg = "(" + arg + ")*";
            reg_exprs.push(arg);
        } else if (input[i] == '+' || input[i] == '.') {
            if (reg_exprs.size() < 2) {
                throw std::invalid_argument(input);
            }
            std::string first_arg, second_arg;
            second_arg = reg_exprs.top();
            reg_exprs.pop();
            first_arg = reg_exprs.top();
            reg_exprs.pop();
            if (input[i] == '.') {
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
    int n = machine.n;
    std::vector<int> can_be_pref(n, 0);
    std::vector<std::vector<int>> rev_graph(n);
    for (int v = 0; v < n; v++) {
        for (int i = 0; i < machine.graph[v].size(); i++) {
            int u = machine.graph[v][i].to;
            rev_graph[u].push_back(v);
        }
    }
    std::queue<int> not_processed_vertex;
    for (int v = 0; v < n; v++) {
        if (machine.terminals[v]) {
            not_processed_vertex.push(v);
            can_be_pref[v] = 1;
        }
    }
    while (!not_processed_vertex.empty()) {
        int v = not_processed_vertex.front();
        not_processed_vertex.pop();
        for (int i = 0; i < rev_graph[v].size(); i++) {
            int u = rev_graph[v][i];
            if (can_be_pref[u] == 0) {
                can_be_pref[u] = 1;
                not_processed_vertex.push(u);
            }
        }
    }
    int ans = 0;
    int cur_vertex = machine.start;
    for (int i = 0; i < str.size(); i++) {
        for (int j = 0; j < machine.graph[cur_vertex].size(); j++) {
            if (machine.graph[cur_vertex][j].word == str.substr(i, 1)) {
                cur_vertex = machine.graph[cur_vertex][str[i] - 'a'].to;
                break;
            }
        }
        if (can_be_pref[cur_vertex]) {
            ans = i + 1;
        }
    }
    return ans;
}
