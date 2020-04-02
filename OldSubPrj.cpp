#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

using namespace std;

    int width; //ширина поля
    int height;  // высота поля
    int myId;  // идентификатор
    string map[20/* height*/];
    string map1[20];
    char Course = 'W';
    char Sc ='1';
bool Look (int x,int y) // ОПРЕДЕЛЯЕТ возможность перемещения на эту клетку
{
    if (x >= width || x < 0 || y >= height || y < 0) return false; // если выходит за границы
    if (map[y][x] == 'x') return false; // если на клетке суша
    if (map1[y][x] == Sc) return false; //проверку  обратного хода
    return true;
}
bool LookNaw(int x, int y, string naw)  // определяет возможность движения в этом направлении
{
    if (naw == "S") return Look(x, y + 1);
    if (naw == "W") return Look(x - 1, y);
    if (naw == "N") return Look(x, y - 1);
    if (naw == "E") return Look(x + 1, y);
    return false;
}
bool LookTarget (int x,int y) // проверка на пуск торпеды
{
    if (x >= width || x < 0 || y >= height || y < 0) return false; // если выходит за границы
    if (map[y][x] == 'x') return false; // если на клетке суша
    return true;
}
string Kurs (int x, int y)
{
    if (Look (x , y + 1)) return "S";
    if (Look (x - 1 , y)) return "W";
    if (Look (x , y - 1)) return "N";
    if (Look (x + 1 , y)) return "E";
    return " ";
}
string Tor (int x, int y) // координаты пуска торпеды
{   
    if (LookTarget (x , y + 2)) return to_string(x) + " " + to_string(y+2);
    if (LookTarget (x - 2 , y)) return to_string(x-2) + " " + to_string(y);
    if (LookTarget (x , y - 2)) return to_string(x) + " " + to_string(y-2);
    if (LookTarget (x + 2 , y)) return to_string(x+2) + " " + to_string(y);
    return "nobody";
}
string Silence(int x, int y)
{
    int s = 0, w = 0, n = 0, e = 0;
    bool sb = true; 
    bool wb = true; 
    bool nb = true; 
    bool eb = true;
    for (int i = 0; i < 4; i++) {
        if (sb && LookNaw(x, y + i, "S") == true) s++; else sb = false;
        if (wb && LookNaw(x - i, y, "W") == true) w++; else wb = false;
        if (nb && LookNaw(x, y - i, "N") == true) n++; else nb = false;
        if (eb && LookNaw(x + i, y, "E") == true) e++; else eb = false;
    }
    int max = s;
    string naw = "S";
    if (w > max) { max=w; naw = "W"; }
    if (n > max) { max=n; naw = "N"; }
    if (e > max) { max=e; naw = "E"; }
        cerr << s << w << n << e << endl;
        return naw;// to_string(max);

}
bool Paint(int x, int y, string naw)
{
    char n = naw[0];
    int d = naw[2] - '0';
    for (int i = 1; i < d; i++) {
        if (n == 'S') y++;
        if (n == 'W') x--;
        if (n == 'N') y--;
        if (n == 'E') x++;
        map1[y][x] = Sc;   // маркер посещения клетки
    }
    return true;
}
int main()
{
    cin >> width >> height >> myId; cin.ignore();

    for (int i = 0; i < height; i++) {
            getline(cin , map[i]);
            map1[i] = map[i];
    }
    int x,y;
    x = 15;
    y = 14;

do { // поиск стартовой позиции
    x--;
    if (x < 0) {
        x = 14; 
        y--;
    }
} while (Look(x,y) == false);

    map1[y][x] = Sc;   // маркер посещения клетки
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
        // Write an action using cout. DON'T FORGET THE "<< endl"
        // To debug: cerr << "Debug messages..." << endl;
        map1[y][x] = Sc;
        string t = Kurs (x,y);
        string mov;
        string wp = "";
        string energy = "";
        //cerr << torpedoCooldown << endl;

        if (t == " ") {             // если плыть некуда - всплываем
            mov = "SURFACE";Sc++;
        }
        else {
            string sil = Silence(x, y);
            if (silenceCooldown == 0 && sil != "1") {
                    mov = "SILENCE " + sil +" 1"; // если есть тихий ход, даем на 2
                    Paint(x, y, sil + " 1");// тут надо закрасить тихий ход
                }
            
            else {
                mov = "MOVE " + sil; //если тихого нет - плывем
            }
        }
        /*
        if (torpedoCooldown == 0){  // если торпеда готова стреляем торпедой
            string t = Tor (x,y);
            if (t != "nobody") {wp = "TORPEDO " + t +"|"; energy = " TORPEDO";}
        }*/
        if (silenceCooldown != 0)
            energy = " SILENCE";     
        if (sonarCooldown != 0)
            energy = " SONAR";
        if (torpedoCooldown != 0) // заряжаем торпеды если они не заряжены
            energy = " TORPEDO";
            
        if (silenceCooldown == 0){ //уходим в тишину
            
        
        }
        cout << wp << mov << energy << endl;
            
    }
}