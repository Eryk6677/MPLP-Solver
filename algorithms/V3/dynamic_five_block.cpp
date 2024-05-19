#include "../pattern.h"

int Pattern::recursive_bd2(int rec_lvl, int max_time, int pat_x, int pat_y,
                           std::chrono::time_point<std::chrono::system_clock> time_point) {
    Node current_node = check_map();

    if (low_b == 0) return 0;

    if (current_node.reached_limit) {
        std::pair<int, int> size_of_elem = {p_l, p_w};
        sizes = {size_of_elem};

        for (const auto &point: eval_points_b1(pat_x, pat_y)) points.push_back(point);
        best_points = points;
        return low_b;
    }

    if (low_b == upp_b) {
        int x1, x2, y1, y2;

        if (current_node.l == p_l && current_node.w == p_w) {
            x1 = current_node.x1;
            x2 = current_node.x2;
            y1 = current_node.y1;
            y2 = current_node.y2;
        } else if (current_node.l == p_w && current_node.w == p_l){
            x1 = current_node.y1;
            x2 = current_node.y2;
            y1 = current_node.x1;
            y2 = current_node.x2;
        }

        return solve_procedure(rec_lvl, max_time, pat_x, pat_y, x1, x2, y1, y2, time_point);
    } else {
        this->evaluate_raster_points();
        int x1_index, x2_index, y1_index, y2_index;

        if (current_node.l == p_l) {
            x1_index = findIndexInVector(l_set, current_node.x1);
            x2_index = findIndexInVector(l_set, current_node.x2);
            y1_index = findIndexInVector(l_set, current_node.y1);
            y2_index = findIndexInVector(l_set, current_node.y2);
        } else {
            x1_index = findIndexInVector(l_set, current_node.y1);
            x2_index = findIndexInVector(l_set, current_node.y2);
            y1_index = findIndexInVector(l_set, current_node.x1);
            y2_index = findIndexInVector(l_set, current_node.x2);
        }

        for (int x1_i = x1_index; x1_i < l_set.size(); x1_i++) {
            for (int x2_i = x2_index; x2_i < l_set.size(); x2_i++) {
                for (int y1_i = y1_index; y1_i < w_set.size(); y1_i++) {
                    for (int y2_i = y2_index; y2_i < w_set.size(); y2_i++) {

                        points.clear();

                        int x1, x2, y1, y2;
                        x1 = l_set.at(x1_i);
                        x2 = l_set.at(x2_i);
                        y1 = w_set.at(y1_i);
                        y2 = w_set.at(y2_i);

                        if (check_rules(x1, x2, y1, y2)) {
                            solve_procedure(rec_lvl, max_time, pat_x, pat_y, x1, x2, y1, y2, time_point);
                            if (low_b == upp_b) return low_b;
                        }
                    }
                }
            }
        }
        return low_b;
    }
}

