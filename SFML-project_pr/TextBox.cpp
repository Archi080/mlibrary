#define DELETE_KEY 8
#define ENTER_KEY 13
#define ESCAPE_KEY 27

#include <SFML/Graphics.hpp>
#include "TextBox.h"

TextBox::TextBox(sf::Texture * texture, float height, sf::Color textColor, sf::Color hintColor, bool isSelected) {
    float scale = height / texture->getSize().y;
    background.setSize(sf::Vector2f(texture->getSize().x * scale, texture->getSize().y * scale));
    background.setTexture(texture);
    textbox.setCharacterSize(height/2);
    textbox.setFillColor(hintColor);
    this->hintColor = hintColor;
    this->textColor = textColor;
    this->isSelected = isSelected;
    textbox.setString(sf::String::fromUtf32(hint.begin(), hint.end()));
}

void TextBox::setSelected(bool isSelected) {
    this->isSelected = isSelected;
    if(isSelected){
        textbox.setFillColor(this->textColor);
        textbox.setString(text + "|");
    }
    else
    {
        if (text.isEmpty()) {
            textbox.setFillColor(this->hintColor);
            textbox.setString(hint);
        }
        else{
            textbox.setString(text);
        }
    }
}

void TextBox::clear() {
    textbox.setString("");
    text.clear();
}

sf::String TextBox::getQuery() {
    return text;
}

void TextBox::setFont(sf::Font &fonts) {
    textbox.setFont(fonts);
}

void TextBox::setLimit(bool hasLimit, int limit) {
    this->hasLimit = hasLimit;
    this->limit = limit - 1;
}

void TextBox::setPosition(sf::Vector2f point) {
    background.setPosition(point);
    textbox.setPosition(sf::Vector2f(point.x + 10, point.y + /*background.getSize().y / 5*/ 10));
}

void TextBox::drawTo(sf::RenderWindow &window) {
    window.draw(background);
    window.draw(textbox);
}

sf::Vector2f TextBox::getSize() {
    return background.getSize();
}

bool TextBox::isMouseOver(sf::RenderWindow &window) {
    int mouseX = sf::Mouse::getPosition(window).x;
    int mouseY = sf::Mouse::getPosition(window).y;

    int btnPosX = background.getPosition().x;
    int btnPosY = background.getPosition().y;

    int btnxPosWidth = background.getPosition().x + background.getSize().x;
    int btnyPosHeight = background.getPosition().y + background.getSize().y;

    if (mouseX < btnxPosWidth && mouseX > btnPosX && mouseY < btnyPosHeight && mouseY > btnPosY) {
        return true;
    }
    return false;
}

void TextBox::typedOn(sf::Event input){
    if (isSelected) {
        sf::Uint32 charTyped = input.text.unicode;
        if (hasLimit) {
            if (text.getSize() <= limit) {
                inputLogic(charTyped);
            }
            else if (text.getSize() > limit && charTyped == DELETE_KEY) {
                deleteLastChar();
            }
        }
        else {
            inputLogic(charTyped);
        }
    }
}

void TextBox::deleteLastChar(){
    text.erase(text.getSize() - 1);
    textbox.setString(sf::String::fromUtf32(text.begin(), text.end()) + "|");
}

void TextBox::inputLogic(sf::Uint32 charTyped) {
    if (charTyped != DELETE_KEY && charTyped != ENTER_KEY && charTyped != ESCAPE_KEY) {
        text += charTyped;
    }
    else if (charTyped == DELETE_KEY) {
        if (text.getSize() > 0) {
            deleteLastChar();
        }
    }
    textbox.setString(sf::String::fromUtf32(text.begin(), text.end()) + "|");
}

void TextBox::setTexture(sf::Texture * texture) {
    background.setTexture(texture);
}

void TextBox::setHint(sf::String string) {
    this->hint = sf::String::fromUtf32(string.begin(), string.end());
    textbox.setString(sf::String::fromUtf32(hint.begin(), hint.end()));
}

void TextBox::setTextCursor(sf::Cursor *cursor) {
    textCursor = cursor;
}

void TextBox::setArrowCursor(sf::Cursor *cursor) {
    arrowCursor = cursor;
}

void TextBox::onHover(sf::RenderWindow &window, bool hover) {
    if(hover){
        if (textCursor) window.setMouseCursor(*textCursor);
    }
    else
    {
        if (arrowCursor) window.setMouseCursor(*arrowCursor);
    }
}

void TextBox::setString(std::string data) {
    this->text = sf::String::fromUtf8(data.begin(), data.end());
    textbox.setFillColor(this->textColor);
    textbox.setString(text);
}
