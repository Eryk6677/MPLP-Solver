#include "pattern.h"

int barnes(int p_l, int p_w, int x, int y) {
    int tmp;
    if ((p_l % x) + (p_w % x) <= x) {
        tmp = ((p_l * p_w) - (p_l % x) * (p_w % x)) / x;
    } else {
        tmp = ((p_l * p_w) - (x - p_l % x) * (x - p_w % x)) / x;
    }
    return std::floor(tmp/y);
}

std::vector <std::pair<int, int>> evaluate_partitions(int length, int b_l, int b_w) {
    std::vector <std::pair<int, int>> partitions;
    for (int x = 0; x < length; x++) {
        for (int y = 0; y < length; y++) {
            int tmp = (x * b_l) + (y * b_w);
            if (((length - b_w) < tmp) && (tmp <= length)) partitions.emplace_back(x, y);
        }
    }
    return partitions;
}

std::vector <int> evaluate_normal_points(int length, int b_l, int b_w) {
    std::vector <int> points;
    for (int x = 0; x < length; x++) {
        for (int y = 0; y < length; y++) {
            int tmp = (x * b_l) + (y * b_w);
            if ((tmp >= 0) && (tmp <= length)) points.push_back(tmp);
        }
    }
    std::sort(points.begin(), points.end());
    return points;
}

void Pattern::get_max_coverages() {
    // max_x
    for (auto & x : *p_set) {
        int tmp = (x.first * b_l) + (x.second * b_w);
        if (tmp > max_x) max_x = tmp;
    }

    // max_y
    for (auto & y : *q_set) {
        int tmp = (y.first * b_l) + (y.second * b_w);
        if (tmp > max_y) max_y = tmp;
    }
}

Pattern::Pattern(int pal_len, int pal_wid, int box_len, int box_wid) {
    p_l = pal_len;
    p_w = pal_wid;
    b_l = box_len;
    b_w = box_wid;

    map_ref = nullptr;
}

Pattern::Pattern(int pal_len, int pal_wid, int box_len, int box_wid, std::map<std::pair<int, int>, Node>* nodes_vector) {
    p_l = pal_len;
    p_w = pal_wid;
    b_l = box_len;
    b_w = box_wid;

    map_ref = nodes_vector;
}

void Pattern::set_p_set(std::vector <std::pair<int, int>>* set) {
    this->p_set = set;
}

void Pattern::set_q_set(std::vector <std::pair<int, int>>* set) {
    this->q_set = set;
}

void Pattern::set_l_normal_set(std::vector<int> *set) {
    this->l_normal_set = set;
}

void Pattern::set_w_normal_set(std::vector<int> *set) {
    this->w_normal_set = set;
}

void Pattern::evaluate_raster_points() {
    // Evaluate set subset of normal points
    std::vector <int> s_l;
    for (auto & l : *l_normal_set) {
        if (l <= p_l) {
            s_l.push_back(l);
            if (max_x < l) max_x = l;
        }
    }

    // Save raster points on L
    for (auto & l : s_l) {
        int tmp = (p_l - l);
        for (int i = s_l.size() - 1; i >= 0; i--) {
            if (s_l.at(i) <= tmp) {
                l_set.push_back(s_l.at(i));
                break;
            }
        }
    }
    sort(l_set.begin(), l_set.end() );
    l_set.erase(unique(l_set.begin(), l_set.end() ), l_set.end());

    // Evaluate set subset of normal points
    std::vector <int> s_w;
    for (auto & w : *w_normal_set) {
        if (w <= p_w) {
            s_w.push_back(w);
            if (max_y < w) max_y = w;
        }
    }

    // Save raster points on W
    for (auto & w : *w_normal_set) {
        int tmp = (p_w - w);
        for (int i = w_normal_set->size() - 1; i >= 0; i--) {
            if (w_normal_set->at(i) <= tmp) {
                w_set.push_back(w_normal_set->at(i));
                break;
            }
        }
    }
    sort(w_set.begin(), w_set.end() );
    w_set.erase( unique(w_set.begin(), w_set.end() ), w_set.end() );
}

void Pattern::get_lower_bound() {
    // Evaluate lower bound
    low_b = (int)std::max(std::floor(p_l / b_l) * std::floor(p_w / b_w), std::floor(p_l / b_w) * std::floor(p_w / b_l));
}

void Pattern::get_upper_bound() {
    upp_b = std::min((int)std::floor((max_x * max_y) / (b_l * b_w)), std::min(barnes(p_l, p_w, b_l, b_w), barnes(p_l, p_w, b_w, b_l)));
}

