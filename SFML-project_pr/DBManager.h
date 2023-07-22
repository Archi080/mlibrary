#pragma once
#include <stdlib.h>
#include <iostream>
#include <string>
#include <memory>
#include "mysql_connection.h"
#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/prepared_statement.h>
#include "cppconn/metadata.h"
#include <map>
#include "mysql_driver.h"
#include "cppconn/statement.h"

using namespace std;



class DBManager {
public:
	DBManager() {

		const string server = "tcp://127.0.0.1:3306";
		const string username = "root";
		const string password = "Root17!_";

		sql::Driver* driver;
		//sql::Statement* stmt;

		try
		{
			driver = get_driver_instance();
			con = driver->connect(server, username, password);
		}
		catch (sql::SQLException e)
		{
			cout << "Could not connect to server. Error message: " << e.what() << endl;
			system("pause");
			exit(1);
		}
		auto dbcon_meta = con->getMetaData();
		
		auto rs = dbcon_meta->getSchemas();
		int row = 1;

		try {
			con->setSchema("quickstartdb");
		}
		catch (sql::SQLException e)
		{
			cout << e.what() << endl;
			system("pause");
			exit(1);
		}
		//while (rs->next()) {
		//	cout << "\t" << row << ". " << rs->getString("TABLE_SCHEM") << endl;
		//	++row;
		//} // while
	};
	void GetRow() {
		
		auto stmt = con->createStatement();
		auto result = stmt->executeQuery("SELECT * FROM actors");
		while (result->next()) {
			cout << "\t" <<  result->getInt("id") << endl;
		} // while
	}
	List<Human>* GetPeople() {
		auto stmt = con->createStatement();
		auto result = stmt->executeQuery("SELECT * FROM people");
		List<Human>* people = new List<Human>;
		while (result->next()) {
			auto NewHuman = new Human(result->getString("name").asStdString(),
				result->getString("surname").asStdString(),
				result->getInt("id"));

			NewHuman->setBio(result->getString("bio").asStdString());
			NewHuman->setPortrait(result->getString("portrait").asStdString());
			people->add(NewHuman);
		}
		return people;
	}
	void GetMovies(List<Human>* people) {
		auto stmt = con->createStatement();
		auto result = stmt->executeQuery("SELECT * FROM movies");
		List<Movie> movies;
		while (result->next()) {
			auto NewMovie = new Movie(result->getString("title").asStdString(),
				result->getInt("year"),
				result->getInt("id"));
			NewMovie->setSynopsis(result->getString("synopsis").asStdString());
			NewMovie->setCountries(result->getString("countries").asStdString());
			NewMovie->setBudget(result->getInt64("budgetUSD"));
			NewMovie->setBoxOffice(result->getInt64("boxOfficeUSD"));
			NewMovie->setRate(result->getDouble("rate"));
			NewMovie->setFilmPath(result->getString("filmPath").asStdString());
			NewMovie->setPoster(result->getString("poster").asStdString());
			auto directorId = result->getInt("director_id");
			auto directorLambda{ [=](Human *human) -> bool {return (human->getId() == directorId); }};
			auto director = people->firstWhere(directorLambda);
			director->addMovie(NewMovie);
			director->setDirector(true);
			NewMovie->setDirector(director);
			auto actorsResult = stmt->executeQuery("SELECT * FROM movies_actors WHERE movie_id = " + to_string(NewMovie->getId()));
			while (actorsResult->next()) {
				auto actorId = actorsResult->getInt("actor_id");
				auto actorLambda{ [=](Human* human) -> bool {return (human->getId() == actorId); } };
				auto actor = people->firstWhere(actorLambda);
				actor->addMovie(NewMovie);
				NewMovie->addActor(actor);
			}

			movies.add(NewMovie);
			/*NewHuman->setBio(result->getString("bio").asStdString());
			NewHuman->setPortrait(result->getString("portrait").asStdString());
			people->add(NewHuman);*/
		}
	}
	/*void ConnectActors(List<Human>* people) {
		auto stmt = con->createStatement();
		auto result = stmt->executeQuery("SELECT * FROM movies_actors");

	}*/
private:
	sql::Connection* con;
};