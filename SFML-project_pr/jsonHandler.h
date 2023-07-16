
#include <fstream>
#include <iostream>
#include "json.hpp"
using json = nlohmann::json;
#include "DataArrays.h"
#include "DataTypes.h"


//Десериализация JSON-строки
List<Human> * deserializeJSON(const std::string& query){
    //Расшифровываем строку как JSON-объект
    json newObject = json::parse(query);
    //Создаем переменную для возвращения
    auto result = new List<Human>;
    //Создаем списки людей и фильмов
    json moviesList = newObject["movies"];
    json peopleList = newObject["people"];
    //Для каждого человека из списка...
    for(auto person : peopleList){
        //Создаём новый экземпляр класса Human
        auto newPerson = new Human(person["name"], person["surname"], (int)person["id"]);
        newPerson->setBio(person["bio"].is_null() ? "" : person["bio"]);
        newPerson->setPortrait(person["portrait"].is_null() ? "" : person["portrait"]);
        //Добавляем новый объект в список для возвращения
        result->add(newPerson);
    }
    //Для каждого фильма из списка...
    for (auto movie : moviesList){
        //Создаем новый экземпляр класса Movie
        auto newMovie = new Movie(movie["title"], (int)movie["year"], (int)movie["id"]);
        newMovie->setSynopsis(movie["synopsis"].is_null() ? "" : movie["synopsis"]);
        newMovie->setBudget(movie["budgetUSD"].is_null() ? 0 : (int)movie["budgetUSD"]);
        newMovie->setBoxOffice(movie["boxOfficeUSD"].is_null() ? 0 : (int)movie["boxOfficeUSD"]);
        newMovie->setCountries(movie["countries"].is_null() ? "" : movie["countries"]);
        newMovie->setRate(movie["rate"].is_null() ? 0.0f : (float)movie["rate"]);
        newMovie->setPoster(movie["poster"].is_null() ? "" : movie["poster"]);
        newMovie->setFilmPath(movie["filmPath"].is_null() ? "" : movie["filmPath"]);
        //Находим режиссера среди недавно созданных персон
        int director = (int)movie["director"];
        for (int i = 0; i < result->getSize(); i++){
            //Если идентификатор человека в списке совпал
            //с идентификатором режиссера фильма
            if(result->operator[](i)->getId() == director){
                //Зачисляем этого человека в список режиссеров
                result->operator[](i)->setDirector(true);
                //Добавляем фильм в его список
                result->operator[](i)->addMovie(newMovie);
                //Устанавливаем у данного фильма режиссера
                newMovie->setDirector(result->operator[](i));
                //Выходим из цикла пораньше
                break;
            }
        }
        //Получаем список актеров фильма
        json actors = movie["actors"];
        //Получаем идентификатор каждого актера
        for(int actor : actors){
            //Ищем человека с таким идентификатором в списке
            for (int i = 0; i < result->getSize(); i++){
                //Если есть совпадение...
                if(actor == result->operator[](i)->getId()){
                    //Добавляем актера в фильм
                    newMovie->addActor(result->operator[](i));
                    //Добавляем этот фильм в список актера
                    result->operator[](i)->addMovie(newMovie);
                    break;
                }
            }
        }
    }
    //Возвращаем результат
    return result;
}

