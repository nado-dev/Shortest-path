#include<stdio.h>
#include<malloc.h>
#include<stdlib.h>
#include<string.h>

#define INF 32767
#define MAXV 10

typedef struct MS{
	int start;
	int end;
	int length;
	int aph[MAXV];

}elm[MAXV];

typedef struct MS1 {
	elm n[MAXV][MAXV];
	int i;
}SP;

//无用的注释
struct edge {
	int u;//边的起点
	int v;//边的终点
	int w;//权值
}edges[MAXV];

typedef struct {
	int no;			//顶点编号
	char info[MAXV];		//顶点其他信息
}VType;//顶点类型

typedef struct {
	int edges[MAXV][MAXV]; //邻接矩阵
	int n;				   //顶点数
	int e;				   //边数
	VType vexs[MAXV];	   //存放顶点信息
}MatGraph;//图邻接矩阵类型

void Dispath(MatGraph g, int dist[], int path[], int S[], int v,int m);
//输出邻接矩阵g
void DispMat(MatGraph g)	
{
	int i, j;
	for (i = 0;i < g.n;i++)
	{
		for (j = 0;j < g.n;j++)
			if (g.edges[i][j] != INF)
				printf("%4d", g.edges[i][j]);
			else
				printf("%4s", "∞");
		printf("\n");
	}
}
void Dijkstra(MatGraph g, int v,int m)	//Dijkstra算法
{
	int dist[MAXV], path[MAXV];
	int S[MAXV];				//S[i]=1表示顶点i在S中, S[i]=0表示顶点i在U中
	int Mindis, i, j, u=0;
	for (i = 0;i < g.n;i++)
	{
		dist[i] = g.edges[v][i];	//距离初始化
		S[i] = 0;					//S[]置空
		if (g.edges[v][i] < INF)	//路径初始化
			path[i] = v;			//顶点v到顶点i有边时，置顶点i的前一个顶点为v
		else
			path[i] = -1;			//顶点v到顶点i没边时，置顶点i的前一个顶点为-1
	}
	S[v] = 1;path[v] = 0;			//源点编号v放入S中
	for (i = 0;i < g.n - 1;i++)			//循环直到所有顶点的最短路径都求出
	{
		Mindis = INF;				//Mindis置最大长度初值
		for (j = 0;j < g.n;j++)		//选取不在S中（即U中）且具有最小最短路径长度的顶点u
			if (S[j] == 0 && dist[j] < Mindis)
			{
				u = j;
				Mindis = dist[j];
			}
		S[u] = 1;					//顶点u加入S中
		for (j = 0;j < g.n;j++)		//修改不在S中（即U中）的顶点的最短路径
			if (S[j] == 0)
				if (g.edges[u][j] < INF && dist[u] + g.edges[u][j] < dist[j])
				{
					dist[j] = dist[u] + g.edges[u][j];
					path[j] = u;
				}
	}
	Dispath(g, dist, path, S, v, m);	//输出最短路径
}

void Dispath_floyd(MatGraph g, int A[][MAXV], int path[][MAXV])	//输出最短路径
{
	FILE *fp;
	errno_t err;
	err = fopen_s(&fp, "AllPath.dat", "w");
	SP sp;
	int i, j, k, s,f=0;
	int apath[MAXV], d;		//存放一条最短路径中间顶点(反向)及其顶点个数
	for (i = 0;i < g.n;i++)
		for (j = 0;j <g.n;j++) {
			if (A[i][j] != INF && i != j)
			{
				fprintf(fp, "%s\t%s\t", g.vexs[i].info, g.vexs[j].info);
				k = path[i][j];
				d = 0; apath[d] = j;
				while (k != -1 && k != i) {
					d++; apath[d] = k;
					k = path[i][k];
				}
				d++; apath[d] = i;
				fprintf(fp, "%s\t", g.vexs[apath[d]].info);
				for (s = d - 1;s >= 0;s--)
					fprintf(fp, "%s\t", g.vexs[apath[s]].info);

				fprintf(fp, "%d\n", A[i][j]);

			}
		}
	fclose(fp);
	printf("所有路径信息已经保存在AllPath.dat下\n");
	FILE *fp1;
	errno_t err1;
	err1 = fopen_s(&fp1, "AllPath1.dat", "w");
	for (i = 0;i < g.n;i++)
		for (j = 0;j < g.n;j++)
		{
			if (A[i][j] != INF && i != j)			//若顶点i和j之间存在路径
			{
				printf("  从%d到%d的路径为:", i, j);

				k = path[i][j];
				d = 0; apath[d] = j;				//路径上添加终点
				while (k != -1 && k != i)			//路径上添加中间点
				{
					d++; apath[d] = k;
					k = path[i][k];
				}
				d++; apath[d] = i;				
				printf("%d", apath[d]);			
				int s1 = d;
				for (s = d - 1;s >= 0;s--) {		
					printf(",%d", apath[s]);
				}
				printf("     \t路径长度为:%d\n", A[i][j]);
				sp.n[i][j]->start = i;
				sp.n[i][j]->end = j;
				for (f = 0;f < MAXV;f++) {
					sp.n[i][j]->aph[f] = apath[f];
				}
				sp.n[i][j]->length = A[i][j];
				sp.i = s1;
			}
		}

	fwrite(&sp,sizeof(sp),1,fp);
	fclose(fp1);
	
}
void Dispath(MatGraph g, int dist[], int path[], int S[], int v, int i)
//输出从顶点v出发的所有最短路径
{
	int j, k;
	int apath[MAXV], d;					//存放一条最短路径(逆向)及其顶点个数


	if (S[i] == 1 && i != v)
	{
		printf("  从顶点%d%s到顶点%d%s的路径长度为:%d\t路径为:", v, g.vexs[v].info, i, g.vexs[i].info, dist[i]);
		d = 0; apath[d] = i;			//添加路径上的终点
		k = path[i];
		if (k == -1)				//没有路径的情况
			printf("无路径\n");
		else						//存在路径时输出该路径
		{
			while (k != v)
			{
				d++; apath[d] = k;
				k = path[k];
			}
			d++; apath[d] = v;		//添加路径上的起点
			printf("%d", apath[d]);	//先输出起点
			for (j = d - 1;j >= 0;j--)	//再输出其他顶点
				printf(",%d", apath[j]);
			printf("\n");
		}
		
	}
	else {
		printf("这两个景点之间不存在路径\n");
	}
}

	void Floyd(MatGraph g)	{					//Floyd算法
		int A[MAXV][MAXV], path[MAXV][MAXV];
		int i, j, k;
		for (i = 0;i < g.n;i++)
			for (j = 0;j < g.n;j++)
			{
				A[i][j] = g.edges[i][j];
				if (i != j && g.edges[i][j] < INF)
					path[i][j] = i;
				else
					path[i][j] = -1;
			}
		for (k = 0;k < g.n;k++)
		{
			for (i = 0;i < g.n;i++)
				for (j = 0;j < g.n;j++)
					if (A[i][j] > A[i][k] + A[k][j])
					{
						A[i][j] = A[i][k] + A[k][j];
						path[i][j] = path[k][j];
					}
		}
		Dispath_floyd(g, A, path);
	}

