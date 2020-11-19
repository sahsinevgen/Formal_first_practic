#include <gtest/gtest.h>
#include <fstream>

#include"libs/Reg_expr_to_state_machine.h"
#include"libs/Make_one_letter_moves.h"
#include"libs/Determinization.h"
#include"libs/Minimization.h"
#include"libs/First_practic.h"

void read_state_machine(std::string file_name, state_machine &g) {
    std::ifstream in(file_name.c_str());
    in >> g;
    in.close();
}

TEST(Reg_expr_to_state_machine_test, one_letter) {
    state_machine g = make_one_letter_moves(reg_expr_to_state_machine("a"));
    EXPECT_TRUE(g.has_word("a"));
    EXPECT_FALSE(g.has_word("b"));
    state_machine ans;
    read_state_machine("../tests/reg_to_machine_test_1.out", ans);
    EXPECT_TRUE(are_equiv(determinization(g), ans));
}

TEST(Reg_expr_to_state_machine_test, concatenation) {
    state_machine g = make_one_letter_moves(reg_expr_to_state_machine("ab"));
    //EXPECT_TRUE(g.has_word("ab"));
    EXPECT_FALSE(g.has_word("a") || g.has_word("b") || g.has_word("ba"));
    state_machine ans;
    read_state_machine("../tests/reg_to_machine_test_2.out", ans);
    EXPECT_TRUE(are_equiv(determinization(g), ans));
}

TEST(Reg_expr_to_state_machine_test, sum) {
    state_machine g = make_one_letter_moves(reg_expr_to_state_machine("a+b"));
    EXPECT_TRUE(g.has_word("a") && g.has_word("b"));
    EXPECT_FALSE(g.has_word("c"));
    state_machine ans;
    read_state_machine("../tests/reg_to_machine_test_3.out", ans);
    EXPECT_TRUE(are_equiv(determinization(g), ans));
}

TEST(Reg_expr_to_state_machine_test, kleene_closure) {
    state_machine g = make_one_letter_moves(reg_expr_to_state_machine("a*"));
    std::string s = "";
    for (int i = 0; i < 1000; i++) {
        s += "a";
    }
    EXPECT_TRUE(g.has_word("a") && 
                g.has_word("aa") && 
                g.has_word("aaaa") && 
                g.has_word(s));
    EXPECT_FALSE(g.has_word("b"));
    state_machine ans;
    read_state_machine("../tests/reg_to_machine_test_4.out", ans);
    EXPECT_TRUE(are_equiv(determinization(g), ans));
}

TEST(Reg_expr_to_state_machine_test, interesing_test) {
    state_machine g = make_one_letter_moves(reg_expr_to_state_machine("(ab+ba)*(#+a+b)"));
    EXPECT_TRUE(g.has_word("abba") && 
                g.has_word("ababab") && 
                g.has_word("a") && 
                g.has_word(""));
    EXPECT_FALSE(g.has_word("bb") || 
                 g.has_word("baaab") || 
                 g.has_word("abbabba") || 
                 g.has_word("baaaa"));
    state_machine ans;
    read_state_machine("../tests/reg_to_machine_test_5.out", ans);
    EXPECT_TRUE(are_equiv(determinization(g), ans));
}

TEST(Determinization_test, just_many_a_moves) {
    state_machine g, ans;
    read_state_machine("../tests/Determinization_test_1.in", g);
    g = determinization(make_one_letter_moves(g));
    read_state_machine("../tests/Determinization_test_1.out", ans);
    EXPECT_TRUE(are_equiv(g, ans));
}

TEST(Determinization_test, epsilon_moves) {
    state_machine g, ans;
    read_state_machine("../tests/Determinization_test_2.in", g);
    g = determinization(make_one_letter_moves(g));
    read_state_machine("../tests/Determinization_test_2.out", ans);
    EXPECT_TRUE(are_equiv(g, ans));
}

TEST(Determinization_test, final_determinization) {
    state_machine g, ans;
    read_state_machine("../tests/Determinization_test_3.in", g);
    g = determinization(make_one_letter_moves(g));
    read_state_machine("../tests/Determinization_test_3.out", ans);
    EXPECT_TRUE(are_equiv(g, ans));
}

TEST(Minimization_test, two_pair_equiv_vertexes) {
    state_machine g, ans;
    read_state_machine("../tests/Minimization_test_1.in", g);
    g = minimization(g);
    read_state_machine("../tests/Minimization_test_1.out", ans);
    EXPECT_TRUE(are_equiv(g, ans));
}

TEST(Minimization_test, do_nothing) {
    state_machine g, ans;
    read_state_machine("../tests/Minimization_test_2.in", g);
    g = minimization(g);
    read_state_machine("../tests/Minimization_test_2.out", ans);
    EXPECT_TRUE(are_equiv(g, ans));
}

TEST(First_practic_test, convert_sum) {
    std::string in = true_reg_expr_from_input("ab+");
    std::string out = "a+b";
    EXPECT_EQ(in, out);
}

TEST(First_practic_test, wrong_inputs) {
    EXPECT_ANY_THROW(true_reg_expr_from_input("."));
    EXPECT_ANY_THROW(true_reg_expr_from_input("abc+"));
    EXPECT_ANY_THROW(true_reg_expr_from_input("aq+"));
    EXPECT_ANY_THROW(true_reg_expr_from_input("a+b"));
}


TEST(First_practic_test, convert_concatination) {
    std::string in = true_reg_expr_from_input("ab.");
    std::string out = "(a)(b)";
    EXPECT_EQ(in, out);
}

TEST(First_practic_test, clini) {
    std::string in = true_reg_expr_from_input("a*");
    std::string out = "(a)*";
    EXPECT_EQ(in, out);
}

TEST(First_practic_test, test) {
    std::string in = true_reg_expr_from_input("ab+*bc..");
    std::string out = "(a+b)*bc";
    EXPECT_TRUE(are_equiv(reg_expr_to_state_machine(in), 
                          reg_expr_to_state_machine(out)));
}

TEST(First_practic_test, solve_test1) {
    std::string input = "a+b";
    state_machine machine;
    machine = reg_expr_to_state_machine(input);
    int n = machine.n;
    machine.add_vertex();
    machine.add_edge(n - 1, n, "abc");
    machine = minimization(determinization(make_one_letter_moves(machine)));
    EXPECT_EQ(get_ans(machine, "abc"), 1);
    EXPECT_EQ(get_ans(machine, "abc"), 1);
}

TEST(First_practic_test, solve_test2) {
    std::string input = "(a+b)*(a+c)*";
    state_machine machine;
    machine = reg_expr_to_state_machine(input);
    EXPECT_EQ(get_ans(machine, "abc"), 3);
}

int main(int argc, char** argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}


