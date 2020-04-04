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
		if (TestXY(x, y) == false) return "#";	// за пределами карты
		if (m[x][y] == 1) return "O";			// суша
		if (m[x][y] == Visit) return "V";		// посещалось
		return ".";								// море

	}
	bool SetVisit(int x, int y) // установка метки посещено
	{
		if (GetInfo(x, y) == ".") {
			m[x][y] = Visit;
			return true;
		}
		return false;
	}
	void Surface() // сброс посещений после всплытия
	{
		Visit++;
	}
};

class Submarina
{

public:
	Submarina()
	{
		xp = 0;
		yp = 0;
		SilCD = -1;
		SonCD = -1;
		TorCD = -1;
		MinCD = -1;
		Suchestvuet = false;
	}
	void PrintField()
	{
		for (int i = -2; i < 17; i++) {	//Проверка заполнения поля
			for (int j = -2; j < 17; j++)
				cerr << map.GetInfo(j, i)<<" ";
			cerr << endl;
		}
		cerr << "Статус : " << Suchestvuet << endl;
	}
	bool Status()
	{
		return Suchestvuet;
	}
	void Init(int x, int y, Field m)
	{
		map = m;//загружаем карту в подлодку
		if (SetPosition(x, y, true) == false) return;// если клетка занята, то не инициируем
		Suchestvuet = true;// активируем подлодку
		return;
	}
	string Run()// автозапуск подлодки
	{
		string a = Answer();
		string e = Energy();
		string b;// = "|MSG " + to_string(n);
		if (a == "SURFACE")
			map.Surface();
		return a + e + b;
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
private:
	int xp, yp;
	int SilCD;
	int SonCD;
	int TorCD;
	int MinCD;
	Field map;
	bool Suchestvuet;
	int Score(int Iteration = 0)// оценка позиции подлодки
	{
		int s = 1;
		if (map.GetInfo(xp, yp + 1) == ".") s++;
		if (map.GetInfo(xp - 1, yp) == ".") s++;
		if (map.GetInfo(xp, yp - 1) == ".") s++;
		if (map.GetInfo(xp + 1, yp) == ".") s++;
		//cerr << s << endl;
		return s;
	}
	int Status(int Iteration)// jwtyrf позиции
	{
		if (Iteration <= 0) return Score();

		int s1 = 0;
		int s2 = 0;
		int s3 = 0;
		int s4 = 0;
		int ss;
		Submarina n1; n1.Init(xp, yp, map);
		Submarina n2; n2.Init(xp, yp, map);
		Submarina n3; n3.Init(xp, yp, map);
		Submarina n4; n4.Init(xp, yp, map);
		if (n1.Move("MOVE S") == true) s1 = n1.Status(Iteration - 1);
		if (n2.Move("MOVE W") == true) s2 = n2.Status(Iteration - 1);
		if (n3.Move("MOVE N") == true) s3 = n3.Status(Iteration - 1);
		if (n4.Move("MOVE E") == true) s4 = n4.Status(Iteration - 1);
		ss = s1 + s2 + s3 + s4 + 1;
		return ss;
	}
	bool Move(string Compas)// смещение подлодки в заданном направлении
	{
		if (Compas == "MOVE S") return SetPosition(xp, yp + 1);
		if (Compas == "MOVE W") return SetPosition(xp - 1, yp);
		if (Compas == "MOVE N") return SetPosition(xp, yp - 1);
		if (Compas == "MOVE E") return SetPosition(xp + 1, yp);
		return false;
	}
	string Look(int Iteration)
	{
		int s1 = 0;
		int s2 = 0;
		int s3 = 0;
		int s4 = 0;
		Submarina n1; n1.Init(xp, yp, map);
		Submarina n2; n2.Init(xp, yp, map);
		Submarina n3; n3.Init(xp, yp, map);
		Submarina n4; n4.Init(xp, yp, map);
		if (n1.Move("MOVE S") == true)			s1 = n1.Status(Iteration);
		if (n2.Move("MOVE W") == true)			s2 = n2.Status(Iteration);
		if (n3.Move("MOVE N") == true)			s3 = n3.Status(Iteration);
		if (n4.Move("MOVE E") == true)			s4 = n4.Status(Iteration);
		int ss = s1;
		string a = "MOVE S";
		if (s2 > s1) { ss = s2; a = "MOVE W"; }
		if (s3 > ss) { ss = s3; a = "MOVE N"; }
		if (s4 > ss) { ss = s4; a = "MOVE E"; }
		if (ss == 0) a = "SURFACE";
		return a;
	}
	string Answer() // желаемое движение
	{
		string a = Look(3);
		if (a == "SURFACE") return a;
		if (SilCD > 0) return a;
		if (a == "MOVE S") return "SILENCE S 1";
		if (a == "MOVE W") return "SILENCE W 1";
		if (a == "MOVE N") return "SILENCE N 1";
		if (a == "MOVE E") return "SILENCE E 1";
		return a;
		//return AutoMove();
	}
	string Energy(string wp = "")
	{
		if (SilCD != 0)	wp = " SILENCE";
		if (SonCD != 0)	wp = " SONAR";
		if (TorCD != 0)	wp = " TORPEDO";
		return wp;
	}
	bool SetPosition(int x, int y,bool forse = false) // установка подлодки в заданную точку
	{
		if (TestXY(x, y) == false) return false;// фолс, если ставим за границы
		string r = map.GetInfo(x, y);
		if (r == "O") return false;// фолс, если ставим на острова
		if ((r == "V") & (forse == false)) return false;// фолс, если ставим на пройденный путь
		xp = x;
		yp = y;
		map.SetVisit(x, y);
		return true;
	}


	bool TestXY(int x, int y)
	{
		if (x < 0 || y < 0 || x >= 15 || y >= 15) return false;
		return true;
	}
	string AutoMove()
	{
		if (map.GetInfo(xp, yp + 1) == ".") return "MOVE S";
		if (map.GetInfo(xp - 1, yp) == ".") return "MOVE W";
		if (map.GetInfo(xp, yp - 1) == ".") return "MOVE N";
		if (map.GetInfo(xp + 1, yp) == ".") return "MOVE E";
		return "SURFACE";// если плыть некуда
	}
};
class CenterControl 
{
public:

