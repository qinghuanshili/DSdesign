#define MAX_V_NUM 20
#define CMD_MAX_LEN 20
#define MAX_FILE_LEN 1024

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

typedef struct cmdnode
{
    char *cmd;
    char *desc;
    int (*handler)();
    struct cmdnode *next;
}tcmdNode;