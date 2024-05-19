#include "launcher.h"
#include "./ui_launcher.h"
#include "algorithms_enum.h"
#include "algorithms/pattern.h"
#include <QFileDialog>
#include <iostream>
#include <vector>
#include <chrono>
#include <map>
#include <QTimer>
#include "utilities/txt_utilities.h"
#include "utilities/csv_utilities.h"
#include "algorithms/V3/node.h"
#include "algorithms/pattern.h"

Launcher::Launcher(QWidget *parent) : QMainWindow(parent), ui(new Ui::Launcher1) {
    ui->setupUi(this);

    connect(ui->file_name_field, &QLineEdit::textChanged, this, &Launcher::checkValue);

    connect(ui->browse_button, &QPushButton::pressed, this, &Launcher::enableBrowser);

    connect(ui->pl_field, &QSpinBox::valueChanged, this, &Launcher::saveValue);
    connect(ui->pw_field, &QSpinBox::valueChanged, this, &Launcher::saveValue);
    connect(ui->el_field, &QSpinBox::valueChanged, this, &Launcher::saveValue);
    connect(ui->ew_field, &QSpinBox::valueChanged, this, &Launcher::saveValue);

    connect(ui->solve_button, &QPushButton::pressed, this, &Launcher::executeAlgorithm);
}

void Launcher::checkValue() {
    bool isEmpty = true;
    if (ui->file_name_field->text() != "") isEmpty = false;

    if (!isEmpty) {
        ui->pl_field->setDisabled(true);
        ui->pw_field->setDisabled(true);
        ui->el_field->setDisabled(true);
        ui->ew_field->setDisabled(true);

        ui->solve_button->setDisabled(false);
    } else {
        ui->pl_field->setDisabled(false);
        ui->pw_field->setDisabled(false);
        ui->el_field->setDisabled(false);
        ui->ew_field->setDisabled(false);

        ui->solve_button->setDisabled(true);
    }
}

void Launcher::enableBrowser() {
    QString file_name = QFileDialog::getOpenFileName(this, "Choose a file", "~", "*.txt");
    ui->file_name_field->setText(file_name);
}

void Launcher::saveValue() {
    int instance[4] = {ui->pl_field->value(), ui->pw_field->value(), ui->el_field->value(), ui->ew_field->value()};

    if (instance[0] != 0 && instance[1] != 0 && instance[2] != 0 && instance[3] != 0) {
        ui->solve_button->setDisabled(false);
    } else {
        ui->solve_button->setDisabled(true);
    }
}

