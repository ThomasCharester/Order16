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

		cout << "Вы успешно вошли в аккаунт!\n";
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

			cout << "Выберите действие:\n\t1 - просмотреть турнир\n\t2 - просмотреть турнирную таблицу\n\t3 - просмотреть места\n\t-1 - выход: ";

			while ((choice < 1 || choice > 3) && choice != -1)
			{
				string temp;

				getline(cin, temp);
				if (!isInteger(temp))
					cout << "Введите число: ";

				else
				{
					choice = stoi(temp);
					if ((choice < 1 || choice > 3) && choice != -1)
						cout << "Введите число: ";
				}
			}

			switch (choice)
			{
			case 1:
				cout << "Пока нет\n";
				pressAnyButton();
				break;
			case 2:
				cout << "Пока нет\n";
				pressAnyButton();
				break;
			case 3:
				cout << "Пока нет\n";
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

			cout << "Выберите действие:\n\t1 - просмотреть турнир\n\t2 - просмотреть турнирную таблицу\n\t3 - просмотреть места\n\t4 - создать турнир заново\n\t5 - удалить турнир\n\t6 - ввести результаты матчей\n\t7 - регистрация нового пользователя\n\t8 - удаление пользователя\n\t-1 - выход: ";

			while ((choice < 1 || choice > 8) && choice != -1)
			{
				string temp;

				getline(cin, temp);
				if (!isInteger(temp))
					cout << "Введите число: ";

				else
				{
					choice = stoi(temp);
					if ((choice < 1 || choice > 8) && choice != -1)
						cout << "Введите число: ";
				}
			}

			switch (choice)
			{
			case 1:
				cout << "Пока нет\n";
				pressAnyButton();
				break;
			case 2:
				cout << "Пока нет\n";
				pressAnyButton();
				break;
			case 3:
				cout << "Пока нет\n";
				pressAnyButton();
				break;
			case 4:
				cout << "Пока нет\n";
				pressAnyButton();
				break;
			case 5:
				cout << "Пока нет\n";
				pressAnyButton();
				break;
			case 6:
				cout << "Пока нет\n";
				pressAnyButton();
				break;
			case 7:
				cout << "Пока нет\n";
				pressAnyButton();
				break;
			case 8:
				cout << "Пока нет\n";
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
		cout << "\nНажмите любую клавишу\n";
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

			cout << "Выберите действие:\t1 - войти в систему\tлюбое другое - выход: ";
			getline(cin, buf);

			if (!isInteger(buf))
				continue;

			choice = stoi(buf);

			if (choice == 1)
				db->login();
		} while (choice == 1);

		cout << "Хорошего дня!";
	}
};