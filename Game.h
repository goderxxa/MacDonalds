//
// Created by sergey on 27.09.23.
//
#include <SFML/Graphics.hpp>
#include <iostream>
#include <ctime>
#include <map>

#ifndef UNTITLED_GAME_H
#define UNTITLED_GAME_H

#define RICH 10     // generate 5-10
#define MID 5       // 3-6
#define POOR 2      // 2
#define POOREST 1    // 1

struct Image    {
    sf::Sprite sprite;
    sf::Texture texture;
};

struct Button   {
    bool isActive;
    sf::Sprite sprite;
    sf::Texture texture;
};

struct Equipment    {
    float power;
    sf::Sprite sprite;
    sf::Texture texture;
    sf::Text text;
};

struct Item     {
    float price;
    float cookingTime;
    sf::Sprite sprite;
    sf::Texture texture;
};

struct Client   {
    sf::Sprite sprite;
    sf::Texture texture;
    int category;
    float patience;
    std::vector<std::string> requests;
};

class Game {
private:

    //load background images
    Image cafeBackground;
    Image backGround;

    // private functions
    void initWindow();
    void initVariables();
    void initObjects();

    //load image files
    std::vector<sf::Texture> poorPersons;
    std::vector<sf::Texture> richPersons;
    std::vector<sf::Texture> midPersons;
    std::vector<sf::Texture> poorestPersons;

    // money of shop
    float allMoney = 0;

    // equipment
    Equipment microWave;
    Equipment stove;
    Equipment fryer;
    Equipment juiceMachine;
    Equipment bin;
    Equipment cash;

    //mouse events
    bool isMouseInputAllowed = true;

    sf::Clock clickClock;
    sf::Time clickDelay = sf::milliseconds(50);

    bool draggin=false;
    bool itemTaken = false;

    // equipment variables
    Equipment* selfEquipment;
    Item* selItem;    // if clicked reference
    sf::Text microWavePower;

    //other events buttons
    Button repeatButtonSprite;
    Button nextButtonSprite;

    Button juiceCoffee;
    Button pour;

    // objects graphical
    sf::Sprite microwave;
    Item hamburger;
    Item cheseburger;
    Item bigmak;
    Item villagePotato;
    Item potato;
    Item souse;
    Item juice;
    Item coffee;

    std::vector<Item*> vecItems = {&hamburger, &cheseburger, &bigmak, &villagePotato,
                                   &potato, &souse, &juice, &coffee };

    std::vector<Equipment*> vecEquipment = {&microWave, &juiceMachine};

    // vector of Clients, checks, cash
    std::vector<Client> clients;

    //mouse position
    sf::Vector2i mousePosition;
    sf::Vector2f mouseClickPos;

    //active objects position
    sf::Vector2f originalObjPos;
    sf::Vector2f origanalOriginPos;

    // text for mouse pos display
    sf::Text mouseText ;
    sf::Font font;

    // variables
    sf::RenderWindow* window;
    sf::Event ev;
    sf::VideoMode videoMode;

    //for return animation
    sf::Clock returnAnimationClock;
    sf::Time returnAnimationDuration = sf::seconds(2.0);
    bool isReturnAnimation = false;

public:
    // constructors destructors
    Game ();
    ~Game ();

    // functions

    std::string getPath();
    void processEvents();

    void updateEvents();
    void updateOrder();
    void updateMousePosition();
    void returnAnimation();

    // equipment processes
    void isEquipment();
    Equipment* checkMouseOnEquipment();
    void powerEquipment();



    void interpolatePosition();

    // take information about item
    Equipment* processSelfitem();

    void renderFrame();
    void renderItems();
    void renderEquipment();
    void renderClient();
    void renderOrder();

    Item *checkMouseOnItem();

    // accessors
    const bool running() const;

    // getting coordinates inside object
    sf::Vector2f getLocalClickPosition(const sf::Sprite& sprite, const sf::Vector2i& mousePosition);

    //generating new client
    void genNewClient();

    //client tell his order
    void tellOrder(Client& client );

    void renderOrder(Client &client);


};


#endif //UNTITLED_GAME_H
