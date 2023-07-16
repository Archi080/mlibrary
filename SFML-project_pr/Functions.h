#include "SFML/Graphics.hpp"
#include "DataArrays.h"
#include "TextBox.h"
#include "Button.h"
#include <random>

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

sf::Texture createBackgroundTexture(int windowWidth, int windowHeight, sf::Color color){
    //Создаем текстуру фона
    sf::Texture backgroundTexture;
    backgroundTexture.create(windowWidth, windowHeight);
    //Массив пикселей
    sf::Uint8 * backgroundPixels = new sf::Uint8 [windowHeight * windowWidth * 4];
    //Раскладываем цвет на составляющие
    sf::Uint8 myColor[4] = {color.r, color.g, color.b, color.a};
    //Записываем цвет в каждый пиксель
    for(int pixel = 0; pixel < windowWidth * windowHeight * 4; pixel++){
        backgroundPixels[pixel] = myColor[pixel % 4];
    }
    //Обновляем текстуру новыми пикселями
    backgroundTexture.update(backgroundPixels);
    //Возвращаем текстуру
    return backgroundTexture;
}

std::string toString(sf::String query){
    std::string result;
    auto resultPre = query.toUtf8();
    for(auto sym : resultPre){
        result += sym;
    }
    return result;
}

int toInt(std::string number){
    int i = 0;
    for(auto sym : number){
        i = i * 10 + sym - 48;
    }
    return i;
}

