#include "lambda_net.h"
#include <iomanip>

void VOID(Port, Port, Net &) {
}
void ERAS(Port left, Port right, Net &net) {
    auto [port1, port2] = net.pop(right);

    net.link(left, port1);
    net.link(left, port2);
}
void ANNI(Port left, Port right, Net &net) {
    auto [port1, port2] = net.pop(left);
    auto [port3, port4] = net.pop(right);

    net.link(port1, port3);
    net.link(port2, port4);
}
void ULCK(Port left, Port right, Net &net) {
    auto [port1, port2] = net[right];
    net[right] = {left, port1};

    net.link({Gamma, right.value}, port2);
}
void PROP(Port left, Port right, Net &net) {
    auto [port1, port2] = net[left];
    auto [port3, port4] = net[right];

    Port var1 = net.new_var();
    Port var2 = net.new_var();
    Port var3 = net.new_var();
    Port var4 = net.new_var();

    Port top = net.push(Delta, var3, var4);
    Port bot = net.push(left.tag, var4, var2);

    net[left] = {var3, var1};
    net[right] = {var1, var2};

    net.link(top, port1);
    net.link({Delta, right.value}, port2);
    net.link(left, port3);
    net.link(bot, port4);
}

constexpr Rule RuleTable[25] = {
    ANNI, NULL, NULL, NULL, NULL,
    PROP, NULL, NULL, NULL, NULL,
    PROP, NULL, NULL, NULL, NULL,
    PROP, PROP, ULCK, ANNI, NULL,
    ERAS, ERAS, ERAS, ERAS, VOID
};
void Net::interact(Port left, Port right) {
    if (left.tag > right.tag)
        RuleTable[left.tag * 5 + right.tag](left, right, *this);
    else
        RuleTable[right.tag * 5 + left.tag](right, left, *this);
}
Port Net::new_var() {
    return {Variable, vars_.push({(size_t)-1, 0})};
}
void Net::link(Port left, Port right) {
    while (true) {
        if (left.tag != Variable) {
            Port tmp = left;
            left = right;
            right = tmp;
        }
        if (left.tag != Variable) {
           cuts_.push_back({left, right});
            return;
        }
        Port got = vars_[left.value];
        if (got.is_index()) {
            vars_[left.value] = right;
            return;
        }
        vars_.pop(left.value);
        left = got;
    }
}
