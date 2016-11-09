#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <dirent.h>
#include <ctype.h>

#define SIZE 4000
struct node1
{
    char word[100];
    int df;
    struct node2 *next;
};
typedef struct node1 Term;

struct node2
{
    char doc_name[100];
    int fre;
    struct node2 *next;
};
typedef struct node2 Propety;

int loadindex(Term head[],char *filename);
void Lowercase (char letter);

void main()
{
    int i = 0;
    int num = 0;
    int isfound = 0;
    int low,mid,hig;
    Term head[SIZE];
    Propety *temp;
    char str[100];

    for(i = 0;i <SIZE;i ++)
    {
        strcpy(head[i].word,"0");
        head[i].df = 0;
        head[i].next = NULL;
    }

    num = loadindex(head,"indexfile");//载入索引，并获得其大小
   
    //用户输入
    printf("please input the word that you want to find:\n");
    scanf("%s",str);

    //将大写字母变成小写
    for(i = 0;str[i] != '\0';i ++)
    {
        Lowercase (str[i]);
    }

    //折半查找
    low = 0;
    hig = num;
    mid = (low + hig) / 2;
    for(i = 0;i < num/2;i ++)
    {
        if(str[0] > head[mid].word[0])
        {
            low = mid;
            mid = (low + hig) / 2;
        }
        if(str[0] < head[mid].word[0])
        {
            hig = mid;
            mid = (low + hig) / 2;
        }
        if(str[0] == head[mid].word[0])
        {
            break;
        }
    }
    i = mid;
    while(str[0] == head[i].word[0])
    {
        if(strcmp(str,head[i].word) == 0)
        {
            temp = head[i].next;
            while(temp != NULL)
            {
                printf("%s\n",temp->doc_name);
                temp = temp->next;
            }
            isfound = 1;
            break;
        }
        else
        {
            i --;
        }
    }
    i = mid + 1;
    while(str[0] == head[i].word[0])
    {
        if(strcmp(str,head[i].word) == 0)
        {
            temp = head[i].next;
            while(temp != NULL)
            {
                printf("%s\n",temp->doc_name);
                temp = temp->next;
            }
            isfound = 1;
            break;
        }
        else
        {
            i ++;
        }
    }
    if(isfound == 0)
    {
        printf("can not find %s\n",str);
    }
}

//从索引中读取数据
int loadindex(Term head[],char *filename)
{
    int i = 0;
    int j = 0;
    int docfre = 0; 
    int sizeofterm = 0;
    FILE *fpi;
    Propety *previous, *current;  
    
    if((fpi = fopen(filename,"r")) == NULL)
    {
        printf("this file can not be opened.\n");
        return 0;
    }

    fscanf(fpi,"%d",&sizeofterm);//先读取索引的大小，即词元的数目
    printf("%d\n",sizeofterm);

    for(i = 0;i < sizeofterm-4;i ++)//将索引存入链表
    {
        fscanf(fpi,"%s",head[i].word);
        fscanf(fpi,"%d",&head[i].df);

        previous = malloc(sizeof(Propety));

        fscanf(fpi,"%s",previous->doc_name);
        fscanf(fpi,"%d",&previous->fre);

        head[i].next = previous;
        previous->next = NULL;
        docfre = head[i].df;

        for(j = 0;j < docfre - 1;j ++)
        {
            current = malloc(sizeof(Propety));

            fscanf(fpi,"%s",current->doc_name);
            fscanf(fpi,"%d",&current->fre);

            current->next = NULL;
            previous->next = current;
            previous = current;
        }
    }
    return sizeofterm;//返回索引大小
}

//将所有字母变成小写
void Lowercase(char letter)
{
    if(letter >= 65 && letter <= 90)
    {
        letter = letter + 32;
    }
}

