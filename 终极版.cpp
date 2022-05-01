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
	bool inHead;
	int force;
	int HP;
	bool alive;
	int number; //编号
	int color;
	int type; //0 dragon 、1 ninja、2 iceman、3 lion、4 wolf 
	void initWeap(int num);
	virtual void marchOn() {
		place += (color == 0 ? 1 : -1);
	}
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
		cout <<setprecision(2)<<fixed << "Its morale is " << morale << endl;
	}
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
		countStep = 0;
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
	void airFight();
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
	void report();
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
	void wholeFight(Headquarter* redBase, Headquarter* blueBase);
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
	if (enemy->HP <= 0) enemy->alive = false;
	force = force * 8 / 10;
}
void bomb::strike(Warrior* owner, Warrior* enemy)
{
	enemy->alive = false;
	owner->alive = false;
	durability = 0;
	cout << setw(3) << setfill('0') << t / 60 << ':' << 38 << ' ';
	printf("%s %s %d used a bomb and killed %s %s %d\n", co[owner->color].c_str(), warrName[owner->type].c_str(), owner->number,
		co[enemy->color].c_str(), warrName[enemy->type].c_str(), enemy->number);
}
void arrow::strike(Warrior* owner, Warrior* enemy)
{
	enemy->HP -= R;
	if (enemy->HP <= 0) enemy->alive = false;
	durability -= 1;
	giveTime();
	if (enemy->alive == false) printf("%s %s %d shot and killed %s %s %d\n", co[owner->color].c_str(), warrName[owner->type].c_str(), owner->number,
		co[enemy->color].c_str(), warrName[enemy->type].c_str(), enemy->number);
	else printf("%s %s %d shot\n", co[owner->color].c_str(), warrName[owner->type].c_str(), owner->number);
}
void City::scareLion()
{
	for (int i = 0; i < 2; i++)
		if (warriors[i] != 0 && warriors[i]->type == 3)
		{
			lion* li = (lion*)warriors[i];
		//	cout << li->loyalty << endl;
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
	if (fr->ws != 0 && fr->ws->force == 0) fr->ws = 0;
	fe->HP -= fr->force;
	giveTime();
	printf("%s %s %d attacked %s %s %d in city %d with %d elements and force %d\n", co[fr->color].c_str(), warrName[fr->type].c_str(), fr->number,
		co[fe->color].c_str(), warrName[fe->type].c_str(), fe->number, number, fr->HP, fr->force);
	if (fe->HP <= 0)
	{
		fe->alive = false;
		if (fe->type == 3)
		{
			lion* li = (lion*)fe;
			fr->HP += li->lastHP;
		}
		giveTime();
		printf("%s %s %d was killed in city %d\n", co[fe->color].c_str(), warrName[fe->type].c_str(), fe->number, number);
		return;
	}
	if (fe->type == 1) return;
	if (fe->ws != 0) fe->ws->strike(fe, fr);
	if (fe->ws != 0 && fe->ws->force == 0) fe->ws = 0;
	fr->HP -= (fe->force / 2);
	giveTime();
	printf("%s %s %d fought back against %s %s %d in city %d\n", co[fe->color].c_str(), warrName[fe->type].c_str(), fe->number,
		co[fr->color].c_str(), warrName[fr->type].c_str(), fr->number, number);
	if (fr->HP <= 0)
	{
		if (fr->type == 3)
		{
			lion* li = (lion*)fr;
			fe->HP += li->lastHP;
		}
		fr->alive = false;
		giveTime();
		printf("%s %s %d was killed in city %d\n", co[fr->color].c_str(), warrName[fr->type].c_str(), fr->number, number);
	}
}
void City::headGainHP()
{
	if (preWarrNum == 1 && HP > 0)
	{
		Warrior* warr = warriors[0] == 0 ? warriors[1] : warriors[0];
		giveTime();
		printf("%s %s %d earned %d elements for his headquarter\n", co[warr->color].c_str(), warrName[warr->type].c_str(), warr->number, HP);
		warr->camp->currHP += HP;
		HP = 0;
	}
	else if (warriors[0] != 0 && warriors[1] != 0 && warriors[0]->alive + warriors[1]->alive == 1 && HP > 0)
	{
		Warrior* warr = warriors[0]->alive ? warriors[0] : warriors[1];
		giveTime();
		printf("%s %s %d earned %d elements for his headquarter\n", co[warr->color].c_str(), warrName[warr->type].c_str(), warr->number, HP);
		warr->camp->currHP += HP;
		HP = 0;
	}

}
void City::bombAttack()
{
	Warrior* fr = warriors[flag];
	Warrior* fe = warriors[1 - flag];
	if (fe == 0 || fr == 0 || !fr->alive || !fe->alive) return;
	int virtualHP = fe->HP;
	if(fr->ws != 0) virtualHP -= fr->ws->force;
	virtualHP -= fr->force; 
	if (virtualHP <= 0 && fe->wb != 0)
	{
		fe->wb->strike(fe, fr);
		return;
	}
	if (fe->type == 1) return;
	virtualHP = fr->HP;
	if(fe->ws != 0) virtualHP -= fe->ws->force;
	virtualHP -= (fe->force / 2);
	if (virtualHP <= 0 && fr->wb != 0)
	{
		fr->wb->strike(fr, fe);
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
		if (flag == 0 && dra->morale > 0.8)
		{
			giveTime();
			printf("%s %s %d yelled in city %d\n", co[dra->color].c_str(), warrName[dra->type].c_str(), dra->number, number);
		}
	}
	if (warriors[1]->type == 0 && warriors[1]->alive)
	{
		dragon* dra = (dragon*)warriors[1];
		if (!warriors[0]->alive) dra->morale -= 0.2;
		else dra->morale += 0.2;
		if (flag == 1 && dra->morale > 0.8)
		{
			giveTime();
			printf("%s %s %d yelled in city %d\n", co[dra->color].c_str(), warrName[dra->type].c_str(), dra->number, number);
		}
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
		if (warRecord >= 0) warRecord = -1;
		else warRecord -= 1;
	}
	else
	{
		if (warRecord <= 0) warRecord = 1;
		else warRecord += 1;
	}
	if (warRecord == 2)
	{
		flag = 1;
		giveTime();
		printf("blue flag raised in city %d\n", number);
	}
	else if (warRecord == -2)
	{
		flag = 0;
		giveTime();
		printf("red flag raised in city %d\n", number);
	}
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
			if (fromCity->warriors[0]->wa->durability == 0) fromCity->warriors[0]->wa = 0;
			if (fromCity->warriors[0]->wa != 0 && i != N && citys[i + 1]->warriors[1] != 0)
			{
				fromCity->warriors[0]->wa->strike(fromCity->warriors[0], citys[i + 1]->warriors[1]);
				if (fromCity->warriors[0]->wa->durability == 0) fromCity->warriors[0]->wa = 0;
			}
		}
		else if (fromCity->warriors[1] != 0 && fromCity->warriors[1]->wa != 0)
		{
			if (i != 1 && citys[i - 1]->warriors[0] != 0)
				fromCity->warriors[1]->wa->strike(fromCity->warriors[1], citys[i - 1]->warriors[0]);
			if (fromCity->warriors[1]->wa->durability == 0) fromCity->warriors[1]->wa = 0;
			if (fromCity->warriors[1]->wa != 0 && i != N && citys[i + 1]->warriors[0] != 0)
			{
				fromCity->warriors[1]->wa->strike(fromCity->warriors[1], citys[i - 1]->warriors[0]);
				if (fromCity->warriors[1]->wa->durability == 0) fromCity->warriors[1]->wa = 0;
			}
		}
	}

}
Warrior::Warrior(int Type, Headquarter* base, int num, int hp)
{
	inHead = false;
	wa = 0; ws = 0; wb = 0;
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
		else if(warriors[i] != 0)
		{
			//delete warriors[i];
			warriors[i] = 0;
		}
	}
	for (int i = aliveNum + 1; i <= totalwarr; i++) warriors[i] = 0; 
}
void Headquarter::march()
{
	for (int i = 1; i <= totalwarr && warriors[i] != 0; i++)
	{
		if (warriors[i]->place == 0 &&  campColor == 1 || warriors[i]->place == N+1 && campColor == 0) continue;
		warriors[i]->marchOn();
	}
}
void Headquarter::recordHP()
{
	for (int i = 1; i <= aliveNum; i++)
	{
		if (warriors[i]->type != 3) return;		
		lion* li = (lion*)warriors[i];
		if (li->HP < 0) li->HP = 0;
		li->lastHP = li->HP;
	}
}
void Headquarter::produce()       
{
	int type = produceOrder[currtype];
	if (warrHP[type] > currHP) return;
	currHP -= warrHP[type];
	warrNum[type]++;
	totalwarr++;	aliveNum++;
	giveTime();
	cout << co[campColor] << ' ' << warrName[type] << ' ' << totalwarr << " born" << endl;
	currtype++; currtype %= 5;
	switch (type)
	{
	case 0:	warriors[totalwarr] = new dragon(type, this, totalwarr, warrHP[type], currHP); break;
	case 1: warriors[totalwarr] = new ninja(type, this, totalwarr, warrHP[type]); break;
	case 2: warriors[totalwarr] = new iceman(type, this, totalwarr, warrHP[type]); break;
	case 3: warriors[totalwarr] = new lion(type, this, totalwarr, warrHP[type], currHP); break;
	case 4: warriors[totalwarr] = new wolf(type, this, totalwarr, warrHP[type]); break;
	}

}

