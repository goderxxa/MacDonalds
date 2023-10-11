#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>

enum Zone {
    RED,
    BLUE
};

int main() {
    sf::RenderWindow window(sf::VideoMode(800, 600), "Interactive Circle Example");

    sf::RectangleShape redZone(sf::Vector2f(400, 600));
    redZone.setFillColor(sf::Color::Red);

    sf::RectangleShape blueZone(sf::Vector2f(400, 600));
    blueZone.setFillColor(sf::Color::Blue);
    blueZone.setPosition(400, 0);

    sf::CircleShape greenCircle(50);
    greenCircle.setFillColor(sf::Color::Green);
    greenCircle.setPosition(200, 200);

    bool isCircleGrabbed = false;
    sf::Vector2f originalPosition = greenCircle.getPosition();
    Zone currentZone = RED;

    // Для анимации возврата
    sf::Clock returnAnimationClock;
    sf::Time returnAnimationDuration = sf::seconds(1.0); // Продолжительность анимации
    sf::Vector2f returnStartPosition; // Начальная позиция анимации
    bool isReturnAnimation = false;

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
            {
                window.close();
            }
            else if (event.type == sf::Event::MouseButtonPressed)
            {
                if (event.mouseButton.button == sf::Mouse::Left) {
                    if (greenCircle.getGlobalBounds().contains(sf::Vector2f(event.mouseButton.x, event.mouseButton.y))) {
                        isCircleGrabbed = true;
                        originalPosition = greenCircle.getPosition();
                    }
                }
            }
            else if (event.type == sf::Event::MouseButtonReleased)
            {
                if (event.mouseButton.button == sf::Mouse::Left)
                {
                    isCircleGrabbed = false;

                    // Проверяем, в какой зоне находится объект при отпускании
                    sf::Vector2f circlePosition = greenCircle.getPosition(); 	// отпечаток позиции объекта
                    if (currentZone == RED && circlePosition.x >= 400) {
                        // Если объект был в красной зоне и переместился в синюю, оставляем его там
                        currentZone = BLUE;
                    } else if (currentZone == BLUE && circlePosition.x < 400) {
                        // Если объект был в синей зоне и переместился в красную, оставляем его там
                        currentZone = RED;
                    } else {
                        // Если объект не переместился в соседнюю зону, начинаем анимацию возврата
                        isReturnAnimation = true;
                        returnAnimationClock.restart();
                        returnStartPosition = circlePosition;
                    }
                }
            }
            else if (event.type == sf::Event::MouseMoved && isCircleGrabbed)
            {
                greenCircle.setPosition(event.mouseMove.x - greenCircle.getRadius(), event.mouseMove.y - greenCircle.getRadius());
            }
        }

        if (isReturnAnimation) {
            // Вычисляем процент завершения анимации
            float progress = returnAnimationClock.getElapsedTime().asSeconds() / returnAnimationDuration.asSeconds();
            if (progress > 1.0) {
                progress = 1.0;
                isReturnAnimation = false;
            }

            // Интерполируем позицию объекта для анимации
            sf::Vector2f interpolatedPosition = returnStartPosition + (originalPosition - returnStartPosition) * progress;
            greenCircle.setPosition(interpolatedPosition);
        }

        window.clear();
        window.draw(redZone);
        window.draw(blueZone);
        window.draw(greenCircle);
        window.display();
    }

    return 0;
}