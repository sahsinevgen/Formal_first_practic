#include"Determinization.h"

state_machine determinization(const state_machine machine) {
    std::queue<my_bitset> not_processed_sets;
    my_bitset start(machine.n);
    start.set(machine.start, 1);
    not_processed_sets.push(start);

    state_machine ans = state_machine();
    std::map<my_bitset, int> names;
    names[start] = 0;
    ans.start = 0;
    ans.add_vertex();

    std::vector<std::string> alphabet;
    for (int u = 0; u < machine.n; u++) {
        for (int i = 0; i < machine.graph[u].size(); i++) {
            alphabet.push_back(machine.graph[u][i].word);
        }
    }
    std::sort(alphabet.begin(), alphabet.end());
    alphabet.erase(std::unique(alphabet.begin(), alphabet.end()), alphabet.end());
    my_bitset base_moves[machine.n][alphabet.size()];

    while (!not_processed_sets.empty()) {
        my_bitset now = not_processed_sets.front();
        not_processed_sets.pop();
        my_bitset moves[alphabet.size()];
        for (int j = 0; j < alphabet.size(); j++) {
            moves[j] = my_bitset(machine.n);
        }
        bool is_terminal = false;
        int u = (now.get(0) ? 0 : now.next_true(0));
        while (u != -1) {
            is_terminal |= machine.terminals[u];
            for (int j = 0; j < alphabet.size(); j++) {
                if (base_moves[u][j].n == -1) {
                    base_moves[u][j] = my_bitset(machine.n);
                    for (int i = 0; i < machine.graph[u].size(); i++) {
                        if (machine.graph[u][i].word == alphabet[j]) {
                            base_moves[u][j].set(machine.graph[u][i].to, 1);
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
                not_processed_sets.push(moves[j]);
            }
            ans.add_edge(names[now], names[moves[j]], alphabet[j]);
        }
    }
    return ans;
}
