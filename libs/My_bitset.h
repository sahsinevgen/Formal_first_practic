#ifndef My_bitset
#define My_bitset

class my_bitset {

public:
    int n;
    int ar_sz;
    unsigned int* ar;
    const int sz = 32;

    my_bitset();

    my_bitset(int n_);

    my_bitset(const my_bitset &a);

    ~my_bitset();

    my_bitset& operator= (const my_bitset& a);

    bool operator<(const my_bitset &a) const;

    bool operator==(const my_bitset &a) const;

    my_bitset& operator|=(const my_bitset &a);

    my_bitset operator|(const my_bitset &a) const;

    void set(int i, int x);

    bool get(int i) const;

    int next_true(int i) const;

    void parse(int i, int &i1, int &i2) const ;
};

#endif //My_bitset