Movie * newMovie(std::string title, int year, std::string syno, std::string country, int budget, int office, std::string poster){
    std::random_device rd;
    std::mt19937 gen(rd());
    auto result = new Movie(title, year, (int)gen());
    result->setSynopsis(syno);
    result->setCountries(country);
    result->setBudget(budget);
    result->setBoxOffice(office);
    result->setPoster("C:\\Users\\Admin\\Desktop\\MovieLibrary\\resources\\images\\" + poster);
    return result;
}
void editMovie(Movie * movie, List<Human> * database, float uiScale, sf::Font * font){
    //Настройки окна программы
    sf::String title = L"Editing a movie";
    const int windowHeight = (int)(629 * uiScale);
    const int windowWidth = (int)(541 * uiScale);
    sf::ContextSettings settings;
    settings.antialiasingLevel = 32;

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
    window.setFramerateLimit(60);

    //Генерируем текстуру фона
    sf::Texture backgroundTexture = createBackgroundTexture(
            windowWidth,
            windowHeight,
            sf::Color(65, 65, 65));
    sf::RectangleShape background;
    background.setSize(sf::Vector2f(windowWidth, windowHeight));
    background.setTexture(&backgroundTexture);

    sf::Texture synoTexture;
    sf::Texture longTexture;
    sf::Texture halfTexture;
    sf::Texture okTexture;
    sf::Texture cancelTexture;

    synoTexture.loadFromFile("C:\\Users\\Admin\\Desktop\\MovieLibrary\\resources\\textures/multiple.png");
    longTexture.loadFromFile("C:\\Users\\Admin\\Desktop\\MovieLibrary\\resources\\textures/full.png");
    halfTexture.loadFromFile("C:\\Users\\Admin\\Desktop\\MovieLibrary\\resources\\textures/half.png");
    okTexture.loadFromFile("C:\\Users\\Admin\\Desktop\\MovieLibrary\\resources\\textures/ok.png");
    cancelTexture.loadFromFile("C:\\Users\\Admin\\Desktop\\MovieLibrary\\resources\\textures/cancel.png");

    auto titleString = movie->getTitle() + ", " + toString(movie->getYear());
    sf::Text titleText(sf::String::fromUtf8(titleString.begin(), titleString.end()), *font, 44 * uiScale);
    titleText.setFillColor(sf::Color::White);
    titleText.setPosition(sf::Vector2f(25 * uiScale, 25 * uiScale));

    TextBox synoBox(&synoTexture, 140 * uiScale);
    synoBox.setPosition(sf::Vector2f(25 * uiScale, 94 * uiScale));
    synoBox.setFont(*font);
    synoBox.setHint(L"Synopsis");
    synoBox.setString(setMaxLength(movie->getSynopsis(), 80));

    TextBox countryBox(&longTexture, 49 * uiScale);
    countryBox.setPosition(sf::Vector2f(25 * uiScale, 259 * uiScale));
    countryBox.setFont(*font);
    countryBox.setHint(L"Country");
    countryBox.setString(movie->getCountries());

    TextBox budgetBox(&halfTexture, 49 * uiScale);
    budgetBox.setPosition(sf::Vector2f(25 * uiScale, 333 * uiScale));
    budgetBox.setFont(*font);
    budgetBox.setHint(L"Budget");
    budgetBox.setString(toString(movie->getBudgetUSD()));

    TextBox officeBox(&halfTexture, 49 * uiScale);
    officeBox.setPosition(sf::Vector2f(283 * uiScale, 333 * uiScale));
    officeBox.setFont(*font);
    officeBox.setHint(L"Box office");
    officeBox.setString(toString(movie->getBoxOfficeUSD()));

    TextBox posterBox(&longTexture, 49 * uiScale);
    posterBox.setPosition(sf::Vector2f(25 * uiScale, 407 * uiScale));
    posterBox.setFont(*font);
    posterBox.setHint(L"Poster");
    posterBox.setString(movie->getPosterPath());

    TextBox actorBox(&longTexture, 49 * uiScale);
    actorBox.setPosition(sf::Vector2f(25 * uiScale, 481 * uiScale));
    actorBox.setFont(*font);
    actorBox.setHint(L"Add an actor");

    Button okButton(&okTexture, 49 * uiScale);
    okButton.setPosition(sf::Vector2f(25 * uiScale, 555 * uiScale));

    Button cancelButton(&cancelTexture, 49 * uiScale);
    cancelButton.setPosition(sf::Vector2f(283 * uiScale, 555 * uiScale));

    TextBox * activeBox = nullptr;

    while (window.isOpen()) {

        sf::Event event;
        while (window.pollEvent(event)) {
            switch (event.type) {
                case sf::Event::Closed:
                    window.close();
                case sf::Event::MouseButtonPressed:
                    if (activeBox) {
                        activeBox->setSelected(false);
                        activeBox = nullptr;
                    }
                    if (synoBox.isMouseOver(window)) {
                        activeBox = &synoBox;
                        synoBox.setSelected(true);
                    } else if (countryBox.isMouseOver(window)) {
                        activeBox = &countryBox;
                        countryBox.setSelected(true);
                    } else if (budgetBox.isMouseOver(window)) {
                        activeBox = &budgetBox;
                        budgetBox.setSelected(true);
                    } else if (officeBox.isMouseOver(window)) {
                        activeBox = &officeBox;
                        officeBox.setSelected(true);
                    } else if (posterBox.isMouseOver(window)) {
                        activeBox = &posterBox;
                        posterBox.setSelected(true);
                    } else if(actorBox.isMouseOver(window)){
                        activeBox = &actorBox;
                        actorBox.setSelected(true);
                    } else if (okButton.isMouseOver(window)) {
                        movie->setSynopsis(toString(synoBox.getQuery()));
                        movie->setCountries(toString(countryBox.getQuery()));
                        movie->setBudget(toInt(toString(budgetBox.getQuery())));
                        movie->setBoxOffice(toInt(toString(officeBox.getQuery())));
                        movie->setPoster(toString(posterBox.getQuery()));
                        for(int i = 0; i < database->getSize(); i++){
                            if(database->operator[](i)->getFullName() == toString(actorBox.getQuery())){
                                movie->addActor(database->operator[](i));
                                database->operator[](i)->addMovie(movie);
                            }
                        }
                        window.close();
                    } else if (cancelButton.isMouseOver(window)) {
                        window.close();
                    }
                    break;
                case sf::Event::TextEntered:
                    if (activeBox) {
                        activeBox->typedOn(event);
                    }
                    break;
                default:
                    break;
            }
        }

        window.clear();
        window.draw(background);
        window.draw(titleText);
        synoBox.drawTo(window);
        countryBox.drawTo(window);
        budgetBox.drawTo(window);
        officeBox.drawTo(window);
        posterBox.drawTo(window);
        actorBox.drawTo(window);
        okButton.drawTo(window);
        cancelButton.drawTo(window);
        window.display();
    }
}

