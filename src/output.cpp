#include "lambda_net.h"
#include <iomanip>

static constexpr const char * TypeNames[6] = {
    "Delta", "Alpha", "Lamda", "Gamma", "Epsilon", "Var  "
};
void print_port(std::ostream &os, const Port &port) {
    if (port.is_index()) {
        os << " ---- " << std::setw(4) << port.value;
        return;
    }
    os << TypeNames[port.tag] << " ";
    if (port.tag == Epsilon) {
        os << "  ";
        return;
    }
    os << std::setw(4) << port.value;
}
std::ostream& operator<<(std::ostream &os, const Net &net) {
    os << "CUTS |       LEFT |      RIGHT\n";
    os << "------------------------------\n";

    for (auto const &cut : net.cuts_) {
        os << "     | ";
        print_port(os, cut.left);
        os << " | ";
        print_port(os, cut.right);
        os << "\n";
    }
    os << "------------------------------\n";
    os << "NODE |       LEFT |      RIGHT\n";
    os << "------------------------------\n";
    for (const auto &[i, node] : net.nodes_) {
        os << std::setw(4) << i << " | ";
        print_port(os, node.left);
        os << " | ";
        print_port(os, node.right);
        os << "\n";
    }
    os << "------------------------------\n";
    os << "VARS |       PORT\n";
    os << "-----------------\n";
    for (const auto &[i, var] : net.vars_) {
        os << std::setw(4) << i << " | ";
        print_port(os, var);
        os << "\n";
    }
    return os;
}
