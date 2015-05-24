#include <iostream>
#include <cstdio>
#include <cstring>
#include <vector>
#include <algorithm>
#include <cmath>
#include <unistd.h>
#include <time.h>

using namespace std;
#define INPUT_FILE_NAME "input.txt"
#define MAX_SIZE 1000
#define INF 0x7FFFFFFF
#define SAFETY_RANGE 10

struct edge
{
	int vx, vy;
	double cost;
	edge(int u, int v, double c)
	{
		vx = u, vy = v, cost = c;
	}
};

bool isMST[MAX_SIZE + SAFETY_RANGE][MAX_SIZE + SAFETY_RANGE];
double graph[MAX_SIZE + SAFETY_RANGE][MAX_SIZE + SAFETY_RANGE];
vector<edge>Edges;												//Store all edges
vector<int>sol;													//Store Solution
bool	visited[MAX_SIZE + SAFETY_RANGE];						//Store if visited
bool	rank[MAX_SIZE + SAFETY_RANGE];							//rank store
int		parent[MAX_SIZE + SAFETY_RANGE];						//Parent store
double	px[MAX_SIZE+SAFETY_RANGE], py[MAX_SIZE + SAFETY_RANGE];	//point position - X axis and Y axis
int		total_number=0, start;

double minDistance = 0;

double	getDistance(int i, int j);		//Calculate distance of 2 place
void	userInput();					//Take input from user
bool	compareEdges(edge a, edge b);	//Copmare function for sorting
int		Find(int p);					//Find an element in the parents
void	Kruskal();						//Run Kruskal Algo
void	fileInput();					//Take input from file
void	Dfs(int u);						//Run DFS Algo
void	printInput();					//Print all input taken
void	Union(int p,int q);				//Unite 2 points - change rank and parent
void	findAllDistances();				//Find all distances of the graph
void	initValues();					//Initialize all the ranks and parents
double	findUsingGreedy();				//Greedy Algorithm to run

int main()
{
	int x, y;
	double c;

	//userInput();
	fileInput();

	printInput();

	//Time calculation Start
	clock_t t;
	t = clock();
	////////////////////////

	cout << findUsingGreedy() << endl;

	//Time calculation End
	t = clock() - t;
  	printf ("It took %f second(s).\n",((float)t)/CLOCKS_PER_SEC);
	////////////////////////

	getchar();
	return 0;
}

void userInput()
{
	cin >> total_number;
	for(int i = 0; i < total_number; i++)
	{
		cin >> px[i] >> py[i];
	}
	cin >> start;
}

void fileInput()
{
	FILE *fp;
	//Get Current Directory
	char cwd[1024];
	if (getcwd(cwd, sizeof(cwd)) == NULL)	//Directory not found
	{
		perror("getcwd() error - Current directory not found !!");
		exit(0);
	}
	strcat(cwd, "\\");
	strcat(cwd, INPUT_FILE_NAME);
	fp = fopen(cwd , "r");

	int i=0;
	fscanf(fp,"%d",&start);
	while(fscanf(fp,"%lf",&px[i])!=EOF)
	{
		fscanf(fp,"%lf",&py[i]);
		i++;
		total_number++;
	}
}

void initValues()
{
	for(int i = 0; i < total_number; i++)
	{
		rank[i] = 1;
		parent[i] = i;
	}
}

void printInput()
{
	cout<<"TOtal Input = ";
	cout<<total_number<<endl<<endl<<"Points are - "<<endl;
	for(int j=0;j<total_number;j++)
	{
		cout<<px[j]<<" "<<py[j]<<endl;
	}
	cout<<"------------------------"<<endl;
}

void findAllDistances()
{
	for(int i = 0; i < total_number - 1; i++)
	{
		for(int j = i + 1; j <  total_number; j++)
		{
			graph[i][j] = graph[j][i] = getDistance(i, j);
			Edges.push_back(edge(i, j, graph[i][j]));
		}
	}
}

double getDistance(int i, int j)
{
	return sqrt((px[i] - px[j])*(px[i] - px[j]) + (py[i] - py[j])*(py[i] - py[j]));
}

bool compareEdges(edge a, edge b)
{
	return (a.cost < b.cost);
}

int Find(int p)
{
	if(parent[p]==p)
		return p;
	return Find(parent[p]);
}

void Union(int p,int q)
{
	if(p == q)
		return;
	if(rank[p] > rank[q])
		swap(p,q);

	rank[q] += rank[p];
	parent[p] = q;
}

void Kruskal()
{
	int cnt = 0;
	for(int i = 0; i < Edges.size(); i++)
	{
		if(cnt == total_number - 1)
			break;
		int p = Find(Edges[i].vx);
		int q = Find(Edges[i].vy);
		if(p != q)
		{
			cnt++;
			Union(p, q);
			isMST[Edges[i].vx][Edges[i].vy] = isMST[Edges[i].vy][Edges[i].vx] = 1;
		}
	}
}

void Dfs(int u)
{
	sol.push_back(u);
	visited[u] = 1;
	for(int i = 0; i < total_number; i++)
	{
		if(isMST[u][i] && !visited[i]) Dfs(i);
	}
}

double findUsingGreedy()
{
	findAllDistances();
	sort(Edges.begin(),Edges.end(),compareEdges);		//Sort all the edges
	initValues();								//Initialize all the values - Parent and
	Kruskal();
	Dfs(start);									//Solution is found and stored in vector
	for(int i = 0; i < sol.size() - 1; i++)		//Get the result
	{
		minDistance += graph[sol[i]][sol[i + 1]];
	}
	return minDistance + graph[sol.back()][start];
}
