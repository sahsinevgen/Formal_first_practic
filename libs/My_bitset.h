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

    my_bitset& operator= (const my_bitset& other);

    bool operator<(const my_bitset &other) const;

    bool operator==(const my_bitset &other) const;

    my_bitset& operator|=(const my_bitset &other);

    my_bitset operator|(const my_bitset &other) const;

    void set(int number, int value);

    bool get(int number) const;

    int next_true(int number) const;

private:
    void parse(int number, int &block_number, int &bit_number) const ;
};

#endif //My_bitset
