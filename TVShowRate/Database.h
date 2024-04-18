#pragma once
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
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

		bool checkLogin(string login) { return this->login == login; }
		bool checkPassword(string password) { return this->password == password; }
		bool checkAdmin() { return isAdmin; };

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
	};
	UserInterface* ui;

	vector<User> users;
public:
	void encodeOrDecodePassword(string& pass)
	{
		char key = pass.size() % 10;

		for (int i = 0; i < pass.size(); i++)
			pass[i] ^= key;
	}

	void addUsersToFile()
	{
		fstream file("users.txt", ios::app);
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

	void login()
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
			ui->admin_menu();
		else
			ui->user_menu();
	}
};

class UserInterface {
private:
	Database* db;
public:

	void user_menu()
	{
		int choice;

		do
		{
			choice = 0;

			cout << "�������� ��������:\n\t1 - ����������� ������\n\t2 - ����������� ��������� �������\n\t3 - ����������� �����\n\t-1 - �����: ";

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
				cout << "���� ���\n";
				pressAnyButton();
				break;
			case 2:
				cout << "���� ���\n";
				pressAnyButton();
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
	void admin_menu()
	{
		int choice;
		do
		{
			choice = 0;

			cout << "�������� ��������:\n\t1 - ����������� ������\n\t2 - ����������� ��������� �������\n\t3 - ����������� �����\n\t4 - ������� ������ ������\n\t5 - ������� ������\n\t6 - ������ ���������� ������\n\t7 - ����������� ������ ������������\n\t8 - �������� ������������\n\t-1 - �����: ";

			while ((choice < 1 || choice > 8) && choice != -1)
			{
				string temp;

				getline(cin, temp);
				if (!isInteger(temp))
					cout << "������� �����: ";

				else
				{
					choice = stoi(temp);
					if ((choice < 1 || choice > 8) && choice != -1)
						cout << "������� �����: ";
				}
			}

			switch (choice)
			{
			case 1:
				cout << "���� ���\n";
				pressAnyButton();
				break;
			case 2:
				cout << "���� ���\n";
				pressAnyButton();
				break;
			case 3:
				cout << "���� ���\n";
				pressAnyButton();
				break;
			case 4:
				cout << "���� ���\n";
				pressAnyButton();
				break;
			case 5:
				cout << "���� ���\n";
				pressAnyButton();
				break;
			case 6:
				cout << "���� ���\n";
				pressAnyButton();
				break;
			case 7:
				cout << "���� ���\n";
				pressAnyButton();
				break;
			case 8:
				cout << "���� ���\n";
				pressAnyButton();
				break;
			default:
				pressAnyButton();
				break;
			}
		} while (choice != -1);
	}
	void pressAnyButton()
	{
		cout << "\n������� ����� �������\n";
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
};