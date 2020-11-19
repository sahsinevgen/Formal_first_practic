#include"Minimization.h"

bool cmp(edge a, edge b) {
    return a.word < b.word;
}

state_machine minimization(state_machine g) {
    std::vector<std::string> alphabet;
    for (int u = 0; u < g.n; u++) {
        std::sort(g.graph[u].begin(), g.graph[u].end(), cmp);
    }
    for (int i = 0; i < g.graph[0].size(); i++) {
        alphabet.push_back(g.graph[0][i].word);
    }

    std::vector<int> colours(g.n);
    std::vector<std::vector<int> > moves_colours(g.n, std::vector<int>(alphabet.size()));
    for (int u = 0; u < g.n; u++) {
        colours[u] = g.terminals[u];
    }
    while (true) {
        std::vector<int> new_colours(g.n);
        for (int u = 0; u < g.n; u++) {
            for (int i = 0; i < g.graph[u].size(); i++) {
                int v = g.graph[u][i].to;
                moves_colours[u][i] = colours[v];
            }
        }
        int cnt = 0;
        for (int u = 0; u < g.n; u++) {
            bool fl = false;
            for (int i = 0; i < u; i++) {
                if (colours[u] == colours[i] && moves_colours[u] == moves_colours[i]) {
                    new_colours[u] = new_colours[i];
                    fl = true;
                    break;
                }
            }
            if (!fl) {
                new_colours[u] = cnt++;
            }
        }
        if (new_colours == colours) {
            break;
        } else {
            colours = new_colours;
        }
    }
    std::vector<int> first_colour_entry(g.n, -1);
    for (int u = 0; u < g.n; u++) {
        if (first_colour_entry[colours[u]] == -1) {
            first_colour_entry[colours[u]] = u;
        }
    }
    state_machine ans = state_machine();
    ans.start = first_colour_entry[colours[g.start]];
    for (int u = 0; u < g.n; u++) {
        if (first_colour_entry[colours[u]] != u) {
            continue;
        }
        ans.add_vertex();
        ans.terminals[colours[u]] = g.terminals[u];
        for (int i = 0; i < alphabet.size(); i++) {
            ans.add_edge(colours[u], colours[g.graph[u][i].to], g.graph[u][i].word);
        }
    }
    return ans;
}