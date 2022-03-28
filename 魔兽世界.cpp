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
class City;
class Warrior;
void giveTime()
{
	cout << setw(3) << setfill('0') << t / 60 << ':' << setw(2) << setfill('0') << t % 60 <<' ';
}
//类的定义
class Weapon   //武器基类
{
	friend City;
	friend Warrior;
protected:
	double durability;
	double force;
public:	
	int type;  //武器种类
	virtual void strike(Warrior* owner, Warrior* enemy)=0;
	Weapon(int num, Warrior* warr);
};
class sword:public Weapon
{
public:
	sword(int num, Warrior* warr);
	void strike(Warrior* owner, Warrior* enemy);
};
class bomb :public Weapon
{
public:
	bomb(int num, Warrior* warr);
	void strike(Warrior* owner, Warrior* enemy);
};
class arrow:public Weapon
{
public:
	arrow(int num, Warrior* warr);
	void strike(Warrior* owner, Warrior* enemy);
};
class Warrior  //武士基类
{
	friend City;
	friend Weapon;
	friend Game;
	friend Headquarter;
protected:
	int place;  
	int force;
	int totalWeap;
	Weapon* weap[10];
	int weapNum[3];
	Headquarter* camp;
public:
	int HP;	
	bool alive;
	int number; //编号
	int color;
	int type; //0 dragon 、1 ninja、2 iceman、3 lion、4 wolf 
	void sortWeap();
	void initWeap(int totalNum , int num);
	void getWeap(Weapon* weap);
	virtual void marchOn() {
		place += (color == 0 ? 1 : -1);
	}
	Warrior(int Type, Headquarter* base, int num, int hp);
};
class dragon:public Warrior
{
	double morale;
public:
	dragon(int Type, Headquarter* base, int num, int hp ,int baseHP) :Warrior(Type, base, num, hp)
	{
		morale = (double)baseHP / hp;
		initWeap(0, num % 3);
		weapNum[num % 3] ++;
	//	cout <<setprecision(2)<<fixed<< "It has a " << weaponName[weap[0]->type] << ",and it's morale is " << morale << endl;
	}
};
class ninja:public Warrior
{
public:
	ninja(int Type, Headquarter* base, int num, int hp) :Warrior(Type, base, num, hp)
	{
		initWeap(0, num % 3);
		initWeap(1, (num+1) % 3);
		weapNum[num % 3] ++; weapNum[(num +1)% 3] ++;
	//	cout << "It has a " << weaponName[weap[0]->type] << " and a " << weaponName[weap[1]->type] << endl;
	}
};
class lion:public Warrior
{
public:
	int loyalty;
	lion(int Type, Headquarter* base, int num, int hp , int baseHP) :Warrior(Type, base, num, hp)
	{
		initWeap(0, num % 3);
		weapNum[num % 3] ++;
		loyalty = baseHP;
		cout << "Its loyalty is " << loyalty << endl;
	}
	void marchOn()
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
};
class iceman:public Warrior
{
public:
	iceman(int Type, Headquarter* base, int num, int hp):Warrior(Type, base, num, hp)
	{
		initWeap(0, num % 3);
		weapNum[num % 3] ++;
//		cout << "It has a " <<  weaponName[weap[0]->type]<< endl;
	}
	void marchOn()
	{
		place += (color == 0 ? 1 : -1);
		HP -= HP / 10;
	}
};

