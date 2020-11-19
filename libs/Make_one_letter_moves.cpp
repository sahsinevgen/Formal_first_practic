#include"Make_one_letter_moves.h"

void dfs(const state_machine &g, 
         std::vector<int> &used,
         int used_colour,
         int root, 
         int u,
         state_machine &new_g) {

    if (g.terminals[u]) {
        new_g.terminals[root] = 1;
    }
    for (int i = 0; i < g.graph[u].size(); i++) {
        int v = g.graph[u][i].to;
        std::string word = g.graph[u][i].word;
        if (word == "") {
            if (used[v] != used_colour) {
                used[v] = used_colour;
                dfs(g, used, used_colour, root, v, new_g);
            }
        } else {
            if (u != root) {
                new_g.add_edge(root, v, word);
            }
        }
    }
}

state_machine make_one_letter_moves(const state_machine &g) {
    state_machine ans = g;
    std::vector<int> used(g.n, -1);
    for (int u = 0; u < g.n; u++) {
        used[u] = u;
        dfs(g, used, u, u, u, ans);
    }
    for (int u = 0; u < ans.n; u++) {
        for (int i = 0; i < ans.graph[u].size(); ) {
            if (ans.graph[u][i].word == "") {
                ans.graph[u].erase(ans.graph[u].begin() + i);
            } else {
                i++;
            }
        }
    }
    for (int u = 0; u < ans.n; u++) {
        std::sort(ans.graph[u].begin(), ans.graph[u].end());
        ans.graph[u].erase(std::unique(ans.graph[u].begin(), ans.graph[u].end()), ans.graph[u].end());
    }
    int cnt = 0;
    for (int u = 0; u < ans.n; u++) {
        for (int i = 0; i < ans.graph[u].size(); i++) {
            int v = ans.graph[u][i].to;
            std::string s = ans.graph[u][i].word;
            if (s.length() > 1) {
                ans.graph[u][i] = edge(ans.n, s[0]);
                ans.add_vertex();
                for (int j = 1; j < s.size() - 1; j++) {
                    ans.add_edge(ans.n - 1, ans.n, s[j]);
                    ans.add_vertex();
                }
                ans.add_edge(ans.n - 1, v, s.back());
            }
        }
    }
    return ans;
}