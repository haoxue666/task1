#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <dirent.h>

#define SIZE 15000
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

int Tokenizer (char *filein,Term head[]);
char Lowercase (char letter);
void Firstlettersort(Term head[],int sizrofterm);
void Relvancesort (Term head[],int sizeofterm);
void Saveindex (Term head[],int sizeofterm,char *filename);


int sizeofterm = 0;//词元大小
void main()
{   
    DIR *dir;
    struct dirent *ptr;
    int n = 0;
    int i = 0;
    Term head[SIZE];
    char dirent[100];
    char temp_dirent[100];
    for(i = 0;i < SIZE;i ++)
    {
        strcpy(head[i].word,"0");
        head[i].df = 0;
        head[i].next = NULL;
    }
    printf("Please input the dirent you want to search:\n");
    scanf("%s",dirent);
    dir = opendir(dirent);
    if((ptr = readdir(dir)) == NULL)
    {
        printf("the dirent can not be find.\n");
    }
    else
    {
        //if((strncmp(ptr->d_name,"text_",5)) == 0)
        //{   
            strcpy(temp_dirent,dirent);
            printf("d_name:%s\n",ptr->d_name);
            strcat(temp_dirent,"/");
            strcat(temp_dirent,ptr->d_name);
            printf("dirent name:%s\n",temp_dirent);
            n = Tokenizer(temp_dirent,head);
            printf("%d\n",n);
        //}
        while((ptr = readdir(dir)) != NULL)
        {
            //if((strncmp(ptr->d_name,"text_",5)) == 0)
            //{ 
                strcpy(temp_dirent,dirent);
                printf("d_name:%s\n",ptr->d_name);
                strcat(temp_dirent,"/");
                strcat(temp_dirent,ptr->d_name);
                printf("dirent name:%s\n",temp_dirent);
                n = Tokenizer(temp_dirent,head);
                printf("%d\n",n);
            //}
        }
    }
    closedir(dir);
    Firstlettersort(head,n);
    Relvancesort(head,n);
    Saveindex(head,n,"indexfile");
}

//将所有字母变成小写
char Lowercase(char letter)
{
    if(letter >= 65 && letter <= 90)
    {
        letter = letter + 32;
    }
    return letter;
}

//将文档分为一个一个单独的单词,并去除标点
int Tokenizer (char *filein,Term head[])
{
    FILE *fpi;
    char str[100];
    if((fpi = fopen(filein,"r")) == NULL)
    {
         printf("this file can not be opened.\n");
         return 0;
    }
    char ch = '0';
    int i = 0,j = 0;
    int existflag = 0;
    int existindoc = 0;
    Propety *temp,*current;

    ch = fgetc(fpi);
    while(ch != EOF)
    {//每获得一个单词循环一次
        while(ch != ' ' && ch != EOF)//每获得一个字母循环一次
        {
            ch = Lowercase (ch);//将大写字母变成小写
            if((ch >= 97 && ch <=122))//去除所有非小写字母的字符
            {
                str[i] = ch;
		if(i < 100)
		{
                     i ++;
		}
            }
            ch = fgetc(fpi);//获取下一个字符
        }
        str[i] = '\0';//添上字符串结束符
        i = 0;
        //如果获得的字符是空字符，直接向下获取下一个字符
        while(ch == ' ' || ch == '\n')
        {
            ch = fgetc(fpi);
        }
        if(sizeofterm == 0)//第一个词元的处理
        {
            strcpy(head[sizeofterm].word,str);
            head[sizeofterm].df = 1;
            current = malloc(sizeof(Propety));
            head[sizeofterm].next = current;
            strcpy(current->doc_name,filein);
            current->fre = 1;
            current->next = NULL;
            sizeofterm ++;
        }
        else//其他词元的处理
        {
            for(j = 0;j < sizeofterm;j ++)
            {   
                if(strcmp(head[j].word,str) == 0)//词元之前出现过
                {
                    for(temp = head[j].next;temp != NULL;temp = temp->next)
                    {
                        if((strcmp(filein,temp->doc_name)) == 0)//词元在此文档中出现过
                        {
                            temp->fre ++;
                            existflag = 1;
                            existindoc = 1;
                            break;
                        }
                    }
                    //词元在此文档第一次出现,但在其他文档中出现过
                    if(existindoc == 0)
                    {
                        head[j].df ++;
                        for(temp = head[j].next;temp->next != NULL;temp = temp->next);
                        current = malloc(sizeof(Propety));
                        temp->next = current;
                        strcpy(current->doc_name,filein);
                        current->fre = 1;
                        current->next = NULL;
                        existflag = 1;
                    }
                    existindoc = 0;
                    break;
                }
            }
            if(existflag == 0)//词元第一次出现
            {
                strcpy(head[sizeofterm].word,str);
                head[sizeofterm].df = 1;
                current = malloc(sizeof(Propety));
                head[sizeofterm].next = current;
                strcpy(current->doc_name,filein);
                current->fre = 1;
                current->next = NULL;
                sizeofterm ++;
            }
            existflag = 0;
        }
    }
    //关闭文件
    fclose(fpi);
    return sizeofterm;
}

//将链表按照首字母大小排序
void Firstlettersort(Term head[],int sizrofterm)
{
    int i = 0;
    int j = 0;
    Term hold;
    for(i = 0;i < sizeofterm-1;i ++)
    {
        for(j = 0;j < sizeofterm-i-1;j ++)
        {
            if(head[j].word[0] > head[j+1].word[0])
            {
                hold = head[j];
                head[j] = head[j+1];
                head[j+1] = hold;
            }
        }
    }

}

//将链表按照相关性排序
void Relvancesort (Term head[],int sizeofterm)
{
    int i = 0;
    int docfre = 0;
    int j = 0;
    int k = 0;
    Propety *previous,*current;
    Propety *hold;
    hold = malloc(sizeof(Propety));
    for(i = 0;i < sizeofterm;i ++)
    {
        docfre = head[i].df;
        for(j = 0;j < docfre-1;j ++ )
        { 
            previous = head[i].next;
            current = previous->next;
            for(k = 0;k < docfre-1-j;k ++)
            {
                if((previous->fre) < (current->fre))
                {
                    strcpy(hold->doc_name,previous->doc_name);
                    hold->fre = previous->fre;
                    strcpy(previous->doc_name,current->doc_name);
                    previous->fre = current->fre;
                    strcpy(current->doc_name,hold->doc_name);
                    current->fre = hold->fre; 
                }
                previous = current;
                current = current ->next;
            }
        }
    }
}

//将构建好的链表索引存入文档中被备用
void Saveindex(Term head[],int sizeofterm,char *fileout)
{
    int i = 0;
    FILE *fpo;
    if((fpo = fopen(fileout,"w")) == NULL)
    {
        printf("this file can not be opened.\n");
        return;
    }
    Propety *temp;
    fprintf(fpo,"%d\n",sizeofterm);
    for(i = 1;i < sizeofterm;i ++)
    {
        fprintf(fpo,"%s %d",head[i].word,head[i].df);

        for(temp = head[i].next;temp != NULL;temp = temp->next)
        {
            fprintf(fpo," %s %d",temp->doc_name,temp->fre);
        }
        fprintf(fpo,"\n");
    }
    fclose(fpo);
}
