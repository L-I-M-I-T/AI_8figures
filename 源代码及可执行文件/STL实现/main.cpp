#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <vector>
#include <queue>
#include<stack>
#include<ctime>

using namespace std;
int a[3][3];

//��չ�ڵ���Ŀ
int expandPoint = 0;
//���ɽڵ�ĸ���
int generatePoint = 0;

struct point {
	int arr[3][3];
	int depth;
	int f;
	int fatherIndex;
	bool state = true;

	bool operator <(const point& dd)const
	{
		return f > dd.f;
	}
};

point startPoint, endPoint;
priority_queue<point>open;
vector<point>close;

//�������
void getMatrix(int arr[3][3])
{
	for (int i = 0; i < 3; i++)
		for (int j = 0; j < 3; j++)
			cin >> arr[i][j];
}

////���㺯��ֵ
////����ƫ�Ƶĸ�������
//void calcf(point &cur)	
//{
//	int temp = 0;
//	for (int i = 0; i < 3; i++) {
//		for (int j = 0; j < 3; j++) {
//			if (cur.arr[i][j] != endPoint.arr[i][j])
//				temp++;
//		}
//	}
//
//	cur.f = cur.depth + temp;
//
//}

//���㺯��
//���������پ������
void calcf(point& cur)
{
	int temp = 0;

	//����cur��ÿ����
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {

			//����cur.arr[i][j]��Ŀ��״̬�������پ���
			for (int m = 0; m < 3; m++) {
				for (int n = 0; n < 3; n++) {
					if (cur.arr[i][j] == endPoint.arr[m][n]) {
						temp += abs(i - m) + abs(j - n);
					}
				}
			}
		}
	}

	cur.f = cur.depth + temp;
}

//�ж����������Ƿ����
bool matrixEqual(int arr1[3][3], int arr2[3][3])
{
	for (int i = 0; i < 3; i++)
		for (int j = 0; j < 3; j++)
			if (arr1[i][j] != arr2[i][j])
				return false;
	return true;
}

//�ҳ�һ��״̬�����ڵ�״̬
void findNeighbor(point curPoint, vector<point>& neighbor)
{
	int index_i, index_j;

	//Ѱ�ҵ�0���ڵ�λ��
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			if (curPoint.arr[i][j] == 0) {
				index_i = i;
				index_j = j;
				break;
			}
		}
	}

	//����0��λ���жϿ����ƶ��ķ���
	if (index_i > 0) { //up
		point temp = curPoint;
		swap(temp.arr[index_i][index_j], temp.arr[index_i - 1][index_j]);
		neighbor.push_back(temp);

		//���ɽڵ����
		generatePoint++;
	}

	if (index_i < 2) { //down
		point temp = curPoint;
		swap(temp.arr[index_i][index_j], temp.arr[index_i + 1][index_j]);
		neighbor.push_back(temp);

		generatePoint++;
	}

	if (index_j > 0) {//left
		point temp = curPoint;
		swap(temp.arr[index_i][index_j], temp.arr[index_i][index_j - 1]);
		neighbor.push_back(temp);

		generatePoint++;
	}

	if (index_j < 2) {//right
		point temp = curPoint;
		swap(temp.arr[index_i][index_j], temp.arr[index_i][index_j + 1]);
		neighbor.push_back(temp);

		generatePoint++;
	}
}

//temp 0 open close ��û�� 
//temp 1 open û��  close�� 
//temp 2 open ��    closeû�� 
int judege(point sonPoint)
{
	int openflag = 0;
	int closeflag = 0;

	priority_queue<point>openCopy = open;

	while (!openCopy.empty()) {
		point openPoint = openCopy.top();
		openCopy.pop();

		if (matrixEqual(sonPoint.arr, openPoint.arr)) {
			openflag = 1;
			break;
		}
	}

	vector<point>closeCopy = close;

	for (int k = 0; k < close.size(); k++) {
		if (!close[k].state)continue;//close ���е���Ч�ڵ�

		point closePoint = closeCopy[k];

		if (matrixEqual(sonPoint.arr, closePoint.arr)) {
			closeflag = 1;
			break;
		}
	}

	int temp = openflag * 10 + closeflag;
	temp %= 4;
	return temp;

}

