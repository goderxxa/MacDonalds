// Позиция объекта перед перемещением (начальная позиция)
sf::Vector2f initialPosition;

// Позиция объекта после перемещения (конечная позиция)
sf::Vector2f finalPosition;

// Позиция, куда объект должен вернуться после отпускания
sf::Vector2f returnPosition;

// Скорость анимации возвращения
float returnSpeed = 100.0f; // Например, пикселей в секунду

// Флаг, который указывает, возвращается ли объект в данный момент
bool isReturning = false;

// Функция для вычисления траектории возвращения
void StartReturnAnimation() {
    isReturning = true;
}

// В основном цикле обновления
while (window.isOpen()) {
sf::Event event;
while (window.pollEvent(event)) {
if (event.type == sf::Event::Closed) {
window.close();
}

// Обработка мыши
if (event.type == sf::Event::MouseButtonReleased && event.mouseButton.button == sf::Mouse::Left) {
sf::Vector2f mousePosition = sf::Vector2f(sf::Mouse::getPosition(window));

// Проверка, достиг ли объект второго объекта (здесь нужно добавить логику)
bool reachedSecondObject = false; // Замените этот флаг на вашу логику

if (!reachedSecondObject) {
// Объект не достиг второго объекта, начинаем анимацию возвращения
StartReturnAnimation();
}
}
}

// Обновление позиции объекта при перемещении мыши

if (isReturning) {

 object.setPosition(object.getPosition() + (returnPosition - object.getPosition()) * (returnSpeed * deltaTime));


 if (sf::Vector2f::distance(object.getPosition(), returnPosition) < 1.0f) {
     isReturning = false;
 }
}

window.clear();
window.draw(object);
window.display();
}

return 0;
}