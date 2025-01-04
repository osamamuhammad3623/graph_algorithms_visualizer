#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    // call view the map after setup
    // [error root cause when just on_start_btn_clicked(): attempt to use ui->graphicsView in the constructor before it has been fully initialized and set up]
    QTimer::singleShot(0, this, SLOT(on_start_btn_clicked()));
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
    int col = static_cast<int>(scenePos.x()) / SQUARE_SIZE;
    int row = static_cast<int>(scenePos.y()) / SQUARE_SIZE;
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

    for (int row = 0; row < (map_height/SQUARE_SIZE); ++row) {
        std::vector<QGraphicsRectItem*> current_row_sqaures;
        for (int col = 0; col < (map_width/SQUARE_SIZE); ++col) {
            QGraphicsRectItem *square = new QGraphicsRectItem(col*SQUARE_SIZE, row*SQUARE_SIZE , SQUARE_SIZE, SQUARE_SIZE);
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
    clear_flags();
}

void MainWindow::clear_visualized_algorithm()
{
    for(int i=0; i< map_squares.size(); i++){
        for(int j=0; j< map_squares[i].size(); j++){
            // only clear lightGray [visited] and blue [reconstructed path] squares
            if(map_squares[i][j]->brush() == Qt::lightGray || map_squares[i][j]->brush() == Qt::blue){
                map_squares[i][j]->setBrush(Qt::red);
            }
        }
    }
    clear_flags();
    // set start point to blue
    map_squares[0][0]->setBrush(Qt::blue);
}

void MainWindow::clear_flags()
{
    ui->statusbar->showMessage("");
    ui->steps_label->setText("");
    target_selected=false;
    current_graph_algorithm->set_target(nullptr);
    current_graph_algorithm->set_stop_flag(false);
}

void MainWindow::set_algorithms_steps(int steps)
{
    ui->steps_label->setText("Algorithm steps: " + QString::fromStdString(std::to_string(steps)));
}

void MainWindow::set_graph_algorithm(graph_algorithm *graph)
{
    current_graph_algorithm = graph;
}

void MainWindow::on_go_clicked()
{
    int steps=0;

    if(ui->algorithm->currentText() == "BFS"){
        set_graph_algorithm(new bfs_graph_algorithm());
    }
    if(ui->algorithm->currentText() == "DFS"){
        set_graph_algorithm(new dfs_graph_algorithm());
    }
    if(ui->algorithm->currentText() == "A*"){
        set_graph_algorithm(new a_star_graph_algorithm(true));
    }
    if(ui->algorithm->currentText() == "Dijkstra"){
        set_graph_algorithm(new a_star_graph_algorithm(false));
    }

    current_graph_algorithm->set_map_squares(map_squares);
    current_graph_algorithm->set_target(target_square);

    steps =current_graph_algorithm->execute();
    ui->statusbar->showMessage("Completed!");
    set_algorithms_steps(steps);
}


void MainWindow::on_reset_algorithm_clicked()
{
    clear_visualized_algorithm();
}


void MainWindow::on_speed_valueChanged(int value)
{
    current_graph_algorithm->set_delay(value);
}


void MainWindow::on_stop_visualization_clicked()
{
    current_graph_algorithm->set_stop_flag(true);
}

