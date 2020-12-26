#include"Make_one_letter_moves.h"

void dfs_on_eps_edges(const state_machine &machine, 
                      std::vector<int> &used,
                      int used_colour,
                      int root, 
                      int vertex,
                      state_machine &new_machine) {

    if (machine.terminals[vertex]) {
        new_machine.terminals[root] = 1;
    }
    for (auto &edge : machine.graph[vertex]) {
        int next_vertex = edge.to;
        std::string word = edge.word;
        if (word == "") {
            if (used[next_vertex] != used_colour) {
                used[next_vertex] = used_colour;
                dfs_on_eps_edges(machine, 
                                 used, 
                                 used_colour, 
                                 root, 
                                 next_vertex, 
                                 new_machine);
            }
        } else {
            if (next_vertex != root) {
                new_machine.add_edge(root, vertex, word);
            }
        }
    }
}

state_machine make_one_letter_moves(const state_machine &machine) {
    state_machine ans_machine = machine;
    std::vector<int> used(machine.size, -1);
    for (int vertex = 0; vertex < machine.size; vertex++) {
        used[vertex] = vertex;
        dfs_on_eps_edges(machine, used, vertex, vertex, vertex, ans_machine);
    }
    for (int vertex = 0; vertex < ans_machine.size; vertex++) {
        for (int edge_id = 0; edge_id < ans_machine.graph[vertex].size(); 
                              edge_id++) {
            if (ans_machine.graph[vertex][edge_id].word == "") {
                ans_machine.graph[vertex].erase(ans_machine.graph[vertex].begin() + edge_id);
            } else {
                edge_id++;
            }
        }
    }
    for (auto &edges_list : ans_machine.graph) {
        std::sort(edges_list.begin(), edges_list.end());
        edges_list.erase(std::unique(edges_list.begin(), edges_list.end()), 
                         edges_list.end());
    }
    int cnt = 0;
    for (int vertex = 0; vertex < ans_machine.size; vertex++) {
        for (auto &edge : ans_machine.graph[vertex]) {
            int vertex_to = edge.to;
            std::string word = edge.word;
            if (word.length() > 1) {
                edge = Edge(ans_machine.size, word[0]);
                ans_machine.add_vertex();
                for (int symbol_id = 1; symbol_id < word.size() - 1; 
                                        symbol_id++) {
                    ans_machine.add_edge(ans_machine.size - 1, 
                                         ans_machine.size, 
                                         word[symbol_id]);
                    ans_machine.add_vertex();
                }
                ans_machine.add_edge(ans_machine.size - 1, vertex_to, word.back());
            }
        }
    }
    return ans_machine;
}
