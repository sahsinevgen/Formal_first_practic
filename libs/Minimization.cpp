#include"Minimization.h"

bool cmp(edge first, edge second) {
    return first.word < second.word;
}

state_machine minimization(state_machine machine) {
    std::vector<std::string> alphabet;
    for (int u = 0; u < machine.n; u++) {
        std::sort(machine.graph[u].begin(), machine.graph[u].end(), cmp);
    }
    for (int i = 0; i < machine.graph[0].size(); i++) {
        alphabet.push_back(machine.graph[0][i].word);
    }

    std::vector<int> colours(machine.n);
    std::vector<std::vector<int> > moves_colours(
                                    machine.n, 
                                    std::vector<int>(alphabet.size()));
    for (int u = 0; u < machine.n; u++) {
        colours[u] = machine.terminals[u];
    }
    while (true) {
        std::vector<int> new_colours(machine.n);
        for (int u = 0; u < machine.n; u++) {
            for (int i = 0; i < machine.graph[u].size(); i++) {
                int v = machine.graph[u][i].to;
                moves_colours[u][i] = colours[v];
            }
        }
        int cnt = 0;
        for (int u = 0; u < machine.n; u++) {
            bool vertex_repeats = false;
            for (int i = 0; i < u; i++) {
                if (colours[u] == colours[i] && 
                    moves_colours[u] == moves_colours[i]) {
                    
                    new_colours[u] = new_colours[i];
                    vertex_repeats = true;
                    break;
                }
            }
            if (!vertex_repeats) {
                new_colours[u] = cnt++;
            }
        }
        if (new_colours == colours) {
            break;
        } else {
            colours = new_colours;
        }
    }
    std::vector<int> first_colour_entry(machine.n, -1);
    for (int u = 0; u < machine.n; u++) {
        if (first_colour_entry[colours[u]] == -1) {
            first_colour_entry[colours[u]] = u;
        }
    }
    state_machine ans_machine = state_machine();
    ans_machine.start = first_colour_entry[colours[machine.start]];
    for (int u = 0; u < machine.n; u++) {
        if (first_colour_entry[colours[u]] != u) {
            continue;
        }
        ans_machine.add_vertex();
        ans_machine.terminals[colours[u]] = machine.terminals[u];
        for (int i = 0; i < alphabet.size(); i++) {
            ans_machine.add_edge(colours[u], 
                                 colours[machine.graph[u][i].to], 
                                         machine.graph[u][i].word);
        }
    }
    return ans_machine;
}
