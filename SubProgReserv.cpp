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
	bool Init(int x, int y, Field m)
	{
		map = m;//загружаем карту в подлодку
		if (SetPosition(x, y, true) == false) return false;// если клетка занята, то не инициируем
		Suchestvuet = true;// активируем подлодку
		return true;
	}
	string Run()// автозапуск подлодки
	{
		string a = Answer();
		string e = Energy();
		string b;// = "|MSG " + to_string(n);
		if (a == "SURFACE")
			Surface();
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
		for (int i = 1; i < n; i++) {
			if (Compas == "MOVE S") if (SetPosition(xp, yp + i)==false) return false;
			if (Compas == "MOVE W") if (SetPosition(xp - i, yp)==false) return false;
			if (Compas == "MOVE N") if (SetPosition(xp, yp - i)==false) return false;
			if (Compas == "MOVE E") if (SetPosition(xp + i, yp)==false) return false;
		}
		return true;
	}
	bool Surface() // всплытие подлодки
	{
		map.Surface();
	}
	int x() { return xp; }		/// доступ к координатам подлодки
	int y() { return yp; }		// к обеим координатам
	void Disable()
	{
		Suchestvuet == false;
	}
	int SectorStatus()
	{
		return (xp / 5) + (yp / 5) * 3 + 1;
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
/*	string AutoMove()
	{
		if (map.GetInfo(xp, yp + 1) == ".") return "MOVE S";
		if (map.GetInfo(xp - 1, yp) == ".") return "MOVE W";
		if (map.GetInfo(xp, yp - 1) == ".") return "MOVE N";
		if (map.GetInfo(xp + 1, yp) == ".") return "MOVE E";
		return "SURFACE";// если плыть некуда
	}*/
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
	void PrintList()	/// вывод списка подлодок
	{
		int flag[15][15];		// флвговый массив
		for (int i = 0; i < 15; i++)
			for (int j = 0; j < 15; j++)
				flag[i][j] = 0;		// изначально инициализируем в 0
		for (int i = 0; i < f.size(); i++)
			flag[f[i].x()][f[i].y()]++;	// увеличиваем посещенную клетку
		for (int i = 0; i < 15; i++) {
			for (int j = 0; j < 15; j++)
				cerr << flag[j][i] << ' ';
			cerr << '\n';
		}
	}
	void LoadOrders(string str)
	{
		//cerr << str << endl;

		int n;
		n = str.find("MOVE");
		move = "";
		if (n != -1) move = str.substr(n, 6);
		
		n = str.find("SURFACE");
		surface = "";
		sf = -1;
		if (n != -1)
		{
			surface = str.substr(n, 9);
			sf = stoi(surface.substr(8, 2));
		}
		
		n = str.find("TORPEDO");
		torpedo = "";
		tx = -1;
		ty = -1;
		if (n != -1)
		{
			torpedo = str.substr(n, 13);
			tx = stoi(torpedo.substr(8, 2));
			ty = stoi(torpedo.substr(10, 3));
		}

		n = str.find("SONAR");
		sonar = "";
		son = -1;
		if (n != -1)
		{
			sonar = str.substr(n, 7);
			son = stoi(sonar.substr(6, 2));
		}
			
		n = str.find("SILENCE");
		silence = "";
		if (n != -1) silence = str.substr(n, 7);
		/*
		cerr <<"Mov "<< move <<"#"<< endl;
		cerr << "Sur " << surface << "#-> " << sf << endl;
		cerr << "Trp " << torpedo << "#-> " << tx << ":" << ty << endl;
		cerr << "Snr " << sonar << "#-> " << son << endl;
		cerr << "Sil "<<silence << "#" << endl;
		*/
	}
	void OneStep()
	{	
		for (fit = 0; fit < f.size(); fit++)	
			if (f[fit].Status() == true)
			{
				if (move != "") RenderMove( move);
				if (surface != "") RenderSurface( sf);
				if (torpedo != "") RenderTorpedo( tx, ty);
				if (sonar != "") RenderSonar( son);
				if (silence != "") RenderSilence();
			}
		f.insert(f.end(), silf.begin(), silf.end());
		silf.clear();
	}
private:
	string move;		// результаты обработки ответа
	string surface;		// если было всплытие
	int sf;				// указывает на сектор всплытия
	string torpedo;		// если была торпеда
	int tx, ty;			// координаты удара
	string sonar;		// если был сонар
	int son;			// указывает сектор
	string silence;		// если было сало

	vector <Submarina> f;  /// вражеские подлодки
	vector <Submarina> silf; // временный массив для отрисовки сайленса
	int fit; // итератор на массив подлодок
	void RenderMove( string mov)		// отработка движения "MOVE"
	{
		if (f[fit].Move(mov) == false) {	// если подолодка не может двигаться
			f.erase(f.begin() + fit);		// в заданном направлении,
			fit--;							// то она удаляется из списка
		}
	}

	void RenderSurface( int n)		// отработка "SURFACE"
	{

		// создать проверку на дубликаты и отрабатывать их удаление
		bool flag[15][15];		// флвговый массив
		for (int i = 0; i < 15; i++)
			for (int j = 0; j < 15; j++)
				flag[i][j] = false;		// изначально инициализируем в false

		if (f[fit].SectorStatus() == n) {			// если сектор всплытия совпадает
			f[fit].Surface();						// то подлодка инициирует всплытие
			if (flag[f[fit].x()][f[fit].y()] == true) {	// если в этой клетке уже есть подлодка
				f.erase(f.begin() + fit);			// то удаляем дубликат
				fit--;								// из списка
			}
			else {									// если подлодка первая в этом квадрате,
				flag[f[fit].x()][f[fit].y()] = true;	// то помечаем что квадрат занят
			}
		}
		else {
			f.erase(f.begin() + fit);		// если не совпадает,
			fit--;							// то удаляется из списка
		}
	}
	void RenderTorpedo( int x, int y)	// отработка пуска торпеды
	{

	}
	void RenderSonar( int n)		// отработка использованного сонара
	{}
	void RenderSilence()		// отработка сайленса
	{
		Submarina tmp;		// создаем временную подлодку
		for (int i = 1; i < 4; i++) {
			tmp = f[fit];		// добавляем до 4-ех подлодок на юг
			if (tmp.MoveSilence("MOVE S", i) == true) silf.push_back(tmp);
		}
		for (int i = 1; i < 4; i++) {
			tmp = f[fit];		// еще до 4-ех в западном направлении
			if (tmp.MoveSilence("MOVE W", i) == true) silf.push_back(tmp);
		}
		for (int i = 1; i < 4; i++) {
			tmp = f[fit];		// максимум еще 4-ре на север
			if (tmp.MoveSilence("MOVE N", i) == true) silf.push_back(tmp);
		}
		for (int i = 1; i < 4; i++) {
			tmp = f[fit];		// и на восток, до 4-ех подлодок
			if (tmp.MoveSilence("MOVE E", i) == true) silf.push_back(tmp);
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
		cup.OneStep();

		sub.InputData(x, y, myLife, oppLife, torpedoCooldown, sonarCooldown, silenceCooldown, mineCooldown);
		sub.InputSonar(sonarResult);
		sub.InputOrder(opponentOrders);

		// Write an action using cout. DON'T FORGET THE "<< endl"
		// To debug: cerr << "Debug messages..." << endl;
		//sub.PrintField();
		cout << sub.Run()<<"|MSG " + to_string(cup.Total()) << endl;
		if (cup.Total() < 200) cup.PrintList();

	}
}