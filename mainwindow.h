#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>
#include <QGraphicsRectItem>
#include <QMouseEvent>
#include <chrono>
#include <thread>
#include <vector>
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
    void clear_gui();
    void on_go_clicked();
    void bfs();
    void dfs();

private:
    Ui::MainWindow *ui;
    QGraphicsScene scene;
    int square_size = 50;
    std::queue<QGraphicsRectItem*> bfs_queue;
    std::stack<QGraphicsRectItem*> dfs_stack;
    bool target_selected=false; // to avoid selecting multiple targets
    std::vector<std::vector<QGraphicsRectItem*>> map_squares;
    std::vector<QGraphicsRectItem*> get_neighbours(QGraphicsRectItem* current_square);
};
#endif // MAINWINDOW_H
