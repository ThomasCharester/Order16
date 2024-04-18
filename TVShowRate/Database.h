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

		const string getLogin() { return login; }
	};

	struct TVShow {
		string name;
		int rate;
		string channel;
		string jenre;
		vector<string> usersRate;
		vector<int> ratings;
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
		int isUserRate(string login) const {
			for (int i = 0; i < usersRate.size(); i++)
				if (usersRate.at(i) == login) return i;
			return -1;
		}
		void balanceRate() {
			if (!ratings.size()) return;
			rate = 0;
			for (int r : ratings)
				rate += r;
			rate /= (int)ratings.size();
		}
		void addToFile()
		{
			fstream file("tvShows.txt", ios::app);
			file << 'S' << ' ' << name << ' ' << rate << ' ' << channel << ' ' << jenre << '\n';
			for (int i = 0; i < usersRate.size(); i++) {
				file << usersRate.at(i) << ' ' << ratings.at(i) << '\n';
			}
			file.close();
		}
	};
	UserInterface* ui;
	vector<TVShow> tvShows;
	vector<User> users;

	User* currentUser;

	void showUsers();
public:
	const string getUserLogin() const {
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
			if (showID) cout << "\n�����: " << i;
			cout << "\n��������: " << tvShows.at(i).name;
			cout << "\n�����: " << tvShows.at(i).channel;
			cout << "\n����: " << tvShows.at(i).jenre;
			cout << "\n�������: " << tvShows.at(i).rate;
		}
		cout << '\n';
	}
	void addTVShow();
	void removeTVShow();
	void editTVShow(int id);

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

		if (file.eof()) return;

		string temp;

		getline(file, temp, '\n');
		int size = stoi(temp);

		getline(file, temp, ' ');
		for (int i = 0; i < size; i++)
		{
			string name, rate, channel, jenre;
			
			getline(file, name, ' ');
			getline(file, rate, ' ');
			getline(file, channel, ' ');
			getline(file, jenre, '\n');

			tvShows.emplace_back(TVShow(name,stoi(rate),channel,jenre));
			do {
				if (file.eof()) break;
				getline(file, temp, ' ');
				if (temp == "S" || file.eof())break;
				tvShows.at(tvShows.size() - 1).usersRate.emplace_back(temp);
				getline(file, temp, '\n');
				tvShows.at(tvShows.size() - 1).ratings.emplace_back(stoi(temp));
			} while (true);
			tvShows.at(tvShows.size() - 1).balanceRate();
		}
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

			choice = inputRange(0, 3, "�������� ��������:\n\t1 - ����������� ��-���\n\t2 - ����� �� �������\n\t3 - ��������� ������\n\t0 - �����: ");

			switch (choice)
			{
			case 1:
				showTVShows();
				break;
			case 2:
				filterTVShows();
				break;
			case 3:
				db->setRate();
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

			choice = inputRange(0, 7, "�������� ��������:\n\t1 - ����������� ��-���\n\t2 - �������� ��-���\n\t3 - ������� ��-���\n\t4 - ������������� ���������� ��-���\n\t5 - �������� ������������\n\t6 - ������� ������������\n\t7 - ����� �� �������\n\t0 - �����: ");

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

				choice = inputRange(0, 4, "�������� �������:\n\t1 - �� ��������\n\t2 - �� �����\n\t3 - �� ��������\n\t4 - �� ������\n\t0 - �����: ");

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
				if(choice) db->showTVShows();
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

				choice = inputRange(0, 2, "�������� ������:\n\t1 - �� ������\n\t2 - �� �����\n\t0 - �����: ");

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
				if ((choice < min || choice > max))
					cout << "������� ����� �� " << min << " �� " << max;
			}
		} while ((choice < min || choice > max));

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

void Database::setRate()
{
	system("cls");
	if (tvShows.empty()) return;
	showTVShows(true);

	int choice = ui->inputRange(0, (int)tvShows.size(), "\n������� ����� ���, ����� �������: ");
	int userRateId = tvShows.at(choice).isUserRate(getUserLogin());
	if (userRateId < 0) {
		tvShows.at(choice).usersRate.push_back(getUserLogin());
		tvShows.at(choice).ratings.push_back(ui->inputRange(0, 100, "\n������� ������ �� 0, �� 100: "));
		tvShows.at(choice).balanceRate();
	}
	else {
		tvShows.at(choice).ratings.at(userRateId) = ui->inputRange(0, 100, "\n������� ������ �� 0, �� 100: ");
		tvShows.at(choice).balanceRate();
	}
}

void Database::filterByJenre()
{
	system("cls");
	cout << "\n\n������� ����: ";
	string jenre;
	getline(cin, jenre);
	bool jenreExist = false;
	for (int i = 0; i < tvShows.size(); i++) {
		if (tvShows.at(i).jenre == jenre) {
			cout << '\n';
			cout << "\n��������: " << tvShows.at(i).name;
			cout << "\n�����: " << tvShows.at(i).channel;
			cout << "\n����: " << tvShows.at(i).jenre;
			cout << "\n�������: " << tvShows.at(i).rate;
			jenreExist = true;
		}
	}
	if (!jenreExist) cout << "\n�� ������� ������ �� �������\n";
}

void Database::filterByChannel()
{
	system("cls");
	cout << "\n\n������� �����: ";
	string channel;
	getline(cin, channel);
	bool channelExist = false;
	for (int i = 0; i < tvShows.size(); i++) {
		if (tvShows.at(i).channel == channel) {
			cout << '\n';
			cout << "\n��������: " << tvShows.at(i).name;
			cout << "\n�����: " << tvShows.at(i).channel;
			cout << "\n����: " << tvShows.at(i).jenre;
			cout << "\n�������: " << tvShows.at(i).rate;
			channelExist = true;
		}
	}
	if (!channelExist) cout << "\n�� ������� ������ �� �������\n";
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

	int rate = ui->inputRange(0, 100, "������� ����� �� 0, �� 100");

	string channel;
	cout << "\n������� �������� ������: ";
	getline(cin, channel);

	string jenre;
	cout << "\n������� ����: ";
	getline(cin, jenre);

	tvShows.push_back(TVShow(name, rate, channel, jenre));
}
void Database::removeTVShow() {
	system("cls");
	showTVShows(true);

	int choice;
	ui->inputInt(choice, "\n������� ����� ���, ����� �������: ");

	int confirm = ui->inputRange(0, 1, "\n�� �������? 1 - ��/0 - ���");
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

			choice = ui->inputRange(0, 4, "�������� ��������:\n\t1 - ��������\n\t2 - ����\n\t3 - �����\n\t4 - �������\n\t0 - �����: ");

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
				string jenre;
				cout << "\n������� ����: ";
				getline(cin, jenre);
				tvShows.at(id).jenre = jenre;
			}
			break;
			case 3:
			{
				string channel;
				cout << "\n������� ����: ";
				getline(cin, channel);
				tvShows.at(id).channel = channel;
			}
			break;
			case 4:
			{
				int rate = ui->inputRange(0, 100, "\n������� ������� (�� 0 �� 100): ");
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
