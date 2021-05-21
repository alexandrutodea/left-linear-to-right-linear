#include "TypeThreeGrammar.h"

TypeThreeGrammar::TypeThreeRule::TypeThreeRule(std::string left_side,
                                               std::string right_side){
  left_side_ = left_side;
  right_side_ = right_side;
}

std::string TypeThreeGrammar::TypeThreeRule::get_right_side() const {
  return right_side_;
}

std::string TypeThreeGrammar::TypeThreeRule::get_left_side() const {
  return left_side_;
}

void TypeThreeGrammar::TypeThreeRule::add_to_rside(std::string new_str) {
  right_side_.append("|");
  right_side_.append(new_str);
}

bool TypeThreeGrammar::TypeThreeRule::get_nt_is_first() const {
  return nt_is_first_;
}

std::string TypeThreeGrammar::TypeThreeRule::get_rside_nt() const {
  return rside_nt_;
}

void TypeThreeGrammar::TypeThreeRule::set_rside_nt(std::string new_nt) {
  rside_nt_ = new_nt;
}

std::string TypeThreeGrammar::TypeThreeRule::get_rside_ts() const {
  return rside_terminals_;
}

void TypeThreeGrammar::TypeThreeRule::set_rside_ts(std::string terminals) {
  rside_terminals_ = terminals;
}

void TypeThreeGrammar::TypeThreeRule::set_nt_is_first(bool is_first) {
  nt_is_first_ = is_first;
}

void TypeThreeGrammar::TypeThreeRule::set_no_nts(bool no_non_terminals) {
  no_nt_ = no_non_terminals;
}

void TypeThreeGrammar::TypeThreeRule::set_nt_is_last(bool is_last) {
  nt_is_last_ = is_last;
}

bool TypeThreeGrammar::TypeThreeRule::get_no_nts() const {
  return no_nt_;
}

bool TypeThreeGrammar::TypeThreeRule::get_nt_is_last() const {
  return nt_is_last_;
}

bool TypeThreeGrammar::insert_non_terminal(std::string nt) {
  bool valid_nt = true;

  if (nt.size() == 2) {
    std::regex two_char_nt_format("[a-zA-z][0-9]");
    if (!std::regex_match(nt, two_char_nt_format)) {
      valid_nt = false;
      std::cout << "A 2-char non-terminal must be formed of a letter followed "
                   "by a number" << std::endl;
      return valid_nt;
    }
  } else if (nt.size() > 2) {
    valid_nt = false;
    std::cout << "A non-terminal can be formed of at most two symbols"
              << std::endl;
    return valid_nt;
  }

  for (std::string current_nt : non_terminals_) {
    if (current_nt.length() == nt.length() and nt.length() == 2) {
      if (nt[0] == current_nt[0] and nt[1] == current_nt[1]) {
        std::cout << current_nt << " is already a non-terminal" << std::endl;
        valid_nt = false;
        return valid_nt;
      }
    } else if (current_nt.length() == nt.length() and nt.length() == 1) {
      if (nt[0] == current_nt[0]) {
        std::cout << current_nt << " is already a non-terminal" << std::endl;
        valid_nt = false;
        return valid_nt;
      }
    }
  }

  for (std::string current_t : terminals_) {
    for (int j = 0; j < current_t.size(); j++) {
      if (current_t[j] == nt[0]) {
        valid_nt = false;
        if (current_t.size() > 1) {
          std::cout << current_t[j] << " is part of terminal " << current_t
          << std::endl;
        } else {
          std::cout << current_t << " is already a terminal" << std::endl;
        }
        return valid_nt;
      }
    }
  }

  non_terminals_.insert(non_terminals_.begin(), nt);
  return valid_nt;
}

bool TypeThreeGrammar::insert_terminal(std::string terminal) {
  bool valid_t = true;

  for (const std::string& current_t : terminals_) {
    for (char j : current_t) {
      for (char k : terminal) {
        if (k == j) {
          if (current_t.length() > 1) {
            std::cout << j << " is part of terminal " << current_t << std::endl;
          } else {
            std::cout << current_t << " is already a terminal" << std::endl;
          }
          valid_t = false;
          return valid_t;
        }
      }
    }
  }

  for (std::string current_nt : non_terminals_) {
    for (char j : terminal) {
      if (current_nt[0] == j) {
        if (current_nt.length() > 1) {
          std::cout << j << " is part of non-terminal " << current_nt
          << std::endl;
        } else {
          std::cout << j << " is a non-terminal" << std::endl;
        }
        valid_t = false;
        return valid_t;
      }
    }
  }
  terminals_.insert(terminals_.begin(), terminal);
  return valid_t;
}

bool TypeThreeGrammar::set_starting_point(std::string starting_point) {
  for (const std::string& nt : non_terminals_) {
    if (nt == starting_point) {
      starting_point_ = starting_point;
      return true;
    }
  }
  std::cout << "The starting point must be a non-terminal" << std::endl;
  return false;
}


