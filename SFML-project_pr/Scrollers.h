#include "SFML/Graphics.hpp"
#include "DataArrays.h"
#include "DataTypes.h"
#include "MoviesException.h"

using namespace sf;

class HumanPortrait{
public:
    HumanPortrait(int portraitDiameter, int charSize, int verticalGap, Human * person, Font * font){
        vGap = verticalGap;
        portrait.setRadius(portraitDiameter / 2);
        name.setCharacterSize(charSize);
        surname.setCharacterSize(charSize);
        name.setFillColor(sf::Color::White);
        surname.setFillColor(sf::Color::White);
        name.setFont(*font);
        surname.setFont(*font);
        name.setString(fromUtf8(person->getName()));
        surname.setString(fromUtf8(person->getSurname()));
        portrait.setTexture(person->getPortrait());
        portrait.setTextureRect(IntRect(0, 0, person->getPortrait()->getSize().x, person->getPortrait()->getSize().x));

    }

    HumanPortrait(int portraitDiameter, int charSize, int verticalGap, Font * font){
        vGap = verticalGap;
        portrait.setRadius(portraitDiameter / 2);
        name.setCharacterSize(charSize);
        surname.setCharacterSize(charSize);
        name.setFillColor(sf::Color::White);
        surname.setFillColor(sf::Color::White);
        name.setFont(*font);
        surname.setFont(*font);
        name.setPosition(Vector2f(0, portraitDiameter + verticalGap));
        surname.setPosition(Vector2f(0, portraitDiameter + verticalGap  + charSize));
    }

    void setHuman(Human * person){
        name.setString(fromUtf8(person->getName()));
        surname.setString(fromUtf8(person->getSurname()));
        portrait.setTexture(person->getPortrait());
        portrait.setTextureRect(IntRect(0, 0, person->getPortrait()->getSize().x, person->getPortrait()->getSize().x));
        name.setPosition(Vector2f(portrait.getPosition().x + (portrait.getRadius() * 2 - name.getGlobalBounds().width) / 2, portrait.getPosition().y + portrait.getRadius() * 2 + vGap));
        surname.setPosition(Vector2f(portrait.getPosition().x + (portrait.getRadius() * 2 - surname.getGlobalBounds().width) / 2, portrait.getPosition().y + portrait.getRadius() * 2 + vGap  + surname.getCharacterSize()));
    }

    void setPosition(sf::Vector2f point){
        portrait.setPosition(point);
        name.setPosition(Vector2f(point.x + (portrait.getRadius() * 2 - name.getGlobalBounds().width) / 2, point.y + portrait.getRadius() * 2 + vGap));
        surname.setPosition(Vector2f(point.x + (portrait.getRadius() * 2 - surname.getGlobalBounds().width) / 2, point.y + portrait.getRadius() * 2 + vGap  + surname.getCharacterSize()));
    }

    void drawTo(sf::RenderTexture * texture){
        texture->draw(portrait);
        texture->draw(name);
        texture->draw(surname);
    }

    void drawTo(sf::RenderWindow * window){
        window->draw(portrait);
        window->draw(name);
        window->draw(surname);
    }

    bool isMouseOver(sf::RenderWindow * window){
        return(sf::Mouse::getPosition(*window).x > portrait.getPosition().x &&
               sf::Mouse::getPosition(*window).x < (portrait.getPosition().x + portrait.getRadius() * 2) &&
               sf::Mouse::getPosition(*window).y > portrait.getPosition().y &&
               sf::Mouse::getPosition(*window).y < (portrait.getPosition().y + portrait.getRadius() * 2));
    }
private:
    static sf::String fromUtf8(const std::string & string){
        return sf::String::fromUtf8(string.begin(), string.end());
    }
    int vGap;
    CircleShape portrait;
    Text name;
    Text surname;
};

