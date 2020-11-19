#include"State_machine.h"

edge::edge(int to_, std::string word_) {
    to = to_;
    word = word_;
}

edge::edge(int to_, char c) {
    to = to_;
    word = "";
    word += c;
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

void state_machine::add_edge(int from, int to, char c) {
    graph[from].emplace_back(to, c);
}

void state_machine::add_edge(int from, edge x) {
    graph[from].emplace_back(x);
}

void state_machine::add_vertex(int cnt) {
    n += cnt;
    graph.resize(n, std::vector<edge>(0));
    terminals.resize(n, 0);
}

bool state_machine::has_word(std::string s) {
    my_bitset now(n);
    now.set(start, 1);
    for (int i = 0; i < s.size(); i++) {
        my_bitset next(s.size());
        int cur_vertex = (now.get(0) ? 0 : now.next_true(0));
        while (cur_vertex != -1) {
            for (int j = 0; j < graph[cur_vertex].size(); j++) {
                if (graph[cur_vertex][j].word[0] == s[i]) {
                    next.set(graph[cur_vertex][j].to, 1);
                }
            }
            cur_vertex = now.next_true(cur_vertex);
        }
        now = next;
    }
    int cur_vertex = (now.get(0) ? 0 : now.next_true(0));
    while (cur_vertex != -1) {
        if (terminals[cur_vertex]) {
            return true;
        }
        cur_vertex = now.next_true(cur_vertex);
    }
    return false;
}

std::istream& operator>>(std::istream& in, state_machine& g) {
    int n;
    g = state_machine();
    in >> n >> g.start;
    g.start--;
    g.add_vertex(n);
    int cnt_terminals;
    in >> cnt_terminals;
    for (int i = 0; i < cnt_terminals; i++) {
        int u;
        in >> u;
        u--;
        g.terminals[u] = 1;
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
        g.add_edge(from, to, word);
    }
    return in;
}

std::ostream& operator<<(std::ostream& out, const state_machine& g) {
    
    out << g.n << "\n" << g.start + 1 << "\n";
    int cnt_terminals = 0;
    for (int u = 0; u < g.n; u++) {
        if (g.terminals[u]) {
            cnt_terminals++;
        }
    }
    out << cnt_terminals << "\n";
    for (int u = 0; u < g.n; u++) {
        if (g.terminals[u]) {
            out << u + 1 << " ";
        }
    }
    out << "\n";
    int m = 0;
    for (int u = 0; u < g.n; u++) {
        m += g.graph[u].size();
    }
    out << m << "\n";
    for (int from = 0; from < g.n; from++) {
        for (int i = 0; i < g.graph[from].size(); i++) {
            std::string move = g.graph[from][i].word;
            if (move == "") {
                move = "#";
            }
            out << from + 1 << " " << g.graph[from][i].to + 1 << " " << move << "\n";
        }
    }

    return out;
}

void dfs_for_equiv(const state_machine &a, const state_machine &b, int* used, int state_in_a, int state_in_b, int len) {
    for (int i = 0; i < a.graph[state_in_a].size(); i++) {
        int new_state_in_a = a.graph[state_in_a][i].to;
        int new_state_in_b = b.graph[state_in_b][i].to;
        if (a.graph[state_in_a][i].word != b.graph[state_in_b][i].word || *(used + a.n * new_state_in_a + new_state_in_b)) {
            continue;
        }
        *(used + b.n * new_state_in_a + new_state_in_b) = 1;
        if (len + 1 < a.n + b.n) {
            dfs_for_equiv(a, b, used, new_state_in_a, new_state_in_b, len + 1);
        }
    }
}

bool are_equiv(state_machine a, state_machine b) {
    int used[a.n][b.n];
    for (int state_in_a = 0; state_in_a < a.n; state_in_a++) {
        for (int state_in_b = 0; state_in_b < b.n; state_in_b++) {
            used[state_in_a][state_in_b] = 0;
        }
    }
    used[a.start][b.start];
    dfs_for_equiv(a, b, (int*) used, a.start, b.start, 0);
    for (int state_in_a = 0; state_in_a < a.n; state_in_a++) {
        for (int state_in_b = 0; state_in_b < b.n; state_in_b++) {
            if (used[state_in_a][state_in_b] && a.terminals[state_in_a] != b.terminals[state_in_b]) {
                return false;
            }
        }
    }
    return true;
}