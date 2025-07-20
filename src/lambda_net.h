#pragma once

#include "sparse_array.h"
#include <deque>
#include <iostream>

struct Port {
    size_t tag;
    size_t value;

    bool   is_index()  const { return tag == (size_t)-1; }
    size_t get_index() const { return value; }
    Port&  set_index(size_t index) {
        tag = (size_t)-1;
        value = index;
        return *this;
    }
    static Port make_index(size_t index) {
        return {(size_t)-1, index};
    }
};

struct Pair {
    Port left;
    Port right;

    bool   is_index()  const { return left.is_index(); }
    size_t get_index() const { return left.get_index(); }
    Pair&  set_index(size_t index) {
        left.set_index(index);
        return *this;
    }
    static Pair make_index(size_t index) {
        return {Port::make_index(index), {}};
    }
};

enum Type { Delta = 0, Alpha = 1, Lambda = 2, Gamma = 3, Epsilon = 4, Variable = 5 };

class Net {
    SparseArray<Port> vars_;
    SparseArray<Pair> nodes_;
    std::deque<Pair> cuts_;
public:
    Net (size_t capacity) : vars_(capacity), nodes_(capacity) {}

    Port push(size_t tag, Port left, Port right) {
        return {tag, nodes_.push({left, right})};
    }
    Pair pop(Port port) {
        return nodes_.pop(port.value);
    }
    bool has_cuts() {
        return !cuts_.empty();
    }
    Pair pop_cut() {
        Pair cut = cuts_.front();
        cuts_.pop_front();
        return cut;
    }
    Pair& operator[](Port port) {
        return nodes_[port.value];
    }
    Port new_var();
    void interact(Port, Port);
    void link(Port, Port);

    friend std::ostream& operator<<(std::ostream &os, const Net &net);
};

using Rule = void(*)(Port, Port, Net&);
