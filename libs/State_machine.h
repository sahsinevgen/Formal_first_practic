#ifndef State_machine
#define State_machine

#include"My_bitset.h"
#include<vector>
#include<string>
#include<iostream>

class edge {
public:
    int to;
    std::string word;
    
    edge(int to_ = -1, std::string word_ = "");
    
    edge(int to_, char letter);

    bool operator < (edge other);

    bool operator == (edge other);
};

class state_machine {
public:
    std::vector<std::vector<edge> > graph;
    std::vector<int> terminals;
    int n;
    int start;

    state_machine();

    state_machine(const state_machine &other);

    state_machine& operator=(const state_machine &other);

    void add_edge(int from, int to, std::string word);

    void add_edge(int from, int to, char letter);

    void add_edge(int from, edge x);

    void add_vertex(int cnt = 1);

    bool has_word(std::string word); // works only for machines with 1-letter moves without epsilon-moves
};

std::istream& operator>>(std::istream& in, state_machine& machine);

std::ostream& operator<<(std::ostream& out, const state_machine& machine);

void dfs_for_equiv(const state_machine &first_machine, 
                   const state_machine &second_machine, 
                   int* used, 
                   int first_state, 
                   int second_state, 
                   int len);

bool are_equiv(state_machine first_machine, 
               state_machine second_machine); //works only for full DKA with same alphabet

#endif //State_machine
