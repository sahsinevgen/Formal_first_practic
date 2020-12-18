#include"My_bitset.h"

my_bitset::my_bitset() {
    n = -1;
    ar = nullptr;
}

my_bitset::my_bitset(int n_) {
    n = n_;
    ar_sz = (n / sz) + bool(n % sz);
    ar = new unsigned int[ar_sz];
    for (int i = 0; i < ar_sz; i++) {
        ar[i] = 0;
    }
}

my_bitset::my_bitset(const my_bitset &other) {
    n = other.n;
    ar_sz = other.ar_sz;
    ar = new unsigned int[ar_sz];
    for (int i = 0; i < ar_sz; i++) {
        ar[i] = other.ar[i];
    }
}

my_bitset::~my_bitset() {
    delete [] ar;
}

my_bitset& my_bitset::operator= (const my_bitset& other) {
    n = other.n;
    ar_sz = other.ar_sz;
    ar = new unsigned int[ar_sz];
    for (int i = 0; i < ar_sz; i++) {
        ar[i] = other.ar[i];
    }
    return *this;
}

bool my_bitset::operator<(const my_bitset &other) const {
    for (int i = 0; i < ar_sz; i++) {
        if (ar[i] != other.ar[i]) {
            return ar[i] < other.ar[i];
        }
    }
    return false;
}

bool my_bitset::operator==(const my_bitset &other) const {
    for (int i = 0; i < ar_sz; i++) {
        if (ar[i] != other.ar[i]) {
            return false;
        }
    }
    return true;
}

my_bitset& my_bitset::operator|=(const my_bitset &other) {
    for (int i = 0; i < ar_sz; i++) {
        ar[i] |= other.ar[i];
    }
    return (*this);
}

my_bitset my_bitset::operator|(const my_bitset &other) const {
    my_bitset ans(*this);
    ans |= other;
    return ans;
}

void my_bitset::set(int number, int value) {
    int block_number, i2;
    parse(number, block_number, i2);
    if (value == 1) {
        ar[block_number] |= (1 << i2);
    } else {
        ar[block_number] |= (1 << i2);
        ar[block_number] ^= (1 << i2);
    }
}

bool my_bitset::get(int number) const {
    int block_number, bit_number;
    parse(number, block_number, bit_number);
    return (ar[block_number] & (1 << bit_number));
}

int my_bitset::next_true(int number) const {
    int block_number, bit_number;
    parse(number, block_number, bit_number);
    bit_number++;
    while (bit_number != sz) {
        if (get((block_number * sz) + bit_number))
            return (block_number * sz) + bit_number;
        bit_number++;
    }
    block_number++;
    while (block_number < ar_sz && ar[block_number] == 0) {
        block_number++;
    }
    if (block_number == ar_sz) {
        return -1;
    }
    bit_number = 0;
    while (1) {
        if (get((block_number * sz) + bit_number))
            return (block_number * sz) + bit_number;
        bit_number++;
    }
}

void my_bitset::parse(int number, int &block_number, int &bit_number) const {
    block_number = number / sz;
    bit_number = number % sz;
}
