#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <vector>
#include <queue>
#include<stack>
#include<ctime>

using namespace std;
int a[3][3];

//扩展节点数目
int expandPoint = 0;
//生成节点的个数
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

//输入矩阵
void getMatrix(int arr[3][3])
{
	for (int i = 0; i < 3; i++)
		for (int j = 0; j < 3; j++)
			cin >> arr[i][j];
}

////计算函数值
////按照偏移的个数计算
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

//计算函数
//按照曼哈顿距离计算
void calcf(point& cur)
{
	int temp = 0;

	//遍历cur的每个数
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {

			//计算cur.arr[i][j]与目标状态的曼哈顿距离
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

//判断两个矩阵是否相等
bool matrixEqual(int arr1[3][3], int arr2[3][3])
{
	for (int i = 0; i < 3; i++)
		for (int j = 0; j < 3; j++)
			if (arr1[i][j] != arr2[i][j])
				return false;
	return true;
}

//找出一个状态的相邻的状态
void findNeighbor(point curPoint, vector<point>& neighbor)
{
	int index_i, index_j;

	//寻找到0所在的位置
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			if (curPoint.arr[i][j] == 0) {
				index_i = i;
				index_j = j;
				break;
			}
		}
	}

	//根据0的位置判断可以移动的方向
	if (index_i > 0) { //up
		point temp = curPoint;
		swap(temp.arr[index_i][index_j], temp.arr[index_i - 1][index_j]);
		neighbor.push_back(temp);

		//生成节点计数
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

//temp 0 open close 都没有 
//temp 1 open 没有  close有 
//temp 2 open 有    close没有 
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
		if (!close[k].state)continue;//close 表中的无效节点

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

//A*算法主体结构
bool AStar()
{
	//设置初始状态，以及目标状态
	getMatrix(startPoint.arr);
	getMatrix(endPoint.arr);

	if (feasibility(startPoint) != feasibility(endPoint)) {
		cout << "无解" << endl;
		exit(0);
	}

	//初始状态进入open表
	startPoint.depth = 0;
	startPoint.fatherIndex = -1;
	calcf(startPoint);
	open.push(startPoint);

	//循环操作
	while (1) {
		//open表为空结束操作
		if (open.empty()) {
			cout << "open empty!\n";
			return false;
		}

		//找到open表中的最小的一个
		point curPoint = open.top();
		open.pop();

		//扩展结点计数
		expandPoint++;

		//与目标状态相同返回
		if (matrixEqual(curPoint.arr, endPoint.arr)) {
			close.push_back(curPoint);
			return true;
		}

		//当前的状态的相邻的状态
		vector<point>neighbor;
		findNeighbor(curPoint, neighbor);
		if (neighbor.empty())continue;

		int index = close.size();

		for (int i = 0; i < neighbor.size(); i++) {
			point sonPoint = neighbor[i];

			//初始化操作
			sonPoint.depth = curPoint.depth + 1;
			calcf(sonPoint);
			sonPoint.fatherIndex = index;

			//判断sonPoint在哪里
			int temp = judege(sonPoint);

			switch (temp) {
			case 0: {			//不在open ,close 表中
				open.push(sonPoint);

				break;
			}
			case 1: {			//仅仅在close表中
				for (vector<point>::iterator it = close.begin(); it < close.end(); it++) {

					//如果新的节点更好，那么把老的节点从close中去掉，并把它加到open表中
					if ((*it).state && matrixEqual(sonPoint.arr, (*it).arr)) {
						if (sonPoint.f < (*it).f) {
							open.push(sonPoint);

							(*it).state = false;//将这个节点指为无效状态
						}
					}
				}

				break;
			}
			case 2: {							//仅仅在open表中
				stack<point>s;
				bool flag = false;

				while (!open.empty()) {
					point tempPoint = open.top();
					open.pop();

					//如果新的节点更好，那么去除原来的节点
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

		//将当前状态放入到close表中
		close.push_back(curPoint);
	}

}

//打印矩阵
void printMatrix(int arr[3][3])
{
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			cout << arr[i][j] << " ";
		}
		cout << endl;
	}
}

//打印结果
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

//主函数
int main()
{
	//输入重定向，包含开始状态以及目标状态
	(void)freopen("test.txt", "r", stdin);
	(void)freopen("ans.txt", "w", stdout);

	clock_t t = clock();

	if (AStar())
		printResult(close.size() - 1);
	else
		;

	printf("生成节点的数目: %d\n", generatePoint);
	printf("扩展节点的数目: %d\n", expandPoint);
	printf("运行时间: %dms\n", clock() - t);


	return 0;
}