void Create(MatGraph &G) {
	int i,j,k,w=0,p;
	char r[MAXV];
	printf_s("输入景点数n和任意两个景点直接相连的路径的数量e（用空格间隔）：");
	scanf_s("%d %d", &G.n, &G.e);
	printf_s("输入景点的地名信息：\n");
	gets_s(r);
	for (i = 0;i < G.n;i++) {
		printf("输入第%d个景点名称，共%d个：",i + 1, G.n);
		gets_s(G.vexs[i].info);	
	}  

	for (i = 0;i < G.n;i++) {
		for (j = 0;j < G.n;j++) {
			G.edges[i][j] = INF;
		}
	}

	for (k = 0;k < G.e;k++) {
		printf_s("输入路径(vi,vj)上的开始景点编号i，结束景点编号j和它们的距离w（用空格间隔）:\n");
		printf("输入第%d条路径，共%d条：", k + 1, G.e);
		scanf_s("%d %d %d", &i, &j, &w);
		G.edges[i][j] = w;
	}
	for (p = 0;p < G.n;p++) {
		G.edges[p][p] = 0;
	}
}

int put_in_all(MatGraph &g) {
	FILE *fp;
	int r, c;
	errno_t err;
	err =fopen_s(&fp, "input_path.dat", "w");
	if (fp != NULL) {
		for (r = 0;r < g.n;r++) {
			for (c = 0;c < g.n;c++) {
				if (g.edges[r][c] != INF) {
					fprintf_s(fp, "%d\t%s\t%d\t%s\t%d\n", r, g.vexs[r].info, c, g.vexs[c].info, g.edges[r][c]);
				}
			}
		}
		printf_s("所有景点和距离信息已保存在input_path.dat文件中\n");
		fclose(fp);
		return 1;
	}
	else {
		printf_s("打开文件失败\n");
		return -1;
	}
		

}

int main(void) {
	int  c=0,k=0,i=0;
	char name1[MAXV],name_b[MAXV];
	char name_e[MAXV];
	MatGraph g;
	FILE *fp2;
	SP sp1;
	errno_t err;
	err = fopen_s(&fp2, "AllPath1.dat", "r");
	if (!fp2) {
		Create(g);
		put_in_all(g);
		printf_s("邻接矩阵：\n");
		DispMat(g);
		printf_s("输入要查找最短路径的起点景点名称：");
		gets_s(name1);
		gets_s(name_b);
		while (c < g.n)
		{
			if (!strcmp(name_b, g.vexs[c].info)) break;
			else c++;
		}
		printf_s("输入要查找最短路径的终点景点名称：");

		gets_s(name_e);
		while (i < g.n)
		{
			if (!strcmp(name_e, g.vexs[i].info)) break;
			else i++;
		}
		printf("%d %d", c, i);
		//	printf("Dijkstra算法求解结果:\n");
		Dijkstra(g, c, i);
		printf("Floyd算法求解结果:\n");
		Floyd(g);

		

		getchar();
		getchar();
		return 0;
	}
	else {
		for (;;) {
			int st, ed,s;
			int count = 0;
			printf("文件AllPath.dat已经存在，输入起点代码和终点代码查询最短路径：\n");
			fread(&sp1, sizeof(sp1), 1, fp2);
			scanf_s("%d %d", &st, &ed);
			if (sp1.n[st][ed]->end == ed && sp1.n[st][ed]->start == st) {
				printf("存在最短路径，长度为%d\n", sp1.n[st][ed]->length);
//				for (s = sp1.i - 1;s >= 0;s--) {
//					printf(",%d",sp1.n[st][ed]->aph[s]);
//				}
			}
			else
			{
				printf("不存在这样的最短路径\n");
			}
		}
		
		fclose(fp2);
		getchar();
		getchar();
	}
	

	
}