int Pattern::solve_procedure(int rec_lvl, int max_time, int pat_x, int pat_y, int x1, int x2, int y1, int y2,
                             std::chrono::time_point<std::chrono::system_clock> time_point) {
    std::pair<int, int> size1 = {x1, p_w - y1};
    std::pair<int, int> size2 = {p_l - x1, p_w - y2};
    std::pair<int, int> size3 = {x2 - x1, y2 - y1};
    std::pair<int, int> size4 = {x2, y1};
    std::pair<int, int> size5 = {p_l - x2, y2};
    sizes = {size1, size2, size3, size4, size5};

    int z1, z2, z3, z4, z5;
    int z_0 = 0;

    auto duration = std::chrono::duration_cast<std::chrono::minutes>(std::chrono::high_resolution_clock::now() - time_point);

    if ((rec_lvl > 0) && (duration.count() < 5)) {
        Pattern pattern1(size1.first, size1.second, b_l, b_w, map_ref);
        pattern1.set_l_normal_set(this->l_normal_set);
        pattern1.set_w_normal_set(this->w_normal_set);
        pattern1.check_map();

        Pattern pattern2(size2.first, size2.second, b_l, b_w, map_ref);
        pattern2.set_l_normal_set(this->l_normal_set);
        pattern2.set_w_normal_set(this->w_normal_set);
        pattern2.check_map();

        Pattern pattern3(size3.first, size3.second, b_l, b_w, map_ref);
        pattern3.set_l_normal_set(this->l_normal_set);
        pattern3.set_w_normal_set(this->w_normal_set);
        pattern3.check_map();

        Pattern pattern4(size4.first, size4.second, b_l, b_w, map_ref);
        pattern4.set_l_normal_set(this->l_normal_set);
        pattern4.set_w_normal_set(this->w_normal_set);
        pattern4.check_map();

        Pattern pattern5(size5.first, size5.second, b_l, b_w, map_ref);
        pattern5.set_l_normal_set(this->l_normal_set);
        pattern5.set_w_normal_set(this->w_normal_set);
        pattern5.check_map();

        int sum_of_bounds = pattern1.upp_b + pattern2.upp_b + pattern3.upp_b + pattern4.upp_b + pattern5.upp_b;

        // TEMPORARY POINTS
        std::vector<std::pair<std::pair<int, int>, char>> rest_of_points;
        // TEMPORARY POINTS

        if (low_b < sum_of_bounds) {
            // FIRST
            z1 = pattern1.recursive_bd2(rec_lvl - 1, max_time, pat_x, pat_y + (p_w - size1.second), time_point);
            for (const auto &point: pattern1.best_points) points.push_back(point);

            z_0 += z1;

            sum_of_bounds = sum_of_bounds - pattern1.upp_b + z1;
            if (low_b < sum_of_bounds) {
                // SECOND
                z2 = pattern2.recursive_bd2(rec_lvl - 1, max_time, pat_x + (p_l - size2.first), pat_y + (p_w - size2.second), time_point);
                for (const auto &point: pattern2.best_points) points.push_back(point);

                z_0 += z2;

                sum_of_bounds = sum_of_bounds - pattern2.upp_b + z2;
                if (low_b < sum_of_bounds) {
                    // THIRD
                    int b3_x_pad, b3_y_pad;
                    if ((size1.first + size5.first) <= p_l) {
                        b3_x_pad = size1.first;
                        b3_y_pad = size4.second;
                    } else {
                        b3_x_pad = size4.first;
                        b3_y_pad = size5.second;
                    }
                    z3 = pattern3.recursive_bd2(rec_lvl - 1, max_time, pat_x + b3_x_pad, pat_y + b3_y_pad, time_point);
                    for (const auto &point: pattern3.best_points) points.push_back(point);

                    z_0 += z3;

                    sum_of_bounds = sum_of_bounds - pattern3.upp_b + z3;
                    if (low_b < sum_of_bounds) {
                        // FOURTH
                        z4 = pattern4.recursive_bd2(rec_lvl - 1, max_time, pat_x, pat_y, time_point);
                        for (const auto &point: pattern4.best_points) points.push_back(point);

                        z_0 += z4;

                        sum_of_bounds = sum_of_bounds - pattern4.upp_b + z4;
                        if (low_b < sum_of_bounds) {
                            // FIFTH
                            z5 = pattern5.recursive_bd2(rec_lvl - 1, max_time, pat_x + (p_l - size5.first), pat_y, time_point);
                            for (const auto &point: pattern5.best_points) points.push_back(point);

                            z_0 += z5;

                            rest_of_points.clear();
                            if (z_0 + rest_of_points.size() > low_b) {
                                low_b = z_0;
                                best_points = points;
                                saveSizeInNode(x1, x2, y1, y2);
                                updateLowerBound();
                                if (low_b == upp_b) return low_b;
                            }
                        } else {
                            rest_of_points.clear();
                            for (const auto &point: eval_points_b5(pat_x, pat_y)) rest_of_points.push_back(point);

                            if (z_0 + rest_of_points.size() > low_b) {
                                low_b = z_0;
                                for (const auto &point: rest_of_points) points.push_back(point);
                                best_points = points;
                                saveSizeInNode(x1, x2, y1, y2);
                                updateLowerBound();
                                if (low_b == upp_b) return low_b;
                            }
                        }
                    } else {
                        rest_of_points.clear();
                        for (const auto &point: eval_points_b4(pat_x, pat_y)) rest_of_points.push_back(point);
                        for (const auto &point: eval_points_b5(pat_x, pat_y)) rest_of_points.push_back(point);

                        if (z_0 + rest_of_points.size() > low_b) {
                            low_b = z_0;
                            for (const auto &point: rest_of_points) points.push_back(point);
                            best_points = points;
                            saveSizeInNode(x1, x2, y1, y2);
                            updateLowerBound();
                            if (low_b == upp_b) return low_b;
                        }
                    }
                } else {
                    rest_of_points.clear();
                    for (const auto &point: eval_points_b3(pat_x, pat_y)) rest_of_points.push_back(point);
                    for (const auto &point: eval_points_b4(pat_x, pat_y)) rest_of_points.push_back(point);
                    for (const auto &point: eval_points_b5(pat_x, pat_y)) rest_of_points.push_back(point);

                    if (z_0 + rest_of_points.size() > low_b) {
                        low_b = z_0;
                        for (const auto &point: rest_of_points) points.push_back(point);
                        best_points = points;
                        saveSizeInNode(x1, x2, y1, y2);
                        updateLowerBound();
                        if (low_b == upp_b) return low_b;
                    }
                }


            } else {
                rest_of_points.clear();
                for (const auto &point: eval_points_b2(pat_x, pat_y)) rest_of_points.push_back(point);
                for (const auto &point: eval_points_b3(pat_x, pat_y)) rest_of_points.push_back(point);
                for (const auto &point: eval_points_b4(pat_x, pat_y)) rest_of_points.push_back(point);
                for (const auto &point: eval_points_b5(pat_x, pat_y)) rest_of_points.push_back(point);

                if (z_0 + rest_of_points.size() > low_b) {
                    low_b = z_0;
                    for (const auto &point: rest_of_points) points.push_back(point);
                    best_points = points;
                    saveSizeInNode(x1, x2, y1, y2);
                    updateLowerBound();
                    if (low_b == upp_b) return low_b;
                }
            }
        } else {
            for (const auto &point: eval_points_b1(pat_x, pat_y)) points.push_back(point);
            for (const auto &point: eval_points_b2(pat_x, pat_y)) points.push_back(point);
            for (const auto &point: eval_points_b3(pat_x, pat_y)) points.push_back(point);
            for (const auto &point: eval_points_b4(pat_x, pat_y)) points.push_back(point);
            for (const auto &point: eval_points_b5(pat_x, pat_y)) points.push_back(point);

            z_0 = points.size();

            if (z_0 > low_b) {
                low_b = z_0;
                best_points = points;
                saveSizeInNode(x1, x2, y1, y2);
                updateLowerBound();
                if (low_b == upp_b) return low_b;
            }
        }
    } else {
        for (const auto &point: eval_points_b1(pat_x, pat_y)) points.push_back(point);
        for (const auto &point: eval_points_b2(pat_x, pat_y)) points.push_back(point);
        for (const auto &point: eval_points_b3(pat_x, pat_y)) points.push_back(point);
        for (const auto &point: eval_points_b4(pat_x, pat_y)) points.push_back(point);
        for (const auto &point: eval_points_b5(pat_x, pat_y)) points.push_back(point);

        z_0 = points.size();

        if (z_0 > low_b) {
            low_b = z_0;
            best_points = points;
            saveSizeInNode(x1, x2, y1, y2);
            updateLowerBound();
            if (low_b == upp_b) return low_b;
        }
    }
    if (points.size() > best_points.size()) {
        best_points = points;
        saveSizeInNode(x1, x2, y1, y2);
    }
    return low_b;
}

