/*
####################################################################
# COMP.CS.110 Programming 2: Structures, 2022                      #
#                                                                  #
# Project4: Snake                                                  #
# Program description: Implements a game called Snake.             #
#                                                                  #
# File: gameboard.hh                                               #
# Description: Declares a class representing the gameboard.        #
#                                                                  #
####################################################################
*
* Lisätyt toiminnallisuudet:
* FindSnakeAndFood
* Tarkistaa onko sille syötetty point ruoka tai madon osa
* ja palauttaa tiedon (hieman kömpelösti) stringinä
*
* getSnakeLenght
* Palauttaa madon (deque<Point> Snake) pituuden
*
* Ohjelman kirjoittaja
* Nimi: Timi Jouppila
* Opiskelijanumero: 151333451
* Käyttäjätunnus: cstijo
* E-Mail: timi.jouppila@tuni.fi
*
*/

#ifndef GAMEBOARD_HH
#define GAMEBOARD_HH

#include "point.hh"
#include <random>
#include <string>
#include <deque> // Better than vector, since we need erase front elements

// Constants for printing the gameboard
const std::string BODY = "~";
const std::string DEAD = "X";
const std::string FOOD = "*";
const std::string HEAD = "@";
const std::string TAIL = "-";
const std::string WALL = "#";
const std::string EMPTY = " ";

const int DEFAULT_WIDTH = 10;
const int DEFAULT_HEIGTH = 10;
const int DEFAULT_SEED = 1;

class GameBoard {
public:
    // Default constructor.
    // Calls the other constructor with default width, height, and seed.
    GameBoard();

    // Constructs a gameboard based the given width and height,
    // rng is used to generate random locations for the food.
    GameBoard(int width, int height, int seed_value);

    // Destructor
    ~GameBoard();

    // Checks if the game is over.
    // Returns false until the game has been either lost or won.
    bool gameOver() const;

    // Checks if the game has been lost.
    // Returns true if the snake is dead.
    bool gameLost() const;

    // Checks if the game has been won.
    // Returns true if the snake has filled the field.
    bool gameWon() const;

    // Moves the snake to the given direction, if possible.
    // Moving is not possible, if game is over, or if the snake would move
    // against a wall.
    // If moving is possible, calls the private method moveSnakeAndFood.
    // Returns true, if moving was possible.
    bool moveSnake(const std::string& direction);

    // Prints the gameboard and the snake in it.
    void print() const;

    // ##
    // ADDED FUNCTIONALITY HERE

    // Basically does the same as print() above, but meant for mainwindows usage.
    // Returns string according to points value: food, head, body, tail.
    std::string findSnakeAndFood(const Point& point);

    // Get the lenght of the snake for score count in the UI
    int getSnakeLength();

    // ADDED FUNCTIONALITY ENDS HERE
    // ##

private:
    // Returns true if the given point is a part of the snake.
    bool isSnakePoint(const Point& point) const;

    // Returns the point of the snake's head.
    const Point& getHead() const;

    // Returns the point of the snake's tail.
    const Point& getTail() const;

    // Relocates food to a random, snakeless location.
    // Hides the food if the snake has completely filled the gameboard.
    void moveFood();

    // Moves the snake unless the new head would be the body of the snake.
    // If the new head is the neck of the snake, nothing happens.
    // If the new head is other body part of the snake, the snake dies.
    // Otherwise the snake moves, whereupon the method returns true.
    // If, in addition, the food got eaten a new one is placed somewhere,
    // and the snake grows a bit.
    bool moveSnakeAndFood(const Point& new_head);

    // Prints the top or bottom wall of the gameboard.
    void printHorizontalWall() const;

    // Tells if the snake is alive and able to move.
    bool dead_ = false;

    // Specifies the width and height of the gameboard.
    const int width_ = 0;
    const int height_ = 0;

    // Generates random numbers used to move the food item to random locations.
    std::default_random_engine rng_;

    // Points currently occupied by the snake, head being the last one.
    std::deque<Point> snake_;

    // The food item's position in the gameboard.
    Point food_;
};  // class GameBoard


#endif  // GAMEBOARD_HH
