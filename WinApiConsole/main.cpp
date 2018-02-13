#include <windows.h>
#include <iostream>
#include <vector>
#include <list>
#include <conio.h>
#include <string>
#include <algorithm>
#include "Dronegine.h"

#define KEY_UP 0x26
#define KEY_DOWN 0x28
#define KEY_LEFT 0x25
#define KEY_RIGHT 0x27
#define SPACEBAR 0x20
#define Y_BUTTON 0x59
#define N_BUTTON 0x4E

using namespace std;

struct DR_POINT
{
	int x;
	int y;
};

bool operator>(const DR_POINT &p1, const DR_POINT &p2)
{
	if (p1.x > p2.x && p1.y > p2.y)
	{
		return true;
	}
	else
	{
		return false;
	}
		
}

bool operator<(const DR_POINT &p1, const DR_POINT &p2)
{
	if (p1.x < p2.x && p1.y < p2.y)
	{
		return true;
	}
	else
	{
		return false;
	}

}

bool operator<=(const DR_POINT &p1, const DR_POINT &p2)
{
	if (p1.x <= p2.x && p1.y <= p2.y)
	{
		return true;
	}
	else
	{
		return false;
	}

}

bool operator>=(const DR_POINT &p1, const DR_POINT &p2)
{
	if (p1.x >= p2.x && p1.y >= p2.y)
	{
		return true;
	}
	else
	{
		return false;
	}

}

class Corneroid : public Dronegine
{
private:
	struct Collider;
	struct Bullet;
	struct Ship;

public:
	Corneroid()
	{

	}

	~Corneroid()
	{
	}

	void SetShipCoordinate(float x, float y)
	{
		CharachterShip.x = x;
		CharachterShip.y = y;
	}

	void UpdateCollider(Ship *sh)
	{
		sh->collider.TOPLEFT = { (int)sh->x, (int)sh->y };
		sh->collider.TOPRIGHT = { (int)sh->x + sh->cubeSize - 1, (int)sh->y };
		sh->collider.BOTTOMLEFT = { (int)sh->x, (int)sh->y + sh->cubeSize - 1 };
		sh->collider.BOTTOMRIGHT = { (int)sh->x + sh->cubeSize - 1, (int)sh->y + sh->cubeSize - 1 };
	}

	bool CheckShipCollision(Ship ship, Ship target)
	{
		if (ship.collider.TOPLEFT >= target.collider.TOPLEFT && ship.collider.TOPLEFT <= target.collider.BOTTOMRIGHT)
		{
			return true;
		}

		if (ship.collider.TOPRIGHT >= target.collider.TOPLEFT && ship.collider.TOPRIGHT <= target.collider.BOTTOMRIGHT)
		{
			return true;
		}

		if (ship.collider.BOTTOMLEFT >= target.collider.TOPLEFT && ship.collider.BOTTOMLEFT <= target.collider.BOTTOMRIGHT)
		{
			return true;
		}

		if (ship.collider.BOTTOMRIGHT >= target.collider.TOPLEFT && ship.collider.BOTTOMRIGHT <= target.collider.BOTTOMRIGHT)
		{
			return true;
		}

		return false;
	}

	bool CheckBulletCollision(Bullet *bul, Ship *sh)
	{
		if (DR_POINT{ (int)bul->x, (int)bul->y } >= sh->collider.TOPLEFT && DR_POINT{ (int)bul->x, (int)bul->y } <= sh->collider.BOTTOMRIGHT)
		{
			sh->x = -100;
			bul->x = -200;
			Score += 100;
			return true;
		}

		return false;
	}

	void Input(float fElapsedtime)
	{
		if (m_keys[KEY_UP].bHeld == true)
		{
			if ((CharachterShip.y - (CharachterShip.speed * fElapsedtime)) > 0)
				CharachterShip.y -= CharachterShip.speed * fElapsedtime;
		}

		if (m_keys[KEY_DOWN].bHeld == true)
		{
			if ((CharachterShip.y + (CharachterShip.speed * fElapsedtime)) + CharachterShip.cubeSize - 1< GetHeight())
				CharachterShip.y += CharachterShip.speed * fElapsedtime;
		}

		if (m_keys[KEY_LEFT].bHeld == true)
		{
			if ((CharachterShip.x - (CharachterShip.speed * fElapsedtime)) > 0)
				CharachterShip.x -= CharachterShip.speed * fElapsedtime;
		}

		if (m_keys[KEY_RIGHT].bHeld == true)
		{
			if ((CharachterShip.x + (CharachterShip.speed * fElapsedtime)) + CharachterShip.cubeSize - 1  < GetWidth())
				CharachterShip.x += CharachterShip.speed * fElapsedtime;
		}

		if (m_keys[SPACEBAR].bHeld == true)
		{
			if (BulletsCount > 0 && CharachterShip.TimeBetweenShoot >= 0.25f)
			{
				Bullets.push_back({ CharachterShip.x + (CharachterShip.cubeSize / 2), CharachterShip.y - 1, 30, '|' });
				BulletsCount--;
				CharachterShip.TimeBetweenShoot = 0;
			}
		}
	}

