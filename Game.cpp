//
// Created by sergey on 27.09.23.
//

#include <iostream>
#include "Game.h"

//videomode
#define X 1280
#define Y 720
#define GameName "myGame"

//get absolute path function
#ifdef _WIN32
#include <Windows.h>
#elif defined(__linux__)
    #include <climits>
    #include <unistd.h>
#elif defined(__APPLE__)
#include <mach-o/dyld.h>
#endif

std::string Game::getPath() {
    std::string executablePath;

#ifdef _WIN32
    char path[MAX_PATH];
    GetModuleFileName(nullptr, path, sizeof(path));
    executablePath = path;
#elif defined(__linux__)
    char path[PATH_MAX];
    ssize_t count = readlink("/proc/self/exe", path, sizeof(path) - 1);
    if (count != -1) {
        path[count] = '\0';
        executablePath = path;
    }
#elif defined(__APPLE__)
    char path[PATH_MAX];
    uint32_t size = sizeof(path);
    if (_NSGetExecutablePath(path, &size) == 0) {
        executablePath = path;
    }
#endif
    size_t lastSlashPos = executablePath.find_last_of("/\\");
    if (lastSlashPos != std::string::npos) {
        executablePath = executablePath.substr(0, lastSlashPos+1);
    }
    return executablePath;
}

//class functions
void Game::initVariables() {
    this->window = nullptr;
    this->videoMode.width = X;
    this->videoMode.height = Y;

    //text setting
    this->font.loadFromFile(getPath() + "arial.ttf");
    this->mouseText.setFillColor(sf::Color::Red);
    this->mouseText.setFont(this->font);
    //load files
}

void Game::initWindow() {
    this->window = new sf::RenderWindow(this->videoMode, GameName );
    this->window->setFramerateLimit(60);
}

Game::Game() {
    initVariables();
    initWindow();
    initObjects();
}
Game::~Game() {
    delete this->window;
}

void Game::processEvents() {
    while (this->window->pollEvent(this->ev))
    {
        switch (this->ev.type)
        {
            case sf::Event::Closed:
                window->close();
                break;
            case sf::Event::KeyPressed:
                if(ev.key.code == sf::Keyboard::Escape)
                    window->close();
                break;
        }
    }
}

void Game::updateEvents() {
    updateMousePosition();
    updateOrder();
    processEvents();
}

void Game::updateOrder()
{

    if(!isReturnAnimation)
    {
        if(ev.type == sf::Event::MouseButtonPressed && ev.mouseButton.button == sf::Mouse::Left && !draggin && isMouseInputAllowed)
        {
            isMouseInputAllowed = false; // Блокируем дополнительные клики
            clickClock.restart();

            powerEquipment();
            selItem = checkMouseOnItem();

            if(selItem!= nullptr){
                originalObjPos = selItem->sprite.getPosition();
                std::cout << originalObjPos.x << " " << originalObjPos.y << std::endl;
                selItem->sprite.setScale(0.4,0.4);          //change size of dragging item
                draggin = true;
            }
        }
        if (!isMouseInputAllowed) {
            // Если мышь была нажата, проверяем задержку
            sf::Time elapsedTime = clickClock.getElapsedTime();
            if (elapsedTime >= clickDelay) {
                isMouseInputAllowed = true; // Разрешаем обработку следующего клика
            }
        }

        if(draggin )
        {
            selItem->sprite.setPosition(sf::Vector2f (this->mousePosition));
        }
        if(ev.type == sf::Event::MouseButtonReleased && ev.mouseButton.button == sf::Mouse::Right)
        {
            if(selItem!= nullptr )
            {
                selItem->sprite.setScale(0.3,0.3);
                mouseClickPos = selItem->sprite.getPosition();
                draggin = false;
                returnAnimationClock.restart();
            }
        }




    }
    if(selItem!= nullptr )
    {
        if(!draggin)
        {
            isEquipment();
        }
    }
}

Equipment* Game::processSelfitem()
{
    for(auto equipment : vecEquipment)
    {
        if(equipment->sprite.getGlobalBounds().contains(mouseClickPos))
            return equipment;
    }
    return nullptr;
}

