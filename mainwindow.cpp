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
    // reseting maps, used data structures, etc..
    map_squares.clear();
    std::queue<QGraphicsRectItem *> empty_queue{};
    std::stack<QGraphicsRectItem *> empty_stack{};
    bfs_queue.swap(empty_queue);
    dfs_stack.swap(empty_stack);
    ui->statusbar->showMessage("");
    target_selected=false;
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
    if(ui->algorithm->currentText() == "BFS"){
        bfs();

    }else if (ui->algorithm->currentText() == "DFS"){
        dfs();
    }

    ui->statusbar->showMessage("Completed!");
}

void MainWindow::bfs()
{
    bfs_queue.push(map_squares[0][0]);
    bool finish = false;

    while(!finish){
        // add neighbours
        for(auto nei: get_neighbours(bfs_queue.front())){
            int row = nei->rect().y() /square_size;
            int col = nei->rect().x() /square_size;

            if(map_squares[row][col]->brush() == Qt::black){
                continue;
            }
            else if (map_squares[row][col]->brush() == Qt::green){
                finish|=true;
                break;
            }
            else if(map_squares[row][col]->brush() == Qt::red){
                map_squares[row][col]->setBrush(Qt::lightGray);
                bfs_queue.push(nei);
            }
        }

        bfs_queue.pop();

        finish |= bfs_queue.empty();

        delay(STEP_DELAY);
    }
}

void MainWindow::dfs()
{
    dfs_stack.push(map_squares[0][0]);
    bool finish = false;

    while (!finish) {
        QGraphicsRectItem * current = dfs_stack.top();
        map_squares[current->rect().y()/square_size][current->rect().x()/square_size]->setBrush(Qt::lightGray);
        dfs_stack.pop();

        // Add neighbours to the stack
        for (auto nei: get_neighbours(current)) {
            int row = nei->rect().y() / square_size;
            int col = nei->rect().x() / square_size;

            if (map_squares[row][col]->brush() == Qt::black) {
                continue;
            }
            else if (map_squares[row][col]->brush() == Qt::green) {
                finish = true;
                break;
            }
            else if (map_squares[row][col]->brush() == Qt::red) {
                dfs_stack.push(nei);
            }
        }

        finish |= dfs_stack.empty();

        delay(STEP_DELAY);
    }
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

