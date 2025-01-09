#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "dstest-H.h"
#define NAME "qinghuanshili"//在此设置姓名
/**************************************************************************************/
/*          The Target File Must Be Named As dstestfile(Nov.).txt and be placed       */
/*      at current director.                                                          */
/*                                                                                    */
/*      Auther:              20232401xpk                                              */
/*      Date:                2025.1.9                                                 */
/*      Language:            C                                                        */
/**************************************************************************************/


/*The defination of some functions*/
Node* Prim(MGraph *graph, int src);
void clearGraph(MGraph *graph);
tcmdNode* FindCmd(tcmdNode* head,char *cmd);
FILE* menu();
int Help();
int List();
int Quit();
int Check();

static tcmdNode head[] =
{
    {"help", "There are some tips!", Help, &head[1]},
    {"list", "Listing useful files...", List, &head[2]},
    {"prod", "Producting the minest tree with Prim...", NULL, &head[3]},
    {"check", "Choose file and have a check...", Check, &head[4]},
    {"quit", "Good Bye~", Quit, NULL}
};

int main(void)
{
    int i=0,j=0,src=0,max=0;
    int temp1=0,temp2=0,temp3=0;
    char ret;
    MGraph graph;

    printf("Hello!Welcome to the menu!Enter \"help\" to learn more.\n");
    
    while(1)
    {
        clearGraph(&graph);//初始化
        FILE *pFile=menu();

        //错误处理————文件名错误
        if(pFile==NULL)
        {
            perror("\e[1;31mWarning\e[0m:Fail in reading file.Maybe there is no such a file.\n");
            continue;
        }

        //错误处理————空文件
        ret=fgetc(pFile);
        if(feof(pFile))
        {
            printf("\e[1;31mWaring\e[0m:It seems this is a empty file.\n");
            fclose(pFile);
            pFile=NULL;
            continue;
        }

        rewind(pFile);//指针复位

        i=0;j=0;//初始化
        while(1)
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
                j++;
            }
            if(feof(pFile)) break;
        }

        fclose(pFile);
        pFile=NULL;

        //错误处理————文件中边数不够
        if(j!=graph.numOfEdges)
        {
            printf("\e[1;31mWaring\e[0m:It seems there are some wrongs with the context of the file.\n");
            fclose(pFile);
            pFile=NULL;
            continue;
        }

        graph.numOfVex=max;//存入点数

        Node *node,*s;
        node=Prim(&graph,src);
    
        s=node->next;
        while(s!=NULL)
        {
            if(s->next==NULL) printf("(%d,%d,%d)\n",s->x,s->y,s->weight);
            else printf("(%d,%d,%d),",s->x,s->y,s->weight);
            s=s->next;
        }
        printf("\n");
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

FILE* menu()
{   
    /*Cmd Line begins*/
    while(1)
    {
        char filename[MAX_FILE_LEN];

        //打印当前目录
        system("chdir > temp.txt");
        FILE *tp=fopen("temp.txt","r");
        char c;
        c=fgetc(tp);
        while(c!='\n')
        {
            printf("\e[1;35m%c",c);
            c=fgetc(tp);
        }
        fclose(tp);
        tp=NULL;
        system("del temp.txt");

        printf("\e[0m@\e[1;36m%s\e[0m$\e[5mInput>>> \e[0m",NAME);
        char cmd[CMD_MAX_LEN];
        scanf("%s",cmd);
        
        tcmdNode* p=FindCmd(head,cmd);

        //错误处理————指令错误
        if(p==NULL)
        {
            printf("Wrong Cmd!\n");
            continue;
        }
        printf("%s : %s\n",p->cmd,p->desc);
        
        if(p->cmd=="prod")
        {
            printf("Please choose the target file:");
            scanf("%s",filename);
            FILE *pfile1=fopen(filename,"r");
            return pfile1;
        }

        else if(p->handler!=NULL)
        {
            p->handler();
        }

        printf("\n");
    }
}

tcmdNode* FindCmd(tcmdNode* head,char *cmd)
{
    if(head==NULL||cmd==NULL)
    {
        return NULL;
    }
    tcmdNode* p=head;
    while(p!=NULL)
    {
        if(strcmp(p->cmd,cmd)==0)
        {
            return p;
        }
        p=p->next;
    }
    return NULL;
}

int Help()
{
    printf("Menu List:\n");
    tcmdNode *p=head;
    printf("+---------------------------------------------------------+");
    while(p!=NULL)
    {
        printf("\n");
        printf("|%-5s%52s|",p->cmd,p->desc);
        p=p->next;
    }
    printf("\n+---------------------------------------------------------+\n");

    return 0;
}

int List()
{
    printf("The target file must be named dstestfile(Nov).txt\n");
    system("dir | findstr /r \"dstestfile[0-9]*.txt\"");
    return 0;
}

int Quit()
{
    printf("Press any key to continue...\n");
    system("pause");
    exit(0);
}

int Check()
{
    char filename[MAX_FILE_LEN];
    printf("Please choose the file:");
    scanf("%s",filename);

    FILE *pfile=fopen(filename,"r");
    char ch;

    ch=fgetc(pfile);
    while(ch!=EOF)
    {
        printf("%c",ch);
        ch=fgetc(pfile);
    }
    fclose(pfile);
    pfile=NULL;

    printf("\n");
    return 0;
}