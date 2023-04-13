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

#ifndef MAINWINDOW_HH
#define MAINWINDOW_HH

#include <QMainWindow>
#include <gameboard.hh>
#include <point.hh>
#include <QGraphicsEllipseItem>
#include <QGraphicsRectItem>
#include <QGraphicsScene>
#include <QTimer>
#include <QString>
#include <QVector>
#include <string>



namespace Ui { class MainWindow; }


class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    // For keypress events
    void keyPressEvent(QKeyEvent* event) override;

private slots:

    // Timer timeout
    // adds seconds to a counter and calls makeSnakeMove()
    void timerTimeOut();

    // UI button clicks
    void on_generatePushButton_clicked();
    void on_startPushButton_clicked();
    void on_pausePushButton_clicked();
    void on_resetPushButton_clicked();
    void on_exitPushButton_clicked();

private:

    // Game setup parameters
    const int STEP = 30;
    const int DURATION = 1000;
    const int BORDER_UP = 0;
          int BORDER_DOWN;
    const int BORDER_LEFT = 0;
          int BORDER_RIGHT;
          int SEED_NUMBER;

    // Mainwindow
    Ui::MainWindow* ui_;

    // String for the movecommand
    QString moveCommand = {"d"};

    // Pointer for the "internal" gameboard
    GameBoard* gb_ = nullptr;

    // Timer and time in seconds
    QTimer timer_;
    int seconds_ = 0;

    // Surface for rectangles and circles to be drawn
    QGraphicsScene* scene_;

    // Reading inputs from UI
    void read_input();

    // Generating the internal map
    // calls gameboard
    void generate_map();

    // Generating the visual gameboard in UI
    void generate_ui_map();

    // Makes the snake move
    // by conveying movecommand string to gameBoard
    void makeSnakeMove();

    // Updates the visual gameboard on call
    void drawBoard();

};

#endif // MAINWINDOW_HH
