#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::mousePressEvent(QMouseEvent *event)
{
    // Get the position where the mouse was clicked relative to the scene
    QPointF scenePos = ui->graphicsView->mapToScene(event->pos());

    // Calculate the row and column of the clicked square
    int col = static_cast<int>(scenePos.x()) / square_size;
    int row = static_cast<int>(scenePos.y()) / square_size;
    //ui->statusbar->showMessage(QString("Row: %1, Column: %2").arg(row).arg(col));

    if(row < map_squares.size() && col < map_squares[0].size()){
        if(event->button() == Qt::LeftButton){
            map_squares[row][col]->setBrush(Qt::black);
        }else if(event->button() == Qt::RightButton && !target_selected){
            map_squares[row][col]->setBrush(Qt::green);
            target_selected=true;
            target_square = map_squares[row][col];
        }
    }
}

void MainWindow::on_start_btn_clicked()
{
    clear_gui();
    int map_height = ui->graphicsView->height();
    int map_width = ui->graphicsView->width();

    scene.setSceneRect(0,0, map_width, map_height);

    for (int row = 0; row < (map_height/square_size); ++row) {
        std::vector<QGraphicsRectItem*> current_row_sqaures;
        for (int col = 0; col < (map_width/square_size); ++col) {
            QGraphicsRectItem *square = new QGraphicsRectItem(col*square_size, row*square_size , square_size, square_size);
            square->setBrush(Qt::red);  // Set color for the squares
            scene.addItem(square);
            current_row_sqaures.push_back(square);
        }
        map_squares.push_back(current_row_sqaures);
    }

    map_squares[0][0]->setBrush(Qt::blue);
    ui->graphicsView->setScene(&scene);
}

void MainWindow::clear_gui()
{
    map_squares.clear(); // reseting the map
    clear_data_structures();
}

void MainWindow::clear_algorithm()
{
    for(int i=0; i< map_squares.size(); i++){
        for(int j=0; j< map_squares[i].size(); j++){
            // only clear lightGray [visited] and blue [reconstructed path] squares
            if(map_squares[i][j]->brush() == Qt::lightGray || map_squares[i][j]->brush() == Qt::blue){
                map_squares[i][j]->setBrush(Qt::red);
            }
        }
    }
    clear_data_structures();
    // set start point to blue
    map_squares[0][0]->setBrush(Qt::blue);
}

void MainWindow::clear_data_structures()
{
    // reseting used data structures, flags, etc..
    std::queue<QGraphicsRectItem *> empty_queue{};
    std::stack<QGraphicsRectItem *> empty_stack{};
    bfs_queue.swap(empty_queue);
    dfs_stack.swap(empty_stack);
    ui->statusbar->showMessage("");
    ui->steps_label->setText("");
    target_selected=false;
    target_square = nullptr;
}

void MainWindow::set_algorithms_steps(int steps)
{
    ui->steps_label->setText("Algorithm steps: " + QString::fromStdString(std::to_string(steps)));
}

void delay(int millisecondsToWait)
{
    QTime dieTime = QTime::currentTime().addMSecs( millisecondsToWait );
    while( QTime::currentTime() < dieTime )
    {
        QCoreApplication::processEvents( QEventLoop::AllEvents, 100 );
    }
}

void MainWindow::on_go_clicked()
{
    int steps=0;
    if(ui->algorithm->currentText() == "BFS"){
        steps=bfs();

    }else if (ui->algorithm->currentText() == "DFS"){
        steps=dfs();

    }else if  (ui->algorithm->currentText() == "A*"){
        steps=A_star();
    }

    ui->statusbar->showMessage("Completed!");
    set_algorithms_steps(steps);
}

int MainWindow::bfs()
{
    bfs_queue.push(map_squares[0][0]);
    bool finish = false;
    int steps=0;

    // Store the predecessor of each node for path reconstruction
    std::unordered_map<QGraphicsRectItem*, QGraphicsRectItem*> came_from;

    while (!finish) {
        // add neighbours
        for (auto nei : get_neighbours(bfs_queue.front())) {
            int row = nei->rect().y() / square_size;
            int col = nei->rect().x() / square_size;

            if (map_squares[row][col]->brush() == Qt::black) {
                continue;
            } else if (map_squares[row][col]->brush() == Qt::green) {
                finish |= true;
                came_from[nei] = bfs_queue.front();  // Record the predecessor
                break;
            } else if (map_squares[row][col]->brush() == Qt::red) {
                map_squares[row][col]->setBrush(Qt::lightGray);
                bfs_queue.push(nei);
                came_from[nei] = bfs_queue.front();  // Record the predecessor
            }
        }

        bfs_queue.pop();
        finish |= bfs_queue.empty();
        delay(STEP_DELAY);
        steps++;
    }

    reconstruct_path(came_from);
    return steps;
}


