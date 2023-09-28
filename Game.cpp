//
// Created by sergey on 27.09.23.
//

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
    processEvents();
    updateMousePosition();
    updateOrder();
}

void Game::updateOrder() {

    if(sf::Mouse::isButtonPressed(sf::Mouse::Left)  )
    {
        selItem = checkMouseOnItem();
        draggin = true;
    }
    if(draggin && selItem!= nullptr)
    {
        selItem->setPosition(sf::Vector2f (this->mousePosition));
    }
    if(sf::Mouse::isButtonPressed(sf::Mouse::Right))
        draggin = false;
}


void Game::renderFrame() {
    window->clear();
    window->draw(mouseText);
    renderEquipment();

    renderItems();

    window->display();
//    std::cout << sf::Mouse::getPosition(*window).x  << " " <<
//                 sf::Mouse::getPosition(*window).y <<  "\n";
}

const bool Game::running() const {

    return this->window->isOpen();
}

void Game::initObjects() {

    //setting objects
    this->juice.texture.loadFromFile(getPath() + "/png/orangeJuice.png");
    this->hamburger.texture.loadFromFile(getPath() + "/png/hamburger.png");
    this->potato.texture.loadFromFile(getPath() + "/png/freePotato.png");

    juice.sprite.setTexture(juice.texture);
    hamburger.sprite.setTexture(hamburger.texture);
    potato.sprite.setTexture(potato.texture);

    //setting by for loop
    sf::Vector2f itemPos(10, 50);
    float offset=0;
    for(auto i : vecItems)
    {
        i->sprite.setPosition(itemPos.x, itemPos.y+offset);
        offset += 110;
        i->sprite.setScale(0.4f, 0.4f);
    }

    //setting equipment on screen
    this->microWaveTexure.loadFromFile(getPath() + "/png/microwave.png");
    this->microWaveSprite.setTexture(microWaveTexure);
    this->microWaveSprite.setPosition(X-microWaveSprite.getLocalBounds().width,0);
    this->microWaveTexure.setSmooth(true);

    this->cashTexture.loadFromFile(getPath() + "/png/cash.png");
    this->cashSprite.setTexture(cashTexture);
    this->cashSprite.setScale(0.7f,0.7f);
    this->cashSprite.setPosition(150,Y- cashSprite.getGlobalBounds().height);
    this->cashTexture.setSmooth(true);

}

void Game::updateMousePosition() {
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
    window->draw(microWaveSprite);
    window->draw(cashSprite);
}

sf::Sprite* Game::checkMouseOnItem()
{
    for (const auto& i : vecItems)
    {
        if (i->sprite.getGlobalBounds().contains(sf::Vector2f(mousePosition)))
        {
            i->sprite.setOrigin(getLocalClickPosition(i->sprite, mousePosition));
            return &i->sprite;
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
    return sf::Vector2f(-1.0f, -1.0f);
}