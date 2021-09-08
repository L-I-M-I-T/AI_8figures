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
};//ÿ�����n����h(n)��g(n)��f(n)��0�����λ��(pos)��״̬�ַ�������״̬�ַ���������

char row_t[9];//Ŀ��״̬ÿ��������������
char col_t[9];//Ŀ��״̬ÿ��������������
char target[10] = { 0 };//����ʼ״̬��Ŀ��״̬���д������ȱ�ʾ���ַ���������״̬Ҳ��ȡ��ͬ����
const char row[9] = { 0,0,0,1,1,1,2,2,2 };//�ַ���ÿһλ��Ӧ����
const char col[9] = { 0,1,2,0,1,2,0,1,2 };//�ַ���ÿһλ��Ӧ����
const char m[9][4] = { {1,3,-1,-1},{0,2,4,-1},{1,5,-1,-1},{0,4,6,-1},{1,3,5,7},{2,4,8,-1},{3,7,-1,-1},{4,6,8,-1},{5,7,-1,-1} };//ʵ�ּ�¼��0�ڸ���λ��ʱ��������Щλ���ƶ�

node a[1814400];//ÿһ��״̬��ദ��1814400��״̬����ͨͼ��
int n, step, num, p[20000000];//hash����20000000

//ȷ��ĳ��״̬��ӳ�䵽��hash����ĸ�λ��  
//hash�����ǽ��ַ���ȥ�������λ������10000000���ڵ�������ӳ�䵽��Ӧ�±������Ԫ�أ�����ÿ��λ������ͻһ�Σ������ͻ�İ취�ǽ���ӳ���Ԫ��������10000000��λ��
//hash����ÿһ��Ԫ�ش洢����ӳ�䵽�����ַ����ڶ��е�λ��
int find(const char st[])
{
	int t1 = atoi(st) / 100;
	int t2 = t1 + 10000000;
	if (p[t1] == 0)
		return 0;
	if (strcmp(a[p[t1]].state, st) == 0)
		return (t1);//������ӳ���еĵ�һ�֣�������ӳ��ʱû������ͻ
	else
		if (p[t2] != 0)
			return (t2);//������ӳ���еĵڶ��֣�������ӳ��ʱ������ͻ��������10000000��Ԫ��
		else
			return 0;//��δ����ӳ��
}

template <typename T>
void Swap1(T& a, T& b)//�������������Ĺ��ߺ���
{
	T t;
	t = a;
	a = b;
	b = t;
}

void Swap2(int x, int tmp)//��������Ԫ��
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

int hn1(const char st[])//��һ�ֹ�ֵ������h(n��,h(n)=����Ŀ��״̬��Ӧλ���ϵ����ֵ�����
{
	int i, s = 0;
	for (i = 0; i < int(strlen(st)); i++)
		if (st[i] != target[i])
			s++;
	return s;
}

int hn2(const char st[])//�ڶ��ֹ�ֵ������h(n��,h(n)=ÿ�����ֺ�Ŀ��״̬��Ӧ����λ�õ������پ�����ܺ�
{
	int i, s = 0;
	for (i = 0; i < int(strlen(st)); i++)
		s = s + abs(row_t[st[i] - 48] - row[i]) + abs(col_t[st[i] - 48] - col[i]);
	return s;
}

int hn3(char st[])//�ڶ��ֹ�ֵ������h(n��,h(n)=ÿ�����ֺ�Ŀ��״̬��Ӧ����λ�õ������پ�����ܺ�
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

//�Ը���״̬��f(n)Ϊ�ؼ�ֵ����С���ѣ�С���Ѽ�¼��open���closed���ؼ�ֵ������MAX_INT��Ԫ�ش���open���ؼ�ֵ����MAX_INT��Ԫ�ش���closed��
void push(int x)//��Ԫ����Ѻ�����ά���ѵ����Ƹ��²�����ͬʱ����hash��
{
	if (x / 2 == 0)
		return;
	if (a[x].fn < a[x / 2].fn)
	{
		Swap2(x, x / 2);
		push(x / 2);
	}
}

void maintain(int x)//�Ѷ�Ԫ�ص���������ά���ѵ����Ƹ��²�����ͬʱ����hash��
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

int cal(const char st[])//ͳ��״̬��Ӧ�ַ���ȥ��0�Ժ������Ը�����������ż�ԣ������������Ƿ��н�
{
	int i, j, s = 0;
	for (i = 0; i < int(strlen(st)); i++)
		for (j = 0; j < i; j++)
			if ((st[i] != '0') && (st[j] > st[i]))
				s++;
	return (s % 2);
}

void check(const char st[])//����ʼ״̬�ܷ񵽴�Ŀ��״̬�����������"�޽�"
{
	if (cal(a[1].state) != cal(target))
	{
		cout << "�޽�" << endl;
		exit(0);
	}
}

