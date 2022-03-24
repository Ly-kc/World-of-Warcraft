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
int n;
int warrHP[5] = { 0 }; //初始生命
int force[5] = { 0 }; //攻击力
int T;
int t = 0,h = 0;//计时
int K; //lion忠诚度下降速度
int N; //城市数
class Headquarter;
class Game;

//类的定义
class weapon   //武器基类
{
protected:

	double durability;
	double force;
public:	
	int type;  //武器种类
	weapon(int num)
	{
		type = num;
	}
//	virtual void attack();
};

class Warrior  //武士基类
{
protected:

	bool marched;
	int HP;
	int place;  
	int weapInHand;
	int weapNum;
	weapon* weap[10];
	Headquarter* camp;
public:	
	bool alive;
	int number; //编号
	int color;
	int type; //0 dragon 、1 ninja、2 iceman、3 lion、4 wolf 
	void grabWeap();
	void sortWeap();
	Warrior(int Type, Headquarter* base, int num, int hp);
};
class dragon:public Warrior
{
	double morale;
public:
	dragon(int Type, Headquarter* base, int num, int hp ,int baseHP) :Warrior(Type, base, num, hp)
	{
		morale = (double)baseHP / hp;
		weap[0] = new weapon(num % 3);
		cout <<setprecision(2)<<fixed<< "It has a " << weaponName[weap[0]->type] << ",and it's morale is " << morale << endl;
	}
};
class ninja:public Warrior
{
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
public:
	iceman(int Type, Headquarter* base, int num, int hp):Warrior(Type, base, num, hp)
	{
		weap[0] = new weapon(num % 3);
		cout << "It has a " <<  weaponName[weap[0]->type]<< endl;
	}
};

class City
{
public:
	int number;
	int warrNum;
	Warrior* warriors[2];
	City(int n) :number(n) 
	{
		warriors[0] = 0;
		warriors[1] = 0;
	};
	void fight();
	void gotIn(Warrior* warr);
	void scareLion();
	void grab();
	void yell();
	void report();
};
void City::scareLion()
{
	for(int i = 0 ; i < 2 ; i ++)
		if (warriors[i]!=0 && warriors[i]->type == 3)
		{

			cout << setw(3) << setfill('0') << t / 60 << ':' << setw(2) << setfill('0') << t % 60;
			printf(" %s lion %d ran away", co[warriors[i]->color].c_str(),warriors[i]->number); 
			warriors[i]->alive = false;
			warriors[i] = 0;
		}
}
class Headquarter //基地
{
	friend Warrior;
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
	bool occupied;
	void updateQueue();
	void march();
	bool exhausted;
	Headquarter(int color, int M);
	void produce();
	void broadcast();
};


class Game //游戏大类
{
	friend Headquarter;
private:
	int headHP;
	int round;
public:
	City* citys[100];
	void play();
	Game(int num);
};

//各种类函数的定义

Warrior::Warrior(int Type, Headquarter* base, int num, int hp)
{
	camp = base;
	color = base->campColor;
	marched = false;
	weapNum = 0;
	place = 0;
	number = num;
	HP = hp;
	type = Type;
}
Headquarter::Headquarter(int M, int color)
{
	for (int i = 0; i < 5; i++)
	{
		produceOrder[i] = order[color][i];
		warrNum[i] = 0;
	}
//	isExhausted = false;
	currHP = M;
	campColor = color;
	totalwarr = 0;
	currtype = 0;
	occupied = false;
}

void Headquarter::produce()
{
	if (exhausted == true) return;
	int type = produceOrder[currtype];	
	if (warrHP[type] > currHP)
	{
		exhausted = true;
		return;
	}
	cout << setw(3) << setfill('0') << t / 60 << ':' << setw(2) << setfill('0') << t % 60;
	currHP -= warrHP[type];
	warrNum[type]++;
	totalwarr++;	
	cout << ' ' << co[campColor] << ' ' << warrName[type] << ' ' << totalwarr << " born" << endl;
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

Game::Game(int num)
{
	round = num;
	cin >> headHP >> N >> K >> T;
	for (int j = 0; j < 5; j++)
		cin >> warrHP[j];
	for (int j = 0; j < 5; j++)
		cin >> force[j];
	for (int i = 1; i <= N; i++)
		citys[i] = new City(i);
	t = 0;
}
void Game::play()
{		
	cout << "Case:" << round << endl;
	Headquarter redBase(headHP,0), blueBase(headHP,1);
	while (t <= T && true) 
	{
		switch (t % 60)
		{
		case 0:
		{
			redBase.produce();
			blueBase.produce();
			break;
		}
		case 5:
		{
			for (int i = 1; i <= N; i++) citys[i]->scareLion();
			break;
		}
		case 10:
		{
			redBase.march();
			blueBase.march();
		}
		case 35:
		{
			for (int i = 1; i <= N; i++) citys[i]->grab();
			break;
		}
		case 40:
		{
			for (int i = 1; i <= N; i++) citys[i]->fight();
			for (int i = 1; i <= N; i++) citys[i]->yell();
			break;
		}
		case 50:
		{
			redBase.broadcast();
			blueBase.broadcast();
			break;
		}
		case 55:
		{
			for (int i = 1; i <= N; i++) citys[i]->report();
			break;
		}
		default:
			break;
		}
		t += 5;
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