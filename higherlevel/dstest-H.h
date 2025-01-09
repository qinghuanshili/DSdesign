#define MAX_V_NUM 20

typedef struct{
    int vexs[MAX_V_NUM];
    int Edges[MAX_V_NUM][MAX_V_NUM];
    int numOfVex;
    int numOfEdges;
}MGraph;

typedef struct node{
    int x, y;
    int weight;
    struct node* next;
}Node,*Nodelist;

typedef struct{
    int adjvex;
    int lowcost;
    int flag;
}Closedge[MAX_V_NUM];