#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

using namespace std;

class Field
{
private:
	bool map[15][15] = {};
	bool vay[15][15] = {};

	bool TestXY(int x, int y)
	{
		if (x < 0 || y < 0 || x >= 15 || y >= 15) return false;
		return true;
	}
public:
	Field() {}  // конструктор карты (обнуляет все клеточки)

	bool SetOstrov(int x, int y)  // отрисовка острова в клетке
	{
		if (TestXY(x, y) == false) return false;
		map[x][y] = true;
		return true;
	}
	char GetInfo(int x, int y)// получение информации о клетке
	{
		if (TestXY(x, y) == false) return '#';	// за пределами карты
		if (map[x][y] == true) return 'O';		// суша
		if (vay[x][y] == true) return 'V';		// посещалось
		return '.';								// море

	}
	bool SetVisit(int x, int y) // установка метки посещено
	{
		if (GetInfo(x, y) == '.') {
			vay[x][y] = true;
			return true;
		}
		return false;
	}
	void Surface() // сброс посещений после всплытия
	{
		for (int i = 0; i < 15; i++)
			for (int j = 0; j < 15; j++) 
				vay[i][j]=false;
	}
};

class Submarina
{

public:
	Submarina()
	{
	}
	void PrintField()
	{
		for (int i = -2; i < 17; i++) {	//Проверка заполнения поля
			for (int j = -2; j < 17; j++)
				cerr << map.GetInfo(j, i)<<" ";
			cerr << endl;
		}
	}

	bool Init(int x, int y, Field m)	// действие используеться только для старта
	{
		map = m;						//загружаем карту в подлодку
		if (SetPosition(x, y) == false) 
			return false;				// если клетка занята, то не инициируем
		return true;
	}
	string Run()// автозапуск подлодки
	{
		string a = AnswerNew();
		string e = Energy();
		string b;// = "|MSG " + to_string(n);

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
		//map.SetVisit(x, y);
	}

	bool Move(string Compas)// смещение подлодки в заданном направлении
	{
		if (Compas == "MOVE S") return SetPosition(xp, yp + 1);
		if (Compas == "MOVE W") return SetPosition(xp - 1, yp);
		if (Compas == "MOVE N") return SetPosition(xp, yp - 1);
		if (Compas == "MOVE E") return SetPosition(xp + 1, yp);
		return false;
	}
	bool MoveSilence(string Compas, int n)// отрисовываем сайленс в заданном направлении на n клеток
	{
		for (int i = 0; i < n; i++) {
			if (Move(Compas) == false) return false;
		}
		return true;
	}
	void Surface()  // всплытие подлодки
	{
		map.Surface();
		map.SetVisit(xp, yp); // и занимаем прежнюю клетку
	}
	int x() { return xp; }		/// доступ к координатам подлодки
	int y() { return yp; }		// к обеим координатам
	int SectorStatus() {return (xp / 5) + (yp / 5) * 3 + 1;}
private:
	int xp = 0;
	int yp=0;
	int SilCD=-1;
	int SonCD=-1;
	int TorCD=-1;
	int MinCD=-1;
	Field map;