class City
{
public:
	int number;
//	int warrNum;
	Warrior* warriors[2];
	City(int n) :number(n) 
	{
		warriors[0] = 0;
		warriors[1] = 0;
	};
	void fight();
	void gotIn(Warrior* warr);
	void scareLion();
	void snatch();
	void grab();
	void air();
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
Weapon::Weapon(int num, Warrior* warr)
{
	durability = 100;
	type = num;
}
arrow::arrow(int num, Warrior* warr) :Weapon(num, warr)
{
	force = warrForce[warr->type] * 3 / 10;
}
bomb::bomb(int num, Warrior* warr) :Weapon(num, warr)
{
	force = warrForce[warr->type] * 4 / 10;
}
sword::sword(int num, Warrior* warr) :Weapon(num, warr)
{
	force = warrForce[warr->type] * 2 / 10;
}
void sword::strike(Warrior* owner, Warrior* enemy)
{
	enemy->HP -= force;
	if (enemy->HP <= 0) enemy->alive = false;
}
void bomb::strike(Warrior* owner, Warrior* enemy)
{
	enemy->HP -= force;
	if(owner->type != 1) owner->HP -= force / 2;
	if (enemy->HP <= 0) enemy->alive = false;
	if (owner->HP <= 0) owner->alive = false;
	durability = 0;
}
void arrow::strike(Warrior* owner, Warrior* enemy)
{
	enemy->HP -= force;
	if (enemy->HP <= 0) enemy->alive = false;
	durability -= 50;
}
void City::scareLion()
{
	for(int i = 0 ; i < 2 ; i ++)
		if (warriors[i]!=0 && warriors[i]->type == 3)
		{
			lion* li = (lion*)warriors[i];
			if (li->loyalty > 0) continue;
			giveTime();
			printf("%s lion %d ran away\n", co[warriors[i]->color].c_str(),warriors[i]->number); 
			warriors[i]->alive = false;
			warriors[i] = 0;
		}
}
void Warrior::sortWeap()
{
	int tempWeap = 0; weapNum[0] = weapNum[1] = weapNum[2] = 0;
	for (int i = 0; i < totalWeap; i++)
		if (weap[i] != 0 && weap[i]->durability > 0)
		{
			weap[tempWeap++] = weap[i];
			weapNum[weap[i]->type] ++;
			if (tempWeap != i+1) weap[i] = 0;
		}
		else if (weap[i] != 0) delete weap[i];
	for (int i = 1; i < tempWeap; i++)
		for (int j = tempWeap - 1; j >= i; j--)
			if (weap[j]->type < weap[j - 1]->type || (weap[j]->type == weap[j - 1]->type && weap[j]->durability < weap[j - 1]->durability))
			{
				Weapon* p = weap[j];
				weap[j] = weap[j - 1];
				weap[j - 1] = p;
			}
	totalWeap = tempWeap;
}
void City::grab() //wolf抢夺000:35 blue wolf 2 took 3 bomb from red dragon 2 in city 4
{
	if (warriors[0] == 0 || warriors[1] == 0)return;
	if ((warriors[0]->type == 4 )+(warriors[1]->type == 4) != 1 ) return; //cout << warriors[0]->totalWeap << endl;
	warriors[0]->sortWeap(); warriors[1]->sortWeap();
	int lang = 0;
	if (warriors[1]->type == 4) lang = 1; int bei = 1 - lang;
	if (warriors[bei]->totalWeap == 0) return;
	int weapType = 0; while (warriors[bei]->weapNum[weapType] == 0) weapType++;
	giveTime();
	printf("%s wolf %d took %d %s from %s %s %d in city %d\n", co[lang].c_str(), warriors[lang]->number,
		warriors[bei]->weapNum[weapType], weaponName[weapType].c_str(), co[bei].c_str(), 
		warrName[warriors[bei]->type].c_str(),warriors[bei]->number, number);
	int getNum = warriors[bei]->weapNum[weapType];
	for (int i = getNum - 1; warriors[lang]->totalWeap < 10 && i >= 0; i--)
	{
		warriors[lang]->getWeap(warriors[bei]->weap[i]);
		warriors[bei]->weap[i] = 0;
	}
}
void City::fight()
{
	if (warriors[0] == 0 || warriors[1] == 0)return;
	int redweap = 0, blueweap = 0;		//cout <<number <<' '<< warriors[0]->number << ' ' << warriors[0]->totalWeap << endl;
	warriors[0]->sortWeap(); warriors[1]->sortWeap();
	if (number % 2 == 0 && warriors[1]->totalWeap > 0)
	{
		warriors[1]->weap[0]->strike(warriors[1],warriors[0]);
		blueweap = (blueweap + 1) % warriors[1]->totalWeap;
	}
	int times = 0;//cout << warriors[0]->number <<' '<<redweap << endl;
//	cout << warriors[0]->weap[0]->type << ' ' << warriors[1]->weap[0]->type<<"dsdas" << endl;
	while (warriors[0]->alive && warriors[1]->alive && times < 200)
	{
		times++;
		if (warriors[0]->totalWeap > 0)
		{
			int searchTime = 0;
			while (warriors[0]->weap[redweap]->durability<= 0 && searchTime < warriors[0]->totalWeap)
			{
				redweap = (redweap + 1) % warriors[0]->totalWeap; 
				searchTime++;
			}
			if (searchTime != warriors[0]->totalWeap)
			{
				warriors[0]->weap[redweap]->strike(warriors[0], warriors[1]);// cout << number << "sdfafdsafafdasfaas" << warriors[0]->number <<' '<<warriors[0]->weap[redweap]->type << endl;
				redweap = (redweap + 1) % warriors[0]->totalWeap;
			}
		}
		if (warriors[1]->alive && warriors[1]->totalWeap > 0)
		{
			int searchTime = 0;
			while (warriors[1]->weap[blueweap]->durability <= 0 && searchTime < warriors[1]->totalWeap)
			{
				blueweap = (blueweap + 1) % warriors[1]->totalWeap;
				searchTime++;
			}
			if (searchTime != warriors[1]->totalWeap)
			{
				warriors[1]->weap[blueweap]->strike(warriors[1], warriors[0]); //cout << number << ' ' << warriors[0]->number << redweap << endl;
				blueweap = (blueweap + 1) % warriors[1]->totalWeap;
			}
		}
	}
}
void City::snatch() //战斗胜利后抢夺
{
	if (warriors[0] == 0 || warriors[1] == 0)return;
	if (warriors[0]->alive + warriors[1]->alive != 1) return;
	warriors[0]->sortWeap(); warriors[1]->sortWeap();
	int winner = 0;
	if (warriors[1]->alive) winner = 1; int loser = 1 - winner;
	int sum = 0;
	for (int weapType = 0; weapType <= 2 && warriors[winner]->totalWeap < 10 ; weapType++)
	{
		sum += warriors[loser]->weapNum[weapType];
		for (int i = 0; i < warriors[loser]->weapNum[weapType] && warriors[winner]->totalWeap < 10; i++)
		{
				warriors[winner]->getWeap(warriors[loser]->weap[sum - i - 1]);
				warriors[loser]->weap[sum - i - 1] = 0;
		}
	}
	warriors[0]->sortWeap(); warriors[1]->sortWeap();
}
void City::air()
{
	if (warriors[0] == 0 || warriors[1] == 0) return;
	giveTime();
	if (warriors[0]->alive && warriors[1]->alive) 
		printf("both red %s %d and blue %s %d were alive in city %d\n", warrName[warriors[0]->type].c_str(),
			warriors[0]->number, warrName[warriors[1]->type].c_str(), warriors[1]->number, number);
	//000:40 red iceman 1 killed blue lion 12 in city 2 remaining 20 elements		
	else if (warriors[0]->alive)
		printf("red %s %d killed blue %s %d in city %d remaining %d elements\n", warrName[warriors[0]->type].c_str(),
			warriors[0]->number, warrName[warriors[1]->type].c_str(), warriors[1]->number, number, warriors[0]->HP);
	else if (warriors[1]->alive) 
		printf("blue %s %d killed red %s %d in city %d remaining %d elements\n", warrName[warriors[1]->type].c_str(),
			warriors[1]->number, warrName[warriors[0]->type].c_str(), warriors[0]->number, number, warriors[1]->HP);
	else printf("both red %s %d and blue %s %d died in city %d\n", warrName[warriors[0]->type].c_str(),
			warriors[0]->number, warrName[warriors[1]->type].c_str(), warriors[1]->number, number);
	if (warriors[0]->type == 0 && warriors[0]->alive)
	{
		giveTime();
		printf("red dragon %d yelled in city %d\n", warriors[0]->number, number);
	}
	if (warriors[1]->type == 0 && warriors[1]->alive)//003:40 blue dragon 2 yelled in city 4
	{
		giveTime();
		printf("blue dragon %d yelled in city %d\n",warriors[1]->number,number);
	}
}
void City::report()//000:55 blue wolf 2 has 2 sword 3 bomb 0 arrow and 7 elements
{
	if (warriors[0] != 0 && warriors[0]->alive)
	{
		warriors[0]->sortWeap();
		Warrior* warr = warriors[0];
		giveTime();
		printf("red %s %d has %d sword %d bomb %d arrow and %d elements\n",warrName[warr->type].c_str(),
			warr->number,warr->weapNum[0], warr->weapNum[1], warr->weapNum[2],warr->HP);
		//if (warr->type == 0 && t == 60 * 7 + 55)cout << warr->weap[0]->durability << endl;
	}
	if (warriors[1] != 0 && warriors[1]->alive)
	{
		warriors[1]->sortWeap();
		Warrior* warr = warriors[1];
		giveTime();
		printf("blue %s %d has %d sword %d bomb %d arrow and %d elements\n", warrName[warr->type].c_str(),
			warr->number, warr->weapNum[0], warr->weapNum[1], warr->weapNum[2], warr->HP);
	}
}
void Warrior::initWeap(int totalNum , int num)
{
	switch (num % 3)
	{
	case 0:weap[totalNum] = new sword(num % 3, this); break;
	case 1:weap[totalNum] = new bomb(num % 3, this); break;
	case 2:weap[totalNum] = new arrow(num % 3, this); break;
	default:
		break;
	}
	totalWeap++;
}
void Warrior::getWeap(Weapon* Weap)
{
	initWeap(totalWeap, Weap->type);
	weap[totalWeap-1]->durability = Weap->durability;
	weapNum[Weap->type] ++;
}
void City::gotIn(Warrior* warr)
{
	warriors[warr->color] = warr;
}
Warrior::Warrior(int Type, Headquarter* base, int num, int hp)
{
	weapNum[0] = weapNum[1] = weapNum[2] = 0;
	camp = base;
	color = base->campColor;
	totalWeap = 0;
	place = color * (N + 1);
	number = num;
	HP = hp;
	type = Type;
	force = warrForce[type];
	alive = true;
}
Headquarter::Headquarter(int M, int color)
{
	for (int i = 0; i < 5; i++)
	{
		produceOrder[i] = order[color][i];
		warrNum[i] = 0;
	}
	exhausted = false;
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
		if (warriors[i]!=0 && warriors[i]->alive == true)
		{
			aliveNum++;
			warriors[aliveNum] = warriors[i];
		}
		else
		{
			delete warriors[i];
			warriors[i] = 0;
		}
	}
	for (int i = aliveNum + 1; i <= totalwarr; i++) warriors[i] = 0;
}
void Headquarter::march()
{
	for (int i = 1; i <= totalwarr && warriors[i] != 0; i++)
	{
		warriors[i]->marchOn();
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
	case 0:	warriors[totalwarr] = new dragon(type,this,totalwarr,warrHP[type] , currHP); break;
	case 1: warriors[totalwarr] = new ninja(type, this, totalwarr, warrHP[type]); break;
	case 2: warriors[totalwarr] = new iceman(type, this, totalwarr, warrHP[type]); break;
	case 3: warriors[totalwarr] = new lion(type, this, totalwarr, warrHP[type] ,currHP); break;
	case 4: warriors[totalwarr] = new wolf(type, this, totalwarr, warrHP[type]); break;
	default:;
	}
	currtype++; currtype %= 5;
}
void Headquarter::broadcast()
{
	giveTime();
	cout << currHP << " elements in " << co[campColor] << " headquarter" << endl;
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
	for (int i = 1; i <= N; i++)
	{
		citys[i]->warriors[0] = 0;
		citys[i]->warriors[1] = 0;
	}
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
//		citys[pla]->warrNum = (citys[pla]->warriors[0] != 0) + (citys[pla]->warriors[1] != 0);
	}
	
}
void Game::outputAfterMarch(Headquarter& redBase, Headquarter& blueBase) //红方基地信号，city信号，蓝方信号;判断游戏结束
{
	if (win[1] == true)
	{
		giveTime();
		cout << "blue " <<warrName[blueBase.warriors[1]->type]<<' ' <<blueBase.warriors[1]->number << " reached red headquarter with "
			<< blueBase.warriors[1]->HP << " elements and force " << blueBase.warriors[1]->force << endl;
		giveTime();
		printf("red headquarter was taken\n");
		//red iceman 1 reached blue headquarter with 20 elements and force 30
	}
	for (int i = 1; i <= N; i++)
	{
		if (citys[i]->warriors[0] != 0)
		{//000:10 red iceman 1 marched to city 1 with 20 elements and force 30
			giveTime();
			cout << "red " << warrName[citys[i]->warriors[0]->type]<< ' ' <<citys[i]->warriors[0]->number << " marched to city " << i << " with "
				<< citys[i]->warriors[0]->HP << " elements and force " << citys[i]->warriors[0]->force << endl;
		}
		if (citys[i]->warriors[1] != 0)
		{
			giveTime();
			cout << "blue " << warrName[citys[i]->warriors[1]->type] << ' ' << citys[i]->warriors[1]->number << " marched to city " << i << " with "
				<< citys[i]->warriors[1]->HP << " elements and force " << citys[i]->warriors[1]->force << endl;
		}
	}
	if (win[0] == true)
	{
		giveTime();
		cout << "red " << warrName[redBase.warriors[1]->type] << ' ' << redBase.warriors[1]->number << " reached blue headquarter with "
			<< redBase.warriors[1]->HP << " elements and force " << redBase.warriors[1]->force << endl;
		giveTime();
		printf("blue headquarter was taken\n");
	}
}
void Game::play()
{		
	cout << "Case " << round <<':' << endl;
	Headquarter redBase(headHP,0), blueBase(headHP,1);
	while (t <= T && !win[0] && !win[1]) 
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
			redBase.updateQueue();blueBase.updateQueue();
			break;
		}
		case 10:
		{
			redBase.march();
			blueBase.march();
			//还需处理特殊物种updateQueue
			enterCity(redBase , blueBase);
			outputAfterMarch(redBase, blueBase);
			if (win[0] || win[1]) break;
			break;
		}
		case 35:
		{
			for (int i = 1; i <= N; i++) citys[i]->grab();
			break;
		}
		case 40:
		{
			for (int i = 1; i <= N; i++) citys[i]->fight();
			for (int i = 1; i <= N; i++) citys[i]->snatch();
			for (int i = 1; i <= N; i++) citys[i]->air();
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