bool TypeThreeGrammar::insert_production_rule(std::string left_side,
                                              std::string right_side) {
  TypeThreeRule new_rule = TypeThreeRule(left_side, right_side);
  bool valid_rule = true;

  if (non_terminals_.empty()) {
    std::cout << "In order to add a production rule, you must add "
                 "at least one non-terminal" << std::endl;
    valid_rule = false;
    return valid_rule;
  }

  for (const TypeThreeRule& prod_rule : production_rules_) {
    if (left_side == prod_rule.get_left_side()
        and right_side == prod_rule.get_right_side()) {
      std::cout << "This production rule has already been added." << std::endl;
      valid_rule = false;
      return valid_rule;
    }
  }

  for (const std::string& nt : non_terminals_) {
    if (nt == left_side) {
      valid_rule = true;
      break;
    } else {
      valid_rule = false;
    }
  }
  if (!valid_rule) {
    std::cout << left_side << " has to be in the set of non-terminals" <<
    std::endl;
    return valid_rule;
  }

  if (right_side == "λ") {
    for (const TypeThreeRule& prod_rule : production_rules_) {
      if (prod_rule.get_right_side().find(left_side) != std::string::npos) {
        std::cout << "You cannot add " << left_side << " -> " << right_side
          << " while " << left_side << " belongs to the right hand side of "
                                       "another rule" << std::endl;
        valid_rule = false;
        return valid_rule;
      }
    }
  }

  for (const TypeThreeRule& prod_rule : production_rules_) {
    if (prod_rule.get_right_side() == "λ") {
      if (right_side.find(prod_rule.get_left_side()) != std::string::npos) {
        std::cout << "You cannot add " << left_side << " -> "
                  << right_side << " while " << prod_rule.get_left_side()
                  << " -> λ exists in the grammar" << std::endl;
        valid_rule = false;
        return valid_rule;
      }
    }
  }

  int rside_len = right_side.length();
  for (const std::string& nt : non_terminals_) {
    int nt_len = nt.length();
    if (right_side.find(nt) != std::string::npos) {
      int found = right_side.find(nt);
      if (found == 0) {
        new_rule.set_nt_is_first(true);
        new_rule.set_rside_nt(nt);
        if (rside_len >= 2) {
          if (nt_len == 2) {
            new_rule.set_rside_ts(right_side.substr(2));
            break;
          } else {
            new_rule.set_rside_ts(right_side.substr(1));
            break;
          }
        } else {
          new_rule.set_nt_is_first(true);
        }
      } else if (found == rside_len-2 and nt_len == 2) {
        new_rule.set_rside_nt(nt);
        new_rule.set_rside_ts(right_side.substr(0,rside_len-2));
        new_rule.set_nt_is_last(true);
        break;
      } else if (found == rside_len-1) {
        new_rule.set_rside_nt(nt);
        new_rule.set_rside_ts(right_side.substr(0,rside_len-1));
        new_rule.set_nt_is_last(true);
        break;
      }
    }
  }
  if (!new_rule.get_nt_is_first() and !new_rule.get_nt_is_last()) {
    new_rule.set_no_nts(true);
    new_rule.set_rside_ts(right_side);
  }

  if (!new_rule.get_rside_ts().empty()) {
    std::string terminal_part = new_rule.get_rside_ts();
    for (const std::string& t : terminals_) {
      while (terminal_part.find(t) != std::string::npos) {
        int start_from = terminal_part.find(t);
        terminal_part.erase(start_from, t.length());
      }
    }
    if (!terminal_part.empty()) {
      std::cout << "Rule is not type-three or it contains characters which are "
                   "not in the language" << std::endl;
      valid_rule = false;
      return valid_rule;
    }
  }

  production_rules_.push_back(new_rule);
  return valid_rule;
}

