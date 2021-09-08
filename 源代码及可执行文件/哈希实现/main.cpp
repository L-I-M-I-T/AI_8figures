#define _CRT_SECURE_NO_WARNINGS 
#define HN hn2
#include<iostream>
#include<stdlib.h>
#include<cstring>
#include<iomanip>
#include<time.h>
#include<cmath>
using namespace std;

struct node
{
	int hn;
	int gn;
	int fn;
	char pos;
	char state[10];
	char pstate[10];
};//每个结点n包括h(n)、g(n)、f(n)、0方块的位置(pos)、状态字符串、父状态字符串等属性

char row_t[9];//目标状态每个数字所处的行
char col_t[9];//目标状态每个数字所处的列
char target[10] = { 0 };//将初始状态和目标状态以行次序优先表示成字符串，其余状态也采取相同操作
const char row[9] = { 0,0,0,1,1,1,2,2,2 };//字符串每一位对应的行
const char col[9] = { 0,1,2,0,1,2,0,1,2 };//字符串每一位对应的列
const char m[9][4] = { {1,3,-1,-1},{0,2,4,-1},{1,5,-1,-1},{0,4,6,-1},{1,3,5,7},{2,4,8,-1},{3,7,-1,-1},{4,6,8,-1},{5,7,-1,-1} };//实现记录好0在各个位置时可以向哪些位置移动

node a[1814400];//每一种状态最多处于1814400个状态的连通图中
int n, step, num, p[20000000];//hash表长度20000000

//确定某个状态被映射到了hash表的哪个位置  
//hash策略是将字符串去除最后两位，看成10000000以内的整型数映射到对应下表的数组元素，这样每个位置最多冲突一次，解决冲突的办法是将后映射的元素往后移10000000个位置
//hash表中每一个元素存储的是映射到它的字符串在堆中的位置
int find(const char st[])
{
	int t1 = atoi(st) / 100;
	int t2 = t1 + 10000000;
	if (p[t1] == 0)
		return 0;
	if (strcmp(a[p[t1]].state, st) == 0)
		return (t1);//是两种映射中的第一种，即当初映射时没发生冲突
	else
		if (p[t2] != 0)
			return (t2);//是两种映射中的第二种，即当初映射时发生冲突，往后移10000000个元素
		else
			return 0;//还未进行映射
}

template <typename T>
void Swap1(T& a, T& b)//交换两个变量的工具函数
{
	T t;
	t = a;
	a = b;
	b = t;
}

void Swap2(int x, int tmp)//交换堆中元素
{
	Swap1(p[find(a[x].state)], p[find(a[tmp].state)]);
	Swap1(a[x].hn, a[tmp].hn);
	Swap1(a[x].gn, a[tmp].gn);
	Swap1(a[x].fn, a[tmp].fn);
	Swap1(a[x].pos, a[tmp].pos);
	char t[10] = { 0 };
	strcpy(t, a[x].state);
	strcpy(a[x].state, a[tmp].state);
	strcpy(a[tmp].state, t);
	strcpy(t, a[x].pstate);
	strcpy(a[x].pstate, a[tmp].pstate);
	strcpy(a[tmp].pstate, t);
}

int hn1(const char st[])//第一种估值函数的h(n）,h(n)=不在目标状态对应位置上的数字的数量
{
	int i, s = 0;
	for (i = 0; i < int(strlen(st)); i++)
		if (st[i] != target[i])
			s++;
	return s;
}

int hn2(const char st[])//第二种估值函数的h(n）,h(n)=每个数字和目标状态对应数字位置的曼哈顿距离的总和
{
	int i, s = 0;
	for (i = 0; i < int(strlen(st)); i++)
		s = s + abs(row_t[st[i] - 48] - row[i]) + abs(col_t[st[i] - 48] - col[i]);
	return s;
}

int hn3(char st[])//第二种估值函数的h(n）,h(n)=每个数字和目标状态对应数字位置的曼哈顿距离的总和
{
	int i, s = 0, t = 0;
	for (i = 0; i<int(strlen(st)); i++)
		if (st[i] == '0')
		{
			t = i;
			break;
		}
	for (i = 0; i < int(strlen(st)); i++)
	{
		if (st[i] == '0')
			continue;
		if ((row_t[st[i - 48]] != row[t]) || (row_t[st[i] - 48] != col[t]))
		{
			Swap1(st[i], st[row_t[i] * 3 + col_t[i]]);
			s = s + 3;
		}
		else
		{
			Swap1(st[i], st[row_t[i] * 3 + col_t[i]]);
			s = s + 1;
		}
	}
	return s;
}

//以各个状态的f(n)为关键值建立小根堆，小根堆记录了open表和closed表，关键值不等于MAX_INT的元素处于open表，关键值等于MAX_INT的元素处于closed表
void push(int x)//新元素入堆后，用以维护堆的上移更新操作，同时更新hash表
{
	if (x / 2 == 0)
		return;
	if (a[x].fn < a[x / 2].fn)
	{
		Swap2(x, x / 2);
		push(x / 2);
	}
}