	CenterControl()
	{
		nTotal = 0;
	}
	void LoadMap(Field map)
	{
		int s = 0;
		for (int i = 0; i < 15; i++)
			for (int j = 0; j < 15; j++) {
				f[s].Init(j, i, map);
				s++;
			}
		nTotal = 0;
		for (int i = 0; i < 1000; i++)
			if (f[i].Status() == true) nTotal++;
	}
	int Total()
	{
		return nTotal;
	}
	void LoadOrders(string str)
	{
		cerr << str << endl;
	}
private:
	int nTotal; /// счетчик подлодок
	Submarina f[1000];  /// вражеские подлодки

};
int main()
{
	int width;
	int height;
	int myId;

	Field mp;// карта поля

	cin >> width >> height >> myId; cin.ignore();
	for (int i = 0; i < height; i++) {
		string line;
		getline(cin, line);
		for (int x = 0; x < width; x++) {
			if (line[x] == 'x') mp.SetOstrov(x, i);
		}
	}

	int x, y;
	x = 6;
	y = 9;

	do { // поиск стартовой позиции
		x--;
		if (x < 0) {
			x = 14;
			y--;
		}
	} while (mp.GetInfo(x, y) == "O");

	Submarina sub; /// наша подложка
	sub.Init(x, y, mp);
	//sub.PrintField();
	CenterControl cup;
	cup.LoadMap(mp);


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

		cup.LoadOrders(opponentOrders);

		sub.InputData(x, y, myLife, oppLife, torpedoCooldown, sonarCooldown, silenceCooldown, mineCooldown);
		sub.InputSonar(sonarResult);
		sub.InputOrder(opponentOrders);

		// Write an action using cout. DON'T FORGET THE "<< endl"
		// To debug: cerr << "Debug messages..." << endl;
		//sub.PrintField();
		cout << sub.Run()<<"|MSG " + to_string(cup.Total()) << endl;

	}
}