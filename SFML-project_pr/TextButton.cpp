#include "TextButton.h"

TextButton::TextButton(sf::Font *font, int height, sf::Color color) {
    text.setFont(*font);
    text.setFillColor(color);
    text.setCharacterSize(height);
}

void TextButton::setText(sf::String text) {
    this->text.setString(sf::String::fromUtf32(text.begin(), text.end()));
}

void TextButton::setPosition(sf::Vector2f point) {
    this->text.setPosition(point);
}

void TextButton::onHover(sf::RenderWindow &window, bool hover) {
    if(hover){
        this->text.setStyle(sf::Text::Underlined);
        if (handCursor) window.setMouseCursor(*handCursor);
    }
    else{
        this->text.setStyle(sf::Text::Regular);
        if (arrowCursor) window.setMouseCursor(*arrowCursor);

    }
}

bool TextButton::isMouseOver(sf::RenderWindow &window) {
    int mouseX = sf::Mouse::getPosition(window).x;
    int mouseY = sf::Mouse::getPosition(window).y;

    int btnPosX = text.getPosition().x;
    int btnPosY = text.getPosition().y;

    int btnxPosWidth = text.getPosition().x + text.getLocalBounds().width;
    int btnyPosHeight = text.getPosition().y + text.getLocalBounds().height * 1.5f;

    if (mouseX < btnxPosWidth && mouseX > btnPosX && mouseY < btnyPosHeight && mouseY > btnPosY) {
        return true;
    }
    return false;
}

void TextButton::drawTo(sf::RenderWindow &window) {
    window.draw(text);
}

void TextButton::setHandCursor(sf::Cursor *cursor) {
    handCursor = cursor;
}

void TextButton::setArrowCursor(sf::Cursor *cursor) {
    arrowCursor = cursor;
}
