#include<iostream>
using namespace std;
#include<string>
#include<iomanip>
#include<algorithm>
string warrName[5] = { "dragon","ninja","iceman","lion","wolf" };
string weaponName[3] = { "sword","bomb","arrow" };
string co[2] = { "red" , "blue" };
int order[2][5] = {
2,3,4,1,0,
3,0,1,2,4 };

int warrHP[5] = { 0 };
int n;
int t = 0;
class Headquarter;
class Game;
class weapon
{
protected:

	int durability;
public:	
	int type;
	weapon(int num)
	{
		type = num;
	}
};

class Warrior
{
protected:
	int type; //0 dragon 、1 ninja、2 iceman、3 lion、4 wolf 
	int HP;
	int number; //编号
	int place;
	Headquarter* camp;
public:
	Warrior(int Type, Headquarter* base , int num , int hp)
	{
		number = num;
		camp = base;
		HP = hp;
		type = Type;
	}
};
class dragon:public Warrior
{
	double morale;
	weapon* weap;
public:
	dragon(int Type, Headquarter* base, int num, int hp ,int baseHP) :Warrior(Type, base, num, hp)
	{
		morale = (double)baseHP / hp;
		weap = new weapon(num % 3);
		cout <<setprecision(2)<<fixed<< "It has a " << weaponName[weap->type] << ",and it's morale is " << morale << endl;
	}
};
class ninja:public Warrior
{
	weapon* weap[2];
public:
	ninja(int Type, Headquarter* base, int num, int hp) :Warrior(Type, base, num, hp)
	{
		weap[0] = new weapon(num % 3);
		weap[1] = new weapon((num + 1) % 3);
		cout << "It has a " << weaponName[weap[0]->type] << " and a " << weaponName[weap[1]->type] << endl;
	}
};
class lion:public Warrior
{
	int loyalty;
public:
	lion(int Type, Headquarter* base, int num, int hp , int baseHP) :Warrior(Type, base, num, hp)
	{
		loyalty = baseHP;
		cout << "It's loyalty is " << loyalty << endl;
	}
};
class wolf:public Warrior
{
public:
	wolf(int Type, Headquarter* base, int num, int hp):Warrior(Type, base, num, hp)
	{

	}
};
class iceman:public Warrior
{
	weapon* weap;
public:
	iceman(int Type, Headquarter* base, int num, int hp):Warrior(Type, base, num, hp)
	{
		weap = new weapon(num % 3);
		cout << "It has a " <<  weaponName[weap->type]<< endl;
	}
};
class Headquarter
{
	friend dragon;
	friend iceman;
	friend ninja;
	friend lion;
private:
	int produceOrder[5];//dragon 、ninja、iceman、lion、wolf
	int campColor; //1 blue,0 red 
	int totalwarr; //士兵总数
	int warrNum[5];//dragon 、ninja、iceman、lion、wolf
	int currHP; //剩余生命元
	int currtype;

	Warrior* warriors[1000];
public:	
	bool isExhausted;
	Headquarter(int color, int M);
	bool exhaust();
	void produce();
};

Headquarter::Headquarter(int M, int color)
{
	for (int i = 0; i < 5; i++)
	{
		produceOrder[i] = order[color][i];
		warrNum[i] = 0;
	}
	isExhausted = false;
	currHP = M;
	campColor = color;
	totalwarr = 0;
	currtype = 0;
}
bool Headquarter::exhaust()
{
	for (int i = 0; i < 5; i++)
	{
		if (warrHP[i] <= currHP) return false;
	}
	return true;
}
void Headquarter::produce()
{
	if (exhaust() == true)
	{
		if (isExhausted == false)
		{
			cout << setw(3) << setfill('0') << t;
			cout << ' ' << co[campColor] << " headquarter stops making warriors" << endl;
			isExhausted = true;
		}
		return;
	}
	while (warrHP[produceOrder[currtype]] > currHP)
	{
		currtype = (currtype + 1) % 5;	//cout << ' ' << currHP <<' '<< endl;
	}
	cout << setw(3) << setfill('0') << t;
	int type = produceOrder[currtype];
	currHP -= warrHP[type];
	warrNum[type]++;
	totalwarr++;	
	cout << ' ' << co[campColor] << ' ' << warrName[type] << ' ' << totalwarr << " born with strength " << warrHP[type] 
		<< ','<< warrNum[type] << ' ' << warrName[type] << " in " << co[campColor] << " headquarter" << endl;
	switch (type)
	{
	case 0:	warriors[totalwarr] = new dragon(type,this,totalwarr,warrHP[type] , currHP); break;
	case 1: warriors[totalwarr] = new ninja(type, this, totalwarr, warrHP[type]); break;
	case 2: warriors[totalwarr] = new iceman(type, this, totalwarr, warrHP[type]); break;
	case 3: warriors[totalwarr] = new lion(type, this, totalwarr, warrHP[type] ,currHP); break;
	case 4: warriors[totalwarr] = new wolf(type, this, totalwarr, warrHP[type]); break;
	default:;
	}
	currtype++; currtype %= 5;
}

class Game
{
	friend Headquarter;
private:
	int headHP;
	int round;
public:
	void play();
	Game(int num);
};
Game::Game(int num)
{
	round = num;
	cin >> headHP;
	for (int j = 0; j < 5; j++)
	{
		cin >> warrHP[j];
	}
	t = 0;
}
void Game::play()
{
	cout << "Case:" << round << endl;
	t = 0;
	Headquarter redBase(headHP,0), blueBase(headHP,1);
	while (redBase.isExhausted != true || blueBase.isExhausted != true)
	{
		redBase.produce();
		blueBase.produce();
		t++;
	}
}
int main()
{
	cin >> n;
	for (int i = 1; i <= n; i++)
	{
		Game game(i);
		game.play();
	}
	system("pause");
	return 0;
}