#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

using namespace std;

class Field
{
private:
	int m[15][15];
	int Visit;
	bool TestXY(int x, int y)
	{
		if (x < 0 || y < 0 || x >= 15 || y >= 15) return false;
		return true;
	}
public:
	Field()  // конструктор карты (обнуляет все клеточки)
	{
		Visit = 2;
		for (int i = 0; i < 15; i++)
			for (int j = 0; j < 15; j++)
				m[j][i] = 0;
	}
	bool SetOstrov(int x, int y)  // отрисовка острова в клетке
	{
		if (TestXY(x, y) == false) return false;
		m[x][y] = 1;
		return true;
	}
	string GetInfo(int x, int y)// получение информации о клетке
	{
		if (TestXY(x, y) == false) return "n";	// за пределами карты
		if (m[x][y] == 1) return "O";			// суша
		if (m[x][y] == Visit) return "V";		// посещалось
		return ".";								// море

	}
	void SetVisit(int x, int y) // установка метки посещено
	{
		m[x][y] = Visit;
	}
	void Surface() // сброс посещений после всплытия
	{
		Visit++;
	}
};

class Submarina
{

public:
	Submarina(int x, int y,Field m)
	{
		xp = x;
		yp = y;
		LMove = "N";
		SilCD = 0;
		SonCD = 0;
		TorCD = 0;
		MinCD = 0;
		map = m;
		//map.SetVisit(x, y);

	}
	int Score(int Iteration = 0)
	{
		if (Move() == "0")
			return 0;

		return 777;
	}
	string Answer()
	{
		/*for (int i = 0; i < 15; i++) {	//Проверка заполнения поля
			for (int j = 0; j < 15; j++)
				cerr << map.GetInfo(j, i)<<" ";
			cerr << endl;
		}*/

		string mov = Move();
		if (mov == "0") {
			mov = "SURFACE";
			map.Surface();
			//map.SetVisit(xp, yp);
		}
		else 
			mov = "MOVE " + mov;
		return mov;
	}
	void InputOrder(string opponentOrders)
	{

	}
	void InputSonar(string sonarResult)
	{

	}
	void InputData(int x, int y, int myLife, int oppLife, int torpedoCooldown, int sonarCooldown, int silenceCooldown, int mineCooldown)
	{
		xp = x;
		yp = y;
		SilCD = silenceCooldown;
		SonCD = sonarCooldown;
		TorCD = torpedoCooldown;
		MinCD = mineCooldown;
		map.SetVisit(x, y);
	}
	bool SetPosition(int x, int y)
	{
		if (TestXY(x, y) == false) return false;
		xp = x;
		yp = y;
		return true;
	}
private:
	int xp, yp;
	string LMove;
	int SilCD;
	int SonCD;
	int TorCD;
	int MinCD;
	Field map;

	bool TestXY(int x, int y)
	{
		if (x < 0 || y < 0 || x >= 15 || y >= 15) return false;
		return true;
	}
	string Move()
	{
		if (map.GetInfo(xp, yp + 1) == ".") return "S";
		if (map.GetInfo(xp - 1, yp) == ".") return "W";
		if (map.GetInfo(xp, yp - 1) == ".") return "N";
		if (map.GetInfo(xp + 1, yp) == ".") return "E";
		return "0";// если плыть некуда
	}
};

int main()
{
	int width;
	int height;
	int myId;

	Field mp;

	cin >> width >> height >> myId; cin.ignore();
	for (int i = 0; i < height; i++) {
		string line;
		getline(cin, line);
		for (int x = 0; x < width; x++) {
			if (line[x] == 'x') mp.SetOstrov(x, i);
		}
	}

	/*for (int i = 0; i < 15; i++) {	//Проверка заполнения поля
		for (int j = 0; j < 15; j++)
			cerr << mp.GetInfo(j, i);
		cerr << endl;
	}*/
	

	int x, y;
	x = 8;
	y = 8;

	do { // поиск стартовой позиции
		x--;
		if (x < 0) {
			x = 14;
			y--;
		}
	} while (mp.GetInfo(x, y) == "O");

	Submarina sub(x,y,mp);

	cout << x << " " << y << endl;

	// game loop
	while (1) {
		int x;
		int y;
		int myLife;
		int oppLife;
		int torpedoCooldown;
		int sonarCooldown;
		int silenceCooldown;
		int mineCooldown;
		cin >> x >> y >> myLife >> oppLife >> torpedoCooldown >> sonarCooldown >> silenceCooldown >> mineCooldown; cin.ignore();
		string sonarResult;
		cin >> sonarResult; cin.ignore();
		string opponentOrders;
		getline(cin, opponentOrders);

		sub.InputData(x, y, myLife, oppLife, torpedoCooldown, sonarCooldown, silenceCooldown, mineCooldown);
		sub.InputSonar(sonarResult);
		sub.InputOrder(opponentOrders);

		// Write an action using cout. DON'T FORGET THE "<< endl"
		// To debug: cerr << "Debug messages..." << endl;

		cout << sub.Answer() << "|MSG " << sub.Score() << endl;

	}
}