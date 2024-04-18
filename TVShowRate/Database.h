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
		string channel;
		string jenre;
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
	};
	UserInterface* ui;
	vector<TVShow> tvShows;
	vector<User> users;

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

	void showTVShows(bool showID = 0) {
		for (int i = 0; i < tvShows.size(); i++) {
			cout << '\n';
			if (showID) cout << "\n�����: " << i;
			cout << "\n��������: " << tvShows.at(i).name;
			cout << "\n�����: " << tvShows.at(i).channel;
			cout << "\n����: " << tvShows.at(i).jenre;
			cout << "\n�������: " << tvShows.at(i).rate;
		}
	}
	void addTVShow();
	void removeTVShow();

	void addUser();
	void removeUser();
	void addUsersToFile()
	{
		fstream file("users.txt", ios::in); ///////// ���� ios::app, �� ������ ������� ����������, �� ��� �������� ��� ������ � ���� ���������
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
			choice = 0;

			cout << "�������� ��������:\n\t1 - ����������� ��-���\n\t2 - ����� �� �������\n\t3 - ��������� ������\n\t-1 - �����: ";

			while ((choice < 1 || choice > 3) && choice != -1)
			{
				string temp;

				getline(cin, temp);
				if (!isInteger(temp))
					cout << "������� �����: ";

				else
				{
					choice = stoi(temp);
					if ((choice < 1 || choice > 3) && choice != -1)
						cout << "������� �����: ";
				}
			}

			switch (choice)
			{
			case 1:
				showTVShows();
				break;
			case 2:
				filterTVShows();
				break;
			case 3:
				cout << "���� ���\n";
				pressAnyButton();
				break;
			default:
				pressAnyButton();
				break;
			}
		} while (choice != -1);
	}
	void adminMenu()
	{
		int choice;
		do
		{
			system("cls");
			choice = 0;

			cout << "�������� ��������:\n\t1 - ����������� ��-���\n\t2 - �������� ��-���\n\t3 - ������� ��-���\n\t4 - ������������� ���������� ��-���\n\t5 - �������� ������������\n\t6 - ������� ������������\n\t7 - ����� �� �������\n\t-1 - �����: ";

			while ((choice < 1 || choice > 7) && choice != -1)
			{
				string temp;

				getline(cin, temp);
				if (!isInteger(temp))
					cout << "������� �����: ";

				else
				{
					choice = stoi(temp);
					if ((choice < 1 || choice > 7) && choice != -1)
						cout << "������� �����: ";
				}
			}

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
				cout << "���� ���\n";
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
		} while (choice != -1);
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
				choice = 0;

				cout << "�������� �������:\n\t1 - �� ��������\n\t2 - �� �����\n\t3 - �� ��������\n\t4 - �� ������\n\t-1 - �����: ";

				while ((choice < 1 || choice > 3) && choice != -1)
				{
					string temp;

					getline(cin, temp);
					if (!isInteger(temp))
						cout << "������� �����: ";

					else
					{
						choice = stoi(temp);
						if ((choice < 1 || choice > 3) && choice != -1)
							cout << "������� �����: ";
					}
				}

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
			} while (choice != -1);

		} while (choice != -1);
	}
	void filterTVShows() {
		int choice;
		do {
			do
			{
				system("cls");
				choice = 0;

				cout << "�������� ������:\n\t1 - �� ������\n\t2 - �� �����\n\t-1 - �����: ";

				while ((choice < 1 || choice > 3) && choice != -1)
				{
					string temp;

					getline(cin, temp);
					if (!isInteger(temp))
						cout << "������� �����: ";

					else
					{
						choice = stoi(temp);
						if ((choice < 1 || choice > 3) && choice != -1)
							cout << "������� �����: ";
					}
				}

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
			} while (choice != -1);

		} while (choice != -1);
	}
};
Database::Database() {
	ui = new UserInterface(this);
	ui->startUI();
}
Database::~Database()
{
	addUsersToFile();
	delete ui;
}

void Database::setRate()
{

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
	int rate;
	while (true) {
		ui->inputInt(rate, "\n������� ������� (�� 0 �� 100): ");
		if (rate >= 0 && rate <= 100) break;
	}
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
	int confirm;
	while (true) {
		ui->inputInt(confirm, "\n�� �������? 1 - ��/0 - ���");
		if (confirm == 1) break;
		else if (confirm == 0) return;
	}
	tvShows.erase(tvShows.begin() + choice);
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
	int admin;
	while (true) {
		ui->inputInt(admin, "\n��� �������������? 1 - ��/0 - ���");
		if (admin >= 0 && admin <= 1) break;
	}

	users.push_back(User(login, password, admin));
	addUsersToFile();
}

void Database::removeUser()
{
	system("cls");
	showUsers();
	int choice;
	ui->inputInt(choice, "\n������� ����� ������� ������, ����� �������: ");
	int confirm;
	while (true) {
		ui->inputInt(confirm, "\n�� �������? 1 - ��/0 - ���");
		if (confirm == 1) break;
		else if (confirm == 0) return;
	}
	users.erase(users.begin() + choice);
}
