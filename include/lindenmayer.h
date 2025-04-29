//
// Created by Niccolo on 29/04/2025.
//

#ifndef LINDENMAYER_H
#define LINDENMAYER_H
#include <map>
#include <set>
#include <string>
#include <vector>


class Lindenmayer {
public:
    Lindenmayer(const std::set<char> &alphabet, const std::map<char, std::vector<std::string>> &production_rules)
        : alphabet(alphabet),
          production_rules(production_rules) {
    }

    std::string iterate(const std::string &current_string);

    std::string cleanup(const std::string &current_string);

    std::string generate(const std::string &axiom, unsigned int n_iterations, bool need_cleanup = false);
private:
    std::set<char> alphabet;
    std::map<char, std::vector<std::string>> production_rules;
};



#endif //LINDENMAYER_H