	int Score(int Iteration = 0)// оценка позиции подлодки
	{
		int s = 1;
		if (map.GetInfo(xp, yp + 1) == '.') s++;
		if (map.GetInfo(xp - 1, yp) == '.') s++;
		if (map.GetInfo(xp, yp - 1) == '.') s++;
		if (map.GetInfo(xp + 1, yp) == '.') s++;
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
		Submarina n1 = *this;
		Submarina n2 = *this;
		Submarina n3 = *this;
		Submarina n4 = *this;
		if (n1.Move("MOVE S") == true) s1 = n1.Status(Iteration - 1);
		if (n2.Move("MOVE W") == true) s2 = n2.Status(Iteration - 1);
		if (n3.Move("MOVE N") == true) s3 = n3.Status(Iteration - 1);
		if (n4.Move("MOVE E") == true) s4 = n4.Status(Iteration - 1);
		ss = s1 + s2 + s3 + s4 + 1;
		return ss;
	}
	string Look(int Iteration)
	{
		int s1 = 0;
		int s2 = 0;
		int s3 = 0;
		int s4 = 0;
		Submarina n1 = *this;
		Submarina n2 = *this;
		Submarina n3 = *this;
		Submarina n4 = *this;
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
	string AnswerNew() // желаемое движение
	{
		string a = Look(3);				// получаем самое выгодное направление
		if (a == "SURFACE") {
			Surface();
			return a;	// если оно всплыть - всплываем
		}
		if (SilCD > 0) {	// если сайленс не заряжен
			Move(a);		// то просто двигаем подлодку
			return a;		// и передаем это движение в оболочку
		}
		// а если заряжен, то даем на 1
		MoveSilence(a, 1);
		if (a == "MOVE S") return "SILENCE S 1";
		if (a == "MOVE W") return "SILENCE W 1";
		if (a == "MOVE N") return "SILENCE N 1";
		if (a == "MOVE E") return "SILENCE E 1";
	}
	string Energy(string wp = "")
	{
		if (SilCD != 0)	wp = " SILENCE";
		if (SonCD != 0)	wp = " SONAR";
		if (TorCD != 0)	wp = " TORPEDO";
		return wp;
	}
	bool SetPosition(int x, int y) // установка подлодки в заданную точку
	{
		if (TestXY(x, y) == false) return false;// фолс, если ставим за границы
		if (map.GetInfo(x, y) != '.') return false;// фолс, если ставим на занятую клетку
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
};
class CenterControl 
{
public:

	CenterControl()
	{

	}
	void LoadMap(Field map)
	{
		for (int i = 0; i < 15; i++)
			for (int j = 0; j < 15; j++) {
				Submarina temp;
				if (temp.Init(j, i, map)==true)	// если подлодка инициируется на море,
					f.push_back(temp);			// то заносим ее в массив
			}									// иначе пропускаем
	}
	int Total()
	{
		return f.size();
	}
	void PrintList()	/// вывод карты распределения виртуальных подлодок
	{
		int flag[15][15] = {};		// флаговый массив
		/*
		for (int i = 0; i < 15; i++)
			for (int j = 0; j < 15; j++)
				flag[i][j] = 0;		// изначально инициализируем в 0*/
		cerr << "Всего вариантов: " << f.size() << endl;
		for (int i = 0; i < f.size(); i++)
			flag[f[i].x()][f[i].y()]++;	// увеличиваем посещенную клетку
		for (int i = 0; i < 15; i++) {
			for (int j = 0; j < 15; j++) {
				cerr << flag[j][i];
				if ((j == 4) | (j == 9)) cerr << '|';
				else cerr << ' ';
			}
			if ((i == 4) | (i == 9)) cerr << "_\n";
			else cerr << '\n';
		}
	}
	void LoadOrders(string str)
	{
		string move="";			// результаты обработки ответа
		string surface="";		// если было всплытие
		int sf=-1;				// указывает на сектор всплытия
		string torpedo="";		// если была торпеда
		int tx = -1;
		int ty = -1;			// координаты удара
		string sonar="";		// если был сонар
		int son=-1;				// указывает сектор
		string silence="";		// если было сало
		int n;
		n = str.find("MOVE");
		if (n != -1) move = str.substr(n, 6);
		n = str.find("SURFACE");
		if (n != -1)
		{
			surface = str.substr(n, 9);
			sf = stoi(surface.substr(8, 2));
		}
		n = str.find("TORPEDO");
		if (n != -1)
		{
			torpedo = str.substr(n, 13);
			tx = stoi(torpedo.substr(8, 2));
			ty = stoi(torpedo.substr(10, 3));
		}
		n = str.find("SONAR");
		if (n != -1)
		{
			sonar = str.substr(n, 7);
			son = stoi(sonar.substr(6, 2));
		}
		n = str.find("SILENCE");
		if (n != -1) silence = str.substr(n, 7);

		for (int i = 0; i < 15; i++)
			for (int j = 0; j < 15; j++)
				p[i][j] = false;

		for (it = 0; it < f.size(); it++) {
			if (move != "") RenderMove(move);
			if (surface != "") RenderSurface(sf);
			if (torpedo != "") RenderTorpedo(tx, ty);
			if (sonar != "") RenderSonar(son);
			if (silence != "") RenderSilence();
		}
		f.insert(f.end(), silf.begin(), silf.end());
		silf.clear();
	}
private:

	vector <Submarina> f;  /// вражеские подлодки
	vector <Submarina> silf; // временный массив для отрисовки сайленса
	int it = 0; // итератор на массив подлодок
	bool p[15][15] = {};		// флвговый массив в false
	void RenderMove( string mov)		// отработка движения "MOVE"
	{
		if (f[it].Move(mov) == false) {	// если подолодка не может двигаться
			f.erase(f.begin() + it);		// в заданном направлении,
			it--;							// то она удаляется из списка
		}
	}
	bool RenderSurface(int n)		// n-сектор всплытия
	{				// отработка "SURFACE"
		// если в этой клетке нет подлодки
		if (f[it].SectorStatus() == n) {
			if (p[f[it].x()][f[it].y()] == false) {
				p[f[it].x()][f[it].y()] = true;
				f[it].Surface();
				return true;
			}
		}
		f.erase(f.begin() + it);
		it--;			
		return false;
	}
	void RenderTorpedo( int x, int y)	// отработка пуска торпеды
	{

	}
	void RenderSonar( int n)		// отработка использованного сонара
	{}
	void RenderSilence()		// отработка сайленса
	{
		Submarina tmp;		// создаем временную подлодку
		for (int i = 1; i <= 4; i++) {
			tmp = f[it];		// добавляем до 4-ех подлодок на юг
			if (tmp.MoveSilence("MOVE S", i) == true) silf.push_back(tmp);
			else break;
		}
		for (int i = 1; i <= 4; i++) {
			tmp = f[it];		// еще до 4-ех в западном направлении
			if (tmp.MoveSilence("MOVE W", i) == true) silf.push_back(tmp);
			else break;
		}
		for (int i = 1; i <= 4; i++) {
			tmp = f[it];		// максимум еще 4-ре на север
			if (tmp.MoveSilence("MOVE N", i) == true) silf.push_back(tmp);
			else break;
		}
		for (int i = 1; i <= 4; i++) {
			tmp = f[it];		// и на восток, до 4-ех подлодок
			if (tmp.MoveSilence("MOVE E", i) == true) silf.push_back(tmp);
			else break;
		}	// еще одна так и останется на месте (SILENCE 0)
	}
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
	} while (mp.GetInfo(x, y) == 'O');

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

		cup.LoadOrders(opponentOrders); // загрузка данных о действии противника
						// и реакция на них одновременно

		sub.InputData(x, y, myLife, oppLife, torpedoCooldown, sonarCooldown, silenceCooldown, mineCooldown);
		sub.InputSonar(sonarResult);
		sub.InputOrder(opponentOrders);

		// Write an action using cout. DON'T FORGET THE "<< endl"
		// To debug: cerr << "Debug messages..." << endl;
		//sub.PrintField();
		cout << sub.Run() << endl;
		// вывод данных об анализе
		if (cup.Total() < 400) cup.PrintList();

	}
}