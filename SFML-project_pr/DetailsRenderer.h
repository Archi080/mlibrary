
#ifndef MOVIELIBRARY_DETAILSRENDERER_H
#define MOVIELIBRARY_DETAILSRENDERER_H

#include "SFML/Graphics.hpp"
#include "DataTypes.h"
#include "Scrollers.h"
#include "MoviesException.h"
#include <iostream>
#include <Windows.h>

class DetailsRenderer{
public:
    DetailsRenderer(sf::Font * bold, sf::Font * medium, sf::Font * regular, sf::Font * light, float uiScale){
        image.setPosition(86 * uiScale, 317 * uiScale);
        image.setSize(sf::Vector2f(429 * uiScale, 624 * uiScale));

        title.setFont(*bold);
        title.setCharacterSize(59 * uiScale);
        title.setPosition(553 * uiScale, 303 * uiScale);
        title.setFillColor(sf::Color::White);

        yearAndCountry.setFont(*light);
        yearAndCountry.setCharacterSize(24 * uiScale);
        yearAndCountry.setPosition(553 * uiScale, 384 * uiScale);
        yearAndCountry.setFillColor(sf::Color::White);

        largeText.setFont(*regular);
        largeText.setCharacterSize(21 * uiScale);
        largeText.setPosition(553 * uiScale, 434 * uiScale);
        largeText.setFillColor(sf::Color::White);
        largeText.setLineSpacing(1.2f);

        budgetLabel.setFont(*bold);
        budgetLabel.setCharacterSize(16 * uiScale);
        budgetLabel.setPosition(553 * uiScale, 657 * uiScale);
        budgetLabel.setFillColor(sf::Color::White);
        budgetLabel.setString(L"BUDGET");

        boxOfficeLabel.setFont(*bold);
        boxOfficeLabel.setCharacterSize(16 * uiScale);
        boxOfficeLabel.setPosition(553 * uiScale, 685 * uiScale);
        boxOfficeLabel.setFillColor(sf::Color::White);
        boxOfficeLabel.setString(L"GLOBAL BOX OFFICE");
        
        
        playButtonTexture.loadFromFile("C:\\Users\\Admin\\Desktop\\MovieLibrary\\resources\\textures\\play_button.png");
        playButton.setTexture(&playButtonTexture,40 * uiScale);
        playButton.setPosition(sf::Vector2f(1213 * uiScale, 661 * uiScale));

        directorLabel.setFont(*bold);
        directorLabel.setCharacterSize(16 * uiScale);
        directorLabel.setPosition(553 * uiScale, 725 * uiScale);
        directorLabel.setFillColor(sf::Color::White);
        directorLabel.setString(L"DIRECTOR");

        actorsLabel.setFont(*bold);
        actorsLabel.setCharacterSize(16 * uiScale);
        actorsLabel.setPosition(742 * uiScale, 725 * uiScale);
        actorsLabel.setFillColor(sf::Color::White);
        actorsLabel.setString(L"STARRING");

        budget.setFont(*medium);
        budget.setCharacterSize(16 * uiScale);
        budget.setPosition(650 * uiScale, 657 * uiScale);
        budget.setFillColor(sf::Color::White);

        boxOffice.setFont(*medium);
        boxOffice.setCharacterSize(16 * uiScale);
        boxOffice.setPosition((float)(748 * uiScale), 685 * uiScale);
        boxOffice.setFillColor(sf::Color::White);

        movies = new horizontalMovieScroller(123 * uiScale, 176 * uiScale, 800 * uiScale, sf::Color(65, 65, 65));
        movies->setPosition(553 * uiScale, 755 * uiScale);
        movies->setGap(18 * uiScale);

        people = new horizontalPersonScroller(128 * uiScale, 20 * uiScale, 10 * uiScale,800 * uiScale, sf::Color(65, 65, 65));
        people->setPosition(742 * uiScale, 755 * uiScale);
        people->setGap(32 * uiScale);
        people->setFont(medium);

        directorPortrait = new HumanPortrait(128 * uiScale, 20 * uiScale, 10 * uiScale, medium);
        directorPortrait->setPosition(sf::Vector2f(553 * uiScale, 752.5f * uiScale));
    }

    void render(Movie * object){
        playing = false;
        image.setTexture(object->getPoster());
        title.setString(fromUtf8(object->getTitle()));
        sf::String yAc = fromUtf8(toString(object->getYear()));
        yAc += L", ";
        yAc += fromUtf8(object->getCountries());
        yearAndCountry.setString(yAc);
        largeText.setString(fromUtf8(setMaxLength(object->getSynopsis(), 150)));
        budget.setString(fromUtf8(toString(object->getBudgetUSD(), true)));
        boxOffice.setString(fromUtf8(toString(object->getBoxOfficeUSD(), true)));
        directorPortrait->setHuman(object->getDirector());
        people->clear();
        if(!object->getActors().isEmpty()){
            auto first = object->getActors().operator[](0);
            people->addHuman(first);
            int i = 1;
            while(object->getActors().operator[](i) != first){
                people->addHuman(object->getActors().operator[](i));
                i++;
            }
        }
        isMovie = true;
        rendered = true;
        lastRenderedMovie = object;
    }

