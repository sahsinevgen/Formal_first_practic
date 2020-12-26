#include"Determinization.h"

state_machine determinization(const state_machine machine) {
    std::queue<my_bitset> not_processed_sets;
    my_bitset start(machine.size);
    start.set(machine.start, 1);
    not_processed_sets.push(start);

    state_machine ans = state_machine();
    std::map<my_bitset, int> names;
    names[start] = 0;
    ans.start = 0;
    ans.add_vertex();

    std::vector<std::string> alphabet;
    for (int vertex = 0; vertex < machine.size; vertex++) {
        for (const auto &edge : machine.graph[vertex]) {
            alphabet.push_back(edge.word);
        }
    }
    std::sort(alphabet.begin(), alphabet.end());
    alphabet.erase(std::unique(alphabet.begin(), alphabet.end()), alphabet.end());
    my_bitset base_moves[machine.size][alphabet.size()];

    while (!not_processed_sets.empty()) {
        my_bitset now = not_processed_sets.front();
        not_processed_sets.pop();
        my_bitset moves[alphabet.size()];
        for (int vertex = 0; vertex < alphabet.size(); vertex++) {
            moves[vertex] = my_bitset(machine.size);
        }
        bool is_terminal = false;
        int cur_vertex = (now.get(0) ? 0 : now.next_true(0));
        while (cur_vertex != -1) {
            is_terminal |= machine.terminals[cur_vertex];
            for (int symbol_id = 0; symbol_id < alphabet.size(); symbol_id++) {
                if (base_moves[cur_vertex][symbol_id].n == -1) {
                    base_moves[cur_vertex][symbol_id] = my_bitset(machine.size);
                    for (const auto &edge : machine.graph[cur_vertex]) {
                        if (edge.word == alphabet[symbol_id]) {
                            base_moves[cur_vertex][symbol_id].set(edge.to, 1);
                        }
                    }
                }
                moves[symbol_id] |= base_moves[cur_vertex][symbol_id];
            }
            cur_vertex = now.next_true(cur_vertex);
        }
        ans.terminals[names[now]] = is_terminal;
        for (int symbol_id = 0; symbol_id < alphabet.size(); symbol_id++) {
            if (!names.count(moves[symbol_id])) {
                names[moves[symbol_id]] = ans.size;
                ans.add_vertex();
                not_processed_sets.push(moves[symbol_id]);
            }
            ans.add_edge(names[now], names[moves[symbol_id]], alphabet[symbol_id]);
        }
    }
    return ans;
}