	void CreateNewEnemyShips()
	{
		if (EnemyShips.size() <= 5 && TimeBetweenNewShip >= 1)
		{		
			EnemyShips.push_back({ (float)(rand() % (GetWidth() - 5)) + 5, (float)3, 3, 5 });
			TimeBetweenNewShip = 0;	
		}
	}

	virtual bool Update(float fElapsedtime)
	{
		if (IsNewGame)
		{
			WriteString(GetWidth() / 2 - 10, GetHeight() / 2, "ARE YOU READY?", FG_GREEN);
			WriteString(GetWidth() / 2 - 10, GetHeight() / 2 + 2, "Y - FOR YES", FG_GREEN);
			WriteString(GetWidth() / 2 - 10, GetHeight() / 2 + 4, "(Actually, you don't have a choice)", FG_GREEN);

			if (m_keys[Y_BUTTON].bHeld == true)
			{
				IsNewGame = false;			
				IsDead = false;
				IsGame = true;
				SetShipCoordinate(GetWidth() / 2, GetHeight() - 5);
				BulletsCount = 100;
				Lives = 5;
				Sleep(100);
			}
			return true;
		}

		if (IsDead)
		{
			WriteString(GetWidth() / 2 - 10, GetHeight() / 2, "U R DEAD", FG_RED);
			WriteString(GetWidth() / 2 - 10, GetHeight() / 2 + 8, "AGAIN?", FG_GREEN);
			WriteString(GetWidth() / 2 - 10, GetHeight() / 2 + 10, "Y - FOR YES", FG_GREEN);
			WriteString(GetWidth() / 2 - 10, GetHeight() / 2 + 12, "N - FOR NO", FG_GREEN);

			if (m_keys[Y_BUTTON].bHeld == true)
			{
				IsNewGame = true;
				IsDead = false;
				IsGame = false;

				for (auto &sh : EnemyShips)
				{
					sh = Ship{ (float)(rand() % GetWidth()), (float)3, 3, 5 };
				}

				Sleep(100);
			}

			if (m_keys[N_BUTTON].bHeld == true)
			{
				IsNewGame = false;
				IsDead = false;
				IsGame = false;
			}
			return true;
		}

		if (IsGame)
		{
			// Проверяем ввод с клавиатуры
			Input(fElapsedtime);

			//Обновляем коллайдер корабля игрока
			UpdateCollider(&CharachterShip);
			
			//Проверяем столкновение нашего корабля с врагами
			for (auto &sh : EnemyShips)
			{
				if (CheckShipCollision(CharachterShip, sh))
				{
					IsGame = false;
					IsDead = true;

					if (Score > BestScore)
					{
						BestScore = Score;
					}

					Score = 0;
				}
			}

			//Обновляем время между высрелам
			CharachterShip.TimeBetweenShoot += fElapsedtime;

			//обновляем корабли
			CreateNewEnemyShips();

			
			TimeBetweenNewShip += fElapsedtime;

			// Рисуем корабль игрока
			DrawCube(CharachterShip.x, CharachterShip.y, CharachterShip.cubeSize, 'X');

			//рисуем вражеские корабли
			for (auto &sh : EnemyShips)
			{
				sh.y += sh.speed * fElapsedtime;
				UpdateCollider(&sh);
				DrawCube(sh.x, sh.y, sh.cubeSize, 'X', FG_YELLOW);

				if (sh.collider.BOTTOMRIGHT.y >= GetHeight() + 2)
					Lives--;
			}

			//Проверяем количество жизней у игрока
			if (Lives == 0)
			{
				IsGame = false;
				IsDead = true;

				if (Score > BestScore)
				{
					BestScore = Score;
				}

				Score = 0;
			}

			//рисуем пули
			for (auto &Bul : Bullets)
			{
				Bul.y -= Bul.speed * fElapsedtime;
				Draw(Bul.x, Bul.y, Bul.symbol);

				for (auto &sh : EnemyShips)
				{
					CheckBulletCollision(&Bul, &sh);
				}
			}

			//Удаляем корабли вышедшие за экран
			if (EnemyShips.size() > 0)
			{
				auto i = remove_if(EnemyShips.begin(), EnemyShips.end(),
					[&](Ship ship) {return ship.x < 0 || ship.y < 0 || ship.x > GetWidth() || ship.y > GetHeight(); });
				if (i != EnemyShips.end())
				{
					EnemyShips.erase(i);
				}
			}		

			// Удаляем пули вышедшие за экран
			if (Bullets.size() > 0)
			{
				auto i = remove_if(Bullets.begin(), Bullets.end(),
					[&](Bullet bul) {return bul.x < 0 || bul.y < 0 || bul.x > GetWidth() || bul.y > GetHeight(); });
				if (i != Bullets.end())
				{
					Bullets.erase(i);
				}
			}


			//WriteString(5, GetHeight() - 2, "Collided?: " + str);

			/*WriteString(20, GetHeight() - 23, "ME");
			WriteString(20, GetHeight() - 21, "TOPLEFT: " + to_string(CharachterShip.collider.TOPLEFT.x) + "   " + to_string(CharachterShip.collider.TOPLEFT.y));
			WriteString(20, GetHeight() - 19, "TOPRIGHT: " + to_string(CharachterShip.collider.TOPRIGHT.x) + "   " + to_string(CharachterShip.collider.TOPRIGHT.y));
			WriteString(20, GetHeight() - 17, "BOTTOMLEFT: " + to_string(CharachterShip.collider.BOTTOMLEFT.x) + "   " + to_string(CharachterShip.collider.BOTTOMLEFT.y));
			WriteString(20, GetHeight() - 15, "BOTTOMRIGHT: " + to_string(CharachterShip.collider.BOTTOMRIGHT.x) + "   " + to_string(CharachterShip.collider.BOTTOMRIGHT.y));*/

			/*WriteString(20, GetHeight() - 13, "ENEMY");
			WriteString(20, GetHeight() - 11, "TOPLEFT: " + to_string(EnemyShip.collider.TOPLEFT.x) + "   " + to_string(EnemyShip.collider.TOPLEFT.y));
			WriteString(20, GetHeight() - 9, "TOPRIGHT: " + to_string(EnemyShip.collider.TOPRIGHT.x) + "   " + to_string(EnemyShip.collider.TOPRIGHT.y));
			WriteString(20, GetHeight() - 7, "BOTTOMLEFT: " + to_string(EnemyShip.collider.BOTTOMLEFT.x) + "   " + to_string(EnemyShip.collider.BOTTOMLEFT.y));
			WriteString(20, GetHeight() - 5, "BOTTOMRIGHT: " + to_string(EnemyShip.collider.BOTTOMRIGHT.x) + "   " + to_string(EnemyShip.collider.BOTTOMRIGHT.y));*/

			WriteString(GetWidth() - 50, GetHeight() - 2, "Lives: " + to_string(Lives), FG_RED);
			WriteString(1, 1, "Score: " + to_string(Score) + " Best Score: " + to_string(BestScore), FG_GREEN);
			WriteString(2, GetHeight() - 2, "FPS: " + to_string(1 / fElapsedtime));
			WriteString(GetWidth() - 20, GetHeight() - 2, "Bullets: " + to_string(BulletsCount));
			return true;
		}

		return false;
	}

public:	
	int BulletsCount = 100;
	int Score = 0;
	int BestScore = 0;
	int Lives = 5;
	float TimeBetweenNewShip = 0;
	bool IsGame = false;
	bool IsDead = false;
	bool IsNewGame = true;
private:

	struct Collider
	{
		DR_POINT TOPLEFT;
		DR_POINT TOPRIGHT;
		DR_POINT BOTTOMLEFT;
		DR_POINT BOTTOMRIGHT;
	};

	struct Bullet
	{
		float x;
		float y;
		int speed;
		char symbol;
	};

	struct Ship
	{
		float x;
		float y;
		int cubeSize = 3;
		int speed = 20;
		float TimeBetweenShoot;
		Collider collider;
	};

	Ship CharachterShip;
	vector <Ship> EnemyShips;
	vector <Bullet> Bullets;
};


int main()
{

	Corneroid dr;

	dr.ConstructConsole(80, 50);
	//dr.ConstructConsole(120, 90);
	dr.SetShipCoordinate(10, 10);
	dr.Start();

	return 1;
}



