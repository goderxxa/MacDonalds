//
// Created by sergey on 27.09.23.
//
#include <SFML/Graphics.hpp>
#include <iostream>

#ifndef UNTITLED_GAME_H
#define UNTITLED_GAME_H


class Game {
private:

    // private functions
    void initWindow();
    void initVariables();
    void initObjects();

    // objects
    sf::RectangleShape current;

    //mouse position
    sf::Vector2i mousePosition;

    // text for mouse pos display
    sf::Text mouseText ;
    sf::Font font;


    // variables
    sf::RenderWindow* window;
    sf::Event ev;
    sf::VideoMode videoMode;


public:
    // constructors destructors
    Game ();

    ~Game ();


    // functions

    std::string getPath();
    void processEvents();
    void updateEvents();
    void RenderFrame();

    void updateMousePosition();

    // accessors
    const bool running() const;


};


#endif //UNTITLED_GAME_H
