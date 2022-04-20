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
int t = 0, h = 0;//计时
int K; //lion忠诚度下降速度
int N; //城市数
int R; //arrow攻击力
class Headquarter;
class Game;
class City;
class Warrior;
void giveTime()
{
	cout << setw(3) << setfill('0') << t / 60 << ':' << setw(2) << setfill('0') << t % 60 << ' ';
}
//类的定义
class sword
{
public:
	int force;
	sword(int num, Warrior* warr);
	void strike(Warrior* owner, Warrior* enemy);
};
class bomb 
{
public:
	int durability;
	bomb(int num, Warrior* warr);
	void strike(Warrior* owner, Warrior* enemy);
};
class arrow 
{
public:
	int durability;
	arrow(int num, Warrior* warr);
	void strike(Warrior* owner, Warrior* enemy);
};
class Warrior  //武士基类
{
	friend City;
	friend Game;
	friend Headquarter;
protected:
	arrow* wa; bomb* wb; sword* ws;
	int place;
	int totalWeap;
	Headquarter* camp;
public:
	int force;
	int HP;
	bool alive;
	int number; //编号
	int color;
	int type; //0 dragon 、1 ninja、2 iceman、3 lion、4 wolf 
	void sortWeap();
	void initWeap(int num);
	virtual void marchOn() {
		place += (color == 0 ? 1 : -1);
	}
	void batter(Warrior* enemy);
	Warrior(int Type, Headquarter* base, int num, int hp);
};
class dragon :public Warrior
{
public:
	double morale;
	dragon(int Type, Headquarter* base, int num, int hp, int baseHP) :Warrior(Type, base, num, hp)
	{
		morale = (double)baseHP / hp;
		initWeap(num % 3);
		cout <<setprecision(2)<<fixed << "It's morale is " << morale << endl;
	}
	void cheer();
};
class ninja :public Warrior
{
public:
	ninja(int Type, Headquarter* base, int num, int hp) :Warrior(Type, base, num, hp)
	{
		initWeap(num % 3);
		initWeap((num + 1) % 3);
	}
};
class lion :public Warrior
{
public:
	int loyalty;
	int lastHP;
	lion(int Type, Headquarter* base, int num, int hp, int baseHP) :Warrior(Type, base, num, hp)
	{
		loyalty = baseHP;
		lastHP = hp;
		cout << "Its loyalty is " << loyalty << endl;
	}
};
class wolf :public Warrior
{
public:
	wolf(int Type, Headquarter* base, int num, int hp) :Warrior(Type, base, num, hp)
	{
	}
};
class iceman :public Warrior
{
	int countStep;
public:
	iceman(int Type, Headquarter* base, int num, int hp) :Warrior(Type, base, num, hp)
	{
		initWeap(num % 3);
	}
	void marchOn()
	{
		countStep++; countStep %= 2;
		place += (color == 0 ? 1 : -1);
		if (countStep == 0)
		{
			HP -= 9;
			if (HP < 1) HP = 1;
			force += 20;
		}
	}
};

class City
{
public:
	int number;
	int HP;
	int preWarrNum;
	int warRecord;
	int flag; 
	bool existFight;
	Warrior* warriors[2];
	City(int n) :number(n)
	{
		warRecord = 0;
		flag = (number + 1) % 2;
		preWarrNum = 0;
		HP = 0;
		warriors[0] = 0;
		warriors[1] = 0;
	};
	void fight();
	void gotIn(Warrior* warr);
	void scareLion();
	void grab();
	void air();
	void report();
	void headGainHP();
	void bombAttack();
	void getAward(Headquarter* head);
	void dragonYell();
	void lionFear();
	void changeFlag();
};

class Headquarter //基地
{
	friend Game;
	friend Warrior;
	friend dragon;
	friend iceman;
	friend ninja;
	friend lion;
	friend City;
private:
	int produceOrder[5];//dragon 、ninja、iceman、lion、wolf
	int campColor; //1 blue,0 red 
	int totalwarr; //士兵总数
	int aliveNum; //当前存活数
	int warrNum[5];//dragon 、ninja、iceman、lion、wolf
	int currHP; //剩余生命元
	int currtype;
	void recordHP();
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
	int win[2];
public:
	City* citys[100];
	void enterCity(Headquarter& redBase, Headquarter& blueBase);
	void outputAfterMarch(Headquarter& redBase, Headquarter& blueBase);
	void play();
	void shoot();
	void afterFight(Headquarter* redBase, Headquarter* blueBase);
	Game(int num);
};