    void render(Human * object){
        image.setTexture(object->getPortrait());
        title.setString(fromUtf8(object->getFullName()));
        largeText.setString(fromUtf8(setMaxLength(object->getBio(), 150)));
        movies->clear();
        for(int i = 0; i < object->getMovies().getSize(); i++){
            movies->addMovie(object->getMovies().operator[](i));
        }
        isMovie = false;
        rendered = true;
        lastRenderedHuman = object;
    }

    bool clickHandler(bool state, Vector2i clickPosition = Vector2i(0, 0)){
        if(!isMovie){
            if(movies->hasPoint(clickPosition)){
                return movies->setDragging(state, clickPosition.x);
            }
            else return false;
        }
        else{
            if(people->hasPoint(clickPosition)){
                return people->setDragging(state, clickPosition.x);
            }
            else return false;
        }

    }

    void clickHandler(Vector2i clickPosition){
        if(!isMovie){
            if(movies->hasPoint(clickPosition)){
                movies->onDrag(clickPosition.x);
            }
        }
        else{
            if(people->hasPoint(clickPosition)){
                people->onDrag(clickPosition.x);
            }
            
        }

    }

    Movie * getLastRenderedMovie(){
        return lastRenderedMovie;
    }

    Human * getLastRenderedHuman(){
        return lastRenderedHuman;
    }

    void tap(Vector2i point){
        
        if(!isMovie){
            render(movies->tap(point.x));
        }
        else{
            render(people->tap(point.x));
        }
    }

    void checkTapOnPlayButton(Vector2i point) {
        if (playButton.isMouseOver(point) && !playing && !lastRenderedMovie->getFilmPath().empty()) {
            playing = true;
            //WinExec(("C:\\Program Files\\VideoLAN\\VLC\\vlc.exe " + lastRenderedMovie->getFilmPath() + " -f").c_str(), 0);
            system(("\""+lastRenderedMovie->getFilmPath()+"\"").c_str());

            //std::cout << "Knopka nashata\n";
        }
    }

    bool isMouseOverScroller(Vector2i point){
        return movies->hasPoint(point);
    }

    void draw(sf::RenderWindow & window){
        if(rendered){
            if(isMovie){
                window.draw(yearAndCountry);
                window.draw(budgetLabel);
                window.draw(budget);
                window.draw(boxOfficeLabel);
                window.draw(boxOffice);
                window.draw(directorLabel);
                window.draw(actorsLabel);
                people->draw(window);
                directorPortrait->drawTo(&window);
                if (!playing && !lastRenderedMovie->getFilmPath().empty()) {
                    playButton.drawTo(window);
                }
            }
            else
            {
                movies->draw(window);
            }
            window.draw(image);
            window.draw(title);
            window.draw(largeText);
        }
    }

private:

    static std::string toString(int number, bool money = false){
        std::string result;
        int digits = 0;
        while (number != 0){
            if(digits % 3 == 0 && money){
                result = " " + result;
            }
            result = (char)((number % 10) + 48) + result;
            number /= 10;
            digits++;
        }
        if(money && !result.empty()) result = "$" + result;
        return result;
    }

    static sf::String fromUtf8(const std::string & string){
        return sf::String::fromUtf8(string.begin(), string.end());
    }

    static std::string setMaxLength(std::string string, int maxLength){
        if(string.size() <= maxLength) return string;
        int symbols = 0;
        while (true){
            if(symbols + maxLength >= string.size()) break;
            while (string[symbols + maxLength - 1] != ' ') symbols--;
            string[symbols + maxLength - 1] = '\n';
            symbols = symbols + maxLength - 1;
        }
        return string;
    }
    bool playing = false;
    sf::Texture playButtonTexture;
    sf::RectangleShape image;
    sf::Text title;
    sf::Text yearAndCountry;
    sf::Text largeText;
    sf::Text budgetLabel;
    sf::Text budget;
    sf::Text boxOfficeLabel;
    sf::Text boxOffice;
    sf::Text directorLabel;
    sf::Text actorsLabel; 
    Button playButton;
    HumanPortrait * directorPortrait = nullptr;
    bool isMovie;
    bool rendered = false;
    horizontalMovieScroller * movies;
    horizontalPersonScroller * people;
    Human * lastRenderedHuman = nullptr;
    Movie * lastRenderedMovie = nullptr;
};

#endif //MOVIELIBRARY_DETAILSRENDERER_H
