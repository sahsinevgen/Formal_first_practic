#include"Reg_expr_to_state_machine.h"

void add_number_to_all_edges(state_machine &g, int inc) {
    for (int u = 0; u < g.n; u++) {
        for (int i = 0; i < g.graph[u].size(); i++) {
            g.graph[u][i].to += inc;
        }
    }
}

state_machine state_machine_with_one_move(std::string move) {
    state_machine ans;
    ans.add_vertex(2);
    ans.start = 0;
    ans.terminals[1] = 1;
    ans.add_edge(0, 1, move);
    return ans;
}

state_machine add_kleene_closure(state_machine a) {
    state_machine ans = a;
    ans.add_vertex(2);
    int new_start = ans.n - 2;
    int new_terminal = ans.n - 1;
    ans.add_edge(new_start, ans.start, "");
    ans.add_edge(ans.start, new_terminal, "");
    for (int u = 0; u < ans.n; u++) {
        if (ans.terminals[u]) {
            ans.add_edge(u, ans.start, "");
            ans.terminals[u] = 0;
        }
    }

    ans.start = new_start;
    ans.terminals[new_terminal] = 1;
    
    return ans;
}

state_machine concatenation(state_machine a, state_machine b) {
    int n_last = a.n;
    a.add_vertex(b.n);
    add_number_to_all_edges(b, n_last);
    for (int u = 0; u < n_last; u++) {
        if (a.terminals[u]) {
            a.add_edge(u, b.start + n_last, "");
            a.terminals[u] = 0;
        }
    }
    for (int u = 0; u < b.n; u++) {
        a.graph[n_last + u] = b.graph[u];
        a.terminals[n_last + u] = b.terminals[u];
    }
    return a;
}

state_machine sum(state_machine a, state_machine b) {

    int n_last = a.n;
    a.add_vertex(b.n + 2);
    int new_start = a.n - 2;
    int new_terminal = a.n - 1;
    add_number_to_all_edges(b, n_last);
    for (int u = 0; u < b.n; u++) {
        a.graph[n_last + u] = b.graph[u];
        a.terminals[n_last + u] = b.terminals[u];
    }
    a.add_edge(new_start, a.start, "");
    a.add_edge(new_start, b.start + n_last, "");
    for (int u = 0; u < a.n; u++) {
        if (a.terminals[u]) {
            a.add_edge(u, new_terminal, "");
            a.terminals[u] = 0;
        }
    }
    a.start = new_start;
    a.terminals[new_terminal] = 1;
    return a;
}

state_machine expr(const std::string &s, int &i);

state_machine term(const std::string &s, int &i) {
    state_machine ans;
    bool is_first = true;
    while (i != s.size() && s[i] != '+' && s[i] != ')') {
        //std::cout << i << std::endl;
        state_machine part;
        if (s[i] == '(') {
            i++;
            part = expr(s, i);
            i++;
        } else {
            std::string move = "";
            if (s[i] != '#') {
                move += s[i];
            }
            part = state_machine_with_one_move(move);
            i++;
        }
        if (i != s.size() && s[i] == '*') {
            part = add_kleene_closure(part);
            i++;
        }
        if (is_first) {
            ans = part;
            is_first = false;
        } else {
            ans = concatenation(ans, part);
        }
    }
    return ans;
}

state_machine expr(const std::string &s, int &i) {
    state_machine ans;
    ans = term(s, i);
    while (i != s.size() && s[i] == '+') {
        i++;
        ans = sum(ans, term(s, i));
    }
    return ans;
}

state_machine reg_expr_to_state_machine(const std::string &s) {
    int i = 0;
    return expr(s, i);
}