void Launcher::executeAlgorithm() {
    ui->results->clear();

    QTimer::singleShot(100, [=]() {
        int max_depth = ui->rec_field->value();
        int max_time = ui->exec_time_field->value();

        Exception_handler handler;
        handler.reset_error();

        bool isEmpty = true;
        if (ui->file_name_field->text() != "") isEmpty = false;

        if (isEmpty) {
            int instance[4] = {ui->pl_field->value(), ui->pw_field->value(), ui->el_field->value(), ui->ew_field->value()};

            Pattern pattern(instance[0], instance[1], instance[2], instance[3]);
            Algorithms chosen_alg = resolve_alg_name((ui->alg_select->currentText().toStdString()));

            // EVALUATION
            int exec_time = solve_instance(chosen_alg, &pattern, max_depth, max_time, instance[0], instance[1], instance[2], instance[3]);

            // write points
            write_points(pattern.best_points, instance[0], instance[1], instance[2], instance[3], &handler);

            // write summary
            std::string result_info = "Algorithm: " + ui->alg_select->currentText().toStdString() + "\n";
            result_info += "Instance: (" + std::to_string(instance[0]) + ", " + std::to_string(instance[1]) + ", " + std::to_string(instance[2]) + ", " + std::to_string(instance[3]) + ")\n";
            result_info += "Packed items: " + std::to_string(pattern.low_b) + "\n";
            result_info += "Upper bound: " + std::to_string(pattern.upp_b) + "\n";
            result_info += "Execution time: " + std::to_string(exec_time) + " ms\n";

            if (handler.error_occured) result_info += handler.error_msg;

            ui->results->setPlainText(QString::fromStdString(result_info));

        } else {
            // reading the input file
            std::vector<std::vector<int>> instances = read_file(ui->file_name_field->text().toStdString(), &handler);
            if (handler.error_occured) {
                ui->results->setPlainText(QString::fromStdString(handler.error_msg));
                return;
            }

            // Execution
            std::vector<std::vector<std::string>> execution_result;

            Algorithms chosen_alg = resolve_alg_name((ui->alg_select->currentText().toStdString()));

            for (int i = 0; i < instances.size(); i++) {
                std::vector<std::string> current_exec;

                std::vector<int> instance = instances.at(i);
                if (instance[0] == 0 || instance[1] == 0 || instance[2] == 0 || instance[3] == 0) continue;

                Pattern pattern(instance[0], instance[1], instance[2], instance[3]);

                current_exec.push_back(std::to_string(instance[0]));
                current_exec.push_back(std::to_string(instance[1]));
                current_exec.push_back(std::to_string(instance[2]));
                current_exec.push_back(std::to_string(instance[3]));

                // EVALUATION
                int exec_time = solve_instance(chosen_alg, &pattern, max_depth, max_time, instance[0], instance[1], instance[2], instance[3]);

                // Save for summary
                current_exec.push_back(std::to_string(pattern.upp_b));
                current_exec.push_back(std::to_string(pattern.low_b));
                current_exec.push_back(std::to_string(exec_time));
                execution_result.push_back(current_exec);

                // write points
                write_points(pattern.best_points, instance[0], instance[1], instance[2], instance[3], &handler);

                // write summary
                std::string result_info = "Finished: " + std::to_string(i + 1) + " out of: " + std::to_string(instances.size()) + " " + handler.error_msg;
                ui->results->setPlainText(QString::fromStdString(result_info));

                handler.reset_error();
            }

            write_summary(execution_result, &handler);

            std::string msg;
            if (handler.error_occured) msg = handler.error_msg;
            else msg = "DONE";
            ui->results->setPlainText(QString::fromStdString(msg));
        }
    });
}

int Launcher::solve_instance(Algorithms chosen_alg, Pattern* pattern, int max_depth, int max_time, int p_l, int p_w, int b_l, int b_w) {
    switch (chosen_alg) {
        case FIVE_BLOCK: {
            auto start = std::chrono::high_resolution_clock::now();

            // evaluation
            std::vector <std::pair<int, int>> p_set = evaluate_partitions(p_l, b_l, b_w);
            std::vector <std::pair<int, int>> q_set = evaluate_partitions(p_w, b_l, b_w);

            pattern->set_p_set(&p_set);
            pattern->set_q_set(&q_set);

            pattern->evaluate_five_block(max_depth, max_time, 0, 0, start);
            auto stop = std::chrono::high_resolution_clock::now();

            // results
            return std::chrono::duration_cast<std::chrono::milliseconds>(stop - start).count();
        }

        case MODIFIED: {
            auto start = std::chrono::high_resolution_clock::now();

            // evaluation
            std::vector <int> normal_l = evaluate_normal_points(p_l, b_l, b_w);
            std::vector <int> normal_w = evaluate_normal_points(p_w, b_l, b_w);

            pattern->set_l_normal_set(&normal_l);
            pattern->set_w_normal_set(&normal_w);
            pattern->evaluate_raster_points();

            pattern->evaluate_improved(max_depth, max_time, 0, 0, start);
            auto stop = std::chrono::high_resolution_clock::now();

            // results
            return std::chrono::duration_cast<std::chrono::milliseconds>(stop - start).count();
        }

        case IMPROVED: {
            auto start = std::chrono::high_resolution_clock::now();

            // evaluation
            std::map<std::pair<int, int>, Node> nodes_vector;
            pattern->map_ref = &nodes_vector;

            std::vector <int> normal_l = evaluate_normal_points(p_l, b_l, b_w);
            std::vector <int> normal_w = evaluate_normal_points(p_w, b_l, b_w);

            pattern->set_l_normal_set(&normal_l);
            pattern->set_w_normal_set(&normal_w);

            pattern->recursive_bd2(max_depth, max_time, 0, 0, start);
            auto stop = std::chrono::high_resolution_clock::now();

            // results
            return std::chrono::duration_cast<std::chrono::milliseconds>(stop - start).count();
        }
    }

    return 0;
}

void Launcher::clear_field() {
    ui->results->clear();
}

Launcher::~Launcher() {
    delete ui;
}

