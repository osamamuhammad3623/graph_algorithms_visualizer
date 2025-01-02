#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>
#include <QGraphicsRectItem>
#include <QMouseEvent>
#include <chrono>
#include <thread>
#include <vector>
#include <A_Star_Node.h>
#include <stack>
#include <QTime>
#include <queue>

#define STEP_DELAY (200)

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

private:
    Ui::MainWindow *ui;
    QGraphicsScene scene;
    int square_size = 50;
    void clear_gui();

    // Visualiaztion-related
    bool target_selected=false; // to avoid selecting multiple targets
    std::vector<std::vector<QGraphicsRectItem*>> map_squares;
    QGraphicsRectItem* target_square;
    std::vector<QGraphicsRectItem*> get_neighbours(QGraphicsRectItem* current_square);
    void reconstruct_path(std::unordered_map<QGraphicsRectItem*, QGraphicsRectItem*> came_from_map, QGraphicsRectItem* target);

    // BFS
    std::queue<QGraphicsRectItem*> bfs_queue;
    void bfs();

    // DFS
    std::stack<QGraphicsRectItem*> dfs_stack;
    void dfs();

    // A*
    int calculate_heuristic(QGraphicsRectItem* current, QGraphicsRectItem* goal);
    void A_star();

};
#endif // MAINWINDOW_H
