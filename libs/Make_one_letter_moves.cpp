#include"Make_one_letter_moves.h"

void dfs_on_eps_edges(const state_machine &machine, 
                      std::vector<int> &used,
                      int used_colour,
                      int root, 
                      int u,
                      state_machine &new_machine) {

    if (machine.terminals[u]) {
        new_machine.terminals[root] = 1;
    }
    for (int i = 0; i < machine.graph[u].size(); i++) {
        int v = machine.graph[u][i].to;
        std::string word = machine.graph[u][i].word;
        if (word == "") {
            if (used[v] != used_colour) {
                used[v] = used_colour;
                dfs_on_eps_edges(machine, 
                                 used, 
                                 used_colour, 
                                 root, 
                                 v, 
                                 new_machine);
            }
        } else {
            if (u != root) {
                new_machine.add_edge(root, v, word);
            }
        }
    }
}

state_machine make_one_letter_moves(const state_machine &machine) {
    state_machine ans_machine = machine;
    std::vector<int> used(machine.n, -1);
    for (int u = 0; u < machine.n; u++) {
        used[u] = u;
        dfs_on_eps_edges(machine, used, u, u, u, ans_machine);
    }
    for (int u = 0; u < ans_machine.n; u++) {
        for (int i = 0; i < ans_machine.graph[u].size(); ) {
            if (ans_machine.graph[u][i].word == "") {
                ans_machine.graph[u].erase(ans_machine.graph[u].begin() + i);
            } else {
                i++;
            }
        }
    }
    for (int u = 0; u < ans_machine.n; u++) {
        std::sort(ans_machine.graph[u].begin(), ans_machine.graph[u].end());
        ans_machine.graph[u].erase(std::unique(ans_machine.graph[u].begin(), 
                                               ans_machine.graph[u].end()), 
                                   ans_machine.graph[u].end());
    }
    int cnt = 0;
    for (int u = 0; u < ans_machine.n; u++) {
        for (int i = 0; i < ans_machine.graph[u].size(); i++) {
            int v = ans_machine.graph[u][i].to;
            std::string word = ans_machine.graph[u][i].word;
            if (word.length() > 1) {
                ans_machine.graph[u][i] = edge(ans_machine.n, word[0]);
                ans_machine.add_vertex();
                for (int j = 1; j < word.size() - 1; j++) {
                    ans_machine.add_edge(ans_machine.n - 1, 
                                         ans_machine.n, 
                                         word[j]);
                    ans_machine.add_vertex();
                }
                ans_machine.add_edge(ans_machine.n - 1, v, word.back());
            }
        }
    }
    return ans_machine;
}
