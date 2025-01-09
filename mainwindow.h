#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>
#include <QGraphicsRectItem>
#include <QMouseEvent>
#include <vector>
#include <QTimer>
#include "graph_algorithm.h"
#include "bfs_graph_algorithm.h"
#include "dfs_graph_algorithm.h"
#include "a_star_graph_algorithm.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void mousePressEvent(QMouseEvent *event) override;
    void on_start_btn_clicked();
    void on_go_clicked();
    void on_reset_algorithm_clicked();
    void on_speed_valueChanged(int value);
    void on_stop_visualization_clicked();

private:
    Ui::MainWindow *ui;
    QGraphicsScene scene;
    void clear_gui();
    void clear_visualized_algorithm();
    void clear_flags();
    void set_algorithms_steps(int steps);
    void set_graph_algorithm(graph_algorithm *graph);
    graph_algorithm *current_graph_algorithm;

    bool target_selected=false; // to avoid selecting multiple targets
    std::vector<std::vector<QGraphicsRectItem*>> map_squares;
    QGraphicsRectItem* target_square;
    int start_row{}, start_col{};
    bool stop_btn_clicked{false};

};
#endif // MAINWINDOW_H