class horizontalMovieScroller{
public:
    horizontalMovieScroller(int posterWidth, int height, int width, Color color){
        this->posterSize = Vector2i(posterWidth, height);
        element.create(width, height);
        background.setSize(Vector2f(width, height));
        backgroundTexture = createBackgroundTexture(width, height, color);
        background.setTexture(&backgroundTexture);
    }

    void setGap(int value){
        this->gap = value;

    }

    void setPosition(int x, int y){
        this->position = Vector2i(x, y);
    }

    void addMovie(Movie * movie){
        this->movies.add(movie);
        auto poster = new RectangleShape();
        poster->setSize(Vector2f(posterSize));
        poster->setTexture(movie->getPoster());
        this->posters.add(poster);
    }

    bool isMouseOver(sf::RenderWindow &window) const {
        int mouseX = sf::Mouse::getPosition(window).x;
        int mouseY = sf::Mouse::getPosition(window).y;
        int elementPositionX = position.x;
        int elementPositionY = position.y;
        int elementPositionXHeight = position.x + (int)element.getSize().x;
        int elementPositionYHeight = position.y + (int)element.getSize().y;

        if ( mouseY < elementPositionYHeight
            && mouseY > elementPositionY
            && mouseX < elementPositionXHeight
            && mouseX > elementPositionX)
        {
            return true;
        }
        return false;
    }

    bool hasPoint(Vector2i point){
        return (point.x > position.x
                && point.y > position.y
                && point.x < (position.x + (int)element.getSize().x)
                && point.y < position.y + (int)element.getSize().y);
    }

    void onDrag(int mouseXPos){
        if(this->dragging){
            offset += mouseXPos - previousMouseXPosition;
            if (offset > 0) offset = 0;
            previousMouseXPosition = mouseXPos;
            dataOffset = offset / (int)(gap + posterSize.x);
        }
    }

    bool setDragging(bool state, int mouseXPos = 0){
        bool toReturn = false;
        this->dragging = state;
        if (state) initialX = mouseXPos;
        else{
            if(initialX == previousMouseXPosition){
                toReturn = true;
            }
        }
        previousMouseXPosition = mouseXPos;
        return toReturn;
    }

    void draw(RenderWindow & window){
        element.clear();
        element.draw(background);
        int visible = (int)element.getSize().x / (posterSize.x + gap) + 2;
        for(int i = 0; i < visible; i++){
            auto currentPosition = Vector2i((offset % (gap + posterSize.x)) + (this->posterSize.x + this->gap) * i, 0);
            posters[i - dataOffset]->setPosition(Vector2f(currentPosition));
            element.draw(*posters[i - dataOffset]);
            //window.draw(*portraits[i - dataOffset]);
        }
        element.display();
        RectangleShape toDraw;
        toDraw.setSize((Vector2f)element.getSize());
        toDraw.setPosition((Vector2f)position);
        toDraw.setTexture(&element.getTexture());
        window.draw(toDraw);
    }

    void clear(){
        while (true){
            try{
                this->movies.pop();
                this->posters.pop();
            }
            catch (bad_addr_exception e){
                break;
            }
        }

    }

    Movie * tap(int mouseXPos){
        int index = (mouseXPos - position.x - offset % (gap + posterSize.x)) / (posterSize.x + gap) - dataOffset;
        return movies.operator[](index);
    }

private:
    RenderTexture element;
    Vector2i posterSize;
    Vector2i position;
    int gap = 0;
    int offset = 0;
    int dataOffset = 0;
    int previousMouseXPosition = 0;
    int initialX = 0;
    bool dragging = false;
    CircularList<Movie> movies;
    CircularList<RectangleShape> posters;
    RectangleShape background;
    Texture backgroundTexture;
};

