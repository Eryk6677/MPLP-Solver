#ifndef LAUNCHER_H
#define LAUNCHER_H

#include "algorithms/pattern.h"
#include "algorithms_enum.h"
#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class Launcher1; }
QT_END_NAMESPACE

class Launcher : public QMainWindow
{
    Q_OBJECT

public:
    Launcher(QWidget *parent = nullptr);
    ~Launcher();

private slots:
    void checkValue();
    void saveValue();
    void executeAlgorithm();
    void enableBrowser();

private:
    Ui::Launcher1 *ui;
    int solve_instance(Algorithms chosen_alg, Pattern* pattern, int max_depth, int max_time, int p_l, int p_w, int b_l, int b_w);
    void clear_field();
};
#endif // LAUNCHER_H
