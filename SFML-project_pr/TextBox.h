
#pragma once

#include <sstream>
#include <SFML/Graphics.hpp>

class TextBox {
public:
    TextBox(sf::Texture * texture,
            float height,
            sf::Color textColor = sf::Color::White,
            sf::Color hintColor = sf::Color(200, 200, 200),
            bool isSelected = false);
    void setSelected(bool isSelected);
    void clear();
    sf::String getQuery();
    void setFont(sf::Font &fonts);
    void setLimit(bool ToF, int lim);
    void setPosition(sf::Vector2f point);
    void drawTo(sf::RenderWindow &window);
    sf::Vector2f getSize();
    bool isMouseOver(sf::RenderWindow &window);
    void typedOn(sf::Event input);
    void setTexture(sf::Texture * texture);
    void setHint(sf::String string);
    void setTextCursor(sf::Cursor * cursor);
    void setArrowCursor(sf::Cursor * cursor);
    void onHover(sf::RenderWindow &window, bool hover);
    void setString(std::string);

private:
    sf::Text textbox;
    sf::String text;
    sf::String hint = L"Поиск";
    sf::Color textColor;
    sf::Color hintColor;
    sf::RectangleShape background;
    bool isSelected = false;
    bool hasLimit = false;
    int limit = 0;
    void deleteLastChar();
    void inputLogic(sf::Uint32 charTyped);
    sf::Cursor * textCursor = nullptr;
    sf::Cursor * arrowCursor = nullptr;
};