void Headquarter::report()
{
	for (int j = 1; j <= aliveNum; j++)
	{
		int i = campColor==1 ? j:aliveNum - j + 1;
		giveTime();
		printf("%s %s %d has ", co[campColor].c_str(), warrName[warriors[i]->type].c_str(), warriors[i]->number);
		int weapnum = 0; Warrior* warr = warriors[i];
		if (warr->wa != 0)
		{
			cout << "arrow(" << warr->wa->durability << ")";
			weapnum++;
		}
		if (warr->wb != 0)
		{
			if (weapnum > 0) cout << ',';
			cout << "bomb";
			weapnum++;
		}
		if (warr->ws != 0)
		{
			if (weapnum > 0) cout << ',';
			cout << "sword(" << warr->ws->force <<')';
			weapnum++;
		}
		if (weapnum == 0) cout << "no weapon";
		cout << endl;
	}
}
void Headquarter::broadcast()
{
	giveTime();
	cout << currHP << " elements in " << co[campColor] << " headquarter" << endl;
}
Game::Game(int num)
{
	round = num;
	cin >> headHP >> N >> R >> K >> T;
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
		if (pla != 0) citys[pla]->gotIn(blueBase.warriors[i]);
	}
	for (int i = 1; i <= redBase.aliveNum; i++)
	{
		int pla = redBase.warriors[i]->place;
		if (pla != N + 1) citys[pla]->gotIn(redBase.warriors[i]);
	}

}
void Game::outputAfterMarch(Headquarter& redBase, Headquarter& blueBase) //红方基地信号，city信号，蓝方信号;判断游戏结束
{
	if (win[1] == 1 && (blueBase.aliveNum > 1 && blueBase.warriors[2]->place == 0)|| 
		win[1] == 0 && (blueBase.aliveNum > 0 && blueBase.warriors[1]->place == 0))
	{
		Warrior* warr = blueBase.warriors[win[1] + 1];
		giveTime();
		cout << "blue " << warrName[warr->type] << ' ' << warr->number << " reached red headquarter with "
			<< warr->HP << " elements and force " << warr->force << endl;	
		win[1] ++;
		if (win[1] == 2)
		{
			giveTime();
			printf("red headquarter was taken\n");
			//red iceman 1 reached blue headquarter with 20 elements and force 30
		}
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
	if (win[0] == 1 && (redBase.aliveNum > 1 && redBase.warriors[2]->place == N+1) 
		|| win[0] == 0 && (redBase.aliveNum > 0 && redBase.warriors[1]->place == N+1))
	{
		Warrior* warr = redBase.warriors[win[0] + 1];
		giveTime();
		cout << "red " << warrName[warr->type] << ' ' << warr->number << " reached blue headquarter with "
			<< warr->HP << " elements and force " << warr->force << endl;
		win[0] ++;
		if (win[0] == 2)
		{
			giveTime();
			printf("blue headquarter was taken\n");
		}
	}
}
void Game::wholeFight(Headquarter* redBase , Headquarter* blueBase)
{
	for (int i = 1; i <= N; i++)
	{
		citys[i]->fight();
		citys[i]->grab();
		citys[i]->dragonYell();
		citys[i]->lionFear();
		citys[i]->headGainHP();
		citys[i]->changeFlag();
	}
	for (int i = 1; i <= N; i++) citys[i]->getAward(blueBase);
	for (int i = N; i >= 1; i--) citys[i]->getAward(redBase);
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
			wholeFight(&redBase,&blueBase);
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
			redBase.updateQueue(); blueBase.updateQueue(); cout << "kk" << endl;
			redBase.report(); blueBase.report();
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