bool Pattern::check_rules(int x1, int x2, int y1, int y2) const {
    bool block_2 = check_2_rule(x1, x2, y1, y2);
    bool block_3 = check_3_rule(x1, x2, y1, y2);
    bool block_4 = check_4_rule(x1, x2, y1, y2);
    bool block_5 = check_5_rule(x1, x2, y1, y2);

    if (block_2 ^ block_3 ^ block_4 ^ block_5) return true;
    else return false;
}

bool Pattern::check_2_rule(int x1, int x2, int y1, int y2) const {
    if (((x1 > 0 && x1 <= std::floor(p_l/2)) && y1 == 0 && x2 == x1 && y2 == 0) ^
        ((y1 > 0 && y1 <= std::floor(p_w/2)) && x1 == 0 && x2 == 0 && y2 == y1)) return true;

    return false;
}

bool Pattern::check_3_rule(int x1, int x2, int y1, int y2) const {
    if (x1 > 0 && y1 == 0 && x2 == x1 && (y2 > 0 && y2 <= std::floor(p_w/2))) return true;

    return false;
}

bool Pattern::check_4_rule(int x1, int x2, int y1, int y2) const {
    if ((x1 > 0 && y1 > 0) &&
    ((x2 == x1 && y2 > y1 && x1 <= std::floor(p_l/2)) ^
    (x2 > x1 && y2 == y1 && y1 <= std::floor(p_w/2)) ^
    (x2 == x1 && y2 == y1 && x1 <= std::floor(p_l/2) && y1 <= std::floor(p_w/2)))) return true;

    return false;
}

bool Pattern::check_5_rule(int x1, int x2, int y1, int y2) const {
    if ((x1 > 0 && y1 > 0 && x2 > x1 && y2 > y1) &&
        (x1 + x2 < p_l) ^ (x1 + x2 == p_l && y1 + y2 <= p_w)) return true;

    return false;
}

std::vector<std::pair<std::pair<int, int>, char>> Pattern::eval_points_b1(int pat_x, int pat_y) {
    int z1_v = (int) (std::floor(sizes[0].first / b_w) * (std::floor(sizes[0].second / b_l)));
    int z1_h = (int) (std::floor(sizes[0].first / b_l) * (std::floor(sizes[0].second / b_w)));
    if (z1_v > z1_h) blocks_orient[0] = 'V';
    else blocks_orient[0] = 'H';

    int dim1, dim2;
    if (blocks_orient[0] == 'V') {
        dim1 = b_w;
        dim2 = b_l;
    } else {
        dim1 = b_l;
        dim2 = b_w;
    }

    std::vector<std::pair<std::pair<int, int>, char>> tmp_points;
    // BLOCK 1
    int b1_y_padding = p_w - sizes[0].second;

    if ((std::floor(sizes[0].first / dim1) != 0) || (std::floor(sizes[0].second / dim2) != 0)) {
        for (int x = 0; x + dim1 <= sizes[0].first; x += dim1) {
            for (int y = b1_y_padding; y + dim2 <= sizes[0].second + b1_y_padding; y += dim2) {
                std::pair<int, int> point = {x + pat_x, y + pat_y};
                std::pair<std::pair<int, int>, char> point_with_orient = {point, blocks_orient[0]};
                tmp_points.push_back(point_with_orient);
            }
        }
    }
    return tmp_points;
}

std::vector<std::pair<std::pair<int, int>, char>> Pattern::eval_points_b2(int pat_x, int pat_y) {
    int z2_v = (int) (std::floor(sizes[1].first / b_w) * (std::floor(sizes[1].second / b_l)));
    int z2_h = (int) (std::floor(sizes[1].first / b_l) * (std::floor(sizes[1].second / b_w)));
    if (z2_v > z2_h) blocks_orient[1] = 'V';
    else blocks_orient[1] = 'H';

    int dim1, dim2;
    if (blocks_orient[1] == 'V') {
        dim1 = b_w;
        dim2 = b_l;
    } else {
        dim1 = b_l;
        dim2 = b_w;
    }

    std::vector<std::pair<std::pair<int, int>, char>> tmp_points;
    // BLOCK 2
    int b2_x_padding = p_l - sizes[1].first;
    int b2_y_padding = p_w - sizes[1].second;

    if ((std::floor(sizes[1].first / dim1) != 0) || (std::floor(sizes[1].second / dim2) != 0)) {
        for (int x = b2_x_padding; x + dim1 <= sizes[1].first + b2_x_padding; x += dim1) {
            for (int y = b2_y_padding; y + dim2 <= sizes[1].second + b2_y_padding; y += dim2) {
                std::pair<int, int> point = {x + pat_x, y + pat_y};
                std::pair<std::pair<int, int> , char> point_with_orient = {point, blocks_orient[1]};
                tmp_points.push_back(point_with_orient);
            }
        }
    }
    return tmp_points;
}

