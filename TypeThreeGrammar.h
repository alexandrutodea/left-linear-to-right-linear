#ifndef FLAT_PROJECT_TYPETHREEGRAMMAR_H
#define FLAT_PROJECT_TYPETHREEGRAMMAR_H

#include <iostream>
#include <vector>
#include <regex>

class TypeThreeGrammar {
    std::vector<std::string>(non_terminals_);
    std::vector<std::string>(terminals_);
    std::string starting_point_;
    class TypeThreeRule {
    public:
        std::string right_side_;
        std::string left_side_;
        std::string rside_terminals_ = right_side_;
        std::string rside_nt_;
        bool no_nt_ = false;
        bool nt_is_first_ = false;
        bool nt_is_last_ = false;

        TypeThreeRule(std::string, std::string);

        std::string get_right_side() const;

        std::string get_left_side() const;

        std::string get_rside_nt() const;

        std::string get_rside_ts() const;

        bool get_nt_is_first() const;

        bool get_no_nts() const;

        bool get_nt_is_last() const;

        void add_to_rside(std::string);

        void set_rside_nt(std::string);

        void set_rside_ts(std::string);

        void set_nt_is_first(bool);

        void set_no_nts(bool);

        void set_nt_is_last(bool);

    };

    std::vector<TypeThreeRule>(production_rules_);

public:

    bool insert_non_terminal(std::string);

    bool insert_terminal(std::string);

    bool set_starting_point(std::string);

    bool insert_production_rule(std::string, std::string);

    TypeThreeGrammar convert_to_right_linear();

    void display_grammar();
};

#endif //FLAT_PROJECT_TYPETHREEGRAMMAR_H
