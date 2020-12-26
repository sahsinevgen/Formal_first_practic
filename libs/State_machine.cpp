#include"State_machine.h"

Edge::Edge(int to_, std::string word_) {
    to = to_;
    word = word_;
}

Edge::Edge(int to_, char letter) {
    to = to_;
    word = "";
    word += letter;
}

bool Edge::operator < (Edge other) {
    if (to != other.to) {
        return to < other.to;
    }
    return word < other.word;
}

bool Edge::operator == (Edge other) {
    return to == other.to && word == other.word;
}


state_machine::state_machine() {
    size = 0;
    start = -1;
}

state_machine::state_machine(const state_machine &other) {
    graph = other.graph;
    terminals = other.terminals;
    size = other.size;
    start = other.start;
}

state_machine& state_machine::operator=(const state_machine &other) {
    graph = other.graph;
    terminals = other.terminals;
    size = other.size;
    start = other.start;
    return *this;
}

void state_machine::add_edge(int from, int to, std::string word) {
    graph[from].emplace_back(to, word);
}

void state_machine::add_edge(int from, int to, char letter) {
    graph[from].emplace_back(to, letter);
}

void state_machine::add_edge(int from, Edge edge) {
    graph[from].emplace_back(edge);
}

void state_machine::add_vertex(int cnt) {
    size += cnt;
    graph.resize(size, std::vector<Edge>(0));
    terminals.resize(size, 0);
}

bool state_machine::has_word(std::string word) {
    my_bitset current_set(size);
    current_set.set(start, 1);
    for (auto symbol : word) {
        my_bitset next_set(word.size());
        int cur_vertex = (current_set.get(0) 
                          ? 0 
                          : current_set.next_true(0));
        while (cur_vertex != -1) {
            for (const auto edge : graph[cur_vertex]) {
                if (edge.word[0] == symbol) {
                    next_set.set(edge.to, 1);
                }
            }
            cur_vertex = current_set.next_true(cur_vertex);
        }
        current_set = next_set;
    }
    int cur_vertex = (current_set.get(0) 
                      ? 0 
                      : current_set.next_true(0));
    while (cur_vertex != -1) {
        if (terminals[cur_vertex]) {
            return true;
        }
        cur_vertex = current_set.next_true(cur_vertex);
    }
    return false;
}

std::istream& operator>>(std::istream& in, state_machine& machine) {
    int size;
    machine = state_machine();
    in >> size >> machine.start;
    machine.start--;
    machine.add_vertex(size);
    int cnt_terminals;
    in >> cnt_terminals;
    while (cnt_terminals--) {
        int vertex;
        in >> vertex;
        vertex--;
        machine.terminals[vertex] = 1;
    }
    int edges_cnt;
    in >> edges_cnt;
    while (edges_cnt--) {
        int from, to;
        std::string word;
        in >> from >> to >> word;
        from--;
        to--;
        if (word == "#") {
            word = "";
        }
        machine.add_edge(from, to, word);
    }
    return in;
}

std::ostream& operator<<(std::ostream& out, const state_machine& machine) {
    out << machine.size << "\n" << machine.start + 1 << "\n";
    int cnt_terminals = 0;
    for (int vertex = 0; vertex < machine.size; vertex++) {
        if (machine.terminals[vertex]) {
            cnt_terminals++;
        }
    }
    out << cnt_terminals << "\n";
    for (int vertex = 0; vertex < machine.size; vertex++) {
        if (machine.terminals[vertex]) {
            out << vertex + 1 << " ";
        }
    }
    out << "\n";
    int edges_cnt = 0;
    for (int vertex = 0; vertex < machine.size; vertex++) {
        edges_cnt += machine.graph[vertex].size();
    }
    out << edges_cnt << "\n";
    for (int from_vertex = 0; from_vertex < machine.size; from_vertex++) {
        for (const auto &edge : machine.graph[from_vertex]) {
            std::string move = edge.word;
            if (move == "") {
                move = "#";
            }
            out << from_vertex + 1 << " " << edge.to + 1 << " " << move << "\n";
        }
    }

    return out;
}

void dfs_for_equiv(const state_machine &first_machine, 
                   const state_machine &second_machine, 
                   int* used, 
                   int first_state, 
                   int second_state, 
                   int len) {
    for (int vertex = 0; vertex < first_machine.graph[first_state].size(); 
                         vertex++) {
        Edge first_edge = first_machine.graph[first_state][vertex];
        Edge second_edge = second_machine.graph[second_state][vertex];
        int first_state = first_edge.to;
        int second_state = second_edge.to;
        if (first_edge.word != second_edge.word || 
            *(used + first_machine.size * first_state + second_state)) {
            continue;
        }
        *(used + second_machine.size * first_state + second_state) = 1;
        if (len + 1 < first_machine.size + second_machine.size) {
            dfs_for_equiv(first_machine, 
                          second_machine, 
                          used, 
                          first_state, 
                          second_state, 
                          len + 1);
        }
    }
}

bool are_equiv(state_machine first_machine, state_machine second_machine) {
    int used[first_machine.size][second_machine.size];
    for (int first_state = 0; first_state < first_machine.size; first_state++) {
        for (int second_state = 0; second_state < second_machine.size; 
                                   second_state++) {
            used[first_state][second_state] = 0;
        }
    }
    used[first_machine.start][second_machine.start];
    dfs_for_equiv(first_machine, 
                  second_machine, 
                  (int*) used, 
                  first_machine.start, 
                  second_machine.start, 
                  0);
    for (int first_state = 0; first_state < first_machine.size; first_state++) {
        for (int second_state = 0; second_state < second_machine.size; 
                                   second_state++) {
            if (used[first_state][second_state] && 
                first_machine.terminals[first_state] != 
                second_machine.terminals[second_state]) {
                return false;
            }
        }
    }
    return true;
}
