#include <SFML/Graphics.hpp>
#include <math.h>

void moveObjectWithAnimation(sf::CircleShape& a, const sf::Vector2f& targetPosition, float speed = 200.0f, sf::RenderWindow& window)
{
    sf::Vector2f startPosition = a.getPosition();
    sf::Vector2f direction = targetPosition - startPosition;
    float distance = std::sqrt(direction.x * direction.x + direction.y * direction.y);

    sf::Clock clock;

    while (distance > 1.0f)
    {
        float deltaTime = clock.restart().asSeconds();
        float movement = speed * deltaTime;

        if (movement > distance)
            movement = distance;

        direction = targetPosition - a.getPosition();
        direction = direction / distance * movement;
        a.move(direction);

        distance -= movement;

        // Очистка экрана и рисование объектов здесь
         window.clear();
         window.draw(a);
         window.draw(b);
         window.display();
    }
}

int main()
{
    sf::RenderWindow window(sf::VideoMode(800, 600), "SFML Window");

    // Создание объектов object1 и object2, установка их позиций
    sf::CircleShape a;
    a.setPosition(30,44);
    sf::CircleShape b;
    b.setPosition(500,40);

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();

            if (event.type == sf::Event::MouseButtonPressed)
            {
                if (event.mouseButton.button == sf::Mouse::Left)
                {
                    // Проверка щелчка на object1
                    // Если щелчок был на object1, вызовем анимацию перемещения к object2
                    moveObjectWithAnimation(a, b.getPosition(),&window);
                }
            }
        }

        // Очистка экрана и рисование объектов здесь
         window.clear();
         window.draw(a);
         window.draw(b);
         window.display();
    }

    return 0;
}