Node Pattern::check_map() {
    if (this->p_l != 0 && this->p_w != 0) {
        auto it1 = map_ref->find({this->p_l, this->p_w});
        if (it1 != map_ref->end()) {
            this->low_b = it1->second.low_b;
            this->upp_b = it1->second.upp_b;

            return it1->second;
        }

        auto it2 = map_ref->find({this->p_w, this->p_l});
        if (it2 != map_ref->end()) {
            this->low_b = it2->second.low_b;
            this->upp_b = it2->second.upp_b;

            return it2->second;
        }

        this->evaluate_raster_points();
        this->get_lower_bound();
        this->get_upper_bound();

        Node new_node = Node(this->p_l, this->p_w);
        new_node.low_b = this->low_b;
        new_node.upp_b = this->upp_b;
        if (this->low_b == this->upp_b) new_node.reached_limit = true;

        map_ref->insert({{this->p_l, this->p_w}, new_node});
        return new_node;
    } else {
        return {0, 0};
    }
}

void Pattern::saveSizeInNode(int x1, int x2, int y1, int y2) {
    auto it1 = map_ref->find({p_l, p_w});
    if (it1 != map_ref->end()) {
        it1->second.x1 = x1;
        it1->second.x2 = x2;
        it1->second.y1 = y1;
        it1->second.y2 = y2;

        return;
    }

    auto it2 = map_ref->find({p_w, p_l});
    if (it2 != map_ref->end()) {
        it2->second.x1 = y1;
        it2->second.x2 = y2;
        it2->second.y1 = x1;
        it2->second.y2 = x2;

        return;
    }
}

void Pattern::updateLowerBound() {
    auto it1 = map_ref->find({p_l, p_w});
    if (it1 != map_ref->end()) {
        it1->second.low_b = low_b;
        return;
    }

    auto it2 = map_ref->find({p_w, p_l});
    if (it2 != map_ref->end()) {
        it2->second.low_b = low_b;
        return;
    }
}

int Pattern::findIndexInVector(std::vector<int> vecOfElements, int element) {
    // Find given element in vector
    auto it = std::find(vecOfElements.begin(), vecOfElements.end(), element);

    if (it != vecOfElements.end()) return distance(vecOfElements.begin(), it);
    else return -1;
}