void TypeThreeGrammar::display_grammar() {
  std::cout << "Non-terminals:" << std::endl;
  for (int i = 0; i < non_terminals_.size(); i++) {
    std::cout << non_terminals_[i];
    int size = non_terminals_.size();
    if (i < size - 1) {
      std::cout << ", ";
    }
  }
  std::cout << std::endl << "Terminals:" << std::endl;
  for (int i = 0; i < terminals_.size(); i++) {
    if (terminals_[i] != "λ") {
      std::cout << terminals_[i];
    }
    int size = terminals_.size();
    if (i < size - 1 and terminals_[i] != "λ") {
      std::cout << ", ";
    }
  }
  std::cout << std::endl << "Starting point:\n" << starting_point_;
  std::cout << std::endl << "Production rules:" << std::endl;

  std::vector<TypeThreeRule> or_rules;

  for (int i = 0; i < production_rules_.size(); i++) {
    if (production_rules_[i].get_rside_ts().length() == 0
    and production_rules_[i].get_no_nts()) {
      production_rules_.erase(production_rules_.begin()+i);
      TypeThreeRule new_rule = TypeThreeRule(production_rules_[i]
        .get_left_side(), "λ");
      production_rules_.push_back(new_rule);
    }
  }

  for (const TypeThreeRule& current_rule : production_rules_) {
    bool rule_repeats = false;
    for (TypeThreeRule& or_rule : or_rules) {
      if (current_rule.get_left_side() == or_rule.get_left_side()) {
        or_rule.add_to_rside(current_rule.get_right_side());
        rule_repeats = true;
        break;
      }
    }
    if (!rule_repeats) {
      or_rules.push_back(current_rule);
    }
  }

  for (int i = 0; i < or_rules.size(); i++) {
    TypeThreeRule rule = or_rules[i];
    std::cout << rule.get_left_side() << "->" << rule.get_right_side();
    int size = or_rules.size();
    if (i < size - 1) {
      std::cout << ", ";
    }
  }
}

TypeThreeGrammar TypeThreeGrammar::convert_to_right_linear() {
  TypeThreeGrammar right_linear_grammar;
  int step_counter = 0;

  for (const std::string& nt : non_terminals_) {
    right_linear_grammar.insert_non_terminal(nt);
  }

  for (const std::string& t : terminals_) {
    right_linear_grammar.insert_terminal(t);
  }

  right_linear_grammar.set_starting_point(starting_point_);

  for (const TypeThreeRule& prod_rule : production_rules_) {
    if (prod_rule.get_right_side().find(starting_point_) !=
        std::string::npos) {
      insert_non_terminal("S0");
      right_linear_grammar.insert_non_terminal("S0");
      std::string starting_point = starting_point_;
      insert_production_rule("S0", starting_point);
      set_starting_point("S0");
      right_linear_grammar.set_starting_point("S0");
      std::cout << step_counter << ") Added rule " << "S0->"<<
      starting_point << " as " << starting_point << " is on the right hand "
                                                    "side of rule " <<
      prod_rule.get_left_side() << "->" << prod_rule.get_right_side() <<
      std::endl;
      step_counter++;
      break;
    }
  }

  for (const TypeThreeRule& prod_rule : production_rules_) {
    if (prod_rule.get_no_nts()) {
      if (starting_point_ == prod_rule.get_left_side()) {
        right_linear_grammar.insert_production_rule(
          prod_rule.get_left_side(),
          prod_rule.get_right_side());
        std::cout << step_counter<< ") Added rule "<< prod_rule.get_left_side()
        << "->" << prod_rule.get_right_side() << " (applied rule 1)" <<
        std::endl;
        step_counter++;
      }
    }
  }


  for (const TypeThreeRule& prod_rule : production_rules_) {
    if (prod_rule.get_no_nts()) {
      std::string new_rule = prod_rule.get_right_side() +
        prod_rule.get_left_side();
      right_linear_grammar.insert_production_rule(starting_point_,
                                                  new_rule);
      std::cout << step_counter << ") Applied rule 2 on "
      << prod_rule.get_left_side() << "->" << prod_rule.get_right_side() <<
      ": " << starting_point_ << "->" << new_rule << std::endl;
      step_counter++;
    }
  }


  for (const TypeThreeRule& prod_rule : production_rules_) {
    if (prod_rule.get_nt_is_first() and
    prod_rule.get_left_side() !=starting_point_ and
    !prod_rule.get_rside_ts().empty()) {
      std::string new_rule = prod_rule.get_rside_ts() +
        prod_rule.get_left_side();
      right_linear_grammar.insert_production_rule( prod_rule.get_rside_nt(),
        new_rule);
      std::cout << step_counter <<") Applied rule 3 on " <<
      prod_rule.get_left_side() << "->" << prod_rule.get_right_side() << ": "
      << prod_rule.get_rside_nt() << "->" << new_rule << std::endl;
      step_counter++;
    }
  }


  for (const TypeThreeRule& prod_rule : production_rules_) {
    if (prod_rule.get_nt_is_first() and
      (prod_rule.get_left_side() == starting_point_)) {
      right_linear_grammar.insert_production_rule( prod_rule.get_rside_nt(),
        prod_rule.get_rside_ts());
      std::cout << step_counter << ") Applied rule 4 on " << prod_rule
      .get_left_side() << "->" << prod_rule.get_right_side() << ": ";
      if (prod_rule.get_rside_ts().length() == 0) {
        std::cout << prod_rule.get_rside_nt() << "-> λ" << std::endl;
      } else {
        std::cout << prod_rule.get_rside_nt() << "->" <<
        prod_rule.get_rside_ts() << std::endl;
      }
      step_counter++;
    }
  }

  return right_linear_grammar;
}