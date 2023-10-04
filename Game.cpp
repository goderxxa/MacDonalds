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

    //load images
    backGround.texture.loadFromFile(getPath() + "/png/gamebackground.png");
    backGround.sprite.setTexture(backGround.texture);
    backGround.sprite.setScale(0.668,0.668);

    cafeBackground.texture.loadFromFile(getPath() + "/png/gameback.png");
    cafeBackground.sprite.setTexture(cafeBackground.texture);
    cafeBackground.sprite.setScale(0.668,0.668);

    //setting objects

    hamburger.texture.loadFromFile(getPath() + "/png/hamburger.png");
    cheseburger.texture.loadFromFile(getPath() + "/png/cheseburger.png");
    bigmak.texture.loadFromFile(getPath() + "/png/bigmak.png");
    villagePotato.texture.loadFromFile(getPath() + "/png/villagepotato.png");
    potato.texture.loadFromFile(getPath() + "/png/freePotato.png");
    souse.texture.loadFromFile(getPath() + "/png/chesesouse.png");
    juice.texture.loadFromFile(getPath() + "/png/orangeJuice.png");
    coffee.texture.loadFromFile(getPath() + "/png/coffee.png");

    hamburger.sprite.setTexture(hamburger.texture);
    cheseburger.sprite.setTexture(cheseburger.texture);
    bigmak.sprite.setTexture(bigmak.texture);
    villagePotato.sprite.setTexture(villagePotato.texture);
    potato.sprite.setTexture(potato.texture);
    souse.sprite.setTexture(souse.texture);
    juice.sprite.setTexture(juice.texture);
    coffee.sprite.setTexture(coffee.texture);

    //setting by for loop
    sf::Vector2f itemPos(10, 10);
    float offset=7;
    for(auto i : vecItems)
    {
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
    window->draw(cafeBackground.sprite);
    window->draw(backGround.sprite);
    window->draw(microWave.sprite);
    window->draw(juiceMachine.sprite);
    window->draw(cash.sprite);


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
    else if(client.category == 1)
    {
        client.category = POOREST;
        client.texture.loadFromFile("persons/poorest.png");
    }
    else if(client.category == 10)
    {
        client.category = RICH;
        client.texture.loadFromFile("persons/rich.png");
    }
    if(client.category == RICH)
    {
        srand(time(0));
        int n = std::rand() % 6 + 5;
        for(int i=0;i<n;++i)
        {

        }
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