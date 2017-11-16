#include <iostream>
#include <fstream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <cstring>
//#include "B.h"
#define max 5
#define maxvalue 1000
using namespace std;
int L=0;
struct Btree
{
    bool flag;
    int count;
    char key[max+1][100];
    int offset[max+1];
    Btree* ptr[max+1];
    Btree* parent,*next,*prior;
};
Btree* b[10],*b_join[5];
struct Triple
{
    Btree*  r;
    int locate;
};
int ccl_cmp(char x[],char y[])
{
    int m,n;
    if(x[0]>='0'&&x[0]<='9'&&y[0]>='0'&&y[0]<='9')
    {
        m=atoi(x);
        n=atoi(y);
        if(m==n)return 0;
        if(m>n)return 1;
        if(m<n)return -1;
    }
    else
    {
        return strcmp(x,y);
    }
}
Triple* search(Btree* b[],char x[],int num)
{
    Triple* triple=new Triple;
    int i=0;
    Btree * p=b[num],*q;
    while(p!=NULL)
    {
        i=1;
        strcpy(p->key[p->count+1],"~~~~~~");
        while(ccl_cmp(x,p->key[i])>0)
            i++;
        // i--;
        if(strcmp(x,p->key[i])==0&&p->flag)
        {
            q=p;
            break;
        }
        // i++;
        if(i>p->count)
        {
            q=p;
            p=p->ptr[i-1];
            //i--;
            continue;
        }
        q=p;
        p=p->ptr[i];
    }
    triple->locate=i;
    triple->r=q;
    return triple;
}
void add(Btree* &m,int j,char x[],Btree* &aq,int offset)
{
    if(m->count==0)
    {
        strcpy(m->key[1],x);
        m->offset[1]=offset;
        m->ptr[1]=aq;
        return;
    }
    else if(j==m->count+1)
    {
        strcpy(m->key[j],x);
        m->offset[j]=offset;
        m->ptr[j]=aq;
        return;
    }
    else
        for(int i=m->count; i>=j; i--)
        {
            strcpy(m->key[i+1],m->key[i]);
            m->offset[i+1]=m->offset[i];
            m->ptr[i+1]=m->ptr[i];
        }
    strcpy(m->key[j],x);
    m->offset[j]=offset;
    m->ptr[j]=aq;
}
void move(Btree* &p,Btree* &q,int s)
{
    int j=1,y=1;
    q->count=s;
    p->count=max-s;
    for(int i=1; i<=s; i++)
    {
        strcpy(q->key[j],p->key[i]);
        q->offset[j]=p->offset[i];
        q->ptr[j]=p->ptr[i];
        j++;
    }
    for(int k=s+1; k<=max; k++)
    {
        strcpy(p->key[y],p->key[k]);
        p->offset[y]=p->offset[k];
        p->ptr[y]=p->ptr[k];
        y++;
    }
    if(p->flag==1)
    {
        if(p->prior==NULL)
        {
            q->next=p;
            p->prior=q;
        }
        else
        {
            q->prior=p->prior;
            p->prior->next=q;
            q->next=p;
            p->prior=q;
        }
    }
}
void Insert(Btree* b[],char x[],int offset,int num)
{
    Triple* p=search(b,x,num);
    //cout<<p->locate<<" "<<p->r->key[p->locate]<<endl;
    Btree *q,*t,*root,*aq=NULL,*bigest=b[num],*tmp;
    int j=p->locate,i,x1,y1,f=0;
    char y[20];
    Btree* m=p->r;
    tmp=bigest->ptr[bigest->count];
    if(ccl_cmp(x,bigest->key[bigest->count])>0&&bigest->ptr[1])
    {
        while(tmp)
        {
            strcpy(bigest->key[bigest->count],x);
            bigest=bigest->ptr[bigest->count];
            tmp=bigest->ptr[bigest->count];
        }
    }
    while(1)
    {
        if(m->count<max-1)
        {
            add(m,j,x,aq,offset);
            m->count++;
            return;
        }
        int s=(max+1)/2;

        add(m,j,x,aq,offset);
        strcpy(y,m->key[m->count+1]);
        y1=m->offset[m->count+1];
        q=new Btree;
        for( i=0; i<max; i++)
            q->ptr[i]=NULL;
        q->next=NULL;
        q->prior=NULL;
        q->flag=m->flag;
        move(m,q,s);
        aq=m;
        strcpy(x,m->key[s]);
        x1=m->offset[s];
        if(m->parent!=NULL)
        {
            t=m->parent;

            j=1;
            strcpy(t->key[t->count+1],"~~~~~~");
            while(ccl_cmp(x,t->key[j])>0)
            {
                j++;
                if(strcmp(x,t->key[j])==0)
                {
                    f=1;
                    j=1;
                    strcpy(x,y);
                }
            }
            //	j++;
            if(f)
            {
                for(i=1; i<=t->count; i++)
                    if(t->ptr[i]==m)
                        t->ptr[i]=q;
            }
            else
            {
                aq=q;
            }
            q->parent=m->parent;
            m=t;
            continue;
        }
        else
        {
            root=new Btree;
            root->flag=0;
            root->count=2;
            root->parent=NULL;
            strcpy(root->key[1],x);
            strcpy(root->key[2],y);
            root->offset[1]=x1;
            root->offset[2]=y1;
            for( i=0; i<max; i++)
                root->ptr[i]=NULL;
            root->ptr[1]=q;
            root->ptr[2]=m;
            m->parent=q->parent=root;
            b[num]=root;
            return;
        }
    }

}
void creat()
{
    char table[100],table1[100],ch;
    int i,num,cc;
    cout<<"Please enter create table order: "<<endl;
    cin.getline(table,100);
    fstream input,newtable,count;
    input.open("model.txt",ios::out|ios::app);
    input<<table;
    input.close();
    count.open("count.txt",ios::out|ios::in);
    ch=count.get();
    ch++;
    cc=(int)ch-48;
    //cout<<count<<endl;
    b[cc]=new Btree;
    b[cc]->count=0;
    b[cc]->parent=b[cc]->next=b[cc]->prior=NULL;
    b[cc]->flag=1;
    for(i=0; i<max; i++)
        b[cc]->ptr[i]=NULL;
    count.seekp(ios::beg);
    count <<ch;
    count.close();
//    cout <<ch <<"--ch" <<endl;
//    count <<ch;
    //********************
    for( i=0;; i++)
    {
        if(table[i]=='(')break;
        table1[i]=table[i];  //table name
    }
    table1[i]='\0';
    strcat(table1,".txt");
//    cout<<table1;      //*******************
    newtable.open(table1,ios::out|ios::app);
    newtable<<ch;
    newtable.close();
}
bool pkey_check(char primary[],int num,char table[])
{
   fstream check;
   check.open(table,ios::in);
    int i,j;
    Btree* p=b[num],*q;
    while(p)
    {
        q=p;
        p=p->ptr[1];
    }
    while(q)
    {
        for(i=1; i<=q->count; i++)
        {
            //cout<<q->key[i]<<" "<<q->offset[i]<<"**" <<endl;   //***************
             check.seekp(q->offset[i]-1,ios::beg);
           char ch=check.get();
            //cout<<q->key[i]<<" "<<q->offset[i]<<"**"<<ch<<endl;
           if(ch!='~')continue;
            if(!strcmp(primary,q->key[i]))
                return 0;  //在

        }
        q=q->next;
    }
    return 1;  //不在
}
void do_insert(char name[],char order [],int pos,char primary[]) //table name,插入语句,'('的位置
{
    fstream in;
    char temp[100],flag[100],ch,ceshi[20],_name[20];
    flag[0]='~';
    flag[1]='\0';
    int j=0,num,k;
    for(k=0; k<strlen(name); k++) //_name 仅用于输出错误。。
        _name[k]=name[k];
    _name[k]='\0';
    strcat(name,".txt");
    in.open(name,ios::out|ios::in);
    for(int i=pos+1;; i++)  //从'('的下一个位置开始
    {
        if(order[i]==')')break;
        temp[j]=order[i];
        j++;
    }
    temp[j]=';';
    temp[j+1]='\0';
    strcat(flag,temp);
//    cout<<flag<<"--flag"<<endl;
//    cout<<temp<<endl;
//    cout<<flag;
    ch=in.get();
//    cout<<ch<<"ch"<<endl;
    num=int(ch)-48;
//    cout<<num<<endl;
    in.close();
    if(pkey_check(primary,num,name))
    {
//        cout<<flag<<"--flag"<<endl;
//        cout <<"I'm here!" <<endl;
        in.open(name,ios::out|ios::app);
        //cout<<in.tellg()<<endl;
        in<<flag;
//        in.close();
//        in.open(name,ios::in|ios::app);
        in.seekp(0,ios::end);
//        cout<<in.tellp()<<" "<<flag<<" "<<strlen(flag)<<endl;
        int xx=in.tellp()-strlen(flag);
//        cout <<xx <<"--XX" <<endl;
        //cout<<in.tellp()<<endl;
//        cout<<flag<<endl;
        //strcpy(ceshi,"78");
        Insert(b,primary,xx,num);
        in.close();
    }
    else cout<<"ERROR! Primary key \"" <<primary <<"\" already exists in table \"" <<_name <<"\"!"<<endl;
    //cout<<"hhh"<<endl;
    /* Triple *p=search(b,"90",num);
     cout<<p->locate<<" "<<p->r->key[p->locate]<<" "<<p->r->offset[p->locate]<<endl;
     in.close();
     in.open(name,ios::in);
     in.seekp(p->r->offset[p->locate],ios::beg);
     cout<<in.tellg();
     in.getline(flag,521,';');
     cout<<flag<<endl;*/
}
void insert()
{
    fstream check;
    char s[100],temp[50],tmp[50],newtmp[50],primary[20];
    int pos,i,j,num=0,k=0;
    cout<<"Please enter insert order: "<<endl;
    cin.getline(s,100);  //插入语句
    for( i=0;; i++)
    {
        if(s[i]=='(')
        {

            for(k=i+1,j=0; s[k]!=','; k++,j++)
                primary[j]=s[k];
            primary[j]='\0';  //第一个属性为主键
            break;
        }
        temp[i]=s[i];
    }

    temp[i]='\0';  //table name
    check.open("model.txt",ios::in);
    while(!check.eof())
    {

        check.getline(tmp,512,';');
//        cout<<tmp<<endl;
        for(j=0;; j++)
        {
            if(tmp[j]=='(')
            {
                pos=j;
                break;
            }
            newtmp[j]=tmp[j];
        }
        newtmp[j]='\0';  //table name

        if((strcmp(newtmp,temp))!=0)  //比对table name
            continue;
        else
        {
            //cout << newtmp<<" "<<s<<" "<<pos<<" "<<primary<<endl;
            do_insert(newtmp,s,pos,primary);
            //cout <<"---do select11" <<endl;
        }
    }
    check.close();
    cout <<"Insert complete!" <<endl;
}
struct sel_where
{
    bool flag;  //int or char[]
    int location;
    char ch;
    int num;
    char buffer[20];
};
int analysis(char from [],char where [],sel_where s_where[],bool f)
{
    //sel_where s_where[10];
    fstream mod;
    char tmp[50],newtmp[50],temp[50];
    int i,j,pos,count=0;
    if(!f)
    {
        strcpy(tmp,from);
        goto next;
    }
    mod.open("model.txt",ios::in);
    while(!mod.eof())
    {
        mod.getline(tmp,512,';');  //取model.txt中的一条元组
        for(j=0;; j++)
        {
            if(tmp[j]=='(')
            {
                pos=j;  //'('的标号
                break;
            }
            newtmp[j]=tmp[j];
        }
        newtmp[j]='\0';  //table name

        if((strcmp(newtmp,from))!=0)  //找到对应的关系模式
            continue;
        else
        {
            tmp[pos]=',';  //'('换成','
            for(i=pos,j=0; tmp[i]!=')'; i++,j++)
                tmp[j]=tmp[i];
            //tmp[j]=','; ----------------------------------
            //tmp[j+1]='\0';--------------------------------
            tmp[j]='\0';
            break;  //',score int,name char[10]'
        }
    }
next:
    //cout<<tmp<<" --"<<endl;//*******************************
    char where_tmp[20],table_tmp[20];
    int m,k,where_loc,table_loc,t=0,p,q,s;
    char c,ch;
    where[strlen(where)+2]='\0';
    where[strlen(where)+1]=' ';
    for(i=strlen(where); i>=1; i--)
        where[i]=where[i-1];
    where[0]=' ';  //' score>60 and name=wjl'
//cout<<where<<endl;//******************************
    for(i=0; i<strlen(where); i++)
    {
        if(where[i]=='>'||where[i]=='<'||where[i]=='=')
        {
            where_loc=i;  //运算符位置
            ch=where[i];  //运算符
            for(j=i-1,k=0; where[j]!=' '; j--,k++)
                where_tmp[k]=where[j];  //待比较属性score、name等的逆序
            where_tmp[k]='\0';
            //cout<<where_tmp<<endl<<j<<" "<<k<<" "<<endl;//******************
            for(m=0,j=k-1; m<j; m++,j--)  //变为顺序
            {
                c=where_tmp[m];
                where_tmp[m]=where_tmp[j];
                where_tmp[j]=c;
            }
            //cout<<where_tmp<<endl;//********************************
            for(m=0; m<strlen(tmp); m++)
            {
                //cout<<m<<endl;//********************************
                //if(tmp[m]==',') //-----
                // t++;  //------
                if(tmp[m]==',')  //---------------
                {
                    t++;
                    // cout<<"t: "<<t<<endl;
//                    for(p=m-1,q=0; tmp[p]!=' '; p--,q++)  //从','往回推
//                        table_tmp[q]=tmp[p];
//                    table_tmp[q]='\0';
//                    table_loc=p;  //关系模式中属性的前一位置（空格）
//                    //if(t==3)
//                    //cout<<"1"<<table_tmp<<endl;//******************************
//                    for(j=0,p=q-1; j<p; j++,p--)
//                    {
//                        c=table_tmp[j];
//                        table_tmp[j]=table_tmp[p];
//                        table_tmp[p]=c;
//                    }
                    for(p=m+1,q=0; tmp[p]!=' '; p++,q++)
                        table_tmp[q]=tmp[p];
                    table_tmp[q]='\0';
                    table_loc=p;  //空格的位置，后一位为'i',则为int;为'c'，则为char[]
                    //if(t==3)
                    //cout<<"2"<<m<<" "<<table_tmp<<endl;//********************************
                    if(strcmp(table_tmp,where_tmp)==0)
                    {
                        //cout<<where<<" "<<where_loc<<"&&"<<table_tmp<<endl;
                        char cmp_tmp[20];
                        for(j=where_loc+1,s=0; where[j]!=' '; j++,s++)
                        {
                            cmp_tmp[s]=where[j];
                        }
                        cmp_tmp[s]='\0';
                        //cout<<cmp_tmp<<"88"<<tmp[table_loc-1]<<endl;//***************************
                        if(tmp[table_loc+1]=='c')
                        {
                            s_where[count].ch=ch;
                            strcpy(s_where[count].buffer,cmp_tmp);
//                            cout <<s_where[count].buffer <<"--buffer" <<endl;
                            s_where[count].flag=0;
                            s_where[count].location=t;
                            // cout<<tmp<<" "<<count<<" "<<s_where[count].ch<<" "<<s_where[count].buffer<<" "<<s_where[count].location<<endl;
                            count++;
                        }
                        else if(tmp[table_loc+1]=='i')
                        {
                            s_where[count].ch=ch;
                            s_where[count].num=atoi(cmp_tmp);
//                            strcpy(s_where[count].buffer,cmp_tmp);
                            s_where[count].flag=1;
                            s_where[count].location=t;
                            // cout<<tmp<<" "<<count<<" "<<s_where[count].ch<<" "<<s_where[count].num<<" "<<s_where[count].location<<endl;
                            count++;
                        }
                        //cout<<m<<"((((";
                        break;
                    }

                }

            }
            t=0;
        }
    }
    mod.close();
    //cout<<s_where[0].buffer<<" "<<s_where[0].location<<" "<<s_where[0].flag<<endl;
    return count-1;
}
int comma(char project_tmp[],char from[],bool f)
{
    //cout<<project_tmp<<" "<<from<<endl;
    int j,pos,i,t=0,table_loc,c,m,p,q;
    char tmp[100],newtmp[50],table_tmp[50];
    fstream mod;
    if(!f)
    {
        strcpy(tmp,from);
        goto next;
    }
    mod.open("model.txt",ios::in);
    while(!mod.eof())
    {
        mod.getline(tmp,512,';');
        for(j=0;; j++)
        {
            if(tmp[j]=='(')
            {
                pos=j;
                break;
            }
            newtmp[j]=tmp[j];
        }
        newtmp[j]='\0';
        strcat(newtmp,".txt");
        //cout<<newtmp<<" "<<from<<endl;
        if((strcmp(newtmp,from))!=0)
            continue;
        else
        {
            //cout<<"ok"<<endl;
            tmp[pos]=',';
            for(i=pos,j=0; tmp[i]!=')'; i++,j++)
                tmp[j]=tmp[i];
            tmp[j]=',';
            tmp[j+1]='\0';
            break;
        }
    }

    //cout<<tmp<<"--tmp"<<endl;
next:
    for(m=0; m<strlen(tmp); m++)
    {
        //cout<<m<<endl;//********************************
        if(tmp[m]==',')  //---------------
        {
            t++;

            for(p=m+1,q=0; tmp[p]!=' '; p++,q++)
                table_tmp[q]=tmp[p];
            table_tmp[q]='\0';
            table_loc=p;
            //cout<<project_tmp<<" --project_tmp"<<table_tmp<<" --table_tmp"<<endl;
            if(strcmp(table_tmp,project_tmp)==0)
            {
                //cout<<"ok"<<t<<endl;
                mod.close();
                return t;
            }
            else
                continue;
        }
    }
    mod.close();
    return 0;
}
void project(char sentence[],char select[],char from[],bool flag)
{
    //cout<<select<<endl;
    sentence[0]=',';
    bool f=flag;
    int i,j,k,m,p,q,number=0,project_comma=0;
    char project_tmp[20];
    if(select[0]!=',')
    {
        select[strlen(select)+1]='\0';
        for(i=strlen(select); i>=1; i--)
            select[i]=select[i-1];
        select[0]=',';
    }
    //cout<<select<<"*"<<endl;
    for(i=0; i<strlen(select); i++)
    {
        if(select[i]==',')
        {
            //cout<<i<<endl;
            for(j=i+1,k=0; select[j]!=','&&select[j]!='\0'; j++,k++)
                project_tmp[k]=select[j];
            project_tmp[k]='\0';
            //cout<<project_tmp<<endl;
//cout<<"*** "<<number<<"*** --count"<<endl;
            number=comma(project_tmp,from,f);
            //cout<<number<<" --count"<<endl;
            if(number)
            {
                //cout<<sentence<<endl;
                for(m=0; m<strlen(sentence); m++)
                {
                    if(sentence[m]==',')
                        project_comma++;
                    if(project_comma==number)
                    {

                        //cout<<project_comma<<endl;
                        for(p=m+1; sentence[p]!=','&&sentence[p]!='\0'; p++)
                        {
                            cout<<sentence[p];
//                            cout <<endl <<"!!!!" <<endl;
                        }
                        cout<<" ";
                        project_comma=0;
                        break;
                    }
                }
            }

        }
        //cout<<select[i]<<endl;
    }
    cout<<endl;
}
bool judge_select(char tmp[],int count,sel_where s_where[])
{
   // cout<<tmp<<endl;
    int point,i,j,value,k;
    bool f=1;
    char table_tmp[20];
    tmp[0]=',';  //delete标志位转为','
    //cout<<tmp<<" hhh"<<endl;
    for(i=0; i<=count; i++)  //每一个元组tmp与每个选择项形成的结构体比较
    {
        if(s_where[i].location==1)
            continue;
        point=0;
        for(j=0; j<strlen(tmp); j++)
        {
            if(tmp[j]==',')
            {
                point++;
            }//cout<<j<<"%"<<point<<endl;}
            if(point==s_where[i].location&&tmp[j]==',')
            {
                // cout<<tmp[j]<<endl;
                for(k=0,j++; tmp[j]!=','&&tmp[j]!='\0'; k++,j++)
                    table_tmp[k]=tmp[j];
                table_tmp[k]='\0';
//                cout<<table_tmp<<"("<<s_where[i].buffer<<endl; //**********
                if(s_where[i].flag)
                {
                    value=atoi(table_tmp);
                    //cout<<value<<endl<<s_where[i].ch<<endl;
                    switch (s_where[i].ch)
                    {
                    case '=':
                        if(s_where[i].num!=value)return 0;
                        break;
                    case '>':
                        if(s_where[i].num>=value)return 0;
                        break;
                    case '<':
                        if(s_where[i].num<=value)return 0;
                        break;
                    }
                }
                else
                {
                    if(strcmp(s_where[i].buffer,table_tmp)!=0)
                        return 0;
                }

                //if(!f)break;
            }
        }
    }
    //cout<<"f: "<<f<<endl;
    //tmp[0]=',';  //消除元组前的逗号
    return 1;
}
void do_select(int count,char select[],char from[],sel_where s_where[])
{
    //cout<<select<<endl;
    Triple *p;
    Btree* q;
    int i,j,k,point=0,value,num,m;
    bool f=1;
    char tmp[20],table_tmp[10],ch,check[20];
    //cout<<count<<" "<<s_where[0].num<<s_where[0].location<<" "<<s_where[0].flag<<endl;
    //cout<<s_where[1].buffer<<s_where[1].location<<" "<<s_where[1].flag<<endl;
    fstream table;
    strcat(from,".txt");
    table.open(from,ios::in);
    ch=table.get();
    num=(int)ch-48;
//    cout<<count<<endl;
    if(count!=-1)
    {
        for(i=0; i<=count; i++)
        {
            if(s_where[i].location==1)
            {
                f=0;
                switch(s_where[i].ch)
                {
                case '=':
                    p=search(b,s_where[i].buffer,num);
                    //cout<<p->locate<<" "<<p->r->key[p->locate]<<" "<<p->r->offset[p->locate]<<endl;
                    if(strcmp(p->r->key[p->locate],s_where[i].buffer))continue;
                    table.seekp(p->r->offset[p->locate]-1,ios::beg);
                    table.getline(check,521,';');
                    //cout<<check<<endl;
                    //cout<<judge_select(check,count,s_where)<<" flag"<<endl;
                    if(strcmp(select,"*")==0&&judge_select(check,count,s_where))
                    {
                       //cout<<check<<endl;
                        for(m=0; m<strlen(check); m++)
                        {
                            if(check[m]==',') cout <<" ";
                            else cout <<check[m];
                        }
                        cout <<endl;
                        return;
                    }
                    cout<<check<<"jj"<<endl;
                    if(check[0]=='~'&&judge_select(check,count,s_where))
                        project(check,select,from,1);
                    break;
                case '>':
                    p=search(b,s_where[i].buffer,num);
                    q=p->r;
                    j=p->locate;
                    if(!ccl_cmp(s_where[i].buffer,q->key[j]))
                    {
                        if(j==q->count)
                        {
                            j=1;
                            q=q->next;
                        }
                        else
                            j++;
                    }
                    while(q)
                    {
                        for(; j<=q->count; j++)
                        {
                            //cout<<q->count<<" "<<j<<" "<<q->key[j]<<" "<<q->offset[j]<<"~"<<endl;
                            table.seekp(q->offset[j]-1,ios::beg);
                            table.getline(check,521,';');
                            if(strcmp(select,"*")==0&&judge_select(check,count,s_where))
                            {
//                                cout<<check<<endl;
                                for(m=0; m<strlen(check); m++)
                                {
                                    if(check[m]==',') cout <<" ";
                                    else cout <<check[m];
                                }
                                cout <<endl;
                                continue;
                            }
                            if((check[0]=='~')&&judge_select(check,count,s_where))
                                project(check,select,from,1);
                        }
                        q=q->next;
                        j=1;
                    }
                    break;
                case '<':
                    p=search(b,s_where[i].buffer,num);
                    q=p->r;
                    j=p->locate;
                    if(j==1)
                    {
                        q=q->prior;
                        j=q->count;
                    }
                    else
                        j--;
                    while(q)
                    {
                        for(; j>=1; j--)
                        {
                            //cout<<q->count<<" "<<j<<" "<<q->key[j]<<" "<<q->offset[j]<<"~"<<endl;
                            table.seekp(q->offset[j]-1,ios::beg);
                            table.getline(check,521,';');
                            if(strcmp(select,"*")==0&&judge_select(check,count,s_where))
                            {
//                                cout<<check<<endl;
                                for(m=0; m<strlen(check); m++)
                                {
                                    if(check[m]==',') cout <<" ";
                                    else cout <<check[m];
                                }
                                cout <<endl;
                                continue;
                            }
                            if(check[0]=='~'&&judge_select(check,count,s_where))
                                project(check,select,from,1);
                        }
                        q=q->prior;
                        if(q)
                            j=q->count;
                    }
                    break;
                }
            }
        }
        if(f)
        {
            table.getline(tmp,512,';');  //获取一个元组，元组间由分号隔开
            while(!table.eof())
            {
                if(strcmp(select,"*")==0&&judge_select(tmp,count,s_where))
                {
//                    cout<<tmp<<endl;
                    for(m=0; m<strlen(tmp); m++)
                    {
                        if(tmp[m]==',') cout <<" ";
                        else cout <<tmp[m];
                    }
                    cout <<endl;
                }
                else if(tmp[0]=='~'&&judge_select(tmp,count,s_where))
                    project(tmp,select,from,1);
                table.getline(tmp,512,';');
            }
        }
    }
    else
    {
        table.getline(tmp,512,';');  //获取一个元组，元组间由分号隔开
        while(!table.eof())
        {
            //cout<<tmp<<" "<<select<<" "<<from<<endl;
//            cout <<tmp <<"--tmp" <<endl;
            if(strcmp(select,"*")==0)
            {
//                cout<<tmp<<endl;
                for(m=0; m<strlen(tmp); m++)
                {
                    if(tmp[m]==',') cout <<" ";
                    else cout <<tmp[m];
                }
                cout <<endl;
            }
            else if(tmp[0]=='~')
            {

                project(tmp,select,from,1);
            }
            table.getline(tmp,512,';');
        }
    }
    //table.getline(tmp,512,';');
    /* tmp[0]=',';  //delete标志位转为','
     //cout<<tmp<<" hhh"<<endl;
     for(i=0; i<=count; i++)  //每一个元组tmp与每个选择项形成的结构体比较
     {
         point=0;
         for(j=0; j<strlen(tmp); j++)
         {
             if(tmp[j]==',')
             {
                 point++;
             }//cout<<j<<"%"<<point<<endl;}
             if(point==s_where[i].location&&tmp[j]==',')
             {
                 // cout<<tmp[j]<<endl;
                 for(k=0,j++; tmp[j]!=','&&tmp[j]!='\0'; k++,j++)
                     table_tmp[k]=tmp[j];
                 table_tmp[k]='\0';
                 // cout<<table_tmp<<"("<<s_where[i].flag<<endl;
                 if(s_where[i].flag)
                 {
                     value=atoi(table_tmp);
                     //cout<<value<<endl<<s_where[i].ch<<endl;
                     switch (s_where[i].ch)
                     {
                     case '=':
                         if(s_where[i].num!=value)f=0;
                         break;
                     case '>':
                         if(s_where[i].num>=value)f=0;
                         break;
                     case '<':
                         if(s_where[i].num<=value)f=0;
                         break;
                     }
                 }
                 else
                 {
                     if(strcmp(s_where[i].buffer,table_tmp)!=0)
                         f=0;
                 }

                 //if(!f)break;
             }
         }
     }
     //cout<<"f: "<<f<<endl;
     //tmp[0]=',';  //消除元组前的逗号
     if(f)
     {
         cout<<tmp<<endl;
         project(tmp,select,from,1);
     }
     f=1;
     table.getline(tmp,512,';');
        }*/

}
void substr(int num,char sstr[],char str[])  //提取第num个逗号处的属性名和类型
{
    int i,j,k,count=0;
    for(i=0; i<strlen(sstr); i++)
    {
        if(sstr[i]==',')
            count++;
        if(count==num&&sstr[i]==',')
        {
            for(j=i+1,k=0; sstr[j]!=','&&sstr[j]!='\0'; j++,k++)
                str[k]=sstr[j];
            str[k]='\0';
        }
        //if(count_1==c_1)cout<<old1<<endl;
    }
}
void join(char join1[],char join2[],char s1[],char s2[],char sel[],char outcome[])
{

    fstream pointer_1,pointer_2,pointer_join;
    char new_table[20],old1[20],old2[20],tmp_1[50],tmp_2[50],tmp[50];
    int c_1,c_2,i,j,k,p,q,count_1=0,count_2=0;
    strcpy (old1,join1);
    strcpy (old2,join2);

    strcat(old1,".txt");
    strcat(old2,".txt");
    //cout<<old1<<" "<<old2<<endl;
    c_1=comma(s1,old1,1);
    //cout<<c_1<<endl;      //**************
    c_2=comma(s2,old2,1);
    //cout<<c_2<<endl;      //**************

    pointer_1.open(old1,ios::in);
    pointer_1.get();
    // pointer_2.open(old2,ios::in);
    strcat(join1,"_");
    strcat(join1,join2);
    strcpy(new_table,join1);
    strcat(new_table,".txt");
    //cout<<new_table<<endl;
    pointer_join.open(new_table,ios::out|ios::app);
    pointer_1.getline(tmp_1,512,';');
    while(!pointer_1.eof())
    {
        //count_1=count_2=0;
//pointer_1.getline(tmp_1,512,';');
        //cout<<tmp_1<<" *1* "<<endl;
        if(tmp_1[0]=='!')
        {
            pointer_1.getline(tmp_1,512,';');
            continue;
        }
        else tmp_1[0]=',';
        substr(c_1,tmp_1,old1);
        cout<<tmp_1<<" "<<old1<<"  --old1"<<endl;
        strcpy(tmp,tmp_1);
        pointer_2.open("person.txt",ios::in);
        pointer_2.get();
        pointer_2.getline(tmp_2,512,';');
        while(!pointer_2.eof())
        {
            count_2=0;

            //cout<<tmp_2<<" *2* "<<endl;
            if(tmp_2[0]=='!')
            {
                pointer_2.getline(tmp_2,512,';');
                continue;
            }
            else tmp_2[0]=',';
            substr(c_2,tmp_2,old2);
            cout<<tmp_2<<" "<<old2<<"  --old2"<<endl;
            if(strcmp(old1,old2)==0)
            {
                //cout<<old1<<"99"<<endl;
                //cout<<tmp_1<<" "<<tmp_2<<endl;
                for(i=strlen(tmp_1),j=0; j<strlen(tmp_2); i++,j++)
                    tmp_1[i]=tmp_2[j];
                tmp_1[i]=';';
                tmp_1[i+1]='\0';
                //cout<<tmp_1<<endl;
                pointer_join<<tmp_1;
                //cout<<sel<<"99"<<endl;
                project(tmp_1,sel,outcome,0);
                strcpy(tmp_1,tmp);


            }
            pointer_2.getline(tmp_2,512,';');
        }
        pointer_2.close();

        pointer_1.getline(tmp_1,512,';');
    }

//cout<<tmp_1<<endl;
}
void combine(char table1[],char table2[],char outcome[])
{
    fstream mod;
    char tmp[50],newtmp[50];
    int pos,i,j,k=0,n=0;
    bool f=0;
    mod.open("model.txt",ios::in);
    mod.getline(tmp,512,';');
//    cout<<tmp<<endl;            //******************
    while(!mod.eof())
    {
        for(j=0;; j++)
        {
            if(tmp[j]=='(')
            {
                pos=j;
                break;
            }
            newtmp[j]=tmp[j];
        }
        newtmp[j]='\0';
//        cout<<"*"<<newtmp<<"*"<<endl;      //******************
        //strcat(newtmp,".txt");
        if((strcmp(newtmp,table1)==0 && n==0)||(n==1 && strcmp(newtmp,table2)==0))
        {
//            cout<<"ok"<<endl;              //******************
            //cout<<newtmp<<endl;
            n++;
            tmp[pos]=',';
            for(i=pos; tmp[i]!=')'; i++,k++)
                outcome[k]=tmp[i];
            //outcome[j]=',';
            outcome[k]='\0';
            mod.close();
            f=1;
            //break;
        }
        if(f)mod.open("model.txt",ios::in);
        f=0;
        mod.getline(tmp,512,';');
    }

    //cout<<outcome<<endl;      //*****************
}
void btree_join(char table0[],char table1[],char s0[],char s1[],char sel[],char outcome[],char where[])
{
    //cout<<table0<<" "<<s0<<" "<<table1<<" "<<s1<<endl;
    sel_where s_where[10];
    fstream pointer_0,pointer_1;
    Btree* p_0,*p_1;
    char tmp[50],table0_tmp[20],table1_tmp[20],index_0[50],index_1[50],new_tmp[50];
    int i,j,k,h,c_1,c_2,count_1=0,count_2=0,offset,count;

    for(i=0; i<2; i++)
    {
        b_join[i]=new Btree;
        b_join[i]->count=0;
        b_join[i]->parent=b_join[i]->next=b_join[i]->prior=NULL;
        b_join[i]->flag=1;
        for(j=0; j<max; j++)
            b_join[i]->ptr[j]=NULL;
    }
    strcat(table0,".txt");
    strcat(table1,".txt");
    c_1=comma(s0,table0,1);
    //cout<<c_1<<endl;
    c_2=comma(s1,table1,1);
    //cout<<c_2<<endl;
    pointer_0.open(table0,ios::in);
    pointer_0.get();
    pointer_0.getline(tmp,512,';');
    while(!pointer_0.eof())
    {
        offset=pointer_0.tellg()-strlen(tmp);
        if(tmp[0]=='~')
        {
            tmp[0]=',';
            for(i=0; tmp[i]!='\0'; i++)
            {
                if(tmp[i]==',')
                    count_1++;
                if(tmp[i]==','&&count_1==c_1)
                {
                    for(i++,j=0; tmp[i]!=','&&tmp[i]!='\0'; i++,j++)
                        table0_tmp[j]=tmp[i];
                    table0_tmp[j]='\0';
                    break;
                }
            }
            count_1=0;
            Insert(b_join,table0_tmp,offset,0);
        }
        pointer_0.getline(tmp,512,';');
    }
    pointer_0.close();


    pointer_1.open(table1,ios::in);
    pointer_1.get();
    pointer_1.getline(tmp,512,';');
    while(!pointer_1.eof())
    {
        offset=pointer_1.tellg()-strlen(tmp);
        if(tmp[0]=='~')
        {
            tmp[0]=',';
            for(i=0; tmp[i]!='\0'; i++)
            {
                if(tmp[i]==',')
                    count_2++;
                if(tmp[i]==','&&count_2==c_2)
                {
                    for(i++,j=0; tmp[i]!=','&&tmp[i]!='\0'; i++,j++)
                        table1_tmp[j]=tmp[i];
                    table1_tmp[j]='\0';
                    break;
                }
            }
            count_2=0;
            Insert(b_join,table1_tmp,offset,1);
        }
        pointer_1.getline(tmp,512,';');
    }
    pointer_1.close();
    Btree* p=b_join[0],*q;
    while(p)
    {
        q=p;
        p=p->ptr[1];
    }
    p_0=q;
    p=b_join[1];
    while(p)
    {
        q=p;
        p=p->ptr[1];
    }
    p_1=q;
    i=1,j=1;
    while(p_0!=NULL&&p_1!=NULL)
    {
        if(ccl_cmp(p_0->key[i],p_1->key[j])==0)
        {
            cout<<p_0->key[i]<<" --1--  "<<p_1->key[j]<<" --2-- "<<endl;
            pointer_0.open(table0,ios::in);
            pointer_1.open(table1,ios::in);
            pointer_0.seekp(p_0->offset[i]-1,ios::beg);
            pointer_0.getline(index_0,521,';');
            pointer_1.seekp(p_1->offset[j]-1,ios::beg);
            pointer_1.getline(index_1,521,';');
            //cout<<index_0<<" "<<index_1<<endl;
            i++;
            j++;
            if(i>p_0->count)
            {
                i=1;
                p_0=p_0->next;
            }
            if(j>p_1->count)
            {
                j=1;
                p_1=p_1->next;
            }
            pointer_0.close();
            pointer_1.close();
            index_0[0]=',';
            index_1[0]=',';
            for(k=strlen(index_0),h=0; h<strlen(index_1); k++,h++)
                index_0[k]=index_1[h];
            index_0[k]=';';
            index_0[k+1]='\0';
            count=analysis(outcome,where,s_where,0);
            if(judge_select(index_0,count,s_where))
            project(index_0,sel,outcome,0);
        }
        else if(ccl_cmp(p_0->key[i],p_1->key[j])<0)
        {
            //cout<<"<"<<endl;
            i++;
            if(i>p_0->count)
            {
                i=1;
                p_0=p_0->next;
            }if(p_0)
             cout<<p_0->key[i]<<" --1-- "<<endl;
        }
        else
        {
            // cout<<">"<<endl;
            j++;
            if(j>p_1->count)
            {
                j=1;
                p_1=p_1->next;
            }if(p_1)
            cout<<p_1->key[j]<<" --2-- "<<endl;
        }
    }
}
void select()
{
    bool flag=0;
    int i,j,k;
    char sel[20],from[20],where[50],table1[20],table2[20],s1[20],s2[20],tmp[20],outcome[50],where_tmp[50];
    char temp[50];
    cout<<"select:"<<endl;
    cin.getline(sel,20);
    cout<<"from:"<<endl;
    cin.getline(from,20);
    cout<<"where:"<<endl;
    cin.getline(where,50);
    //sel_where* s_where;
    sel_where s_where[20];
    for(i=0; i<strlen(from); i++)
        if(from[i]==',')  //最多支持2个表连接
        {
            flag=1;
            for(j=0; j<i; j++)
                table1[j]=from[j];
            table1[j]='\0';
            for(k=i+1,j=0; from[k]!='\0'; k++,j++)
                table2[j]=from[k];
            table2[j]='\0';
            break;
        }
    //cout<<table1<<" "<<table2<<endl;
    if(!flag)      //单表选择
    {
        int count=analysis(from,where,s_where,1);
        //cout<<count;
        do_select(count,sel,from,s_where);
    }
    else           //2个表选择、连接
    {
        for(i=0; where[i]!='.'; i++)         //student.name=teacher.name
            tmp[i]=where[i];
        tmp[i]='\0';
        //cout<<tmp<<endl;
        i++;
        if(strcmp(tmp,table1)==0)
        {
            for(j=0; where[i]!='='; i++,j++)
                s1[j]=where[i];
            s1[j]='\0';
        }
        else if(strcmp(tmp,table2)==0)
        {
            for(j=0; where[i]!='='; i++,j++)
                s2[j]=where[i];
            s2[j]='\0';
        }
        else cout<<"error1"<<endl;
        i++;
        for(j=0; where[i]!='.'; i++,j++)
            tmp[j]=where[i];
        tmp[j]='\0';
        i++;
        //cout<<tmp<<endl;
        if(strcmp(tmp,table1)==0)
        {
            for(j=0; where[i]!=' '&&where[i]!='\0'; i++,j++)
                s1[j]=where[i];
            s1[j]='\0';
        }
        else if(strcmp(tmp,table2)==0)
        {
            for(j=0; where[i]!=' '&&where[i]!='\0'; i++,j++)
                s2[j]=where[i];
            s2[j]='\0';
        }
        else cout<<"error2"<<endl;
        if(where[i]==' ')
        {
            i+=5;
            for(j=0; where[i]!='\0'; i++,j++)
            {
                where_tmp[j]=where[i];
            }
            where_tmp[j]='\0';
        }
        combine(table1,table2,outcome);
//        cout<<table1<<" "<<table2<<endl;          //****************
//        cout<<outcome<<"  --outcome"<<endl;       //****************
//cout<<where_tmp<<endl;
        btree_join(table1,table2,s1,s2,sel,outcome,where_tmp);
    }
}
void index()
{
    char table[100],table1[100],table2[100],index_insert[20];
    int i,j,k,count,offset;
    char ch;
    fstream input,index_table,in;
    input.open("model.txt",ios::in);
    //cout<<"jj";
    input.getline(table,512,';');
//    cout<<"jj";
//    cout<<table;
    //********************
    while(!input.eof())
    {
//cout<<table<<endl;
        for( i=0;; i++)
        {
            if(table[i]=='(')break;
            table1[i]=table[i];  //table name
        }
        table1[i]='\0';
        strcat(table1,".txt");
        //cout<<table1<<endl;
        index_table.open(table1,ios::in);
        ch=index_table.get();
        //cout<<ch<<endl;
        count=(int)ch-48;
        //cout<<count<<endl;
        b[count]=new Btree;
        b[count]->count=0;
        b[count]->parent=b[count]->next=b[count]->prior=NULL;
        b[count]->flag=1;
        for(i=0; i<max; i++)
            b[count]->ptr[i]=NULL;
        index_table.getline(table2,512,';');
        //cout<<table2;
        while(!index_table.eof())
        {
            if(table2[0]!='~')
            {index_table.getline(table2,512,';');cout<<table2<<endl;continue;}
            offset=index_table.tellg()-strlen(table2);
            //cout<<offset<<endl;
            for(i=1,j=0; table2[i]!=','; i++,j++)
                index_insert[j]=table2[i];
            index_insert[j]='\0';
            //cout<< index_insert<<" "<<count<<endl;
            Insert(b,index_insert,(int)offset,count);
//cout<<"hh";
            index_table.getline(table2,512,';');
        }
        index_table.close();
        input.getline(table,512,';');
    }
    input.close();
    //Triple*     p=search(b,"100",0);
    //cout<<p->locate<<" "<<p->r->key[p->locate]<<" "<<p->r->offset[p->locate]<<endl;
    //Btree *q=p->r;
    //cout<<q->key[1]<<endl;
//    cout <<"haha" <<endl;
}
void describe()
{
    fstream mod;
    char table_name[20],tmp[50],newtmp[50];
    int i;
    cout <<"describe:" <<endl;
    cin.getline(table_name,20);
    mod.open("model.txt",ios::in);
    //mod.getline(tmp,512,';');
    while(!mod.eof())
    {
        mod.getline(tmp,512,';');
        for(i=0;; i++)
        {
            if(tmp[i]=='(')
                break;
            else newtmp[i]=tmp[i];
        }
        newtmp[i]='\0';
        if(strcmp(table_name,newtmp)!=0)
            continue;
        else
            for(i=0; i<strlen(tmp); i++)
                cout <<tmp[i];
    }
    mod.close();
}
void display()
{
    fstream dis;
    char table_name[20],tmp[20],ch;
    int i;
    cout <<"display:" <<endl;
    cin.getline(table_name,20);
    strcat(table_name,".txt");
    dis.open(table_name,ios::in);
    ch=dis.get();  //文件指针跳过b树标志位
    while(!dis.eof())
    {
        dis.getline(tmp,512,';');
        if(tmp[0]=='~')
        {
            for(i=1; i<strlen(tmp); i++)
                cout <<tmp[i];
            cout <<endl;
        }
    }
    dis.close();
}
void do_delete(int count,char from[],sel_where s_where[])
{
    int i,j,k,point=0,value;
    bool f=1;
    char tmp[20],table_tmp[20];
    //cout<<s_where[0].num<<s_where[0].location<<" "<<s_where[0].flag<<endl;
    fstream table;
    strcat(from,".txt");
    table.open(from,ios::in|ios::out);  //可读可写
    table.get();
    table.getline(tmp,512,';');  //获取一个元组，元组间由分号隔开
    while(!table.eof())
    {
        //table.getline(tmp,512,';');
        tmp[0]=',';  //delete标志位转为','
        //cout<<tmp<<" hhh"<<endl;
        for(i=0; i<=count; i++)  //每一个元组tmp与每个选择项形成的结构体比较
        {
            point=0;
            for(j=0; j<strlen(tmp); j++)
            {
                if(tmp[j]==',')
                {
                    point++;
                }//cout<<j<<"%"<<point<<endl;}
                if(point==s_where[i].location&&tmp[j]==',')
                {
                    // cout<<tmp[j]<<endl;
                    for(k=0,j++; tmp[j]!=','&&tmp[j]!='\0'; k++,j++)
                        table_tmp[k]=tmp[j];
                    table_tmp[k]='\0';
                    // cout<<table_tmp<<"("<<s_where[i].flag<<endl;
                    if(s_where[i].flag)
                    {
                        value=atoi(table_tmp);
                        //cout<<value<<endl<<s_where[i].ch<<endl;
                        switch (s_where[i].ch)
                        {
                        case '=':
                            if(s_where[i].num!=value)f=0;
                            break;
                        case '>':
                            if(s_where[i].num>=value)f=0;
                            break;
                        case '<':
                            if(s_where[i].num<=value)f=0;
                            break;
                        }
                    }
                    else
                    {
                        if(strcmp(s_where[i].buffer,table_tmp)!=0)
                            f=0;
                    }

                    //if(!f)break;
                }
            }
        }
        //cout<<"f: "<<f<<endl;
        if(f)
        {
            table.seekp(-(int)strlen(tmp)-1,ios::cur);
            table<<'!';
            table.seekp((int)strlen(tmp),ios::cur);
        }  //将符合条件的元组首字符由'~'(正常)变为'!'（删除）
        f=1;
        table.getline(tmp,512,';');
    }
    table.close();
}
void del()
{
    char from[20],where[50];
    cout <<"delete" <<endl;
    cout <<"from:" <<endl;
    cin.getline(from,20);
    cout <<"where:" <<endl;
    cin.getline(where,50);
    sel_where s_where[20];
    int count=analysis(from,where,s_where,1);
    do_delete(count,from,s_where);
    cout <<"Delete complete!" <<endl;
}
void do_update(int count,int s_count,char from[],sel_where set_order[],sel_where s_where[])
{
    int i,j,k,point=0,value,t=0;
    bool f=1;
    char tmp[20],table_tmp[20],set_tmp[20],primary[20];
    //cout<<s_where[0].num<<s_where[0].location<<" "<<s_where[0].flag<<endl;
    fstream table;
    strcat(from,".txt");
    table.open(from,ios::in|ios::out);  //可读可写
    table.seekp(0,ios::end);
//    char a;
//    table >>a;
//    cout <<a <<"***" <<endl;
    int end_pos=table.tellp(),pos;
    table.seekp(0,ios::beg);
    table.getline(tmp,512,';');  //获取一个元组，元组间由分号隔开
    for(int m=0; m<strlen(tmp); m++)
        tmp[m]=tmp[m+1];
    pos=table.tellp();
    while(!table.eof()&&pos<=end_pos)
    {
        //table.getline(tmp,512,';');
        tmp[0]=',';  //delete标志位转为','
        //cout<<tmp<<" hhh"<<endl;
        for(i=0; i<=count; i++)  //每一个元组tmp与每个选择项形成的结构体比较
        {
            point=0;
            for(j=0; j<strlen(tmp); j++)
            {
                if(tmp[j]==',')
                {
                    point++;
                }//cout<<j<<"%"<<point<<endl;}
                if(point==s_where[i].location&&tmp[j]==',')
                {
                    // cout<<tmp[j]<<endl;
                    for(k=0,j++; tmp[j]!=','&&tmp[j]!='\0'; k++,j++)
                        table_tmp[k]=tmp[j];
                    table_tmp[k]='\0';
                    // cout<<table_tmp<<"("<<s_where[i].flag<<endl;
                    if(s_where[i].flag)
                    {
                        value=atoi(table_tmp);
                        //cout<<value<<endl<<s_where[i].ch<<endl;
                        switch (s_where[i].ch)
                        {
                        case '=':
                            if(s_where[i].num!=value)f=0;
                            break;
                        case '>':
                            if(s_where[i].num>=value)f=0;
                            break;
                        case '<':
                            if(s_where[i].num<=value)f=0;
                            break;
                        }
                    }
                    else
                    {
                        if(strcmp(s_where[i].buffer,table_tmp)!=0)
                            f=0;
                    }
                    //if(!f)break;
                }
            }
        }
        //cout<<"f: "<<f<<endl;
        if(f)
        {
            table.seekp(-(int)strlen(tmp)-1,ios::cur);
            table<<'!';
            table.seekp((int)strlen(tmp),ios::cur);
//            cout <<tmp <<"*tmp" <<endl;
            bool yes=0;
            t=0,k=0;
            set_tmp[k++]='~';
            for(i=0; i<strlen(tmp);)
            {
                if(tmp[i]==',')
                {
                    i++;
                    t++;
                    for(j=0; j<=s_count; j++)
                    {
                        if(set_order[j].location==t)
                        {
                            yes=1;
                            if(!set_order[j].flag)
                                for(int r=0; r<strlen(set_order[j].buffer); r++,k++)
                                    set_tmp[k]=set_order[j].buffer[r];
                            else
                            {
                                char val[10];
//                                itoa(set_order[j].num,val,10);//10进制数转字符串
                                sprintf(val,"%d",set_order[j].num);
//                                cout <<set_order[j].num <<"---num" <<endl;
//                                cout <<val <<"---val" <<endl;
                                for(int r=0; r<strlen(val); r++,k++)
                                    set_tmp[k]=val[r];
                            }
                            while(tmp[i]!=','&&tmp[i]!='\0') i++;
                            break;
                        }
                    }
                    if(!yes)
                    {
                        for(; tmp[i]!=','&&tmp[i]!='\0'; k++,i++)
                            set_tmp[k]=tmp[i];
                    }
                    yes=0;
                    set_tmp[k++]=',';
                }
                //cout <<endl <<"*"<<i <<endl;
            }
//        set_tmp[--k]='\0';
            set_tmp[k-1]=';';
            set_tmp[k]='\0';
//            cout <<set_tmp <<"  ***" <<endl;
            for(i=1,j=0; set_tmp[i]!=','; i++,j++)
                primary[j]=set_tmp[i];
            primary[j]='\0';
//            cout <<primary <<"***" <<endl;
            //table <<set_tmp;
            pos=table.tellp();//**************cur_pos
//        cout <<pos <<"***" <<endl;
//        char a;
//        table >>a;
//        cout <<a <<endl;
            table.seekp(0,ios::beg);
//        cout <<table.tellp() <<"**" <<endl;


            char ch=table.get();
            int num=int(ch)-48;
//            cout <<ch <<"--ch" <<"  " <<num <<"--num" <<endl;
            if(pkey_check(primary,num,from))
            {
//                cout <<"I'm here!" <<endl;
//                in.open(name,ios::out|ios::app);
                table.seekp(0,ios::end);
                table <<set_tmp;
//                cout <<set_tmp <<"@#$%^&*" <<endl;
                int xx=table.tellp()-strlen(set_tmp);
                Insert(b,primary,xx,num);
            }
            else {cout<<"error"<<endl;}
            table.seekp(pos);
        }
        f=1;
        table.getline(tmp,512,';');
        pos=table.tellp();
    }
    table.close();
}
void update()
{
    char from[20],set[50],where[50];
    cout <<"update:" <<endl;
    cin.getline(from,20);
    cout <<"set:" <<endl;
    cin.getline(set,50);
    cout <<"where:" <<endl;
    cin.getline(where,50);
    sel_where s_where[20],set_order[20];
    int count=analysis(from,where,s_where,1);
    int s_count=analysis(from,set,set_order,1);
    do_update(count,s_count,from,set_order,s_where);
    cout <<"Update complete!" <<endl;
}
int main()
{
    //cout<<"j";
    int order;
    index();
    while(1)
    {
        cout <<endl <<endl;
        cout <<"*********************************************************"<<endl;
        cout <<"************************ LLL DBMS ***********************"<<endl;
        cout <<"*********************************************************"<<endl;
        cout <<"\t 1、create\t 2、insert\t 3、delete\t"<<endl;
        cout <<"\t 4、update\t 5、select\t 6、describe\t"<<endl;
        cout <<"\t 7、display\t 0、exit\t"<<endl;
        cout <<"*********************************************************"<<endl;
        cout <<"Please enter your choice：";
        cin >>order;
        getchar();  //吸收回车
        switch (order)
        {
        case 1:
            creat();
            break;
        case 2:
            insert();
            break;
        case 3:
            del();
            break;
        case 4:
            update();
            break;
        case 5:
            select();
            break;
        case 6:
            describe();
            break;
        case 7:
            display();
            break;
        case 0:
            cout<<"Goodbye!";
            return 0;
            break;
        }
    }
}
