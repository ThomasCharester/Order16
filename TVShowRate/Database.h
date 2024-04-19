#pragma once
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

using namespace std;

class UserInterface;

class Database {
private:
	class User
	{
	private:
		string login;
		string password;
		bool isAdmin;

	public:
		User() = default;
		User(string login, string password, bool isAdmin) {
			this->login = login;
			this->password = password;
			this->isAdmin = isAdmin;
		}

		const bool checkLogin(string login) const { return this->login == login; }
		const bool checkPassword(string password) const { return this->password == password; }
		const bool checkAdmin() const { return isAdmin; };

		string encodePassword() {
			string encoded = password;

			char key = password.size() % 10;

			for (int i = 0; i < password.size(); i++)
				encoded[i] ^= key;

			return encoded;
		}
		void addToFile()
		{
			fstream file("users.txt", ios::app);
			file << login << ' ' << encodePassword() << ' ' << isAdmin << '\n';
			file.close();
		}
		void showInfo() const {
			cout << "\nЛогин: " << login;
			cout << "\tПрава администратора: " << isAdmin;
		}
	};

	struct TVShow {
		string name;

		int rate;
		int spectators;
		double averageWatchTime;

		static int maxSpectators;
		static double maxAverageWatchTime;

		bool operator<(const TVShow& tvShow) {
			return rate < tvShow.rate;
		}
		TVShow() = default;
		TVShow(string name, int spectators, double averageWatchTime, int rate = 0) {
			this->name = name;
			this->spectators = spectators;
			this->averageWatchTime = averageWatchTime;
			this->rate = rate;
			balanceMaxes();
		}
		void balanceRate() {
			rate = (int)((averageWatchTime * spectators) * 100 / (maxSpectators * maxAverageWatchTime));
		}
		void balanceMaxes() const{
			if (spectators > maxSpectators) maxSpectators = spectators;
			if (averageWatchTime > maxAverageWatchTime) maxAverageWatchTime = averageWatchTime;
		}
		void addToFile() const
		{
			fstream file("tvShows.txt", ios::app);
			file << name << ' ' << spectators << ' ' << averageWatchTime << ' ' << rate << '\n';

			file.close();
		}
	};

	UserInterface* ui;
	vector<TVShow> tvShows;
	vector<User> users;

	User* currentUser;

	void showUsers();
public:

	Database();
	~Database();

	void sortByName() {
		sort(tvShows.begin(), tvShows.end(), [](const TVShow& tv1, const TVShow& tv2) {return tv1.name[0] < tv2.name[0]; });
	}
	void sortByRate() {
		sort(tvShows.begin(), tvShows.end());
	}

	void encodeOrDecodePassword(string& password)
	{
		char key = password.size() % 10;

		for (int i = 0; i < password.size(); i++)
			password[i] ^= key;
	}

	void showTVShows(bool showID = false) {
		for (int i = 0; i < tvShows.size(); i++) {
			cout << '\n';
			if (showID) cout << "\nНомер: " << i;
			cout << "\nНазвание: " << tvShows.at(i).name;
			cout << "\nРейтинг: " << tvShows.at(i).rate;
			cout << "\nКоличество зрителей: " << tvShows.at(i).spectators;
			cout << "\nСреднее количество просмотров: " << tvShows.at(i).averageWatchTime;
		}
		cout << '\n';
	}
	void addTVShow();
	void removeTVShow();
	void editTVShow(int id);
	void rebalanceAllMaxes() {
		if (tvShows.empty()) return;
		TVShow::maxAverageWatchTime = 0;
		TVShow::maxSpectators = 0;
		for (TVShow tv : tvShows) {
			tv.balanceMaxes();
		}
		rebalanceAllRate();
	}
	void rebalanceAllRate() {
		if (tvShows.empty()) return;
		for (TVShow tv : tvShows) {
			tv.balanceRate();
		}
	}

	void addUser();
	void removeUser();

	void writeTVShowsToFile() {
		fstream file("tvShows.txt", ios::out);
		file << tvShows.size() << '\n';
		file.close();

		for (int i = 0; i < tvShows.size(); i++)
			tvShows[i].addToFile();
	}
	void readTVShowsFromFile()
	{
		tvShows.clear();

		fstream file("tvShows.txt", ios::in);

		if (file.eof()) {
			file.close();
			return; }

		string temp;

		getline(file, temp, '\n');
		int size = stoi(temp);

		for (int i = 0; i < size; i++)
		{
			string name, rate, spectators, averageWatchTime;

			getline(file, name, ' ');
			getline(file, spectators, ' ');
			getline(file, averageWatchTime, ' ');
			getline(file, rate, '\n');

			tvShows.emplace_back(TVShow(name, stoi(spectators), stod(averageWatchTime), stoi(rate)));

			tvShows.at(tvShows.size() - 1).balanceRate();
		}
		file.close();
	}