std::vector<std::pair<std::pair<int, int>, char>> Pattern::eval_points_b3(int pat_x, int pat_y) {
    int z3_v = (int) (std::floor(sizes[2].first / b_w) * (std::floor(sizes[2].second / b_l)));
    int z3_h = (int) (std::floor(sizes[2].first / b_l) * (std::floor(sizes[2].second / b_w)));
    if (z3_v > z3_h) blocks_orient[2] = 'V';
    else blocks_orient[2] = 'H';

    int dim1, dim2;
    if (blocks_orient[2] == 'V') {
        dim1 = b_w;
        dim2 = b_l;
    } else {
        dim1 = b_l;
        dim2 = b_w;
    }

    std::vector<std::pair<std::pair<int, int>, char>> tmp_points;
    // BLOCK 3
    int b3_x_padding, b3_y_padding;
    if ((sizes[0].first + sizes[4].first) <= p_l) {
        b3_x_padding = sizes[0].first;
        b3_y_padding = sizes[3].second;
    } else {
        b3_x_padding = sizes[3].first;
        b3_y_padding = sizes[4].second;
    }

    if ((std::floor(sizes[2].first / dim1) != 0) || (std::floor(sizes[2].second / dim2) != 0)) {
        for (int x = b3_x_padding; x + dim1 <= sizes[2].first + b3_x_padding - dim1; x += dim1) {
            for (int y = b3_y_padding; y + dim2 <= sizes[2].second + b3_y_padding - dim2; y += dim2) {
                std::pair<int, int> point = {x + pat_x, y + pat_y};
                std::pair<std::pair<int, int>, char> point_with_orient = {point, blocks_orient[2]};
                tmp_points.push_back(point_with_orient);
            }
        }
    }
    return tmp_points;
}

std::vector<std::pair<std::pair<int, int>, char>> Pattern::eval_points_b4(int pat_x, int pat_y) {
    int z4_v = (int) (std::floor(sizes[3].first / b_w) * (std::floor(sizes[3].second / b_l)));
    int z4_h = (int) (std::floor(sizes[3].first / b_l) * (std::floor(sizes[3].second / b_w)));
    if (z4_v > z4_h) blocks_orient[3] = 'V';
    else blocks_orient[3] = 'H';

    int dim1, dim2;
    if (blocks_orient[3] == 'V') {
        dim1 = b_w;
        dim2 = b_l;
    } else {
        dim1 = b_l;
        dim2 = b_w;
    }

    std::vector<std::pair<std::pair<int, int>, char>> tmp_points;
    // BLOCK 4
    if ((std::floor(sizes[3].first / dim1) != 0) || (std::floor(sizes[3].second / dim2) != 0)) {
        for (int x = 0; x + dim1 <= sizes[3].first; x += dim1) {
            for (int y = 0; y + dim2 <= sizes[3].second; y += dim2) {
                std::pair<int, int> point = {x + pat_x, y + pat_y};
                std::pair<std::pair<int, int>, char> point_with_orient = {point, blocks_orient[3]};
                tmp_points.push_back(point_with_orient);
            }
        }
    }
    return tmp_points;
}

std::vector<std::pair<std::pair<int, int>, char>> Pattern::eval_points_b5(int pat_x, int pat_y) {
    int z5_v = (int) (std::floor(sizes[4].first / b_w) * (std::floor(sizes[4].second / b_l)));
    int z5_h = (int) (std::floor(sizes[4].first / b_l) * (std::floor(sizes[4].second / b_w)));
    if (z5_v > z5_h) blocks_orient[4] = 'V';
    else blocks_orient[4] = 'H';

    int dim1, dim2;
    if (blocks_orient[4] == 'V') {
        dim1 = b_w;
        dim2 = b_l;
    } else {
        dim1 = b_l;
        dim2 = b_w;
    }

    std::vector<std::pair<std::pair<int, int>, char>> tmp_points;
    // BLOCK 5
    int b5_x_padding = p_l - sizes[4].first;

    if ((std::floor(sizes[4].first / dim1) != 0) && (std::floor(sizes[4].second / dim2) != 0)) {
        for (int x = b5_x_padding; x + dim1 <= sizes[4].first + b5_x_padding; x += dim1) {
            for (int y = 0; y + dim2 <= sizes[4].second; y += dim2) {
                std::pair<int, int> point = {x + pat_x, y + pat_y};
                std::pair<std::pair<int, int>, char> point_with_orient = {point, blocks_orient[4]};
                tmp_points.push_back(point_with_orient);
            }
        }
    }
    return tmp_points;
}