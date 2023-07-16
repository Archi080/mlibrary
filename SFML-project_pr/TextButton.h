#include <SFML/Graphics.hpp>

class TextButton {
public:
    TextButton(sf::Font * font, int height, sf::Color color = sf::Color::White);
    void setText(sf::String text);
    void setHandCursor(sf::Cursor * cursor);
    void setArrowCursor(sf::Cursor * cursor);
    void setPosition(sf::Vector2f point);
    void onHover(sf::RenderWindow &window, bool hover);
    bool isMouseOver(sf::RenderWindow &window);
    void drawTo(sf::RenderWindow &window);
private:
    sf::Text text;
    sf::Cursor * handCursor = nullptr;
    sf::Cursor * arrowCursor = nullptr;
};

