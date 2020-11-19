#include<First_practic.h>

std::string true_reg_expr_from_input(std::string input) {
    std::stack<std::string> reg_exprs;
    for (int i = 0; i < input.size(); i++) {
        if ('a' <= input[i] && input[i] <= 'c') {
            reg_exprs.push(input.substr(i, 1));
        } else if (input[i] == '*') {
            if (reg_exprs.size() < 1) {
                throw std::invalid_argument(input);
            }
            std::string a;
            a = reg_exprs.top();
            reg_exprs.pop();
            a = "(" + a + ")*";
            reg_exprs.push(a);
        } else if (input[i] == '+' || input[i] == '.') {
            if (reg_exprs.size() < 2) {
                throw std::invalid_argument(input);
            }
            std::string a, b;
            b = reg_exprs.top();
            reg_exprs.pop();
            a = reg_exprs.top();
            reg_exprs.pop();
            if (input[i] == '.') {
                a = "(" + a + ")(" + b + ")";
            } else {
                a = a + "+" + b;
            }
            reg_exprs.push(a);
        } else {
            throw std::invalid_argument(input);
        }
    }
    if (reg_exprs.size() != 1) {
        throw std::invalid_argument(input);
    }
    return reg_exprs.top();
}

int get_ans(state_machine &machine, std::string s) {
    int n = machine.n;
    std::vector<int> can_be_pref(n, 0);
    std::vector<std::vector<int>> rev_graph(n);
    for (int v = 0; v < n; v++) {
        for (int i = 0; i < machine.graph[v].size(); i++) {
            int u = machine.graph[v][i].to;
            char c = machine.graph[v][i].word[0];
            rev_graph[u].push_back(v);
        }
    }
    std::queue<int> q;
    for (int v = 0; v < n; v++) {
        if (machine.terminals[v]) {
            q.push(v);
            can_be_pref[v] = 1;
        }
    }
    while (!q.empty()) {
        int v = q.front();
        q.pop();
        for (int i = 0; i < rev_graph[v].size(); i++) {
            int u = rev_graph[v][i];
            if (can_be_pref[u] == 0) {
                can_be_pref[u] = 1;
                q.push(u);
            }
        }
    }
    int ans = 0;
    int cur_vertex = machine.start;
    for (int i = 0; i < s.size(); i++) {
        for (int j = 0; j < machine.graph[cur_vertex].size(); j++) {
            if (machine.graph[cur_vertex][j].word == s.substr(i, 1)) {
                cur_vertex = machine.graph[cur_vertex][s[i] - 'a'].to;
                break;
            }
        }
        if (can_be_pref[cur_vertex]) {
            ans = i + 1;
        }
    }
    return ans;
}