Movie * createNewMovie(float uiScale, sf::Font * font){
    //Настройки окна программы
    sf::String title = L"Add new film";
    const int windowHeight = (int)(560 * uiScale);
    const int windowWidth = (int)(541 * uiScale);
    sf::ContextSettings settings;
    settings.antialiasingLevel = 32;

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
    window.setFramerateLimit(60);

    //Генерируем текстуру фона
    sf::Texture backgroundTexture = createBackgroundTexture(
            windowWidth,
            windowHeight,
            sf::Color(65, 65, 65));
    sf::RectangleShape background;
    background.setSize(sf::Vector2f(windowWidth, windowHeight));
    background.setTexture(&backgroundTexture);

    sf::Texture titleTexture;
    sf::Texture yearTexture;
    sf::Texture synoTexture;
    sf::Texture longTexture;
    sf::Texture halfTexture;
    sf::Texture addTexture;
    sf::Texture cancelTexture;

    titleTexture.loadFromFile("C:\\Users\\Admin\\Desktop\\MovieLibrary\\resources\\textures/major.png");
    yearTexture.loadFromFile("C:\\Users\\Admin\\Desktop\\MovieLibrary\\resources\\textures/year.png");
    synoTexture.loadFromFile("C:\\Users\\Admin\\Desktop\\MovieLibrary\\resources\\textures/multiple.png");
    longTexture.loadFromFile("C:\\Users\\Admin\\Desktop\\MovieLibrary\\resources\\textures/full.png");
    halfTexture.loadFromFile("C:\\Users\\Admin\\Desktop\\MovieLibrary\\resources\\textures/half.png");
    addTexture.loadFromFile("C:\\Users\\Admin\\Desktop\\MovieLibrary\\resources\\textures/add.png");
    cancelTexture.loadFromFile("C:\\Users\\Admin\\Desktop\\MovieLibrary\\resources\\textures/cancel.png");

    TextBox titleBox(&titleTexture, 49 * uiScale);
    titleBox.setPosition(sf::Vector2f(25 * uiScale, 25 * uiScale));
    titleBox.setFont(*font);
    titleBox.setHint(L"Title");

    TextBox yearBox(&yearTexture, 49 * uiScale);
    yearBox.setPosition(sf::Vector2f(412 * uiScale, 25 * uiScale));
    yearBox.setFont(*font);
    yearBox.setHint(L"Year");

    TextBox synoBox(&synoTexture, 140 * uiScale);
    synoBox.setPosition(sf::Vector2f(25 * uiScale, 99 * uiScale));
    synoBox.setFont(*font);
    synoBox.setHint(L"Synopsis");

    TextBox countryBox(&longTexture, 49 * uiScale);
    countryBox.setPosition(sf::Vector2f(25 * uiScale, 264 * uiScale));
    countryBox.setFont(*font);
    countryBox.setHint(L"Country");

    TextBox budgetBox(&halfTexture, 49 * uiScale);
    budgetBox.setPosition(sf::Vector2f(25 * uiScale, 338 * uiScale));
    budgetBox.setFont(*font);
    budgetBox.setHint(L"Budget");

    TextBox officeBox(&halfTexture, 49 * uiScale);
    officeBox.setPosition(sf::Vector2f(283 * uiScale, 338 * uiScale));
    officeBox.setFont(*font);
    officeBox.setHint(L"Box office");

    TextBox posterBox(&longTexture, 49 * uiScale);
    posterBox.setPosition(sf::Vector2f(25 * uiScale, 412 * uiScale));
    posterBox.setFont(*font);
    posterBox.setHint(L"Poster");

    Button addButton(&addTexture, 49 * uiScale);
    addButton.setPosition(sf::Vector2f(25 * uiScale, 486 * uiScale));

    Button cancelButton(&cancelTexture, 49 * uiScale);
    cancelButton.setPosition(sf::Vector2f(283 * uiScale, 486 * uiScale));

    TextBox * activeBox = nullptr;
    Movie * result = nullptr;

    while (window.isOpen()) {

        sf::Event event;
        while (window.pollEvent(event)) {
            switch (event.type) {
                case sf::Event::Closed:
                    window.close();
                case sf::Event::MouseButtonPressed:
                    if (activeBox) {
                        activeBox->setSelected(false);
                        activeBox = nullptr;
                    }
                    if (titleBox.isMouseOver(window)) {
                        activeBox = &titleBox;
                        titleBox.setSelected(true);
                    } else if (yearBox.isMouseOver(window)) {
                        activeBox = &yearBox;
                        yearBox.setSelected(true);
                    } else if (synoBox.isMouseOver(window)) {
                        activeBox = &synoBox;
                        synoBox.setSelected(true);
                    } else if (countryBox.isMouseOver(window)) {
                        activeBox = &countryBox;
                        countryBox.setSelected(true);
                    } else if (budgetBox.isMouseOver(window)) {
                        activeBox = &budgetBox;
                        budgetBox.setSelected(true);
                    } else if (officeBox.isMouseOver(window)) {
                        activeBox = &officeBox;
                        officeBox.setSelected(true);
                    } else if (posterBox.isMouseOver(window)) {
                        activeBox = &posterBox;
                        posterBox.setSelected(true);
                    } else if (addButton.isMouseOver(window)) {
                        result = newMovie(toString(titleBox.getQuery()),
                                          toInt(toString(yearBox.getQuery())),
                                          toString(synoBox.getQuery()),
                                          toString(countryBox.getQuery()),
                                          toInt(toString(budgetBox.getQuery())),
                                          toInt(toString(officeBox.getQuery())),
                                          toString(posterBox.getQuery()));
                        window.close();
                    } else if (cancelButton.isMouseOver(window)) {
                        window.close();
                    }
                    break;
                case sf::Event::TextEntered:
                    if (activeBox) {
                        activeBox->typedOn(event);
                    }
                    break;
                default:
                    break;
            }
        }

        window.clear();
        window.draw(background);
        titleBox.drawTo(window);
        yearBox.drawTo(window);
        synoBox.drawTo(window);
        countryBox.drawTo(window);
        budgetBox.drawTo(window);
        officeBox.drawTo(window);
        posterBox.drawTo(window);
        addButton.drawTo(window);
        cancelButton.drawTo(window);
        window.display();
    }
    return result;
}