//Сериализация данных
json serializeJSON(List<Human> * people){
    //Создаем объект JSON
    json database;
    //Создаем JSON-массивы
    json moviesList = json::array();
    json peopleList = json::array();
    //Для каждого человека из списка...
    for(int i = 0; i < people->getSize(); i++){
        //Вывод в консоль имени человека
        std::cout << i + 1 << ". " << people->operator[](i)->getFullName() << std::endl;
        //Если человек не режиссёр,
        if(!people->operator[](i)->isDirector()){
            //пока пропускаем его
            continue;
        }
        //Создаем JSON-объект
        json director;
        //Заполняем поля объекта
        director["id"] = people->operator[](i)->getId();
        director["name"] = people->operator[](i)->getName();
        director["surname"] = people->operator[](i)->getSurname();
        director["bio"] = people->operator[](i)->getBio();
        director["portrait"] = people->operator[](i)->getPortraitPath();
        //Создаем JSON-массив фильмов
        json directorsMovies = json::array();
        //Получаем список фильмов человека
        auto movies = people->operator[](i)->getMovies();
        //Для каждого фильма...
        for(int j = 0; j < movies.getSize(); j++){
            //Выводим в консоль название фильма и год выпуска
            std::cout << "\t" << j + 1 << ". " << movies[j]->getTitle() << ", " << movies[j]->getYear() << std::endl;
            //Создаем JSON-объект фильма
            json movie;
            //Добавляем фильм в список текущего человека
            directorsMovies.push_back(movies[j]->getId());
            //Заполняем поля объекта
            movie["director"] = people->operator[](i)->getId();
            movie["id"] = movies[j]->getId();
            movie["title"] = movies[j]->getTitle();
            movie["year"] = movies[j]->getYear();
            movie["synopsis"] = movies[j]->getSynopsis();
            movie["countries"] = movies[j]->getCountries();
            movie["budgetUSD"] = movies[j]->getBudgetUSD();
            movie["boxOfficeUSD"] = movies[j]->getBoxOfficeUSD();
            movie["rate"] = movies[j]->getRate();
            movie["poster"] = movies[j]->getPosterPath();
            movie["filmPath"] = movies[j]->getFilmPath();

            //Получаем список актеров фильма
            auto actors = movies[j]->getActors();
            //Так как для хранения актеров используется
            //кольцевой односвязный список, я не могу узнать
            //количество его элементов. Поэтому я извлекаю
            //из него элементы пока не вылетит исключение
            try{
                while(true){
                    //Извлекаем одного актера из списка
                    auto newActor = actors.pop();
                    //Добавляем идентификатор актера в фильм
                    movie["actors"].push_back(newActor->getId());
                    //Проверяем, добавляли ли мы уже этого человека?
                    bool exist = false;
                    //Проверяем всех людей из списка
                    if(director["id"] == newActor->getId()){
                        exist = true;
                        break;
                    } else {
                        for(auto human : peopleList){
                            //Если находим совпадение...
                            if(newActor->getId() == human["id"]){
                                //Добавляем в список этого человека идентификатор фильма
                                human["movies"].push_back(movies[j]->getId());
                                exist = true;
                                break;
                            }
                        }
                    }
                    //Если человек не существует
                    if(!exist){
                        //Создаем JSON-объект актера
                        json actor;
                        //Заполняем поля объекта
                        actor["id"] = newActor->getId();
                        actor["name"] = newActor->getName();
                        actor["surname"] = newActor->getSurname();
                        actor["bio"] = newActor->getBio();
                        actor["portrait"] = newActor->getPortraitPath();
                        actor["movies"].push_back(movies[j]->getId());
                        //Добавляем нового актера в список людей
                        peopleList.push_back(actor);
                    }
                }
            }
            //Отлавливаем исключение
            catch (bad_addr_exception e){}
            //Добавляем созданный фильма в список фильмов
            moviesList.push_back(movie);
        }
        //Присваиваем режиссерам их фильмы
        director["movies"] = directorsMovies;
        //Добавляем режиссера в список людей
        peopleList.push_back(director);
    }
    //Добавляем оба списка в один JSON-объект
    database["people"] = peopleList;
    database["movies"] = moviesList;
    //Возвращаем объект
    return database;
}

//Чтение файла
std::string loadFromFile(std::string path){
    //Открываем входной поток
    std::ifstream file(path);
    //Создаем строковую переменную
    std::string query;
    //До конца файла считываем
    //строки и добавляем их
    //в query
//    while(!file.eof()){
//        auto * newLine = new char;
//        file.getline(newLine, 1024);
//        query += newLine;
//    }

    std::string temp;
    while(getline(file, temp)){
        query += temp;
    }
    //Закрываем поток
    file.close();
    //Возвращаем результат
    return query;
}

//Запись в файл
void writeToFile(std::string path, std::string content){
    //Открываем выходной поток
    std::ofstream file(path);
    //Очищаем файл
    file.clear();
    //Передаем контент в поток
    file << content;
    //Закрываем поток
    file.close();
}