class horizontalPersonScroller{
public:
    horizontalPersonScroller(int portraitDiameter, int charSize, int verticalGap, int width, Color color){
        portraitSize = portraitDiameter;
        element.create(width, portraitDiameter + verticalGap + 2 * charSize);
        background.setSize(Vector2f(width, portraitDiameter + verticalGap + 2 * charSize));
        backgroundTexture = createBackgroundTexture(width, portraitDiameter + verticalGap + 2 * charSize, color);
        background.setTexture(&backgroundTexture);
        this->verticalGap = verticalGap;
        this->charSize = charSize;
    }

    void setGap(int value){
        this->gap = value;

    }

    void setPosition(int x, int y){
        this->position = Vector2i(x, y);
    }

    void addHuman(Human * person){
        this->people.add(person);
        auto portrait = new HumanPortrait(portraitSize, charSize, verticalGap, person, font);
        portraits.add(portrait);
    }

    bool isMouseOver(sf::RenderWindow &window) const {
        int mouseX = sf::Mouse::getPosition(window).x;
        int mouseY = sf::Mouse::getPosition(window).y;
        int elementPositionX = position.x;
        int elementPositionY = position.y;
        int elementPositionXHeight = position.x + (int)element.getSize().x;
        int elementPositionYHeight = position.y + (int)element.getSize().y;

        if ( mouseY < elementPositionYHeight
             && mouseY > elementPositionY
             && mouseX < elementPositionXHeight
             && mouseX > elementPositionX)
        {
            return true;
        }
        return false;
    }

    bool hasPoint(Vector2i point){
        return (point.x > position.x
                && point.y > position.y
                && point.x < (position.x + (int)element.getSize().x)
                && point.y < position.y + (int)element.getSize().y);
    }

    void onDrag(int mouseXPos){
        if(this->dragging){
            offset += mouseXPos - previousMouseXPosition;
            if (offset > 0) offset = 0;
            previousMouseXPosition = mouseXPos;
            dataOffset = offset / (int)(gap + portraitSize);
        }
    }

    bool setDragging(bool state, int mouseXPos = 0){
        bool toReturn = false;
        this->dragging = state;
        if (state) initialX = mouseXPos;
        else{
            if(initialX == previousMouseXPosition){
                toReturn = true;
            }
        }
        previousMouseXPosition = mouseXPos;
        return toReturn;
    }

    void draw(RenderWindow & window){
        if(portraits.isEmpty()) return;
        element.clear();
        element.draw(background);
        int visible = (int)element.getSize().x / (portraitSize + gap) + 2;
        for(int i = 0; i < visible; i++){
            int currentPositionX = (offset % (gap + portraitSize)) + (this->portraitSize+ this->gap) * i;
            portraits[i - dataOffset]->setPosition(Vector2f((float)currentPositionX, 0));
            portraits[i - dataOffset]->drawTo(&element);
        }
        element.display();
        RectangleShape toDraw;
        toDraw.setSize((Vector2f)element.getSize());
        toDraw.setPosition((Vector2f)position);
        toDraw.setTexture(&element.getTexture());
        window.draw(toDraw);
    }

    void clear(){
        while (true){
            try{
                this->people.pop();
                this->portraits.pop();
//                this->names.pop();
//                this->surnames.pop();
            }
            catch (bad_addr_exception e){
                break;
            }
        }

    }

    Human * tap(int mouseXPos){
        int index = (mouseXPos - position.x - offset % (gap + portraitSize)) / (portraitSize + gap) - dataOffset;
        return people.operator[](index);
    }

    void setFont(Font * font){
        this->font = font;
    }

private:
    static sf::String fromUtf8(const std::string & string){
        return sf::String::fromUtf8(string.begin(), string.end());
    }
    RenderTexture element;
    int portraitSize;
    Vector2i position;
    int verticalGap;
    int charSize;
    int gap = 0;
    int offset = 0;
    int dataOffset = 0;
    int previousMouseXPosition = 0;
    int initialX = 0;
    bool dragging = false;
    CircularList<Human> people;
    CircularList<HumanPortrait> portraits;
    RectangleShape background;
    Texture backgroundTexture;
    Font * font = nullptr;
};