	void writeUsersToFile()
	{
		fstream file("users.txt", ios::out); ///////// Было ios::app, но теперь стирает содержимое, тк уже читаются все записи в базу программы
		file << users.size() << '\n';
		file.close();

		for (int i = 0; i < users.size(); i++)
			users[i].addToFile();
	}
	void readUsersFromFile()
	{
		users.clear();
		fstream file("users.txt", ios::in);

		if (file.eof())
		{
			cout << "\nУчётных записей нет, создайте новую";
			addUser();
		}

		string temp;

		getline(file, temp, '\n');
		int size = stoi(temp);

		for (int i = 0; i < size; i++)
		{
			string login, password;
			getline(file, login, ' ');
			getline(file, password, ' ');

			encodeOrDecodePassword(password);
			getline(file, temp, '\n');
			bool isAdmin = stoi(temp);

			users.emplace_back(login, password, isAdmin);
		}
		file.close();
	}

	void login();
};

class UserInterface {
private:
	Database* db;
public:
	UserInterface(Database* db) {
		this->db = db;
	}
	void userMenu()
	{
		int choice;

		do
		{
			system("cls");

			choice = inputRange(0, 1, "Выберите действие:\n\t1 - просмотреть ТВ-шоу\n\t0 - выход: ");

			switch (choice)
			{
			case 1:
				showTVShows();
				break;
			default:
				pressAnyButton();
				break;
			}
		} while (choice != 0);
	}
	void adminMenu()
	{
		int choice;
		do
		{
			system("cls");

			choice = inputRange(0, 7, "Выберите действие:\n\t1 - просмотреть ТВ-шоу\n\t2 - добавить ТВ-шоу\n\t3 - удалить ТВ-шоу\n\t4 - редактировать информацию ТВ-шоу\n\t5 - добавить пользователя\n\t6 - удалить пользователя\n\t0 - выход: ");

			switch (choice)
			{
			case 1:
				showTVShows();
				break;
			case 2:
				db->addTVShow();
				break;
			case 3:
				db->removeTVShow();
				break;
			case 4:
				editTVShow();
				break;
			case 5:
				db->addUser();
				break;
			case 6:
				db->removeUser();
				break;
			default:
				pressAnyButton();
				break;
			}
		} while (choice != 0);
	}
	void pressAnyButton()
	{
		system("pause");
		system("cls");
	}

	bool isInteger(string input)
	{
		try
		{
			int temp = stoi(input);
		}
		catch (invalid_argument)
		{
			return false;
		}

		return true;
	}
	bool isDouble(string input)
	{
		try
		{
			double temp = stod(input);
		}
		catch (invalid_argument)
		{
			return false;
		}

		return true;
	}

	void startUI() {
		setlocale(LC_ALL, "Russian");

		db->readUsersFromFile();

		int choice = 0;

		do
		{
			choice = 0;

			string buf;

			cout << "Выберите действие:\t1 - войти в систему\tЛюбое другое - выход: ";
			getline(cin, buf);

			if (!isInteger(buf))
				continue;

			choice = stoi(buf);

			if (choice == 1)
				db->login();
		} while (choice == 1);

		cout << "Хорошего дня!";
	}

	void inputInt(int& data, string text) {
		string temp;
		cout << text;
		while (true) {
			getline(cin, temp);
			if (isInteger(temp)) {
				data = stoi(temp);
				break;
			}
		}
	}

	void showTVShows() {
		int choice;
		do {
			do
			{
				system("cls");

				choice = inputRange(0, 2, "Выберите порядок:\n\t1 - по названию\n\t2 - по рейтингу\n\t0 - выход: ");

				switch (choice)
				{
				case 1:
					db->sortByName();
					break;
				case 2:
					db->sortByRate();
					break;
				default:
					break;
				}
				if (choice) db->showTVShows();
				pressAnyButton();
			} while (choice != 0);

		} while (choice != 0);
	}
	void editTVShow() {
		system("cls");
		db->showTVShows(true);
		int choice;
		inputInt(choice, "\nВведите номер шоу, чтобы редактировать: ");
		db->editTVShow(choice);
	}

