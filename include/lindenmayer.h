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
    explicit Lindenmayer(const std::map<char, std::map<std::string, float>> &production_rules)
        : production_rules(production_rules) {
    }

    std::string extract_rule(const std::map<std::string, float>& stochastic_rule);

    std::string iterate(const std::string &current_string);

    std::string cleanup(const std::string &current_string);

    std::string generate(const std::string &axiom, unsigned int n_iterations, bool need_cleanup = false);
private:
    std::map<char, std::map<std::string, float>> production_rules;
};



#endif //LINDENMAYER_H
