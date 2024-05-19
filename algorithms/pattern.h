#ifndef PRACA_INZ_PATTERN_H
#define PRACA_INZ_PATTERN_H


#include <vector>
#include <bits/stdc++.h>
#include "V3/node.h"

int barnes(int p_l, int p_w, int x, int y);
std::vector<int> evaluate_normal_points(int length, int b_l, int b_w);
std::vector <std::pair<int, int>> evaluate_partitions(int length, int b_l, int b_w);

class Pattern {
private:
    // instance dimensions
    int p_l, p_w, b_l, b_w;
public:
    // maximum coverage of pallet side
    int max_x = 0, max_y = 0;
    void get_max_coverages();

    // bounds
    int low_b = 0, upp_b = 0;

    // P and Q sets used in five_block
    std::vector <std::pair<int, int>>* p_set;
    std::vector <std::pair<int, int>>* q_set;
    void set_p_set(std::vector <std::pair<int, int>>* set);
    void set_q_set(std::vector <std::pair<int, int>>* set);

    // L and W sets used in improved and dynamic five_block
    std::vector <int>* l_normal_set;
    std::vector <int>* w_normal_set;
    void set_l_normal_set(std::vector <int>* set);
    void set_w_normal_set(std::vector <int>* set);

    std::vector <int> l_set, w_set;

    // dimensions of generated blocks
    std::vector<std::pair<int, int>> sizes;
    char blocks_orient[5]{};

    // points -> current points, best_points -> best found points until now
    std::vector<std::pair<std::pair<int, int>, char>> points;
    std::vector<std::pair<std::pair<int, int>, char>> best_points;

    // pointer to the map of already evaluated instances
    std::map<std::pair<int, int>, Node>* map_ref;

    Pattern(int pal_len, int pal_wid, int box_len, int box_wid, std::map<std::pair<int, int>, Node>* nodes_vector);
    Pattern(int pal_len, int pal_wid, int box_len, int box_wid);

    void evaluate_raster_points();

    void get_lower_bound();
    void get_upper_bound();

    int evaluate_five_block(int rec_lvl, int max_time, int pat_x, int pat_y,
                            std::chrono::time_point<std::chrono::system_clock> time_point);

    int evaluate_improved(int rec_lvl, int max_time, int pat_x, int pat_y,
                          std::chrono::time_point<std::chrono::system_clock> time_point);

    int recursive_bd2(int rec_lvl, int max_time, int pat_x, int pat_y,
                      std::chrono::time_point<std::chrono::system_clock> time_point);
    int solve_procedure(int rec_lvl, int max_time, int pat_x, int pat_y, int x1, int x2, int y1, int y2,
                        std::chrono::time_point<std::chrono::system_clock> time_point);

    // map utilities
    Node check_map();
    void saveSizeInNode(int x1, int x2, int y1, int y2);
    void updateLowerBound();
    static int findIndexInVector(std::vector<int> vecOfElements, int element);

    // functions used to check if block do not overlap and there is no symmetry
    [[nodiscard]] bool check_rules(int x1, int x2, int y1, int y2) const;
    [[nodiscard]] bool check_2_rule(int x1, int x2, int y1, int y2) const;
    [[nodiscard]] bool check_3_rule(int x1, int x2, int y1, int y2) const;
    [[nodiscard]] bool check_4_rule(int x1, int x2, int y1, int y2) const;
    [[nodiscard]] bool check_5_rule(int x1, int x2, int y1, int y2) const;

    // functions used to evaluate elements located in given block
    void eval_points_with_predefined_orientation(int pat_x, int pat_y);
    std::vector<std::pair<std::pair<int, int>, char>> eval_points_b1(int pat_x, int pat_y);
    std::vector<std::pair<std::pair<int, int>, char>> eval_points_b2(int pat_x, int pat_y);
    std::vector<std::pair<std::pair<int, int>, char>> eval_points_b3(int pat_x, int pat_y);
    std::vector<std::pair<std::pair<int, int>, char>> eval_points_b4(int pat_x, int pat_y);
    std::vector<std::pair<std::pair<int, int>, char>> eval_points_b5(int pat_x, int pat_y);
};


#endif //PRACA_INZ_PATTERN_H
