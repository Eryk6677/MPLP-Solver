#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <filesystem>
#include "../exception_handler.h"

std::vector<std::vector<int>> read_file(const std::string& file_name, Exception_handler* handler) {
    std::vector<std::vector<int>> instances;

    std::ifstream file_reader (file_name);
    if (file_reader.is_open()) {

        std::string cur_line;
        int line_num = 1;

        while (getline(file_reader, cur_line)) {
            std::vector<int> instance;

            std::istringstream string_stream(cur_line);

            std::string num;
            while (string_stream >> num) {
                int parsed_param;

                try {
                    parsed_param = std::stoi(num);
                    instance.push_back(parsed_param);
                } catch(std::exception &err) {
                    handler->set_error("Error reading instance in line: " + std::to_string(line_num) + " - Parameter is NOT a number");
                    return instances;
                }
            }

            if (instance.size() != 4) {
                handler->set_error("Error reading instance in line: " + std::to_string(line_num) + " - Incorrect number of parameters");
                return instances;
            }

            instances.push_back(instance);
            line_num++;
        }
        file_reader.close();

    } else handler->set_error("Unable to read file: " + file_name);

    return instances;
}

void write_points(const std::vector<std::pair<std::pair<int, int>, char>> &points, int p_l, int p_w, int b_l, int b_w, Exception_handler* handler) {
    std::filesystem::path cwd = std::filesystem::current_path();
    std::filesystem::create_directory("results");

    std::ofstream my_file;
    std::string file_name = std::to_string(p_l) + "_" + std::to_string(p_w) + "_" + std::to_string(b_l) + "_" + std::to_string(b_w);

    my_file.open("results/" + file_name + ".txt");

    if (!my_file.is_open()) {
        handler->set_error("Unable to write POINTS for instance: " + file_name);
        return;
    }

    my_file << p_l << " " << p_w << " " << b_l << " " << b_w << "\n";

    for (auto & point : points) {
        my_file << point.first.first << " " << point.first.second << " " << point.second << "\n";
    }

    my_file.close();
}
