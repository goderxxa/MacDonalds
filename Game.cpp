//
// Created by sergey on 27.09.23.
//

#include "Game.h"

//videomode
#define X 800
#define Y 600
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
}


void Game::RenderFrame() {
    window->clear();
    window->draw(current);
    window->draw(mouseText);

    window->display();
//    std::cout << sf::Mouse::getPosition(*window).x  << " " <<
//                 sf::Mouse::getPosition(*window).y <<  "\n";
}

const bool Game::running() const {

    return this->window->isOpen();
}

void Game::initObjects() {
#define X 100
#define Y 100

    this->current.setFillColor(sf::Color::Green);
    this->current.setSize(sf::Vector2f(X,Y));
    this->current.setPosition(100, 100);
    this->current.setOrigin(sf::Vector2f(X/2, Y/2));

}

void Game::updateMousePosition() {
    this->mousePosition= sf::Mouse::getPosition(*this->window);
    this->mouseText.setString(std::to_string(sf::Mouse::getPosition(*this->window).x) + " " + std::to_string(sf::Mouse::getPosition(*this->window).y) );

}


