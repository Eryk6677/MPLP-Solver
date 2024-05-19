#ifndef ALGORITHMS_ENUM_H
#define ALGORITHMS_ENUM_H

#include <string>


enum Algorithms {
    FIVE_BLOCK,
    MODIFIED,
    IMPROVED,
};

inline Algorithms resolve_alg_name(std::string input) {
    if (input == "Modified Five Block Heuristic") return MODIFIED;
    else if (input == "Improved Five Block Heuristic") return MODIFIED;
    return FIVE_BLOCK;
}

#endif // ALGORITHMS_ENUM_H
