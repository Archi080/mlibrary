
#include "Button.h"

Button::Button(Texture * texture, float height) {
    float scale = height / texture->getSize().y;
    button.setSize(Vector2f(texture->getSize().x * scale, texture->getSize().y * scale));
    button.setTexture(texture);
}
Button::Button(){}
void Button::setTexture(Texture* texture, float height) {
    float scale = height / texture->getSize().y;
    button.setSize(Vector2f(texture->getSize().x * scale, texture->getSize().y * scale));
    button.setTexture(texture);
}
void Button::setPosition(sf::Vector2f point) {
    button.setPosition(point);
}
void Button::drawTo(sf::RenderWindow &window) {
    window.draw(button);
}
Vector2f Button::getSize() {
    return button.getSize();
}
bool Button::isMouseOver(sf::RenderWindow &window) {
    int mouseX = sf::Mouse::getPosition(window).x;
    int mouseY = sf::Mouse::getPosition(window).y;

    int btnPosX = button.getPosition().x;
    int btnPosY = button.getPosition().y;

    int btnxPosWidth = button.getPosition().x + button.getSize().x;
    int btnyPosHeight = button.getPosition().y + button.getSize().y;

    if (mouseX < btnxPosWidth && mouseX > btnPosX && mouseY < btnyPosHeight && mouseY > btnPosY) {
        return true;
    }
    return false;
}
bool Button::isMouseOver(Vector2i position) {
    int btnPosX = button.getPosition().x;
    int btnPosY = button.getPosition().y;

    int btnxPosWidth = button.getPosition().x + button.getSize().x;
    int btnyPosHeight = button.getPosition().y + button.getSize().y;

    if (position.x < btnxPosWidth && position.x > btnPosX && position.y < btnyPosHeight && position.y > btnPosY) {
        return true;
    }
    return false;
}