void Game::isEquipment()
{
    if(processSelfitem()!=nullptr)
    {
        selItem->sprite.setPosition(processSelfitem()->sprite.getPosition() );
    }
    else
    {
        isReturnAnimation = true;
        returnAnimation();
    }
}

void Game::powerEquipment() {
    if(checkMouseOnEquipment()!= nullptr)
    {
        selfEquipment = checkMouseOnEquipment();
        if(selfEquipment->text.getString() == "Off")
            selfEquipment->text.setString("On");
        else
            selfEquipment->text.setString("Off");
    }
}

void Game::returnAnimation()
{
    float progress = returnAnimationClock.getElapsedTime().asSeconds() / returnAnimationDuration.asSeconds();
    if(progress > 0.3)
    {
        progress = 1.0;
        isReturnAnimation = false;
    }
    sf::Vector2f interpolatedPosition = selItem->sprite.getPosition() + (originalObjPos - selItem->sprite.getPosition()) * progress;
    selItem->sprite.setPosition(interpolatedPosition);
//    std::cout << progress <<std::endl;
//    std::cout << mouseClickPos.x << " " <<mouseClickPos.y <<std::endl;
//    std::cout << isReturnAnimation <<std::endl;

}

void Game::renderFrame() {
    window->clear();
    window->draw(mouseText);

    renderEquipment();
    renderItems();

    window->display();
}

const bool Game::running() const {

    return this->window->isOpen();
}

void Game::initObjects() {

    //load images
    backGround.texture.loadFromFile(getPath() + "/png/gamebackground.png");
    backGround.sprite.setTexture(backGround.texture);
    backGround.sprite.setScale(0.668,0.668);

    cafeBackground.texture.loadFromFile(getPath() + "/png/gameback.png");
    cafeBackground.sprite.setTexture(cafeBackground.texture);
    cafeBackground.sprite.setScale(0.668,0.668);

    //setting objects

    hamburger.texture.loadFromFile(getPath() + "/png/hamburger.png");
    hamburger.price = 1;
    hamburger.cookingTime = 30;

    cheseburger.texture.loadFromFile(getPath() + "/png/cheseburger.png");
    cheseburger.price = 1.2;
    cheseburger.cookingTime = 40;

    bigmak.texture.loadFromFile(getPath() + "/png/bigmak.png");
    bigmak.price = 3;
    bigmak.cookingTime = 60;

    villagePotato.texture.loadFromFile(getPath() + "/png/villagepotato.png");
    villagePotato.price = 2;
    villagePotato.cookingTime = 60;

    potato.texture.loadFromFile(getPath() + "/png/freePotato.png");
    potato.price = 1.3;
    potato.cookingTime = 60;

    souse.texture.loadFromFile(getPath() + "/png/chesesouse.png");
    souse.price = 0.5;
    souse.cookingTime = 2;

    juice.texture.loadFromFile(getPath() + "/png/orangeJuice.png");
    juice.price = 1.5;
    juice.cookingTime = 10;

    coffee.texture.loadFromFile(getPath() + "/png/coffee.png");
    coffee.price = 1.4;
    coffee.cookingTime = 20;

    hamburger.sprite.setTexture(hamburger.texture);
    cheseburger.sprite.setTexture(cheseburger.texture);
    bigmak.sprite.setTexture(bigmak.texture);
    villagePotato.sprite.setTexture(villagePotato.texture);
    potato.sprite.setTexture(potato.texture);
    souse.sprite.setTexture(souse.texture);
    juice.sprite.setTexture(juice.texture);
    coffee.sprite.setTexture(coffee.texture);

    //setting by for loop
    sf::Vector2f itemPos(55, 60);
    float offset=7;
    for(auto i : vecItems)
    {
        i->sprite.setOrigin(i->sprite.getLocalBounds().width/2, i->sprite.getLocalBounds().height/2);
        i->sprite.setPosition(itemPos.x+offset, itemPos.y);
        offset += 94;
        i->sprite.setScale(0.3f, 0.3f);
        i->texture.setSmooth(true);
    }

    //setting equipment on screen
    microWave.texture.loadFromFile(getPath() + "/png/microwave.png");
    microWave.sprite.setTexture(microWave.texture);
    microWave.sprite.setPosition(X-microWave.sprite.getLocalBounds().width,126);
    microWave.texture.setSmooth(true);
    microWave.text.setFont(font);
    microWave.text.setPosition(microWave.sprite.getPosition().x + 180, microWave.sprite.getPosition().y+7);
    microWave.text.setString("Off");

    cash.texture.loadFromFile(getPath() + "/png/cash.png");
    cash.sprite.setTexture(cash.texture);
    cash.sprite.setScale(0.7f,0.7f);
    cash.sprite.setPosition(10,Y- cash.sprite.getGlobalBounds().height -10);
    cash.texture.setSmooth(true);

    juiceMachine.texture.loadFromFile(getPath() + "/png/coffeeMachine.png");
    juiceMachine.sprite.setTexture(juiceMachine.texture);
    juiceMachine.sprite.setScale(0.3f,0.3f);
    juiceMachine.sprite.setPosition(X-juiceMachine.sprite.getGlobalBounds().width *2 +25 ,Y - juiceMachine.sprite.getGlobalBounds().height - 30);
    juiceMachine.texture.setSmooth(true);

}

