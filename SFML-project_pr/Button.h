#ifndef MOVIELIBRARY_BUTTON_H
#define MOVIELIBRARY_BUTTON_H

#include "SFML/Graphics.hpp"
using namespace sf;

class Button {
public:
    Button(Texture * texture, float height);
    Button();
    void setTexture(Texture* texture, float height);
    void setPosition(sf::Vector2f point);
    void drawTo(sf::RenderWindow &window);
    Vector2f getSize();
    bool isMouseOver(sf::RenderWindow &window);
    bool isMouseOver(Vector2i position);
private:
    sf::RectangleShape button;
};



#endif //MOVIELIBRARY_BUTTON_H
