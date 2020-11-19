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

my_bitset::my_bitset(const my_bitset &a) {
    n = a.n;
    ar_sz = a.ar_sz;
    ar = new unsigned int[ar_sz];
    for (int i = 0; i < ar_sz; i++) {
        ar[i] = a.ar[i];
    }
}

my_bitset::~my_bitset() {
    delete [] ar;
}

my_bitset& my_bitset::operator= (const my_bitset& a) {
    n = a.n;
    ar_sz = a.ar_sz;
    ar = new unsigned int[ar_sz];
    for (int i = 0; i < ar_sz; i++) {
        ar[i] = a.ar[i];
    }
    return *this;
}

bool my_bitset::operator<(const my_bitset &a) const {
    for (int i = 0; i < ar_sz; i++) {
        if (ar[i] != a.ar[i]) {
            return ar[i] < a.ar[i];
        }
    }
    return false;
}

bool my_bitset::operator==(const my_bitset &a) const {
    for (int i = 0; i < ar_sz; i++) {
        if (ar[i] != a.ar[i]) {
            return false;
        }
    }
    return true;
}

my_bitset& my_bitset::operator|=(const my_bitset &a) {
    for (int i = 0; i < ar_sz; i++) {
        ar[i] |= a.ar[i];
    }
    return (*this);
}

my_bitset my_bitset::operator|(const my_bitset &a) const {
    my_bitset ans(*this);
    ans |= a;
    return ans;
}

void my_bitset::set(int i, int x) {
    int i1, i2;
    parse(i, i1, i2);
    if (x == 1) {
        ar[i1] |= (1 << i2);
    } else {
        ar[i1] |= (1 << i2);
        ar[i1] ^= (1 << i2);
    }
}

bool my_bitset::get(int i) const {
    int i1, i2;
    parse(i, i1, i2);
    return (ar[i1] & (1 << i2));
}

int my_bitset::next_true(int i) const {
    int i1, i2;
    parse(i, i1, i2);
    i2++;
    while (i2 != sz) {
        if (get((i1 * sz) + i2))
            return (i1 * sz) + i2;
        i2++;
    }
    i1++;
    while (i1 < ar_sz && ar[i1] == 0) {
        i1++;
    }
    if (i1 == ar_sz) {
        return -1;
    }
    i2 = 0;
    while (1) {
        if (get((i1 * sz) + i2))
            return (i1 * sz) + i2;
        i2++;
    }
}

void my_bitset::parse(int i, int &i1, int &i2) const {
    i1 = i / sz;
    i2 = i % sz;
}