void Game::updateMousePosition() 
{
    this->mousePosition= sf::Mouse::getPosition(*this->window);
    this->mouseText.setString(std::to_string(sf::Mouse::getPosition(*this->window).x) + " " + std::to_string(sf::Mouse::getPosition(*this->window).y) );
}

void Game::renderItems() {
    for(auto i : vecItems)
    {
        window->draw(i->sprite);
    }
}

void Game::renderEquipment() {
    window->draw(cafeBackground.sprite);
    window->draw(backGround.sprite);
    window->draw(microWave.sprite);
    window->draw(juiceMachine.sprite);
    window->draw(cash.sprite);
    window->draw(microWave.text);
}

Item* Game::checkMouseOnItem()
{
    for (const auto& i : vecItems)
    {
        if (i->sprite.getGlobalBounds().contains(sf::Vector2f(mousePosition)))
        {
            return i;
        }
    }
    return nullptr;
}

Equipment* Game::checkMouseOnEquipment()
{
    for (const auto& i : vecEquipment)
    {
        if (i->sprite.getGlobalBounds().contains(sf::Vector2f(mousePosition)))
        {
            return i;
        }
    }
    return nullptr;
}

sf::Vector2f Game::getLocalClickPosition(const sf::Sprite& sprite, const sf::Vector2i& mousePosition)
{
    // Получаем позицию мыши в локальных координатах объекта
    sf::Vector2f localMousePos = sprite.getInverseTransform().transformPoint(static_cast<sf::Vector2f>(mousePosition));

    // Получение локальных границ спрайта
    sf::FloatRect bounds = sprite.getLocalBounds();

    // Проверка, щелкнули ли на объекте
    if (bounds.contains(localMousePos))
    {
        return localMousePos;
    }
    // Если щелчок мыши не на объекте, возвращаем пустой вектор
    return sf::Vector2f(-0.0f, -0.0f);
}

void Game::genNewClient()
{
    Client client;
    srand(time(0));
    client.category = rand() % RICH;
    if(client.category < RICH && client.category > MID)
    {
        client.category = MID;
        client.texture.loadFromFile("persons/mid.png");
    }
    else if(client.category < MID && client.category > POOR)
    {
        client.category = POOR;
        client.texture.loadFromFile("persons/poor.png");
    }
    else if(client.category == POOREST)
    {
        client.category = POOREST;
        client.texture.loadFromFile("persons/poorest.png");
    }
    else if(client.category < RICH-2)
    {
        client.category = RICH;
        client.texture.loadFromFile("persons/rich.png");
    }
    clients.push_back(client);

}

void Game::renderClient() {
    if(!clients.empty())
    {
        for(auto i : clients)
        {
            window->draw(i.sprite);
        }
    }
}
void Game::renderOrder(Client& client)
{
    sf::Text text;
    sf::Font font;
    font.loadFromFile("arial.ttf");
    for(auto request : client.requests)
    {
        text.setString(request);
        window->draw(text);
    }
}