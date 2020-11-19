#include"Determinization.h"

state_machine determinization(const state_machine &g) {
    std::queue<my_bitset> q;
    my_bitset start(g.n);
    start.set(g.start, 1);
    q.push(start);

    state_machine ans = state_machine();
    std::map<my_bitset, int> names;
    names[start] = 0;
    ans.start = 0;
    ans.add_vertex();

    std::vector<std::string> alphabet;
    for (int u = 0; u < g.n; u++) {
        for (int i = 0; i < g.graph[u].size(); i++) {
            alphabet.push_back(g.graph[u][i].word);
        }
    }
    std::sort(alphabet.begin(), alphabet.end());
    alphabet.erase(std::unique(alphabet.begin(), alphabet.end()), alphabet.end());
    my_bitset base_moves[g.n][alphabet.size()];

    while (!q.empty()) {
        my_bitset now = q.front();
        q.pop();
        my_bitset moves[alphabet.size()];
        for (int j = 0; j < alphabet.size(); j++) {
            moves[j] = my_bitset(g.n);
        }
        bool is_terminal = false;
        int u = (now.get(0) ? 0 : now.next_true(0));
        while (u != -1) { 
            is_terminal |= g.terminals[u];
            for (int j = 0; j < alphabet.size(); j++) {
                if (base_moves[u][j].n == -1) {
                    base_moves[u][j] = my_bitset(g.n);
                    for (int i = 0; i < g.graph[u].size(); i++) {
                        if (g.graph[u][i].word == alphabet[j]) {
                            base_moves[u][j].set(g.graph[u][i].to, 1);
                        }
                    }
                }
                moves[j] |= base_moves[u][j];
            }
            u = now.next_true(u);
        }
        ans.terminals[names[now]] = is_terminal;
        for (int j = 0; j < alphabet.size(); j++) {
            if (!names.count(moves[j])) {
                names[moves[j]] = ans.n;
                ans.add_vertex();
                q.push(moves[j]);
            }
            ans.add_edge(names[now], names[moves[j]], alphabet[j]);
        }
    }
    return ans;
}