/* snake_gui
 *
 * Kuvaus:
 * Ohjelma on perinteinen matopeli.
 * Matoa liikutetaan WASD-näppäimillä.
 * Pelin tarkoitus on ohjata mato ruuan luokse.
 * Ruokaa syödessään mato kasvaa pituutta.
 * Mato liikkuu haluttuun suuntaan sekunnin välein
 * Seinään tai itseensä osuessaan mato kuolee ja peli loppuu.
 * Pelin voittaa kun saa täytettyä koko peliruudukon madolla,
 * eli syö niin paljon ruokaa, että ruudukossa ei ole enää
 * yhtään tyhjää uudelle ruualle.
 *
 * Pelissä syötetään satunnaislukugeneraatorille seed -luku
 * ja haluttu pelialueen koko leveydessä ja korkeudessa.
 *
 * Halutessaan pelaaja voi keskeyttää pelin ( pause ),
 * ja resetoida sen alkuun ( reset ).
 * Pelaaja voi myös generoida täysin uuden pelialueen.
 *
 *
 * Ohjelman kirjoittaja
 * Nimi: Timi Jouppila
 * Opiskelijanumero: 151333451
 * Käyttäjätunnus: cstijo
 * E-Mail: timi.jouppila@tuni.fi
 *
 * Huomioita ohjelmasta ja sen toteutuksesta:
 *
 * Vaikka UI:ssa näennäinen scoreboard onkin,
 * siinä ei ole minkäänlaista toiminnallisuutta.
 * Sen toteutus jäi tekemättä.
 *
 * */

#include "mainwindow.hh"
#include "ui_mainwindow.h"
#include <gameboard.hh>
#include <point.hh>
#include <QDebug>
#include <QTimer>
#include <QKeyEvent>
#include <QGraphicsEllipseItem>
#include <QGraphicsRectItem>
#include <string>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui_(new Ui::MainWindow)

{
    // Setting up the UI
    ui_->setupUi(this);
    this->setStyleSheet("background-color: lightGray;");

    // At startup these buttons are disabled:
    ui_->startPushButton->setEnabled(false);
    ui_->pausePushButton->setEnabled(false);
    ui_->resetPushButton->setEnabled(false);

    // Button connections
    connect(ui_->generatePushButton, &QPushButton::clicked, this, &MainWindow::on_generatePushButton_clicked);
    connect(ui_->startPushButton, &QPushButton::clicked, this, &MainWindow::on_startPushButton_clicked);
    connect(ui_->pausePushButton, &QPushButton::clicked, this, &MainWindow::on_pausePushButton_clicked);
    connect(ui_->resetPushButton, &QPushButton::clicked, this, &MainWindow::on_resetPushButton_clicked);
    connect(ui_->exitPushButton, &QPushButton::clicked, this, &MainWindow::on_exitPushButton_clicked);

    // Timer
    timer_.setSingleShot(false);
    connect(&timer_, &QTimer::timeout, this, &MainWindow::timerTimeOut);

}

MainWindow::~MainWindow()
{
    delete ui_;
}


void MainWindow::read_input()
{
    // Reading inputs from UI
    BORDER_RIGHT = ui_->heigthSelectBox->value();
    BORDER_DOWN = ui_->widthSelectBox->value();
    QString seedinput = ui_->seedInputBox->text();
    // Converting read text to int for random seed generator
    SEED_NUMBER = seedinput.toInt();
}

void MainWindow::generate_map()
{
    // Call for "internal" map creation in gameBaord
    gb_ = new GameBoard(BORDER_RIGHT, BORDER_DOWN, SEED_NUMBER);
}

void MainWindow::generate_ui_map()
{
    // Setting up scene
    scene_ = new QGraphicsScene(this);

    // The graphicsview object is placed on the window
    // at the following coordinates:
    int left_margin = 208;  // x
    int top_margin = 68;    // y

    // Center vertically but up horizontally alling "up".
    ui_->gameView->setGeometry(left_margin+((300-BORDER_RIGHT*STEP)/2), top_margin,
                               BORDER_RIGHT*STEP + 2, BORDER_DOWN*STEP + 2);

    ui_->gameView->setScene(scene_);
    scene_->setSceneRect(0, 0, BORDER_RIGHT*STEP - 1, BORDER_DOWN*STEP - 1);

    // Update UI gameboard view
    drawBoard();
}

// Keypress events
void MainWindow::keyPressEvent(QKeyEvent* event) {
    // Checking if desired move is impossible
    // ie. going the opposite way.
    // Changing the moveCommand accordingly
    // Else: do nothing

    // Command for going up is "w"
    if(event->key() == Qt::Key_W) {
        if (moveCommand != "s") {
            moveCommand = "w";
        } else {
            return;
        }
    }

    // Command for going down is "s"
    if(event->key() == Qt::Key_S) {
        if (moveCommand != "w") {
            moveCommand = "s";
        } else {
            return;
        }
    }

    // Command for going left is "a"
    if(event->key() == Qt::Key_A) {
        if (moveCommand != "d") {
            moveCommand = "a";
        } else {
            return;
        }
    }

    // Command for going right is "d"
    if(event->key() == Qt::Key_D) {
        if (moveCommand != "a") {
            moveCommand = "d";
        } else {
            return;
        }
    }
}




void MainWindow::timerTimeOut()
{
    // Add one second to the counter and move the snake
    seconds_++;
    makeSnakeMove();
}