int feasibility(point Point)
{
	int ans = 0;
	int an[8];
	int index = 0;

	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			if (Point.arr[i][j] != 0) {
				an[index++] = Point.arr[i][j];
			}
		}
	}

	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < i; j++) {
			if (an[j] > an[i])++ans;
		}
	}

	return ans % 2;
}

//A*�㷨����ṹ
bool AStar()
{
	//���ó�ʼ״̬���Լ�Ŀ��״̬
	getMatrix(startPoint.arr);
	getMatrix(endPoint.arr);

	if (feasibility(startPoint) != feasibility(endPoint)) {
		cout << "�޽�" << endl;
		exit(0);
	}

	//��ʼ״̬����open��
	startPoint.depth = 0;
	startPoint.fatherIndex = -1;
	calcf(startPoint);
	open.push(startPoint);

	//ѭ������
	while (1) {
		//open��Ϊ�ս�������
		if (open.empty()) {
			cout << "open empty!\n";
			return false;
		}

		//�ҵ�open���е���С��һ��
		point curPoint = open.top();
		open.pop();

		//��չ������
		expandPoint++;

		//��Ŀ��״̬��ͬ����
		if (matrixEqual(curPoint.arr, endPoint.arr)) {
			close.push_back(curPoint);
			return true;
		}

		//��ǰ��״̬�����ڵ�״̬
		vector<point>neighbor;
		findNeighbor(curPoint, neighbor);
		if (neighbor.empty())continue;

		int index = close.size();

		for (int i = 0; i < neighbor.size(); i++) {
			point sonPoint = neighbor[i];

			//��ʼ������
			sonPoint.depth = curPoint.depth + 1;
			calcf(sonPoint);
			sonPoint.fatherIndex = index;

			//�ж�sonPoint������
			int temp = judege(sonPoint);

			switch (temp) {
			case 0: {			//����open ,close ����
				open.push(sonPoint);

				break;
			}
			case 1: {			//������close����
				for (vector<point>::iterator it = close.begin(); it < close.end(); it++) {

					//����µĽڵ���ã���ô���ϵĽڵ��close��ȥ�����������ӵ�open����
					if ((*it).state && matrixEqual(sonPoint.arr, (*it).arr)) {
						if (sonPoint.f < (*it).f) {
							open.push(sonPoint);

							(*it).state = false;//������ڵ�ָΪ��Ч״̬
						}
					}
				}

				break;
			}
			case 2: {							//������open����
				stack<point>s;
				bool flag = false;

				while (!open.empty()) {
					point tempPoint = open.top();
					open.pop();

					//����µĽڵ���ã���ôȥ��ԭ���Ľڵ�
					s.push(tempPoint);
					if (matrixEqual(sonPoint.arr, tempPoint.arr) && sonPoint.f < tempPoint.f) {
						s.pop();
					}
				}

				while (!s.empty()) {
					point tempPoint = s.top();
					s.pop();
					open.push(tempPoint);
				}

				break;
			}

			default: break;
			}

		}

		//����ǰ״̬���뵽close����
		close.push_back(curPoint);
	}

}

//��ӡ����
void printMatrix(int arr[3][3])
{
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			cout << arr[i][j] << " ";
		}
		cout << endl;
	}
}

//��ӡ���
void printResult(int index)
{
	point tempPoint = close[index];

	if (tempPoint.fatherIndex == -1) {
		cout << "------->" << tempPoint.depth << " " << tempPoint.f << endl;
		printMatrix(tempPoint.arr);
		cout << endl;
		return;
	}

	printResult(tempPoint.fatherIndex);

	cout << "------->" << tempPoint.depth << " " << tempPoint.f << endl;
	printMatrix(tempPoint.arr);
	cout << endl;
}

//������
int main()
{
	//�����ض��򣬰�����ʼ״̬�Լ�Ŀ��״̬
	(void)freopen("test.txt", "r", stdin);
	(void)freopen("ans.txt", "w", stdout);

	clock_t t = clock();

	if (AStar())
		printResult(close.size() - 1);
	else
		;

	printf("���ɽڵ����Ŀ: %d\n", generatePoint);
	printf("��չ�ڵ����Ŀ: %d\n", expandPoint);
	printf("����ʱ��: %dms\n", clock() - t);


	return 0;
}