void print1(const node t)//��ӡ�����ɹ������·��
{
	if (int(strlen(t.pstate)) != 0)
		print1(a[p[find(t.pstate)]]);
	else
		cout << "��ʼ״̬��" << "f(n)=" << t.gn + t.hn << endl;
	if (step != 0)
		cout << "��" << setw(3) << step << "����" << "f(n)=" << t.gn + t.hn << endl;
	int i;
	for (i = 0; i < 9; i++)
	{
		cout << setw(4) << t.state[i];
		if ((i + 1) % 3 == 0)
			cout << endl;
	}
	step++;
}

void print2(const int tim)//��ӡ��չ�ڵ����������ɽڵ���������ʱ����Ϣ
{
	cout << "����չ���" << num << "��" << endl;
	cout << "�����ɽ��" << n << "��" << endl;
	cout << "��ʱ";
	if (tim < 1)
		cout << "<1";
	else
		cout << tim;
	cout << "ms" << endl;
}

//int main(int ac, char** ag)//ͨ����������main��������cmd������������

int main()
{
	//��exe�ļ��в����ض���ʽ��������
	(void)freopen("test.txt", "r", stdin);
	(void)freopen("ans.txt", "w", stdout);

	int i, x;
	node t;
	char st[10];
	n = 1;
	//����Ŀ����������������С��б��

	//strcpy(target, ag[2]);

	cin >> a[1].state;
	cin >> target;
	for (i = 0; i<int(strlen(target)); i++)
		if ((target[i] >= '0') && (target[i] <= '9'))
		{
			row_t[target[i] - '0'] = i / 3;
			col_t[target[i] - '0'] = i % 3;
		}

	//��ʼ����ʼ״̬���
	//strcpy(a[1].state, ag[1]);


	int t1 = int(clock()), t2;
	check(a[1].state);
	a[1].hn = HN(a[1].state);
	a[1].gn = 0;
	a[1].fn = a[1].gn + a[1].hn;
	p[atoi(a[1].state) / 100] = 1;//hash��ʼ״̬
	for (i = 0; i < 10; i++)
		a[1].pstate[i] = 0;
	for (i = 0; i < int(strlen(a[1].state)); i++)//ȷ����ʼ0��λ��
		if (a[1].state[i] == '0')
		{
			a[1].pos = i;
			break;
		}
	while (1)
	{
		t = a[1];//��չopen����f(n)��С�ģ�Ҳ���ǶѶ�Ԫ��
		if (strcmp(t.state, target) == 0)
		{
			t2 = int(clock());
			print1(t);
			cout << "����Ŀ��״̬������";
			cout << t.fn;
			cout << "��";
			cout << endl;
			break;
		}
		num++;
		a[1].fn = 2147483647;//��f(n)��ΪMAX_INT������ý�㴦��closed��
		maintain(1);
		for (i = 0; i < 4; i++)//���Խ�0�ƶ��������ƶ���λ��
		{
			if (m[t.pos][i] == -1)//���������п��ƶ�λ���ˣ���ǰ����ѭ��
				break;
			strcpy(st, t.state);
			Swap1(st[m[t.pos][i]], st[t.pos]);//��0�ƶ��������ƶ���λ�õõ���״̬
			int t3 = find(st);//�ж���״̬�Ƿ���open���closed����
			if (t3 == 0)//��״̬�Ȳ���open����Ҳ����closed����
			{
				//��¼��״̬
				n++;
				strcpy(a[n].state, st);
				strcpy(a[n].pstate, t.state);//��¼·��
				a[n].hn = HN(a[n].state);
				a[n].gn = t.gn + 1;
				a[n].fn = a[n].gn + a[n].hn;//�������״̬�Ĺ��ۺ���
				a[n].pos = m[t.pos][i];
				int t1 = atoi(st) / 100;
				int t2 = t1 + 10000000;
				if (p[t1] != 0)//ӳ�䵽hash���У��ж����޳�ͻ
					p[t2] = n;//�г�ͻ������ӳ��
				else
					p[t1] = n;//�޳�ͻ
				push(n);//������״̬���뵽open���У�ά����
			}
			else//��״̬��open�����closed����
			{
				x = p[t3];
				if ((t.gn + 1) < a[x].gn)//����״̬������һ������open/closed��������·�����̵�·�������
				{
					//�������·���������ֵ����
					strcpy(a[x].pstate, t.state);
					a[x].gn = t.gn + 1;
					a[x].fn = a[x].gn + a[x].hn;//���ڴ���closed����Ԫ����˵��ʹ����f(n)���ٵ���MAX_INT���൱�ڴ�open��������closed��
					push(x);//ά����
				}
			}
		}
	}
	print2(t2 - t1);
	return 0;
}