	int inputRange(int min, int max, string text = "") {
		int choice = min - 1;
		cout << text;
		do
		{
			string temp;

			getline(cin, temp);
			if (!isInteger(temp))
				cout << "Введите число: ";
			else
			{
				choice = stoi(temp);
				if ((choice < min || (choice > max && max != min)))
					cout << "Введите число от " << min << " до " << (max != min) ? to_string(max) : "infinite";
			}
		} while ((choice < min || (choice > max && max != min)));

		return choice;
	}
	double inputRange(double min, double max, string text = "") {
		double choice = min - 1.0;
		cout << text;
		do
		{
			string temp;

			getline(cin, temp);
			if (!isDouble(temp))
				cout << "Введите число: ";
			else
			{
				choice = stod(temp);
				if ((choice < min || (choice > max && max != min)))
					cout << "Введите число от " << min << " до " << (max != min) ? to_string(max) : "infinite";
			}
		} while ((choice < min || (choice > max && max != min)));

		return choice;
	}
};
Database::Database() {
	ui = new UserInterface(this);
	currentUser = nullptr;
	readTVShowsFromFile();
	ui->startUI();
}
Database::~Database()
{
	writeTVShowsToFile();
	writeUsersToFile();
	delete ui;
}


void Database::login()
{
	bool isLoginRight = false, isPasswordRight = false;

	string temp;

	int i;

	do
	{
		cout << "Введите логин: ";
		getline(cin, temp);

		for (i = 0; i < users.size(); i++)
		{
			if (users.at(i).checkLogin(temp))
			{
				isLoginRight = true;
				break;
			}
		}

		if (!isLoginRight)
			cout << "Пользователя с таким логином не существует!\n";
	} while (!isLoginRight);

	do
	{
		cout << "Введите пароль: ";
		getline(cin, temp);

		if (users.at(i).checkPassword(temp))
		{
			isPasswordRight = true;
			break;
		}

		if (!isPasswordRight)
			cout << "Неправильный пароль!\n";
	} while (!isPasswordRight);

	currentUser = &(users.at(i));

	cout << "Вы успешно вошли в аккаунт!\n";
	ui->pressAnyButton();

	if (users.at(i).checkAdmin())
		ui->adminMenu();
	else
		ui->userMenu();
}

void Database::addTVShow() {
	system("cls");

	cout << "\n\nВведите название: ";
	string name;
	getline(cin, name);

	int spectators = ui->inputRange(0, 0, "Введите число зрителей");

	double averageWatchTime = ui->inputRange(0.0, 0.0, "Введите число зрителей");

	tvShows.push_back(TVShow(name, spectators, averageWatchTime));
}
void Database::removeTVShow() {
	system("cls");
	showTVShows(true);

	int choice;
	ui->inputInt(choice, "\nВведите номер шоу, чтобы удалить: ");

	int confirm = ui->inputRange(0, 1, "\nВы уверены? 1 - Да/0 - Нет");
	if (!confirm) return;

	tvShows.erase(tvShows.begin() + choice);
	rebalanceAllMaxes();
}
void Database::editTVShow(int id)
{
	int choice;
	do
	{
		system("cls");

		choice = ui->inputRange(0, 2, "Выберите параметр:\n\t1 - название\n\t2 - количество зрителей\n\t3 - среднее время просмотра\n\t0 - выход: ");

		switch (choice)
		{
		case 1:
		{
			string name;
			cout << "\nВведите название: ";
			getline(cin, name);
			tvShows.at(id).name = name;
		}
		break;
		case 2:
		{
			int spectators = ui->inputRange(0, 0, "Введите число зрителей");
			tvShows.at(id).spectators = spectators;
		}
		break;
		case 3:
		{
			double averageWatchTime = ui->inputRange(0.0, 0.0, "Введите число зрителей");
			tvShows.at(id).averageWatchTime = averageWatchTime;
		}
		break;
		default:
			break;
		}
	} while (choice != 0);
	rebalanceAllMaxes();
}

void Database::showUsers()
{
	for (int i = 0; i < users.size(); i++) {
		cout << "\nНомер: " << i;
		users.at(i).showInfo();
		cout << '\n';
	}
}
void Database::addUser()
{
	system("cls");
	cout << "\n\nВведите логин: ";
	string login;
	getline(cin, login);
	string password;
	cout << "\nВведите пароль: ";
	getline(cin, password);
	int admin = ui->inputRange(0, 1, "\nВы уверены? 1 - Да/0 - Нет");

	users.push_back(User(login, password, admin));
	writeUsersToFile();
}
void Database::removeUser()
{
	system("cls");
	showUsers();
	int choice;
	ui->inputInt(choice, "\nВведите номер учётной записи, чтобы удалить: ");

	int confirm = ui->inputRange(0, 1, "\nВы уверены? 1 - Да/0 - Нет");
	if (!confirm) return;

	users.erase(users.begin() + choice);
}

int Database::TVShow::maxSpectators = 0;
double Database::TVShow::maxAverageWatchTime = 0;
