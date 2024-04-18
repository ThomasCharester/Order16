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

		const string getLogin() { return login; }
	};

	struct TVShow {
		string name;
		int rate;
		string channel;
		string jenre;
		vector<string> usersRate;
		bool operator<(const TVShow& tvShow) {
			return rate < tvShow.rate;
		}
		TVShow() = default;
		TVShow(string name, int rate, string channel, string jenre) {
			this->name = name;
			this->rate = rate;
			this->channel = channel;
			this->jenre = jenre;
		}
		bool isUserRate(string login) const{
			for (string str : usersRate)
				if (str == login) return true;
			return false;
		}
	};
	UserInterface* ui;
	vector<TVShow> tvShows;
	vector<User> users;

	User* currentUser;

	void showUsers();
public:
	const string getUserLogin() const{
		return currentUser->getLogin();
	}
	Database();
	~Database();
	void sortByName() {
		sort(tvShows.begin(), tvShows.end(), [](const TVShow& tv1, const TVShow& tv2) {return tv1.name[0] < tv2.name[0]; });
	}
	void sortByRate() {
		sort(tvShows.begin(), tvShows.end());
	}
	void sortByChannel() {
		sort(tvShows.begin(), tvShows.end(), [](const TVShow& tv1, const TVShow& tv2) {return tv1.channel[0] < tv2.channel[0]; });
	}
	void sortByJenre() {
		sort(tvShows.begin(), tvShows.end(), [](const TVShow& tv1, const TVShow& tv2) {return tv1.jenre[0] < tv2.jenre[0]; });
	}
	void setRate();
	void filterByJenre();
	void filterByChannel();
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
			cout << "\nКанал: " << tvShows.at(i).channel;
			cout << "\nЖанр: " << tvShows.at(i).jenre;
			cout << "\nРейтинг: " << tvShows.at(i).rate;
		}
	}
	void addTVShow();
	void removeTVShow();
	void editTVShow(int id);

	void addUser();
	void removeUser();
	void addUsersToFile()
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

			choice = inputRange(0, 3, "Выберите действие:\n\t1 - просмотреть ТВ-шоу\n\t2 - поиск по фильтру\n\t3 - поставить оценку\n\t0 - выход: ");

			switch (choice)
			{
			case 1:
				showTVShows();
				break;
			case 2:
				filterTVShows();
				break;
			case 3:
				cout << "Пока нет\n";
				pressAnyButton();
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

			choice = inputRange(0, 7, "Выберите действие:\n\t1 - просмотреть ТВ-шоу\n\t2 - добавить ТВ-шоу\n\t3 - удалить ТВ-шоу\n\t4 - редактировать информацию ТВ-шоу\n\t5 - добавить пользователя\n\t6 - удалить пользователя\n\t7 - поиск по фильтру\n\t0 - выход: ");

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
				cout << "Пока нет\n";
				pressAnyButton();
				break;
			case 5:
				db->addUser();
				break;
			case 6:
				db->removeUser();
				break;
			case 7:
				filterTVShows();
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

				choice = inputRange(0, 4, "Выберите порядок:\n\t1 - по названию\n\t2 - по жанру\n\t3 - по рейтингу\n\t4 - по каналу\n\t0 - выход: ");

				switch (choice)
				{
				case 1:
					db->sortByName();
					break;
				case 2:
					db->sortByJenre();
					break;
				case 3:
					db->sortByRate();
					break;
				case 4:
					db->sortByChannel();
					break;
				default:
					break;
				}
				db->showTVShows();
				pressAnyButton();
			} while (choice != 0);

		} while (choice != 0);
	}
	void filterTVShows() {
		int choice;
		do {
			do
			{
				system("cls");

				choice = inputRange(0, 2, "Выберите фильтр:\n\t1 - по каналу\n\t2 - по жанру\n\t0 - выход: ");

				switch (choice)
				{
				case 1:
					db->filterByChannel();
					break;
				case 2:
					db->filterByJenre();
					break;
				default:
					break;
				}
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
				if ((choice < min || choice > max))
					cout << "Введите число от " << min << " до " << max;
			}
		} while ((choice < min || choice > max));

		return choice;
	}
};
Database::Database() {
	ui = new UserInterface(this);
	currentUser = nullptr;
	ui->startUI();
}
Database::~Database()
{
	addUsersToFile();
	delete ui;
}

void Database::setRate()
{
	system("cls");
	showTVShows(true);

	
	int choice = ui->inputRange(0,tvShows.size(), "\nВведите номер шоу, чтобы оценить: ");

	if (!tvShows.at(choice).isUserRate(getUserLogin())) {
		
	}
}

void Database::filterByJenre()
{
	system("cls");
	cout << "\n\nВведите жанр: ";
	string jenre;
	getline(cin, jenre);
	bool jenreExist = false;
	for (int i = 0; i < tvShows.size(); i++) {
		if (tvShows.at(i).jenre == jenre) {
			cout << '\n';
			cout << "\nНазвание: " << tvShows.at(i).name;
			cout << "\nКанал: " << tvShows.at(i).channel;
			cout << "\nЖанр: " << tvShows.at(i).jenre;
			cout << "\nРейтинг: " << tvShows.at(i).rate;
			jenreExist = true;
		}
	}
	if (!jenreExist) cout << "\nПо запросу ничего не нашлось\n";
}

void Database::filterByChannel()
{
	system("cls");
	cout << "\n\nВведите канал: ";
	string channel;
	getline(cin, channel);
	bool channelExist = false;
	for (int i = 0; i < tvShows.size(); i++) {
		if (tvShows.at(i).channel == channel) {
			cout << '\n';
			cout << "\nНазвание: " << tvShows.at(i).name;
			cout << "\nКанал: " << tvShows.at(i).channel;
			cout << "\nЖанр: " << tvShows.at(i).jenre;
			cout << "\nРейтинг: " << tvShows.at(i).rate;
			channelExist = true;
		}
	}
	if (!channelExist) cout << "\nПо запросу ничего не нашлось\n";
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

	int rate = ui->inputRange(0, 100, "Введите число от 0, до 100");

	string channel;
	cout << "\nВведите название канала: ";
	getline(cin, channel);

	string jenre;
	cout << "\nВведите жанр: ";
	getline(cin, jenre);

	tvShows.push_back(TVShow(name, rate, channel, jenre));
}
void Database::removeTVShow() {
	system("cls");
	showTVShows(true);

	int choice;
	ui->inputInt(choice, "\nВведите номер шоу, чтобы удалить: ");

	int confirm = ui->inputRange(0, 1, "\nВы уверены? 1 - Да/0 - Нет");
	if (!confirm) return;

	tvShows.erase(tvShows.begin() + choice);
}
void Database::editTVShow(int id)
{
	int choice;
	do {
		do
		{
			system("cls");

			choice = ui->inputRange(0, 4, "Выберите параметр:\n\t1 - название\n\t2 - жанр\n\t3 - канал\n\t4 - рейтинг\n\t0 - выход: ");

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
				string jenre;
				cout << "\nВведите жанр: ";
				getline(cin, jenre);
				tvShows.at(id).jenre = jenre;
			}
			break;
			case 3:
			{
				string channel;
				cout << "\nВведите жанр: ";
				getline(cin, channel);
				tvShows.at(id).channel = channel;
			}
			break;
			case 4:
			{
				int rate = ui->inputRange(0, 100, "\nВведите рейтинг (от 0 до 100): ");
				tvShows.at(id).rate = rate;
			}
			break;
			default:
				break;
			}
		} while (choice != 0);

	} while (choice != 0);
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
	addUsersToFile();
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
