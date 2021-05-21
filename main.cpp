#include <iostream>
#include <vector>
#include <regex>
#include "TypeThreeGrammar.h"

std::string get_text_input() {
  std::string user_input;
  std::getline(std::cin, user_input);
  std::regex whitespace("\\s+");
  std::string no_spaces = std::regex_replace(user_input, whitespace, "");
  return no_spaces;
}

bool get_yes_no_input() {
  bool yes_no = false;
  char c;
  std::cin >> c;
  while ((c != 'n') and (c != 'N') and (c != 'y') and (c != 'Y')) {
    std::cout << "Invalid input. Try again:" << std::endl;
    std::cin >> c;
  }
  switch (c) {
    case 'n':
    case 'N':
      yes_no = false;
      break;
    case 'y':
    case 'Y':
      yes_no = true;
      break;
    default:
      //do nothing
      break;
  }
  getchar();
  return yes_no;
}

void get_grammar_input(char set_initial, TypeThreeGrammar& grammar
                       , bool& go_on) {
  std::regex comma_re(",");
  std::regex sep_re("\\|");
  std::sregex_token_iterator end;
  std::string a_string;

  switch (set_initial) {
    case 'n':
      std::cout << "Enter the non-terminals, separated by a comma:"
                << std::endl;
      break;
    case 't':
      std::cout << "Enter the terminals, separated by a comma:" << std::endl;
      break;
    case 's':
      std::cout << "Enter the starting point:" << std::endl;
      break;
    case 'p':
      std::cout << "Enter the production rules, separated by a comma:" <<
      std::endl;
      break;
    default:
      std::cout << "Invalid set initial" << std::endl;
  }

  switch (set_initial) {
    case 'n': {
      a_string = get_text_input();
      std::sregex_token_iterator pos(a_string.cbegin(), a_string.cend(),
                                     comma_re, -1);
      go_on = false;
      while (pos != end) {
        if (grammar.insert_non_terminal(pos->str())) {}
        else {
          go_on = true;
        }
        pos++;
      }
      if (go_on) {
        std::cout << "Do you want to retry entering any non-terminals? y\\n" <<
                  std::endl;
        if (!get_yes_no_input()) {
          go_on = false;
        }
      }
      break;
    }

    case 't': {
      a_string = get_text_input();
      std::sregex_token_iterator pos(a_string.cbegin(), a_string.cend(),
                                     comma_re, -1);
      go_on = false;
      while (pos != end) {
        if (grammar.insert_terminal(pos->str())) {}
        else {
          go_on = true;
        }
        pos++;
      }
      if (go_on) {
        std::cout << "Do you want to retry entering any terminals? y\\n" <<
                  std::endl;
        if (!get_yes_no_input()) {
          go_on = false;
        }
      }
      break;
    }

    case 's':
      go_on = false;
      a_string = get_text_input();
      if (grammar.set_starting_point(a_string)){} else {
        go_on = true;
      }
      if (go_on) {
        std::cout << "Do you want to retry entering the starting point? y\\n" <<
        std::endl;
        if (!get_yes_no_input()) {
          go_on = false;
        }
      }
      break;

    case 'p': {
      a_string = get_text_input();
      std::sregex_token_iterator pos(a_string.cbegin(), a_string.cend(),
                                     comma_re, -1);
      go_on = false;
      std::regex prod_rule_re("(.+)->(.+)");
      std::smatch smatch;
      while (pos != end) {
        std::string tmp = pos->str();
        std::regex_search(tmp, smatch, prod_rule_re);
        std::string tmp_two = smatch[2].str();
        std::sregex_token_iterator pos_two(tmp_two.cbegin(),
                                           tmp_two.cend(),
                                           sep_re,-1);
        while (pos_two != end) {
          if (grammar.insert_production_rule(smatch[1].str(), pos_two->str()))
          {} else {
            go_on = true;
          }
          pos_two++;
        }
        if (go_on) {
          std::cout << "Do you want to retry entering any of the production "
                       "rules? y\\n" << std::endl;
          if (!get_yes_no_input()) {
            go_on = false;
          }
        }
        pos++;
      }
    }
    break;
    default:
      std::cout << "Invalid set initial." << std::endl;
  }
}

int main() {
  TypeThreeGrammar grammar;
  std::regex comma_re(",");
  std::string a_string;
  grammar.insert_terminal("λ");
  bool go_on = true;
  while (go_on) {
    get_grammar_input('n', grammar, go_on);
  }
  go_on = true;
  while (go_on) {
    get_grammar_input('t', grammar, go_on);
  }
  go_on = true;
  while (go_on) {
    get_grammar_input('s', grammar, go_on);
  }
  go_on = true;
  while (go_on) {
    get_grammar_input('p', grammar, go_on);
  }
  TypeThreeGrammar new_grammar = grammar.convert_to_right_linear();
  std::string user_input;
  std::cout << std::endl;
  std::cout << "Press any key to display the right-linear grammar" << std::endl;
  user_input = get_text_input();
  new_grammar.display_grammar();
  std::cout << std::endl;
  return 0;
}