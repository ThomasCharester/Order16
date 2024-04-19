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
			cout << "\n�����: " << login;
			cout << "\t����� ��������������: " << isAdmin;
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
			if (showID) cout << "\n�����: " << i;
			cout << "\n��������: " << tvShows.at(i).name;
			cout << "\n�������: " << tvShows.at(i).rate;
			cout << "\n���������� ��������: " << tvShows.at(i).spectators;
			cout << "\n������� ���������� ����������: " << tvShows.at(i).averageWatchTime;
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
		fstream file("users.txt", ios::out); ///////// ���� ios::app, �� ������ ������� ����������, �� ��� �������� ��� ������ � ���� ���������
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
			cout << "\n������� ������� ���, �������� �����";
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

			choice = inputRange(0, 1, "�������� ��������:\n\t1 - ����������� ��-���\n\t0 - �����: ");

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

			choice = inputRange(0, 7, "�������� ��������:\n\t1 - ����������� ��-���\n\t2 - �������� ��-���\n\t3 - ������� ��-���\n\t4 - ������������� ���������� ��-���\n\t5 - �������� ������������\n\t6 - ������� ������������\n\t0 - �����: ");

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

			cout << "�������� ��������:\t1 - ����� � �������\t����� ������ - �����: ";
			getline(cin, buf);

			if (!isInteger(buf))
				continue;

			choice = stoi(buf);

			if (choice == 1)
				db->login();
		} while (choice == 1);

		cout << "�������� ���!";
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

				choice = inputRange(0, 2, "�������� �������:\n\t1 - �� ��������\n\t2 - �� ��������\n\t0 - �����: ");

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
		inputInt(choice, "\n������� ����� ���, ����� �������������: ");
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
				cout << "������� �����: ";
			else
			{
				choice = stoi(temp);
				if ((choice < min || (choice > max && max != min)))
					cout << "������� ����� �� " << min << " �� " << (max != min) ? to_string(max) : "infinite";
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
				cout << "������� �����: ";
			else
			{
				choice = stod(temp);
				if ((choice < min || (choice > max && max != min)))
					cout << "������� ����� �� " << min << " �� " << (max != min) ? to_string(max) : "infinite";
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
		cout << "������� �����: ";
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
			cout << "������������ � ����� ������� �� ����������!\n";
	} while (!isLoginRight);

	do
	{
		cout << "������� ������: ";
		getline(cin, temp);

		if (users.at(i).checkPassword(temp))
		{
			isPasswordRight = true;
			break;
		}

		if (!isPasswordRight)
			cout << "������������ ������!\n";
	} while (!isPasswordRight);

	currentUser = &(users.at(i));

	cout << "�� ������� ����� � �������!\n";
	ui->pressAnyButton();

	if (users.at(i).checkAdmin())
		ui->adminMenu();
	else
		ui->userMenu();
}

void Database::addTVShow() {
	system("cls");

	cout << "\n\n������� ��������: ";
	string name;
	getline(cin, name);

	int spectators = ui->inputRange(0, 0, "������� ����� ��������");

	double averageWatchTime = ui->inputRange(0.0, 0.0, "������� ����� ��������");

	tvShows.push_back(TVShow(name, spectators, averageWatchTime));
}
void Database::removeTVShow() {
	system("cls");
	showTVShows(true);

	int choice;
	ui->inputInt(choice, "\n������� ����� ���, ����� �������: ");

	int confirm = ui->inputRange(0, 1, "\n�� �������? 1 - ��/0 - ���");
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

		choice = ui->inputRange(0, 2, "�������� ��������:\n\t1 - ��������\n\t2 - ���������� ��������\n\t3 - ������� ����� ���������\n\t0 - �����: ");

		switch (choice)
		{
		case 1:
		{
			string name;
			cout << "\n������� ��������: ";
			getline(cin, name);
			tvShows.at(id).name = name;
		}
		break;
		case 2:
		{
			int spectators = ui->inputRange(0, 0, "������� ����� ��������");
			tvShows.at(id).spectators = spectators;
		}
		break;
		case 3:
		{
			double averageWatchTime = ui->inputRange(0.0, 0.0, "������� ����� ��������");
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
		cout << "\n�����: " << i;
		users.at(i).showInfo();
		cout << '\n';
	}
}
void Database::addUser()
{
	system("cls");
	cout << "\n\n������� �����: ";
	string login;
	getline(cin, login);
	string password;
	cout << "\n������� ������: ";
	getline(cin, password);
	int admin = ui->inputRange(0, 1, "\n�� �������? 1 - ��/0 - ���");

	users.push_back(User(login, password, admin));
	writeUsersToFile();
}
void Database::removeUser()
{
	system("cls");
	showUsers();
	int choice;
	ui->inputInt(choice, "\n������� ����� ������� ������, ����� �������: ");

	int confirm = ui->inputRange(0, 1, "\n�� �������? 1 - ��/0 - ���");
	if (!confirm) return;

	users.erase(users.begin() + choice);
}

int Database::TVShow::maxSpectators = 0;
double Database::TVShow::maxAverageWatchTime = 0;
