#include "parser.h"
#include <cstring>

Port buffer_variable(const Port &node, Net &net) {
    if (node.tag == Alpha or node.tag == Lambda) {
        Port var = net.new_var();
        net[node].left = var;
        return var;
    }
    return node;
}
Port parse_term(const std::string &s, const term_map &defs, Net &net) {
    iss is(s);
    port_map vars = {};
    Port term = parse_body(is, vars, defs, net);
    return buffer_variable(term, net);
}

Port parse_var(iss &is, port_map &vars, const term_map &defs, Net &net) {
    char c;
    is >> c;

    auto it = vars.find(c);
    if (it != vars.end()) {
        Port head = it->second;
        Port left = net[head].left;

        if (left.tag == Epsilon) {
            return {Alpha, head.value};
        }
        Port alpha = net.push(Alpha, {}, left);
        net[head].left = alpha;
        return alpha;
    }
    std::string s(1, c);
    while ((c = is.get()) != EOF and strchr(reserved, c) == nullptr)
        s += c;
    is.unget();

    auto it2 = defs.find(s);
    if (it2 != defs.end()) {
        std::string term = it2->second;
        iss is2(term);
        port_map local_context = {};
        return parse_body(is2, local_context, defs, net);
    }
    std::cerr << "ERROR not defined: " << s << '\n';
    return {Epsilon, 0};
}
Port parse_body(iss &is, port_map &vars, const term_map &defs, Net &net) {
    char c;
    Port next;
    Port previous = Port::make_index(0);

    if (is.eof())
        return {};

    while ((is >> c) and c != ')') {
        if (c == '\\' or c == '@') {
            port_map local_context = {};
            next = parse_head(is, local_context, defs, net);
            is.putback(')');
        }
        else if (c == '(') {
            next = parse_body(is, vars, defs, net);
        }
        else {
            is.unget();
            next = parse_var(is, vars, defs, net);
        }

        if (previous.is_index()) {
            previous = buffer_variable(next, net);
            continue;
        }
        previous = buffer_variable(previous, net);

        Port lambda = net.push(Lambda, {}, previous);

        if (next.tag == Gamma) {
            net.link(next, lambda);
        }
        else {
            net[next].left = lambda;
        }
        previous = lambda;
    }
    return previous;
}
Port parse_head(iss &is, port_map &vars, const term_map &defs, Net &net) {
    // Assumes the lambda symbol was just read
    char c;
    Port first;
    Port *previous = &first;

    while ((is >> c) and c != '.') {
        Port head = net.push(Gamma, {Epsilon, 0}, {});
        vars[c] = head;
        *previous = head;
        previous = &net[head].right;
    }
    Port body = parse_body(is, vars, defs, net);
    body = buffer_variable(body, net);

    *previous = body;
    return first;
}
