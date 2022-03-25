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
int warrForce[5] = { 0 }; //攻击力
int T;
int t = 0,h = 0;//计时
int K; //lion忠诚度下降速度
int N; //城市数
class Headquarter;
class Game;

void giveTime()
{
	cout << setw(3) << setfill('0') << t / 60 << ':' << setw(2) << setfill('0') << t % 60 <<' ';
}
//类的定义
class weapon   //武器基类
{
protected:

	double durability;
	double force;
public:	
	int type;  //武器种类
//	virtual void attack();
	weapon(int num)
	{
		type = num;
	}
//	virtual void attack();
};
class sword:public weapon
{
	sword(int num , int vio) :weapon(num)
	{
		
	}
};
class bomb :public weapon
{

};
class arrow:public weapon
{

};
class Warrior  //武士基类
{
	friend Game;
	friend Headquarter;
protected:
	int HP;
	int place;  
	int weapInHand;
	int weapNum;
	int force;
	weapon* weap[10];
	Headquarter* camp;
public:	
	bool alive;
	int number; //编号
	int color;
	int type; //0 dragon 、1 ninja、2 iceman、3 lion、4 wolf 
	void grabWeap();
	void sortWeap();
	virtual void martchOn() {};
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
	void martchOn()
	{
		place += (color == 0 ? 1 : -1);
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
	void martchOn()
	{
		place += (color == 0 ? 1 : -1);
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
	void martchOn()
	{
		place += (color == 0 ? 1 : -1);
		loyalty -= K;
	}
};
class wolf:public Warrior
{
public:
	wolf(int Type, Headquarter* base, int num, int hp):Warrior(Type, base, num, hp)
	{

	}
	void martchOn()
	{
		place += (color == 0 ? 1 : -1);
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
	void martchOn()
	{
		place += (color == 0 ? 1 : -1);
		HP -= HP / 10;
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

class Headquarter //基地
{
	friend Game;
	friend Warrior;
	friend dragon;
	friend iceman;
	friend ninja;
	friend lion;
private:
	int produceOrder[5];//dragon 、ninja、iceman、lion、wolf
	int campColor; //1 blue,0 red 
	int totalwarr; //士兵总数
	int aliveNum; //当前存活数
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
//class Message
//{
//public:
//	string text;
//	int occurTime;
//	int 
//	Message();
//};
//class MessageArrange
//{
//	int num;
//public:
//	MessageArrange();
//	void addMess();
//	void sortMess();
//	void output();
//};
class Game //游戏大类
{
	friend Headquarter;
private:
	int headHP;
	int round;
	bool win[2];
public:
	City* citys[100];
	void enterCity(Headquarter& redBase , Headquarter& blueBase);
	void outputAfterMarch(Headquarter& redBase, Headquarter& blueBase);
	void play();
	Game(int num);
};

//各种类函数的定义
void City::scareLion()
{
	for(int i = 0 ; i < 2 ; i ++)
		if (warriors[i]!=0 && warriors[i]->type == 3)
		{

			giveTime();
			printf("%s lion %d ran away", co[warriors[i]->color].c_str(),warriors[i]->number); 
			warriors[i]->alive = false;
			warriors[i] = 0;
		}
}
void City::gotIn(Warrior* warr)
{
	warriors[warr->color] = warr;
}
Warrior::Warrior(int Type, Headquarter* base, int num, int hp)
{
	camp = base;
	color = base->campColor;
	weapNum = 0;
	place = color * (N + 1);
	number = num;
	HP = hp;
	type = Type;
	force = warrForce[type];
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
	aliveNum = 0;
	totalwarr = 0;
	currtype = 0;
	occupied = false;
}
void Headquarter::updateQueue()
{
	aliveNum = 0;
	for (int i = 1; i <= totalwarr; i++)
	{
		if (warriors[i]->alive == true)
		{
			aliveNum++;
			warriors[aliveNum] = warriors[i];
		}
		else delete warriors[i];
	}
	for (int i = aliveNum + 1; i <= totalwarr; i++) warriors[i] = 0;
}
void Headquarter::march()
{
	for (int i = 1; i <= totalwarr && warriors[i] != 0; i++)
	{
		warriors[i]->martchOn();
	}
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
	giveTime();
	currHP -= warrHP[type];
	warrNum[type]++;
	totalwarr++;	aliveNum++;
	cout << co[campColor] << ' ' << warrName[type] << ' ' << totalwarr << " born" << endl;
	switch (type)
	{
	case 0:	warriors[aliveNum] = new dragon(type,this,totalwarr,warrHP[type] , currHP); break;
	case 1: warriors[aliveNum] = new ninja(type, this, totalwarr, warrHP[type]); break;
	case 2: warriors[aliveNum] = new iceman(type, this, totalwarr, warrHP[type]); break;
	case 3: warriors[aliveNum] = new lion(type, this, totalwarr, warrHP[type] ,currHP); break;
	case 4: warriors[aliveNum] = new wolf(type, this, totalwarr, warrHP[type]); break;
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
		cin >> warrForce[j];
	for (int i = 1; i <= N; i++)
		citys[i] = new City(i);
	t = 0;
	win[0] = win[1] = false;
}
void Game::enterCity(Headquarter& redBase, Headquarter& blueBase)
{
	for (int i = 1; i <= blueBase.aliveNum; i++)
	{
		int pla = blueBase.warriors[i]->place;
		if (pla == 0)
		{
			win[1] = true;
			continue;
		}
		citys[pla]->gotIn(blueBase.warriors[i]);
	}
	for (int i = 1; i <= redBase.aliveNum; i++)
	{
		int pla = redBase.warriors[i]->place;
		if (pla == N + 1)
		{
			win[0] = true;
			continue;
		}
		citys[pla]->gotIn(redBase.warriors[i]);
		citys[pla]->warrNum = (citys[pla]->warriors[0] != 0) + (citys[pla]->warriors[1] != 0);
	}
	
}
void Game::outputAfterMarch(Headquarter& redBase, Headquarter& blueBase) //红方基地信号，city信号，蓝方信号;判断游戏结束
{
	if (win[1] == true)
	{
		giveTime();
		cout << "blue " << blueBase.warriors[0]->number << " reached red headquarter with " 
			<< blueBase.warriors[0]->HP << " elements and force " << blueBase.warriors[0]->force << endl;
		//red iceman 1 reached blue headquarter with 20 elements and force 30
	}
	for (int i = 1; i <= N; i++)
	{
		if (citys[i]->warriors[0] != 0)
		{//000:10 red iceman 1 marched to city 1 with 20 elements and force 30
			giveTime();
			cout << "red " << citys[i]->warriors[0]->number << " marched to city "<<i<<" with "
				<< citys[i]->warriors[0]->HP << " elements and force " << citys[i]->warriors[0]->force << endl;
		}
		if (citys[i]->warriors[1] != 0)
		{
			giveTime();
			cout << "blue " << citys[i]->warriors[1]->number << " marched to city " << i << " with "
				<< citys[i]->warriors[1]->HP << " elements and force " << citys[i]->warriors[1]->force << endl;
		}
	}
	if (win[0] == true)
	{
		giveTime();
		cout << "red " << redBase.warriors[0]->number << " reached blue headquarter with "
			<< redBase.warriors[0]->HP << " elements and force " << redBase.warriors[0]->force << endl;
	}
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
			redBase.updateQueue(); blueBase.updateQueue();
			break;
		}
		case 10:
		{
			redBase.march();
			blueBase.march();
			//还需处理特殊物种updateQueue
			enterCity(redBase , blueBase);
			outputAfterMarch(redBase, blueBase);
			break;
		}
		//case 35:
		//{
		//	for (int i = 1; i <= N; i++) citys[i]->grab();
		//	break;
		//}
		//case 40:
		//{
		//	for (int i = 1; i <= N; i++) citys[i]->fight();
		//	for (int i = 1; i <= N; i++) citys[i]->yell();
		//	break;
		//}
		//case 50:
		//{
		//	redBase.broadcast();
		//	blueBase.broadcast();
		//	break;
		//}
		//case 55:
		//{
		//	for (int i = 1; i <= N; i++) citys[i]->report();
		//	break;
		//}
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