#include <fstream>
#include <vector>
#include "../exception_handler.h"

void write_summary(const std::vector<std::vector<std::string>> &content, Exception_handler* handler) {
    std::ofstream my_file;
    my_file.open("summary.csv", std::ios::out);

    if (!my_file.is_open()) {
        handler->set_error("Unable to write SUMMARY");
        return;
    }

    // Initial line:
    my_file << "P_length,P_width,B_length,B_width,Upp_B,Result,Time\n";

    // Content
    for (const auto& current_instance : content) {
        for (const auto& element : current_instance) {
            my_file << element << ",";
        }
        my_file << "\n";
    }

    my_file.close();
}
