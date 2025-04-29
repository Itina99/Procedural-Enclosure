//
// Created by Niccolo on 29/04/2025.
//
#include "lindenmayer.h"
#include <iterator>
#include <algorithm>
#include <random>

std::string Lindenmayer::iterate(const std::string &current_string) {
    std::string next_string = "";
    for (char c : current_string) {
        auto it = production_rules.find(c);
        if (it != production_rules.end()) {
            std::vector<std::string> extracted{};
            std::sample(it->second.begin(), it->second.end(), std::back_inserter(extracted), 1, std::mt19937 {std::random_device{}()});
            next_string += extracted[0];
        }
        else {
            // Produzione identità
            next_string += c;
        }
    }
    return next_string;
}

std::string Lindenmayer::cleanup(const std::string &current_string) {
    std::string next_string = "";
    for (char c : current_string) {
        if (c != 'X') {
            next_string += c;
        }
        else {
            //next_string += 'P';
        }
    }
    return next_string;
}

std::string Lindenmayer::generate(const std::string &axiom, unsigned int n_iterations, bool need_cleanup) {
    std::string next = axiom;
    for (unsigned int i = 0; i < n_iterations; i++) {
        next = iterate(next);
    }
    if (need_cleanup) {
        next = cleanup(next);
    }
    return next;
}