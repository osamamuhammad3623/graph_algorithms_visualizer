#include "graph_algorithm.h"

void graph_algorithm::set_stop_flag(bool state)
{
    this->stop_flag = state;
}

void graph_algorithm::set_map_squares(std::vector<std::vector<QGraphicsRectItem *> > map)
{
    this->map_squares = map;
}

void graph_algorithm::set_delay(int delay)
{
    this->step_delay = delay;
}

void graph_algorithm::set_target(QGraphicsRectItem *target)
{
    this->target_square = target;
}


std::vector<QGraphicsRectItem *> graph_algorithm::get_neighbours(QGraphicsRectItem *current_square){
    int row = current_square->rect().y() /SQUARE_SIZE;
    int col = current_square->rect().x() /SQUARE_SIZE;
    std::vector<QGraphicsRectItem *> result{};

    // top right neighbour
    if((row > 0) && (col < map_squares[0].size() -1)){
        result.push_back(map_squares[row-1][col+1]);
    }

    // right neighbour
    if(col < map_squares[0].size() -1){
        result.push_back(map_squares[row][col+1]);
    }

    // bottom right neighbour
    if((col < map_squares[0].size() -1) && (row < map_squares.size() -1)){
        result.push_back(map_squares[row+1][col+1]);
    }

    // bottom neighbour
    if(row < map_squares.size() -1){
        result.push_back(map_squares[row+1][col]);
    }

    // bottom left neighbour
    if((row < map_squares.size() -1) && (col > 0)){
        result.push_back(map_squares[row+1][col-1]);
    }

    // left neighbour
    if(col > 0){
        result.push_back(map_squares[row][col-1]);
    }

    // top left neighbour
    if((col > 0) && (row > 0)){
        result.push_back(map_squares[row-1][col-1]);
    }

    // top neighbour
    if(row > 0){
        result.push_back(map_squares[row-1][col]);
    }

    return result;
}

void graph_algorithm::reconstruct_path(std::unordered_map<QGraphicsRectItem *, QGraphicsRectItem *> came_from_map)
{
    QGraphicsRectItem* current = target_square;
    while (current != map_squares[start_row][start_col]) {
        current->setBrush(Qt::blue);  // Mark the path (blue for the final path)
        current = came_from_map[current];
    }
    map_squares[start_row][start_col]->setBrush(Qt::blue);
}

void graph_algorithm::delay()
{
    QTime dieTime = QTime::currentTime().addMSecs(step_delay);
    while( QTime::currentTime() < dieTime )
    {
        QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
    }
}

void graph_algorithm::set_start_point(int r, int c)
{
    start_row = r;
    start_col = c;
}
