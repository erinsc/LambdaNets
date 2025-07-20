#include "lambda_net.h"
#include "parser.h"

int main() {
    Net IFC(8192);

    term_map tm = {
        {"T", "\\tf.t"},
        {"F", "\\tf.f"},
        {"|", "\\a.aa"},
        {"&", "\\ab.aba"},

        {"0", "\\fx.x"},
        {"1", "\\fx.fx"},
        {"2", "\\fx.f(fx)"},
        {"3", "\\fx.f(f(fx))"},
        {"4", "\\fx.f(f(f(fx)))"},
        {"5", "\\fx.f(f(f(f(fx))))"},
        {"6", "\\fx.f(f(f(f(f(fx)))))"},
        {"++", "\\nfx.nf(fx)"},
        {"+", "\\ab.a ++ b"},
        {"*", "\\abf.a(bf)"},
        {"--", "\\nfx.n((\\fgh.h(gf))f)((\\xu.x)x)(\\u.u)"},
        {"-", "\\ab.a -- b"},
        {"=0", "\\n.n (\\x.F) T"},
        {"<=", "\\nm.=0(- n m)"},
        {"==", "\\nm.&(<= m n)(<= n m)"},
        {"!", "@n.n(@p.p(@lrq.q((@lfx.lf(fx))l)((@lrf.l(rf))lr)))(@p.p(@x.x)(@y.y))(@tf.f)"},
        //{"", ""},
    };

    std::string s = "== (! 4) (* 4 6)";
    Port var0 = IFC.new_var();

    IFC.link(parse_term(s, tm, IFC), var0);


    std::cout << IFC << "\n\n";
    //getchar();

    while(IFC.has_cuts()) {
        //system("clear");
        Pair cut = IFC.pop_cut();
        IFC.interact(cut.left, cut.right);
        //std::cout << IFC << "\n\n";
    }
    std::cout << IFC << "\n\n";
}
