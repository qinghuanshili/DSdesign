#include<stdio.h>
#include<stdlib.h>
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

Node* Prim(MGraph *graph, int src);
void clearGraph(MGraph *graph);

int main(void)
{
    
    int i=0,src=0,max=0;
    int temp1=0,temp2=0,temp3=0;
    MGraph graph;
    
    clearGraph(&graph);//初始化

    FILE *pFile;
    pFile=fopen("dstestfile2.txt","r");
    if(pFile==NULL){
        perror("Fial in reading file");
        return -1;
    }
    
    while(true)
    {
        //赋值起点
        if(i==0){
            fscanf(pFile,"%d",&src);
            i++;
        }

        //赋值边数
        else if(i==1){
            fscanf(pFile,"%d",&graph.numOfEdges);
            i++;
        }
        
        else{
            fscanf(pFile,"%d %d %d",&temp1,&temp2,&temp3);
            //存入图中
            graph.Edges[temp1-1][temp2-1]=temp3;
            graph.Edges[temp2-1][temp1-1]=temp3;

            //寻找所用的最大点，确定点数
            max=(max>temp1)?max:temp1;
            max=(max>temp2)?max:temp2;
        }
        if(feof(pFile)) break;
    }
    fclose(pFile);
    pFile=NULL;

    graph.numOfVex=max;//存入点数

    Node *node,*s;
    node=Prim(&graph,src);
    
    s=node->next;
    while(s!=NULL){
        if(s->next==NULL) printf("(%d,%d,%d)\n",s->x,s->y,s->weight);
        else printf("(%d,%d,%d),",s->x,s->y,s->weight);
        s=s->next;
    }

    system("pause");
    return 0;
}

Node* Prim(MGraph *graph,int src)
{
    int i=0,j=0,min=0,flag2=0;
    Closedge closedge;
    Node *node;//头指针
    Node *s;
    Node *p;//尾指针
    --src;//对齐数组
    for(i=0;i<MAX_V_NUM;i++)
    {
        closedge[i].adjvex=0;
        closedge[i].lowcost=0;
        closedge[i].flag=0;
    }//初始化

    node=(Nodelist)malloc(sizeof(Node));
    node->next=NULL;//头结点
    p=node;

    for(i=0;i<graph->numOfVex-1;i++)//n个点n-1条边
    {
        if(i!=0) src=min;//更新src
        closedge[src].flag=1;//标记已纳入最小生成树

        for(j=0;j<graph->numOfVex;j++)
        {
            if(graph->Edges[src][j]!=0&&!(graph->vexs[src]&&graph->vexs[j]))//保证每次有新点加入
            {
                if(closedge[j].lowcost==0||(!closedge[j].flag&&closedge[j].lowcost>graph->Edges[src][j]))//加入新点；去掉已经被选中的边；比较新边
                {
                    closedge[j].adjvex=src;
                    closedge[j].lowcost=graph->Edges[src][j];
                }
            }
        }

        min=-1;//重置min
        for(j=0;j<graph->numOfVex;j++)
        {
            if((closedge[j].flag==1||closedge[j].lowcost==0)||(graph->vexs[src]&&graph->vexs[j])) continue;//跳过已纳入的边的和未纳入的点；防止形成回路
            else if(min==-1) min=j;
            else{
                min=(closedge[j].lowcost<closedge[min].lowcost)?j:min;//定位和已纳入的点相连的所有边中最短的一条
            }
        }

        //头插法创建并链接新结点
        // s=(Nodelist)malloc(sizeof(Node));
        // s->next=node->next;
        // s->weight=closedge[min].lowcost;
        // s->x=closedge[min].adjvex+1;
        // s->y=min+1;
        // node->next=s;

        //尾插法创建并链接新结点
        s=(Nodelist)malloc(sizeof(Node));
        s->next=NULL;
        s->weight=closedge[min].lowcost;
        s->x=closedge[min].adjvex+1;
        s->y=min+1;
        p->next=s;
        p=s;

        graph->vexs[closedge[min].adjvex]=1;
        graph->vexs[min]=1;//标记已访问的点，防止把已访问的点当作新点再次连接
    }
    return node;
}

void clearGraph(MGraph *graph)
{
    int i=0,j=0;
    for(i=0;i<MAX_V_NUM;i++)
    {
        graph->vexs[i]=0;
        for(j=0;j<MAX_V_NUM;j++)
        {
            graph->Edges[i][j]=0;
        }
    }
}

