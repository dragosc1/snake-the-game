#include <iostream>
#include <conio.h>
#include <cstdlib>
#include <ctime>
#include <windows.h>
#include <map>
using namespace std;

bool GAMEON = 1;
int SCORE;

const int Width = 41;
const int Height = 21;

const int di[] = {-1, 0, 1, 0};
const int dj[] = {0, 1, 0, -1};

char Map[Height + 3][Width + 3];

void ClearScreen() {
    HANDLE hOut;
    COORD Position;
    hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    Position.X = Position.Y = 0;
    SetConsoleCursorPosition(hOut, Position);
}

struct Point {
    int x, y;
    bool operator< (const Point &b) const {
        if (x < b.x)
            return 1;
        else if (x == b.x && y < b.y)
            return 1;
        return 0;
    }
    bool operator!= (const Point &b) const {
        if (x != b.x)
            return 1;
        else if (x == b.x && y != b.y)
            return 1;
        return 0;
    }
};

map<Point, bool> unused;

class Snake {
private:
    int lenght;
    Point cell[(Height + 3) * (Width + 3) + 1];
public:
    int getlen() {
        return lenght;
    }
    Point getcellat(int ind) {
        return cell[ind];
    }
    void setlen(int val) {
        lenght = val;
    }
    void setcellat(int ind, Point val) {
        cell[ind] = val;
        Map[val.x][val.y] = '*';
        if (unused.find(val) != unused.end())
            unused.erase(unused.find(val));
    }
} snake;

void InitSnake() {
    snake.setlen(1);
    snake.setcellat(1, {(Height + 3) / 2, (Width + 3) / 2});
}

void InitUnused() {
    int i, j;
    for (i = 2; i <= Height + 1; i++)
        for (j = 2; j <= Width + 1; j++)
            unused[{i, j}] = 1;
}

void AddFruit() {
    int x = rand() % unused.size();
    map<Point, bool>::iterator it = unused.begin();
    advance(it, x);
    Map[it->first.x][it->first.y] = 'o';
}

void BuildMap() {
    int i, j;
    for (i = 1; i <= Width + 2; i++)
        Map[1][i] = '#';
    for (i = 1; i <= Height; i++) {
        Map[i + 1][1] = '#';
        for (j = 1; j <= Width; j++)
            Map[i + 1][j + 1] = ' ';
        Map[i + 1][j + 1] = '#';
    }
    for (i = 1; i <= Width + 2; i++)
        Map[Height + 2][i] = '#';
}

void Build() {
    BuildMap();
    InitUnused();
    InitSnake();
    AddFruit();
}

void ShowMap() {
    int i, j;
    for (i = 1; i <= Height + 2; i++, cout << '\n')
        for (j = 1; j <= Width + 2; j++)
            cout << Map[i][j];
    cout << "\n                SCORE : " << SCORE;
}

void MOVE(int key) {
    if (key == -1)
        return;
    Point newc = snake.getcellat(1);
    newc.x += di[key], newc.y += dj[key];
    if (Map[newc.x][newc.y] == '#' || (Map[newc.x][newc.y] == '*' && newc != snake.getcellat(snake.getlen()))) {
        GAMEON = 0;
        return;
    }
    else if (Map[newc.x][newc.y] == 'o') {
        snake.setlen(snake.getlen() + 1);
        if (unused.find(newc) != unused.end())
            unused.erase(unused.find(newc));
        AddFruit();
        SCORE++;
        unused[newc] = 1;
    }
    else {
        Point tail = snake.getcellat(snake.getlen());
        Map[tail.x][tail.y] = ' ';
        unused[tail] = 1;
    }
    for (int i = snake.getlen(); i >= 2; i--)
        snake.setcellat(i, snake.getcellat(i - 1));
    snake.setcellat(1, newc);
}

void RUN(int key) {
    if (GAMEON == 0) {
        system("cls");
        cout << "GAME OVER!!!";
        return;
    }
    ClearScreen();
    Sleep(125);
    MOVE(key);
    ShowMap();
    if (kbhit()) {
        switch(getch()) {
            case 'w':
                if (key != 2)
                    RUN(0);
                else RUN(key);
                break;
            case 's':
                if (key != 0)
                    RUN(2);
                else RUN(key);
                break;
            case 'a':
                if (key != 1)
                    RUN(3);
                else RUN(key);
                break;
            case 'd':
                if (key != 3)
                    RUN(1);
                else RUN(key);
                break;
            default:
                RUN(key);
                break;
        }
    }
    else RUN(key);
}

int main() {
    srand(time(0));
    Build();
    RUN(-1);
    return 0;
}