void MainWindow::makeSnakeMove()
{
    // Convert moveCommand QString to string
    const std::string cmd_ = moveCommand.toStdString();

    // Check if game is over
    if (gb_->gameOver() == true) {

        // Stop the timer
        timer_.stop();

        // Set button availibility to gameover -status
        ui_->startPushButton->setEnabled(false);
        ui_->pausePushButton->setEnabled(false);
        ui_->resetPushButton->setEnabled(true);

        // Set gamestatuslabel and backround -color accordingly
        // LOST - red
        // WON - green
        if (gb_->gameLost() == true) {
            this->setStyleSheet("background-color: red;");
            ui_->gameStatusLabel->setText("GAME LOST!");
        }
        if (gb_->gameWon() == true) {
            this->setStyleSheet("background-color: green;");
             ui_->gameStatusLabel->setText("GAME WON!");
        }

    // Else just move the snake in "internal" gameboard
    // and update visual gameboard in UI
    } else {
        gb_->moveSnake(cmd_);
        drawBoard();
    }
}


void MainWindow::drawBoard()
{
    // Defining the color and outline of the parts of the snake + food
    QBrush headBrush(Qt::darkCyan);     // Snake head
    QBrush bodyBrush(Qt::darkGreen);    // Snake body
    QBrush tailBrush(Qt::green);        // Snake tail
    QBrush foodBrush(Qt::yellow);       // Food
    QPen blackPen(Qt::black);           // Shared outline
    blackPen.setWidth(2);               // Pen width

    // Clearing the "old" scene before drawint the new one
    scene_->clear();

    // Going trough the rows and columns of "internal" gameboard
    // and draw or dont an item to the visual gameboard
    // Using findSnakeAndFood in gameBoard
    for (int row = 0; row < BORDER_DOWN; ++row)
    {
        for (int col = 0; col < BORDER_RIGHT; ++col)
        {
            const Point position(col, row);
            if (gb_->findSnakeAndFood(position) == "food" ) {
                scene_->addEllipse(col*STEP, row*STEP, STEP, STEP, blackPen, foodBrush);
            }
            if (gb_->findSnakeAndFood(position) == "head" ) {
                scene_->addEllipse(col*STEP, row*STEP, STEP, STEP, blackPen, headBrush);
            }
            if (gb_->findSnakeAndFood(position) == "tail" ) {
                scene_->addEllipse(col*STEP+5, row*STEP+5, 20, 20, blackPen, tailBrush);
            }
            if (gb_->findSnakeAndFood(position) == "body" ) {
                scene_->addEllipse(col*STEP+3, row*STEP+3, 24, 24, blackPen, bodyBrush);
            }
            if (gb_->findSnakeAndFood(position) == "" ) {
                continue;
            }

        }
    }

    // Use the snakes lenght for score label
    ui_->scoreNumberLabel->setText(QString::number(gb_->getSnakeLength()-1));

    // Timer minutes and seconds
    QString minutesStr = QString::number(seconds_/60);
    int remainingSecs = seconds_%60;
    QString remainingSecsStr = QString::number(remainingSecs);

    // Set up time labels accordingly
    ui_->timeMinutesLabel->setText(minutesStr + " :");
    ui_->timeSecondsLabel->setText(remainingSecsStr);
}


void MainWindow::on_generatePushButton_clicked()
{
    // Deleting the "old" gameboard
    delete gb_;
    gb_ = nullptr;

    // Get input values from the UI
    read_input();

    // Gameboard constructor call to generate new "internal" gameboard
    generate_map();

    // Update the visual gameboard
    generate_ui_map();

    // Enabling the startbutton
    ui_->startPushButton->setEnabled(true);

}

void MainWindow::on_startPushButton_clicked()
{
    // Resetting the startbutton text
    ui_->startPushButton->setText("START");
    // Clearing the status label
    ui_->gameStatusLabel->setText("");

    // Starting the timer
    timer_.start(DURATION);

    // Setting button availability according to game status
    ui_->generatePushButton->setEnabled(false);
    ui_->pausePushButton->setEnabled(true);
    ui_->resetPushButton->setEnabled(false);

}

void MainWindow::on_pausePushButton_clicked()
{
    // Timer stop
    timer_.stop();

    // Changing game status label and start-button text
    ui_->gameStatusLabel->setText("Game paused.");
    ui_->startPushButton->setText("CONTINUE");

    // Enabling the reset button
    ui_->resetPushButton->setEnabled(true);

}

void MainWindow::on_resetPushButton_clicked()
{
    // Deleting the "old" gameboard
    delete gb_;
    gb_ = nullptr;

    // Resetting the game
    this->setStyleSheet("background-color: lightGray;");
    ui_->gameStatusLabel->setText("");
    ui_->startPushButton->setText("START");
    moveCommand = "d";

    ui_->pausePushButton->setEnabled(false);
    ui_->generatePushButton->setEnabled(true);
    ui_->startPushButton->setEnabled(true);

    // Stoppping  and resetting the timer
    timer_.stop();
    seconds_ = 0;

    // Generating new gameboard without reading the UI inputs,
    // Ie: resetting the same map
    generate_map();
    generate_ui_map();

}

void MainWindow::on_exitPushButton_clicked()
{

    // Deleting the gameboard
    delete gb_;
    gb_ = nullptr;

    // Deleting the UI gameboard
    delete scene_;
    scene_ = nullptr;

    // Closing the app
    close();
}