void maintain(int x)//堆顶元素弹出后，用以维护堆的下移更新操作，同时更新hash表
{
	if (x * 2 > n)
		return;
	int tmp;
	if ((x * 2 + 1 > n) || (a[x * 2].fn < a[x * 2 + 1].fn))
		tmp = x * 2;
	else
		tmp = x * 2 + 1;
	Swap2(x, tmp);
	maintain(tmp);
}

int cal(const char st[])//统计状态对应字符串去除0以后的逆序对个数，返回奇偶性，用来方便检查是否有解
{
	int i, j, s = 0;
	for (i = 0; i < int(strlen(st)); i++)
		for (j = 0; j < i; j++)
			if ((st[i] != '0') && (st[j] > st[i]))
				s++;
	return (s % 2);
}

void check(const char st[])//检查初始状态能否到达目标状态，不能则输出"无解"
{
	if (cal(a[1].state) != cal(target))
	{
		cout << "无解" << endl;
		exit(0);
	}
}

void print1(const node t)//打印搜索成功的最短路径
{
	if (int(strlen(t.pstate)) != 0)
		print1(a[p[find(t.pstate)]]);
	else
		cout << "初始状态：" << "f(n)=" << t.gn + t.hn << endl;
	if (step != 0)
		cout << "第" << setw(3) << step << "步：" << "f(n)=" << t.gn + t.hn << endl;
	int i;
	for (i = 0; i < 9; i++)
	{
		cout << setw(4) << t.state[i];
		if ((i + 1) % 3 == 0)
			cout << endl;
	}
	step++;
}

void print2(const int tim)//打印扩展节点数量、生成节点数量、用时等信息
{
	cout << "共扩展结点" << num << "个" << endl;
	cout << "共生成结点" << n << "个" << endl;
	cout << "用时";
	if (tim < 1)
		cout << "<1";
	else
		cout << tim;
	cout << "ms" << endl;
}

//int main(int ac, char** ag)//通过带参数的main函数，在cmd命令行下运行

int main()
{
	//在exe文件中采用重定向方式，方便检查
	(void)freopen("test.txt", "r", stdin);
	(void)freopen("ans.txt", "w", stdout);

	int i, x;
	node t;
	char st[10];
	n = 1;
	//计算目标结点各数字所处的行、列编号

	//strcpy(target, ag[2]);

	cin >> a[1].state;
	cin >> target;
	for (i = 0; i<int(strlen(target)); i++)
		if ((target[i] >= '0') && (target[i] <= '9'))
		{
			row_t[target[i] - '0'] = i / 3;
			col_t[target[i] - '0'] = i % 3;
		}

	//初始化初始状态结点
	//strcpy(a[1].state, ag[1]);


	int t1 = int(clock()), t2;
	check(a[1].state);
	a[1].hn = HN(a[1].state);
	a[1].gn = 0;
	a[1].fn = a[1].gn + a[1].hn;
	p[atoi(a[1].state) / 100] = 1;//hash初始状态
	for (i = 0; i < 10; i++)
		a[1].pstate[i] = 0;
	for (i = 0; i < int(strlen(a[1].state)); i++)//确定初始0的位置
		if (a[1].state[i] == '0')
		{
			a[1].pos = i;
			break;
		}
	while (1)
	{
		t = a[1];//扩展open表中f(n)最小的，也就是堆顶元素
		if (strcmp(t.state, target) == 0)
		{
			t2 = int(clock());
			print1(t);
			cout << "到达目标状态，共计";
			cout << t.fn;
			cout << "步";
			cout << endl;
			break;
		}
		num++;
		a[1].fn = 2147483647;//将f(n)记为MAX_INT，代表该结点处于closed表
		maintain(1);
		for (i = 0; i < 4; i++)//尝试将0移动到可以移动的位置
		{
			if (m[t.pos][i] == -1)//尝试完所有可移动位置了，提前结束循环
				break;
			strcpy(st, t.state);
			Swap1(st[m[t.pos][i]], st[t.pos]);//将0移动到可以移动的位置得到子状态
			int t3 = find(st);//判断子状态是否在open表和closed表中
			if (t3 == 0)//子状态既不在open表中也不在closed表中
			{
				//记录子状态
				n++;
				strcpy(a[n].state, st);
				strcpy(a[n].pstate, t.state);//记录路径
				a[n].hn = HN(a[n].state);
				a[n].gn = t.gn + 1;
				a[n].fn = a[n].gn + a[n].hn;//计算该子状态的估价函数
				a[n].pos = m[t.pos][i];
				int t1 = atoi(st) / 100;
				int t2 = t1 + 10000000;
				if (p[t1] != 0)//映射到hash表中，判断有无冲突
					p[t2] = n;//有冲突，二次映射
				else
					p[t1] = n;//无冲突
				push(n);//将该子状态加入到open表中，维护堆
			}
			else//子状态在open表或者closed表中
			{
				x = p[t3];
				if ((t.gn + 1) < a[x].gn)//该子状态是沿着一条比在open/closed表中已有路径更短的路径到达的
				{
					//更新最短路径走向及其估值函数
					strcpy(a[x].pstate, t.state);
					a[x].gn = t.gn + 1;
					a[x].fn = a[x].gn + a[x].hn;//对于处于closed表中元素来说，使得其f(n)不再等于MAX_INT，相当于从open表移入了closed表
					push(x);//维护堆
				}
			}
		}
	}
	print2(t2 - t1);
	return 0;
}