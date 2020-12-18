#include"State_machine.h"

edge::edge(int to_, std::string word_) {
    to = to_;
    word = word_;
}

edge::edge(int to_, char letter) {
    to = to_;
    word = "";
    word += letter;
}

bool edge::operator < (edge other) {
    if (to != other.to) {
        return to < other.to;
    }
    return word < other.word;
}

bool edge::operator == (edge other) {
    return to == other.to && word == other.word;
}


state_machine::state_machine() {
    n = 0;
    start = -1;
}

state_machine::state_machine(const state_machine &other) {
    graph = other.graph;
    terminals = other.terminals;
    n = other.n;
    start = other.start;
}

state_machine& state_machine::operator=(const state_machine &other) {
    graph = other.graph;
    terminals = other.terminals;
    n = other.n;
    start = other.start;
    return *this;
}

void state_machine::add_edge(int from, int to, std::string word) {
    graph[from].emplace_back(to, word);
}

void state_machine::add_edge(int from, int to, char letter) {
    graph[from].emplace_back(to, letter);
}

void state_machine::add_edge(int from, edge x) {
    graph[from].emplace_back(x);
}

void state_machine::add_vertex(int cnt) {
    n += cnt;
    graph.resize(n, std::vector<edge>(0));
    terminals.resize(n, 0);
}

bool state_machine::has_word(std::string word) {
    my_bitset current_set(n);
    current_set.set(start, 1);
    for (int i = 0; i < word.size(); i++) {
        my_bitset next_set(word.size());
        int cur_vertex = (current_set.get(0) 
                          ? 0 
                          : current_set.next_true(0));
        while (cur_vertex != -1) {
            for (int j = 0; j < graph[cur_vertex].size(); j++) {
                if (graph[cur_vertex][j].word[0] == word[i]) {
                    next_set.set(graph[cur_vertex][j].to, 1);
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
    int n;
    machine = state_machine();
    in >> n >> machine.start;
    machine.start--;
    machine.add_vertex(n);
    int cnt_terminals;
    in >> cnt_terminals;
    for (int i = 0; i < cnt_terminals; i++) {
        int u;
        in >> u;
        u--;
        machine.terminals[u] = 1;
    }
    int m;
    in >> m;
    for (int i = 0; i < m; i++) {
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
    out << machine.n << "\n" << machine.start + 1 << "\n";
    int cnt_terminals = 0;
    for (int u = 0; u < machine.n; u++) {
        if (machine.terminals[u]) {
            cnt_terminals++;
        }
    }
    out << cnt_terminals << "\n";
    for (int u = 0; u < machine.n; u++) {
        if (machine.terminals[u]) {
            out << u + 1 << " ";
        }
    }
    out << "\n";
    int m = 0;
    for (int u = 0; u < machine.n; u++) {
        m += machine.graph[u].size();
    }
    out << m << "\n";
    for (int from = 0; from < machine.n; from++) {
        for (int i = 0; i < machine.graph[from].size(); i++) {
            std::string move = machine.graph[from][i].word;
            if (move == "") {
                move = "#";
            }
            out << from + 1 << " " << machine.graph[from][i].to + 1 
                            << " " << move << "\n";
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
    for (int i = 0; i < first_machine.graph[first_state].size(); i++) {
        int first_state = first_machine.graph[first_state][i].to;
        int second_state = second_machine.graph[second_state][i].to;
        if (first_machine.graph[first_state][i].word != 
            second_machine.graph[second_state][i].word || 
            *(used + first_machine.n * first_state + second_state)) {
            continue;
        }
        *(used + second_machine.n * first_state + second_state) = 1;
        if (len + 1 < first_machine.n + second_machine.n) {
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
    int used[first_machine.n][second_machine.n];
    for (int first_state = 0; first_state < first_machine.n; first_state++) {
        for (int second_state = 0; second_state < second_machine.n; second_state++) {
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
    for (int first_state = 0; first_state < first_machine.n; first_state++) {
        for (int second_state = 0; second_state < second_machine.n; second_state++) {
            if (used[first_state][second_state] && 
                first_machine.terminals[first_state] != 
                second_machine.terminals[second_state]) {
                return false;
            }
        }
    }
    return true;
}
