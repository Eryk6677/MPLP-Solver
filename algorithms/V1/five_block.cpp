#include "../pattern.h"

int Pattern::evaluate_five_block(int rec_lvl, int max_time, int pat_x, int pat_y,
                                 std::chrono::time_point<std::chrono::system_clock> time_point) {
    // When block is smaller than shorter side of element it's impossible to place any elements
    if (p_l < b_w || p_w < b_w) return 0;

    // Evaluate bounds
    get_lower_bound();

    get_max_coverages();
    get_upper_bound();

    if (low_b == upp_b) {
        std::pair<int, int> size_of_elem = {p_l, p_w};
        sizes = {size_of_elem};

        for (const auto &point: eval_points_b1(pat_x, pat_y)) points.push_back(point);
        best_points = points;
        return low_b;
    }

    blocks_orient[0] = 'H';
    blocks_orient[1] = 'V';
    blocks_orient[3] = 'V';
    blocks_orient[4] = 'H';

    for (auto & p1 : *p_set) {
        for (auto & p2 : *p_set) {

            int length1 = p1.first * b_l + p1.second * b_w;
            int length2 = p2.first * b_l + p2.second * b_w;
            if (length1 <= p_l && length2 <= p_l && length1 <= length2) {

                for (auto & q1 : *q_set) {
                    for (auto & q2 : *q_set) {

                        int width1 = q1.first * b_l + q1.second * b_w;
                        int width2 = q2.first * b_l + q2.second * b_w;
                        if (width1 <= p_w && width2 <= p_w && width1 <= width2) {

                            std::pair<int, int> size1 = {p1.first * b_l, q1.second * b_w};
                            std::pair<int, int> size2 = {p1.second * b_w, q2.first * b_l};
                            std::pair<int, int> size4 = {p2.second * b_w, q1.first * b_l};
                            std::pair<int, int> size5 = {p2.first * b_l, q2.second * b_w};

                            int len_3, wid_3;
                            if ((size1.first + size5.first) <= p_l) {
                                len_3 = p_l - size1.first - size5.first;
                                wid_3 = p_w - size2.second - size4.second;
                            } else {
                                len_3 = p_l - size2.first - size4.first;
                                wid_3 = p_w - size1.second - size5.second;
                            }

                            std::pair<int, int> size3 = {len_3, wid_3};

                            sizes = {size1, size2, size3, size4, size5};

                            // Check if block do not overlap
                            if (((size1.first + size5.first <= p_l) or (size1.second + size5.second <= p_w)) and
                                ((size2.first + size4.first <= p_l) or (size2.second + size4.second <= p_w))) {

                                int z1, z2, z3, z4, z5;
                                int tmp_low_b;
                                points.clear();

                                auto duration = std::chrono::duration_cast<std::chrono::minutes>(
                                        std::chrono::high_resolution_clock::now() - time_point);

                                if ((rec_lvl > 0) && (duration.count() < max_time)) {
                                    // Block 4
                                    Pattern pattern4(size4.first, size4.second, b_l, b_w);
                                    pattern4.set_p_set(this->p_set);
                                    pattern4.set_q_set(this->q_set);

                                    z4 = pattern4.evaluate_five_block(rec_lvl - 1, max_time, pat_x, pat_y, time_point);

                                    // Block 1
                                    Pattern pattern1(size1.first, size1.second, b_l, b_w);
                                    pattern1.set_p_set(this->p_set);
                                    pattern1.set_q_set(this->q_set);

                                    z1 = pattern1.evaluate_five_block(rec_lvl - 1, max_time, pat_x, pat_y + (p_w - size1.second),
                                                                      time_point);

                                    // Block 5
                                    Pattern pattern5(size5.first, size5.second, b_l, b_w);
                                    pattern5.set_p_set(this->p_set);
                                    pattern5.set_q_set(this->q_set);

                                    z5 = pattern5.evaluate_five_block(rec_lvl - 1, max_time, pat_x + (p_l - size5.first), pat_y,
                                                                      time_point);

                                    // Block 2
                                    Pattern pattern2(size2.first, size2.second, b_l, b_w);
                                    pattern2.set_p_set(this->p_set);
                                    pattern2.set_q_set(this->q_set);

                                    z2 = pattern2.evaluate_five_block(rec_lvl - 1, max_time, pat_x + (p_l - size2.first),
                                                                      pat_y + (p_w - size2.second), time_point);

                                    // Block 3
                                    Pattern pattern3(size3.first, size3.second, b_l, b_w);
                                    pattern3.set_p_set(this->p_set);
                                    pattern3.set_q_set(this->q_set);

                                    int b3_x_pad, b3_y_pad;
                                    if ((size1.first + size5.first) <= p_l) {
                                        b3_x_pad = size1.first;
                                        b3_y_pad = size4.second;
                                    } else {
                                        b3_x_pad = size4.first;
                                        b3_y_pad = size5.second;
                                    }
                                    z3 = pattern3.evaluate_five_block(rec_lvl - 1, max_time, pat_x + b3_x_pad, pat_y + b3_y_pad,
                                                                      time_point);

                                    tmp_low_b = z1 + z2 + z3 + z4 + z5;

                                    for (const auto &point: pattern1.best_points) points.push_back(point);
                                    for (const auto &point: pattern2.best_points) points.push_back(point);
                                    for (const auto &point: pattern3.best_points) points.push_back(point);
                                    for (const auto &point: pattern4.best_points) points.push_back(point);
                                    for (const auto &point: pattern5.best_points) points.push_back(point);

                                } else {
                                    z1 = std::max(
                                            (int) (std::floor(size1.first / b_l) * (std::floor(size1.second / b_w))),
                                            (int) (std::floor(size1.first / b_w) * (std::floor(size1.second / b_l))));
                                    z2 = std::max(
                                            (int) (std::floor(size2.first / b_l) * (std::floor(size2.second / b_w))),
                                            (int) (std::floor(size2.first / b_w) * (std::floor(size2.second / b_l))));

                                    z4 = std::max(
                                            (int) (std::floor(size4.first / b_l) * (std::floor(size4.second / b_w))),
                                            (int) (std::floor(size4.first / b_w) * (std::floor(size4.second / b_l))));
                                    z5 = std::max(
                                            (int) (std::floor(size5.first / b_l) * (std::floor(size5.second / b_w))),
                                            (int) (std::floor(size5.first / b_w) * (std::floor(size5.second / b_l))));

                                    int z3_v = (int) (std::floor(size3.first / b_w) * (std::floor(size3.second / b_l)));
                                    int z3_h = (int) (std::floor(size3.first / b_l) * (std::floor(size3.second / b_w)));
                                    if (z3_v > z3_h) {
                                        blocks_orient[2] = 'V';
                                        z3 = z3_v;
                                    } else {
                                        blocks_orient[2] = 'H';
                                        z3 = z3_h;
                                    }

                                    tmp_low_b = z1 + z2 + z3 + z4 + z5;

                                    eval_points_with_predefined_orientation(pat_x, pat_y);
                                }
                                if (tmp_low_b > low_b) {
                                    low_b = tmp_low_b;
                                    best_points = points;
                                    if (low_b == upp_b) return low_b;
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    if (points.size() > best_points.size()) best_points = points;
    return low_b;
}

void Pattern::eval_points_with_predefined_orientation(int pat_x, int pat_y) {
    std::vector<std::pair<std::pair<int, int>, char>> result;

    // BLOCK 4
    if ((std::floor(sizes[3].first/b_w) != 0) && (std::floor(sizes[3].second/b_l) != 0)) {
        for (int x = 0; x < sizes[3].first; x += b_w) {
            for (int y = 0; y < sizes[3].second; y += b_l) {
                std::pair<int, int> point = {x + pat_x, y + pat_y};
                std::pair<std::pair<int, int>, char> point_with_orient = {point, blocks_orient[3]};
                result.push_back(point_with_orient);
            }
        }
    }

    // BLOCK 1
    int b1_y_padding = p_w - sizes[0].second;

    if ((std::floor(sizes[0].first/b_l) != 0) && (std::floor(sizes[0].second/b_w) != 0)) {
        for (int x = 0; x < sizes[0].first; x += b_l) {
            for (int y = b1_y_padding; y < sizes[0].second + b1_y_padding; y += b_w) {
                std::pair<int, int> point = {x + pat_x, y + pat_y};
                std::pair<std::pair<int, int>, char> point_with_orient = {point, blocks_orient[0]};
                result.push_back(point_with_orient);
            }
        }
    }

    // BLOCK 3
    int dim1, dim2;
    if (blocks_orient[2] == 'V') {
        dim1 = b_w;
        dim2 = b_l;
    } else {
        dim1 = b_l;
        dim2 = b_w;
    }

    int b3_x_padding, b3_y_padding;
    if ((sizes[0].first + sizes[4].first) <= p_l) {
        b3_x_padding = sizes[0].first;
        b3_y_padding = sizes[3].second;
    } else {
        b3_x_padding = sizes[3].first;
        b3_y_padding = sizes[4].second;
    }

    if ((std::floor(sizes[2].first/dim1) != 0) || (std::floor(sizes[2].second/dim2) != 0)) {
        for (int x = b3_x_padding; x <= sizes[2].first + b3_x_padding - dim1; x += dim1) {
            for (int y = b3_y_padding; y <= sizes[2].second + b3_y_padding - dim2; y += dim2) {
                std::pair<int, int> point = {x + pat_x, y + pat_y};
                std::pair<std::pair<int, int>, char> point_with_orient = {point, blocks_orient[2]};
                result.push_back(point_with_orient);
            }
        }
    }

    // BLOCK 5
    int b5_x_padding = p_l - sizes[4].first;

    if ((std::floor(sizes[4].first/b_l) != 0) && (std::floor(sizes[4].second/b_w) != 0)) {
        for (int x = b5_x_padding; x < sizes[4].first + b5_x_padding; x += b_l) {
            for (int y = 0; y < sizes[4].second; y += b_w) {
                std::pair<int, int> point = {x + pat_x, y + pat_y};
                std::pair<std::pair<int, int>, char> point_with_orient = {point, blocks_orient[4]};
                result.push_back(point_with_orient);
            }
        }
    }

    // BLOCK 2
    int b2_x_padding = p_l - sizes[1].first;
    int b2_y_padding = p_w - sizes[1].second;

    if ((std::floor(sizes[1].first/b_w) != 0) && (std::floor(sizes[1].second/b_l) != 0)) {
        for (int x = b2_x_padding; x < sizes[1].first + b2_x_padding; x += b_w) {
            for (int y = b2_y_padding; y < sizes[1].second + b2_y_padding; y += b_l) {
                std::pair<int, int> point = {x + pat_x, y + pat_y};
                std::pair<std::pair<int, int> , char> point_with_orient = {point, blocks_orient[1]};
                result.push_back(point_with_orient);
            }
        }
    }

    points = result;
}
