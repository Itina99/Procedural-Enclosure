//
// Created by Niccolo on 29/04/2025.
//
#include "lindenmayer.h"
#include <iterator>
#include <algorithm>
#include <random>

std::string Lindenmayer::iterate(const std::string &current_string) {
    std::string next_string;
    for (char c : current_string) {
        auto it = production_rules.find(c);
        if (it != production_rules.end()) {
            std::string extracted = extract_rule(it->second);

            next_string += extracted;
        }
        else {
            // Produzione identità
            next_string += c;
        }
    }
    return next_string;
}

std::string Lindenmayer::cleanup(const std::string &current_string) {
    std::string next_string;
    for (char c : current_string) {
        if (c != 'X') {
            next_string += c;
        }
        else {
            next_string += 'F';
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

std::string Lindenmayer::extract_rule(const std::map<std::string, float>& stochastic_rule) {
    std::random_device dev;
    std::mt19937 rng(dev());
    std::uniform_real_distribution<> dist(0,1);
    double rand = dist(rng);
    float cumulative = 0;
    for (const auto & it : stochastic_rule) {
        cumulative += it.second;
        if (rand < cumulative) {
            return it.first;
        }
    }
    return "";
}