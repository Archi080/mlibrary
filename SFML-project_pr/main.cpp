#include "json.hpp"
#include "DataArrays.h"
#include "DataTypes.h"
#include "jsonHandler.h"
#include "Functions.h"
#include "SFML/Graphics.hpp"
#include "TextButton.h"
#include "TextBox.h"
#include "DetailsRenderer.h"
#include "DBManager.h"
using json = nlohmann::json;

#define UI_SCALE 0.9f

int main() {
    DBManager dbmanager;
    
    //Настройки окна программы
    sf::String title = L"Фильмотека";
    const int windowHeight = (int)(1024 * UI_SCALE);
    const int windowWidth = (int)(1440 * UI_SCALE);
    sf::ContextSettings settings;
    //settings.antialiasingLevel = 32;

    //Вызов окна
    sf::RenderWindow window(
        sf::VideoMode(
            windowWidth,
            windowHeight),
        sf::String::fromUtf32(
            title.begin(),
            title.end()),
        sf::Style::Close,
        settings);
    //window.setFramerateLimit(60);

    //Генерируем текстуру фона
    sf::Texture backgroundTexture = createBackgroundTexture(
        windowWidth,
        windowHeight,
        sf::Color(65, 65, 65));
    sf::RectangleShape background;
    background.setSize(sf::Vector2f(windowWidth, windowHeight));
    background.setTexture(&backgroundTexture);

    //Подключаем файлы шрифтов
    sf::Font helveticaNeue_Light;
    sf::Font helveticaNeue_Regular;
    sf::Font helveticaNeue_Medium;
    sf::Font helveticaNeue_Bold;
    if (!(helveticaNeue_Light.loadFromFile("C:\\Users\\Admin\\Desktop\\MovieLibrary\\resources\\fonts\\HelveticaNeueCyr-Light.ttf") &&
        helveticaNeue_Regular.loadFromFile("C:\\Users\\Admin\\Desktop\\MovieLibrary\\resources\\fonts\\HelveticaNeueCyr-Roman.ttf") &&
        helveticaNeue_Medium.loadFromFile("C:\\Users\\Admin\\Desktop\\MovieLibrary\\resources\\fonts\\HelveticaNeueCyr-Medium.ttf") &&
        helveticaNeue_Bold.loadFromFile("C:\\Users\\Admin\\Desktop\\MovieLibrary\\resources\\fonts\\HelveticaNeueCyr-Bold.ttf"))) {
        throw res_file_not_loaded();
    }

    //Подключаем файлы текстур
    sf::Texture searchTexture;
    sf::Texture addButtonTexture;
    sf::Texture editButtonTexture;
    sf::Texture deleteButtonTexture;
   
    if (!(searchTexture.loadFromFile("C:\\Users\\Admin\\Desktop\\MovieLibrary\\resources\\textures\\textBox_texture.png")
        && addButtonTexture.loadFromFile("C:\\Users\\Admin\\Desktop\\MovieLibrary\\resources\\textures\\plus.png")
        && editButtonTexture.loadFromFile("C:\\Users\\Admin\\Desktop\\MovieLibrary\\resources\\textures\\edit.png")
        && deleteButtonTexture.loadFromFile("C:\\Users\\Admin\\Desktop\\MovieLibrary\\resources\\textures\\delete.png"))) {
        throw res_file_not_loaded();
    }

    //Загружаем курсоры мыши из системы
    sf::Cursor arrowCursor;
    sf::Cursor handCursor;
    sf::Cursor textCursor;
    arrowCursor.loadFromSystem(sf::Cursor::Arrow);
    handCursor.loadFromSystem(sf::Cursor::Hand);
    textCursor.loadFromSystem(sf::Cursor::Text);

    //Инициализируем элементы интерфейса
    TextBox searchBox(&searchTexture, 49 * UI_SCALE);
    searchBox.setPosition(sf::Vector2f(992 * UI_SCALE, 38 * UI_SCALE));
    searchBox.setFont(helveticaNeue_Medium);
    searchBox.setArrowCursor(&arrowCursor);
    searchBox.setTextCursor(&textCursor);

    TextButton searchType(&helveticaNeue_Bold, 49 * UI_SCALE);
    searchType.setPosition(sf::Vector2f(86 * UI_SCALE, 38 * UI_SCALE));
    searchType.setText(L"Фильмы");
    searchType.setArrowCursor(&arrowCursor);
    searchType.setHandCursor(&handCursor);

    Button addButton(&addButtonTexture, 24 * UI_SCALE);
    addButton.setPosition(sf::Vector2f(952 * UI_SCALE, 50.5f * UI_SCALE));

    Button editButton(&editButtonTexture, 24 * UI_SCALE);
    editButton.setPosition(sf::Vector2f(872 * UI_SCALE, 50.5f * UI_SCALE));

    Button deleteButton(&deleteButtonTexture, 24 * UI_SCALE);
    deleteButton.setPosition(sf::Vector2f(912 * UI_SCALE, 50.5f * UI_SCALE));

    

    DetailsRenderer details(&helveticaNeue_Bold, &helveticaNeue_Medium, &helveticaNeue_Regular, &helveticaNeue_Light, UI_SCALE);

    //Загружаем базу данных из файла
    List<Human>* database;
    database = deserializeJSON(loadFromFile("C:\\Users\\Admin\\Desktop\\MovieLibrary\\resources\\input.json"));

    horizontalMovieScroller searchResults(123 * UI_SCALE, 176 * UI_SCALE, 1300 * UI_SCALE, sf::Color(65, 65, 65));
    searchResults.setPosition(86 * UI_SCALE, 107 * UI_SCALE);
    searchResults.setGap(18 * UI_SCALE);
    for (int i = 0; i < database->getSize(); i++) {
        if (!database->operator[](i)->isDirector()) continue;
        for (int j = 0; j < database->operator[](i)->getMovies().getSize(); j++) {
            searchResults.addMovie(database->operator[](i)->getMovies().operator[](j));
        }
    }



    //Обновление кадра
    while (window.isOpen())
    {
        sf::Event event{};
        while (window.pollEvent(event))
        {
            switch (event.type) {
            case sf::Event::Closed:
                window.close();
                break;
            case sf::Event::MouseButtonPressed:
                if (searchResults.isMouseOver(window)) {
                    searchResults.setDragging(true, sf::Mouse::getPosition(window).x);
                }
                else if (searchBox.isMouseOver(window)) {
                    searchBox.setSelected(true);
                }
                else if (details.isMouseOverScroller(sf::Mouse::getPosition(window))) {
                    details.clickHandler(true, sf::Mouse::getPosition(window));
                }
                else if (addButton.isMouseOver(window)) {
                    auto newMovie = createNewMovie(UI_SCALE, &helveticaNeue_Medium);
                    if (newMovie) {
                        newMovie->setDirector(details.getLastRenderedHuman());
                        details.getLastRenderedHuman()->addMovie(newMovie);
                        details.render(newMovie);
                        searchResults.clear();
                        //TODO: Вынести в отдельную функцию?
                        for (int i = 0; i < database->getSize(); i++) {
                            if (!database->operator[](i)->isDirector()) continue;
                            for (int j = 0; j < database->operator[](i)->getMovies().getSize(); j++) {
                                searchResults.addMovie(database->operator[](i)->getMovies().operator[](j));
                            }
                        }
                    }

                }
                else if (editButton.isMouseOver(window)) {
                    editMovie(details.getLastRenderedMovie(), database, UI_SCALE, &helveticaNeue_Medium);
                    details.render(details.getLastRenderedMovie());
                    searchResults.clear();
                    //TODO: Вынести в отдельную функцию?
                    for (int i = 0; i < database->getSize(); i++) {
                        if (!database->operator[](i)->isDirector()) continue;
                        for (int j = 0; j < database->operator[](i)->getMovies().getSize(); j++) {
                            searchResults.addMovie(database->operator[](i)->getMovies().operator[](j));
                        }
                    }
                }
                else {
                    searchBox.setSelected(false);
                }
                break;
            case sf::Event::MouseButtonReleased:
                details.checkTapOnPlayButton(sf::Mouse::getPosition(window));
                if (searchResults.setDragging(false, sf::Mouse::getPosition(window).x) && searchResults.hasPoint(sf::Mouse::getPosition(window))) {
                    details.render(searchResults.tap(sf::Mouse::getPosition(window).x));
                }
                if (details.clickHandler(false, sf::Mouse::getPosition(window))) {
                    details.tap(sf::Mouse::getPosition(window));
                }
                break;
            case sf::Event::TextEntered:
                searchBox.typedOn(event);
                break;
            default:
                break;
            }
        }

        window.clear();
        window.draw(background);
        searchBox.drawTo(window);
        //window.draw(films);
        details.draw(window);
        searchType.onHover(window, searchType.isMouseOver(window));
        searchBox.onHover(window, searchBox.isMouseOver(window));
        searchType.drawTo(window);
        searchResults.onDrag(sf::Mouse::getPosition(window).x);
        details.clickHandler(sf::Mouse::getPosition(window));
        searchResults.draw(window);
        addButton.drawTo(window);
        editButton.drawTo(window);
        deleteButton.drawTo(window);
        window.display();
    }
    writeToFile("C:\\Users\\Admin\\Desktop\\MovieLibrary\\resources\\output.json", serializeJSON(database).dump(4));
    return 0;
}
