//
// Created by sergey on 27.09.23.
//
#include <SFML/Graphics.hpp>
#include <iostream>

#ifndef UNTITLED_GAME_H
#define UNTITLED_GAME_H

struct Item
{
    float price;
    float cookingTime;
    sf::Sprite sprite;
    sf::Texture texture;
};

struct Client
{

    sf::Sprite sprite;
    sf::Texture texture;
};

class Game {
private:

    // private functions
    void initWindow();
    void initVariables();
    void initObjects();

    // money of shop
    float allMoney;

    // equipment
    sf::Sprite microWave;
    sf::Sprite stove;
    sf::Sprite fryer;
    sf::Sprite juiceMachine;

    //mouse events
    bool draggin=false;

    sf::Sprite* selItem;

    // objects
    sf::Sprite microwave;
    Item juice;
    Item potato;
    Item hamburger;

    std::vector<Item*> vecItems = {&juice, &hamburger, &potato};


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
    void renderFrame();
    void updateOrder();

    void updateMousePosition();

    void renderItems();

    sf::Sprite *checkMouseOnItem();

    // accessors
    const bool running() const;

    // getting coordinates inside object
    sf::Vector2f getLocalClickPosition(const sf::Sprite& sprite, const sf::Vector2i& mousePosition);


};


#endif //UNTITLED_GAME_H
