#include"Minimization.h"

bool cmp(Edge first, Edge second) {
    return first.word < second.word;
}

state_machine minimization(state_machine machine) {
    std::vector<std::string> alphabet;
    for (auto &edges_list : machine.graph) {
        std::sort(edges_list.begin(), edges_list.end(), cmp);
    }
    for (const auto &edge : machine.graph[0]) {
        alphabet.push_back(edge.word);
    }

    std::vector<int> colours(machine.size);
    std::vector<std::vector<int> > moves_colours(
                                    machine.size, 
                                    std::vector<int>(alphabet.size()));
    for (int vertex = 0; vertex < machine.size; vertex++) {
        colours[vertex] = machine.terminals[vertex];
    }
    while (true) {
        std::vector<int> new_colours(machine.size);
        for (int vertex = 0; vertex < machine.size; vertex++) {
            for (int edge_id = 0; edge_id < machine.graph[vertex].size(); 
                                  edge_id++) {
                int vertex_to = machine.graph[vertex][edge_id].to;
                moves_colours[vertex][edge_id] = colours[vertex_to];
            }
        }
        int cnt = 0;
        for (int vertex = 0; vertex < machine.size; vertex++) {
            bool vertex_repeats = false;
            for (int prev_vertex = 0; prev_vertex < vertex; 
                                      prev_vertex++) {
                if (colours[vertex] == colours[prev_vertex] && 
                    moves_colours[vertex] == moves_colours[prev_vertex]) {
                    
                    new_colours[vertex] = new_colours[prev_vertex];
                    vertex_repeats = true;
                    break;
                }
            }
            if (!vertex_repeats) {
                new_colours[vertex] = cnt++;
            }
        }
        if (new_colours == colours) {
            break;
        } else {
            colours = new_colours;
        }
    }
    std::vector<int> first_colour_entry(machine.size, -1);
    for (int vertex = 0; vertex < machine.size; vertex++) {
        if (first_colour_entry[colours[vertex]] == -1) {
            first_colour_entry[colours[vertex]] = vertex;
        }
    }
    state_machine ans_machine = state_machine();
    ans_machine.start = first_colour_entry[colours[machine.start]];
    for (int vertex = 0; vertex < machine.size; vertex++) {
        if (first_colour_entry[colours[vertex]] != vertex) {
            continue;
        }
        ans_machine.add_vertex();
        ans_machine.terminals[colours[vertex]] = machine.terminals[vertex];
        for (int edge_id = 0; edge_id < alphabet.size(); edge_id++) {
            ans_machine.add_edge(colours[vertex], 
                                 colours[machine.graph[vertex][edge_id].to], 
                                         machine.graph[vertex][edge_id].word);
        }
    }
    return ans_machine;
}
