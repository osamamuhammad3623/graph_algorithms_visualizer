#ifndef GRAPH_ALGORITHM_H
#define GRAPH_ALGORITHM_H
#include <vector>
#include <QGraphicsRectItem>
#include <QGraphicsScene>
#include <QTime>
#include <QCoreApplication>

#define SQUARE_SIZE (50)

class graph_algorithm{
public:
    virtual int execute() =0;

    void set_stop_flag(bool state);
    void set_map_squares(std::vector<std::vector<QGraphicsRectItem*>> map);
    void set_delay(int delay);
    void set_target(QGraphicsRectItem* target);
    std::vector<QGraphicsRectItem *> get_neighbours(QGraphicsRectItem *current_square);
    void reconstruct_path(std::unordered_map<QGraphicsRectItem *, QGraphicsRectItem *> came_from_map);
    void delay();

protected:
    std::vector<std::vector<QGraphicsRectItem*>> map_squares;
    QGraphicsRectItem* target_square;
    int step_delay{200};
    bool stop_flag{false};
};

#endif // GRAPH_ALGORITHM_H