int MainWindow::dfs()
{
    dfs_stack.push(map_squares[0][0]);
    bool finish = false;
    int steps=0;

    // Store the predecessor of each node for path reconstruction
    std::unordered_map<QGraphicsRectItem*, QGraphicsRectItem*> came_from;

    while (!finish) {
        QGraphicsRectItem* current = dfs_stack.top();
        map_squares[current->rect().y() / square_size][current->rect().x() / square_size]->setBrush(Qt::lightGray);
        dfs_stack.pop();

        // Add neighbours to the stack
        for (auto nei : get_neighbours(current)) {
            int row = nei->rect().y() / square_size;
            int col = nei->rect().x() / square_size;

            if (map_squares[row][col]->brush() == Qt::black) {
                continue;
            } else if (map_squares[row][col]->brush() == Qt::green) {
                finish = true;
                came_from[nei] = current;  // Record the predecessor
                break;
            } else if (map_squares[row][col]->brush() == Qt::red) {
                dfs_stack.push(nei);
                came_from[nei] = current;  // Record the predecessor
            }
        }

        finish |= dfs_stack.empty();
        delay(STEP_DELAY);
        steps++;
    }

    reconstruct_path(came_from);
    return steps;
}

int MainWindow::A_star()
{
    // Initialize the start and goal positions
    QGraphicsRectItem* start = map_squares[0][0];  // Example start position
    int steps=0;


    // The std::priority_queue is used to process nodes in the order of their f-cost,
    //      which ensures that we explore the least costly paths first.
    std::priority_queue<Node, std::vector<Node>, std::greater<Node>> open_list;
    std::unordered_map<QGraphicsRectItem*, QGraphicsRectItem*> came_from;  // For path reconstruction
    std::unordered_map<QGraphicsRectItem*, int> g_cost_map;  // Store g_cost for each node

    // Initialize the start node
    open_list.push(Node{start, 0, calculate_heuristic(start, target_square)});
    g_cost_map[start] = 0;

    bool finish = false;

    while (!open_list.empty() && !finish) {
        // Get the node with the lowest f_cost
        Node current_node = open_list.top();
        open_list.pop();

        QGraphicsRectItem* current = current_node.item;
        int row = current->rect().y() / square_size;
        int col = current->rect().x() / square_size;

        // Mark the current node as visited
        map_squares[row][col]->setBrush(Qt::lightGray);

        if (current == target_square) {
            finish = true;
            break;
        }

        // Process the neighbors of the current node
        for (auto nei: get_neighbours(current)) {
            row = nei->rect().y() / square_size;
            col = nei->rect().x() / square_size;

            if (map_squares[row][col]->brush() == Qt::black) {  // Skip blocked nodes
                continue;
            }

            int tentative_g_cost = g_cost_map[current] + 1;  // Assuming uniform cost for simplicity
            int h_cost = calculate_heuristic(nei, target_square);

            // If this path is better, or the neighbor is not in the open list yet
            if (g_cost_map.find(nei) == g_cost_map.end() || tentative_g_cost < g_cost_map[nei]) {
                g_cost_map[nei] = tentative_g_cost;
                open_list.push(Node{nei, tentative_g_cost, h_cost});
                came_from[nei] = current;  // Store the path for reconstruction
            }
        }

        delay(STEP_DELAY);
        steps++;
    }

    reconstruct_path(came_from);
    return steps;
}


std::vector<QGraphicsRectItem *> MainWindow::get_neighbours(QGraphicsRectItem *current_square)
{
    int row = current_square->rect().y() /square_size;
    int col = current_square->rect().x() /square_size;
    std::vector<QGraphicsRectItem *> result{};

    // right neighbour
    if(col < map_squares[0].size() -1){
        result.push_back(map_squares[row][col+1]);
    }

    // bottom neighbour
    if(row < map_squares.size() -1){
        result.push_back(map_squares[row+1][col]);
    }

    // left neighbour
    if(col > 0){
        result.push_back(map_squares[row][col-1]);
    }

    // top neighbour
    if(row > 0){
        result.push_back(map_squares[row-1][col]);
    }

    return result;
}

void MainWindow::reconstruct_path(std::unordered_map<QGraphicsRectItem *, QGraphicsRectItem *> came_from_map)
{
    QGraphicsRectItem* current = target_square;
    while (current != map_squares[0][0]) {
        current->setBrush(Qt::blue);  // Mark the path (blue for the final path)
        current = came_from_map[current];
    }
    map_squares[0][0]->setBrush(Qt::blue);  // Mark the start node
}

int MainWindow::calculate_heuristic(QGraphicsRectItem *current, QGraphicsRectItem *goal)
{
    int current_row = current->rect().y() / square_size;
    int current_col = current->rect().x() / square_size;
    int goal_row = goal->rect().y() / square_size;
    int goal_col = goal->rect().x() / square_size;

    // Manhattan distance heuristic
    return std::abs(current_row - goal_row) + std::abs(current_col - goal_col);
}


void MainWindow::on_reset_algorithm_clicked()
{
    clear_algorithm();
}