//各种类函数的定义
arrow::arrow(int num, Warrior* warr)
{
	durability = 3;
}
bomb::bomb(int num, Warrior* warr) 
{
	durability = 1;
}
sword::sword(int num, Warrior* warr) 
{
	force = warrForce[warr->type] * 2 / 10;
}
void sword::strike(Warrior* owner, Warrior* enemy)
{
	enemy->HP -= force;
	force = force * 8 / 10;
}
void bomb::strike(Warrior* owner, Warrior* enemy)
{
	enemy->alive = false;
	owner->alive = false;
	durability = 0;
	printf
}
void arrow::strike(Warrior* owner, Warrior* enemy)
{
	enemy->HP -= R;
	if (enemy->HP <= 0) enemy->alive = false;
	durability -= 1;
	printf
}
void City::scareLion()
{
	for (int i = 0; i < 2; i++)
		if (warriors[i] != 0 && warriors[i]->type == 3)
		{
			lion* li = (lion*)warriors[i];
			if (li->loyalty > 0) continue;
			giveTime();
			printf("%s lion %d ran away\n", co[warriors[i]->color].c_str(), warriors[i]->number);
			warriors[i]->alive = false;
			warriors[i] = 0;
		}
}

void City::grab() //wolf抢夺000:35 blue wolf 2 took 3 bomb from red dragon 2 in city 4
{
	if (warriors[0] == 0 || warriors[1] == 0)return;
	if ((warriors[0]->type == 4) + (warriors[1]->type == 4) != 1) return;
	warriors[0]->sortWeap(); warriors[1]->sortWeap();
	int lang = 0;
	if (warriors[1]->type == 4) lang = 1; int bei = 1 - lang;
	if (warriors[bei]->alive == true) return;
	if (warriors[lang]->wa == 0 && warriors[bei]->wa != 0) warriors[lang]->wa = warriors[bei]->wa;
	if (warriors[lang]->wb == 0 && warriors[bei]->wb != 0) warriors[lang]->wb = warriors[bei]->wb;
	if (warriors[lang]->ws == 0 && warriors[bei]->ws != 0) warriors[lang]->ws = warriors[bei]->ws;
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
		printf("blue dragon %d yelled in city %d\n", warriors[1]->number, number);
	}
}
void City::report()//000:55 blue wolf 2 has 2 sword 3 bomb 0 arrow and 7 elements
{
	if (warriors[0] != 0 && warriors[0]->alive)
	{
		warriors[0]->sortWeap();
		Warrior* warr = warriors[0];
		giveTime();
		printf("red %s %d has %d sword %d bomb %d arrow and %d elements\n", warrName[warr->type].c_str(),
			warr->number, warr->weapNum[0], warr->weapNum[1], warr->weapNum[2], warr->HP);
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
void Warrior::initWeap(int num)
{
	switch (num % 3)
	{
	case 0:ws = new sword(num % 3, this); break;
	case 1:wb = new bomb(num % 3, this); break;
	case 2:wa = new arrow(num % 3, this); break;
	default:
		break;
	}
	totalWeap++;
}

void City::gotIn(Warrior* warr)
{
	warriors[warr->color] = warr;
	preWarrNum++;
}
void City::fight()//输出进攻反击与战死（被箭射死不算）
{
	Warrior* fr = warriors[flag];
	Warrior* fe = warriors[1 - flag];
	if (fe == 0 || fr == 0 || !fr->alive || !fe->alive) return;
	if(fr->ws != 0 ) fr->ws->strike(fr, fe);
	fe->HP -= fr->force;
	if (fe->HP <= 0)
	{
		输出，取生命元
	}
	if (fe->type == 1) return;
	if (fe->ws != 0) fe->ws->strike(fe, fr);
	fr->HP -= (fe->force / 2);
	if (fr->HP <= 0)
	{

	}
	
}
void City::headGainHP()
{
	if (preWarrNum == 1)
	{
		Warrior* warr = warriors[0] == 0 ? warriors[1] : warriors[0];
		warr->camp->currHP += HP;
		HP = 0;
	}
}
void City::bombAttack()
{
	Warrior* fr = warriors[flag];
	Warrior* fe = warriors[1 - flag];
	if (fe == 0 || fr == 0 || !fr->alive || !fe->alive) return;
	if (fr->ws != 0) fr->ws->strike(fr, fe);
	fe->HP -= fr->force;
	if (fe->HP <= 0)
	{
		if (fe->wb != 0)
		{
			fe->alive = false; fe->HP = 0;
			fr->alive = false; fr->HP = 0;
		}
		return;
	}
	if (fe->type == 1) return;
	if (fe->ws != 0) fe->ws->strike(fe, fr);
	fr->HP -= (fe->force / 2);
	if (fr->HP <= 0)
	{
		if (fr->wb != 0)
		{
			fe->alive = false; fe->HP = 0;
			fr->alive = false; fr->HP = 0;
		}
		return;
	}
}
void City::getAward(Headquarter* head)
{
	int color = head->campColor;
	if (warriors[0] == 0 || warriors[1] == 0 || !warriors[color]->alive || warriors[1-color]->alive) return;
	if (head->currHP >= 8)
	{
		warriors[color]->HP += 8;
		head->currHP -= 8;
	}
}
void City::dragonYell()
{
	if (warriors[0] == 0 || warriors[1] == 0 || warriors[0]->type != 0 && warriors[1]->type != 0) return;
	if (warriors[0]->type == 0 && warriors[0]->alive)
	{
		dragon* dra = (dragon*)warriors[0];
		if (!warriors[1]->alive) dra->morale -= 0.2;
		else dra->morale += 0.2;
		if(flag == 0 && dra->morale > 0.8) printf
	}
	if (warriors[1]->type == 0 && warriors[1]->alive)
	{
		dragon* dra = (dragon*)warriors[1];
		if (!warriors[0]->alive) dra->morale -= 0.2;
		else dra->morale += 0.2;
		if (flag == 1 && dra->morale > 0.8) printf
	}
}
void City::lionFear()
{
	if (warriors[0] == 0 || warriors[1] == 0 ||(warriors[0]->type!=3 && warriors[1]->type!=3)) return;
	if (warriors[0]->type == 3 && warriors[1]->type != 3)
	{
		lion* li = (lion*)warriors[0];
		if (warriors[1]->alive) li->loyalty -= K;
		if (!li->alive) warriors[1]->HP += li->lastHP;
	}
	else if (warriors[0]->type == 3 && warriors[0]->type != 3)
	{
		lion* li = (lion*)warriors[1];
		if (warriors[1]->alive) li->loyalty -= K;
		if (!li->alive) warriors[0]->HP += li->lastHP;       //判断生死用的是alive，故不会复生
	}
	else
	{
		lion* li1 = (lion*)warriors[0]; lion* li2 = (lion*)warriors[1];
		if (li1->alive && li2->alive)
		{
			li1->loyalty -= K;
			li2->loyalty -= K;
		}
		else if (li1->alive) li2->HP += li1->lastHP;
		else li2->HP += li1->lastHP;
	}
}
void City::changeFlag()
{
	if (warriors[0] == 0 || warriors[1] == 0) return;
	if (warriors[0]->alive + warriors[1]->alive != 1) warRecord = 0;
	else if (warriors[0]->alive)
	{
		if (warRecord > 0) warRecord = -1;
		else warRecord -= 1;
	}
	else
	{
		if (warRecord < 0) warRecord = 1;
		else warRecord += 1;
	}
	if (warRecord >= 2) flag = 1;
	else flag = 0;
}
void Game::shoot()
{
	for (int i = 1; i <= N; i++)
	{
		City* fromCity = citys[i];
		if (fromCity->warriors[0] != 0 && fromCity->warriors[0]->wa != 0)
		{
			if (i != 1 && citys[i - 1]->warriors[1] != 0)
				fromCity->warriors[0]->wa->strike(fromCity->warriors[0],citys[i - 1]->warriors[1]);
			if(i != N && citys[i + 1]->warriors[1] != 0)
				fromCity->warriors[0]->wa->strike(fromCity->warriors[0], citys[i + 1]->warriors[1]);
		}
		else if (fromCity->warriors[1] != 0 && fromCity->warriors[1]->wa != 0)
		{
			if (i != 1 && citys[i - 1]->warriors[0] != 0)
				fromCity->warriors[1]->wa->strike(fromCity->warriors[1], citys[i - 1]->warriors[0]);
			if (i != N && citys[i + 1]->warriors[0] != 0)
				fromCity->warriors[1]->wa->strike(fromCity->warriors[1], citys[i - 1]->warriors[0]);
		}
	}

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
	currHP = M;
	campColor = color;
	aliveNum = 0;
	totalwarr = 0;
	currtype = 0;
}
void Headquarter::updateQueue()
{
	aliveNum = 0;
	for (int i = 1; i <= totalwarr; i++)
	{
		if (warriors[i] != 0 && warriors[i]->alive == true)
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
void Headquarter::recordHP()
{
	for (int i = 1; i <= totalwarr; i++)
	{
		if (warriors[i]->type != 3) return;		
		lion* li = (lion*)warriors[i];
		li->lastHP = li->HP;
	}
}
void Headquarter::produce()         //-----------------------------------------需要新的生成法则
{
	int type = produceOrder[currtype];
	if (warrHP[type] > currHP) return;
	currHP -= warrHP[type];
	warrNum[type]++;
	totalwarr++;	aliveNum++;
	giveTime();
	cout << co[campColor] << ' ' << warrName[type] << ' ' << totalwarr << " born" << endl;
	switch (type)
	{
	case 0:	warriors[totalwarr] = new dragon(type, this, totalwarr, warrHP[type], currHP); break;
	case 1: warriors[totalwarr] = new ninja(type, this, totalwarr, warrHP[type]); break;
	case 2: warriors[totalwarr] = new iceman(type, this, totalwarr, warrHP[type]); break;
	case 3: warriors[totalwarr] = new lion(type, this, totalwarr, warrHP[type], currHP); break;
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
	win[0] = win[1] = 0;
}
void Game::enterCity(Headquarter& redBase, Headquarter& blueBase)
{
	for (int i = 1; i <= N; i++)
	{
		citys[i]->preWarrNum = 0;
		citys[i]->warriors[0] = 0;
		citys[i]->warriors[1] = 0;
	}
	for (int i = 1; i <= blueBase.aliveNum; i++)
	{
		int pla = blueBase.warriors[i]->place;
		if (pla == 0) win[1] ++;
		else citys[pla]->gotIn(blueBase.warriors[i]);
	}
	for (int i = 1; i <= redBase.aliveNum; i++)
	{
		int pla = redBase.warriors[i]->place;
		if (pla == N + 1)	win[0] = true;
		else citys[pla]->gotIn(redBase.warriors[i]);
	}

}
void Game::outputAfterMarch(Headquarter& redBase, Headquarter& blueBase) //红方基地信号，city信号，蓝方信号;判断游戏结束
{
	if (win[1] == true)
	{
		giveTime();
		cout << "blue " << warrName[blueBase.warriors[1]->type] << ' ' << blueBase.warriors[1]->number << " reached red headquarter with "
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
			cout << "red " << warrName[citys[i]->warriors[0]->type] << ' ' << citys[i]->warriors[0]->number << " marched to city " << i << " with "
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
void Game::afterFight(Headquarter* redBase , Headquarter* blueBase)
{
	for (int i = 1; i <= N; i++) citys[i]->grab();
	for (int i = 1; i <= N; i++) citys[i]->dragonYell();
	for (int i = 1; i <= N; i++) citys[i]->lionFear();
	for (int i = 1; i <= N; i++) citys[i]->getAward(blueBase);
	for (int i = N; i >= 1; i--) citys[i]->getAward(redBase);
	for (int i = 1; i <= N; i++) citys[i]->headGainHP();
	for (int i = 1; i <= N; i++) citys[i]->changeFlag();
}
void Game::play()
{
	cout << "Case " << round << ':' << endl;
	Headquarter redBase(headHP, 0), blueBase(headHP, 1);
	while (t <= T && win[0] < 2 && win[1] < 2)
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
			enterCity(redBase, blueBase);
			outputAfterMarch(redBase, blueBase);
			if (win[0] == 2 || win[1] == 2) break;
			for (int i = 1; i <= N; i++) citys[i]->HP += 10;
			break;
		}
		case 30:
		{
			for (int i = 1; i <= N; i++) citys[i]->headGainHP();
			break;
		}
		case 35:
		{
			shoot();
			for (int i = 1; i <= N; i++) citys[i]->bombAttack();
			redBase.recordHP(); blueBase.recordHP();
			break;
		}
		case 40:
		{
			for (int i = 1; i <= N; i++) citys[i]->fight();
			afterFight(&redBase,&blueBase);
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