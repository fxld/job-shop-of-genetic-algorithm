#include <iostream>
#include <string>
#include <ctime>
#include <cmath>
#include <algorithm>
#include <fstream>
#include <stdio.h>
#include <string.h>
using namespace std;

#define chrome 305 //染色体长度
#define gennumber 100 //种群容量
#define Iteration 500 //迭代次数
#define chiocechance 0.5 //选择几率
#define crosschance 0.65//交叉互换的概率
#define exchangefactor 0.24//染色体的交换因子的概率
#define mutantchance 0.25//变异的概率


class data
{
public:
    int line; //机械号
    int time; //加工时间
};


class Chrome
{
public:
    int gen[chrome]={0};
    int makespan=0;
    double fixness=0;
    Chrome operator = (const Chrome& a)
    {
        this->makespan=a.makespan;
        this->fixness=a.fixness;
        for (int i=0; i<chrome; i++)
        {
            this->gen[i]=a.gen[i];
        }
        return *this;
    }
};


int machine;
int element;
int job=0;
int Produce[100]={0};//储存产品工序
int maxproduce=0;
data **record;
Chrome island[gennumber];//储存种群的岛

bool cmp(const Chrome&a,const Chrome&b) //使用sort为Chrome中的数组排序
{
    return a.makespan<b.makespan;
}
bool cmp2(int i,int j )
{
    return (i>j);
}


void input() //输入
{
    int elementid;//产品id
    int produce;  //工序
    int i;
    cin>>machine>>element;
    record=new data* [element+1];
    for (elementid=1; elementid<=element; elementid++)
    {
        cin>>produce;
        job=job+produce;
        record[elementid]=new data [produce+1];
        Produce[elementid]=produce;
        if (produce>maxproduce)
        {
            maxproduce=produce;
        }
        for (i=1; i<=produce; i++)
        {
            scanf("%d%d",&record[elementid][i].line,&record[elementid][i].time);
        }
        record[elementid][i].line=-1;
        record[elementid][i].time=-1;
    }
}


void Code() // 以产品编号编码
{
    Chrome first;
    int temp;
    int key;
    int k=0;
    srand(time(NULL));
    for (int i=1; i<=element; i++) //先生成第一条标准染色体，再由交换位置变化出种群
    {
        for (int j=1; j<=machine&&record[i][j].line!=-1; j++)
        {
            first.gen[k]=i;
            k++;
        }
    }
    for (int i=0; i<gennumber; i++) // 生成种群
    {
        for (int j=job-1; j>=0; j--)
        {
            temp=first.gen[j];
            if(j>0)
            {
                key=rand()%j;
                first.gen[j]=first.gen[key];
                first.gen[key]=temp;
            }
        }
        island[i]=first;
        first=island[i];
    }
}


void decode2(Chrome& x)//解码
{
    int num[100]={0}; //让工件顺利度过第一个工序而加工第一个工序
    int num2[100]={0};//让工件再加工完第一个工序之后往后推个工序
    int count[100]={0,0};//判断整个工序是否全部完成完成则变为1不用再做这个工件了
    int retime[100]={0,0};
    num[x.gen[0]]++;
    num2[x.gen[0]]++;
    x.makespan=0;
    retime[0]=record[x.gen[0]][num[x.gen[0]]].time;
    for (int i=1; i<job; i++) //使用贪心算法来解码（获取到染色体最大的运行时间）在求出最大时间的同时判断之前的工序有没有加工过
    {
        num[x.gen[i]]++;
        for (int j=1; j<=element; j++)
        {
            if (j==x.gen[i]&&num[i]<num2[j])
            {
                break;
            }
            if (num[j]!=0&&j!=(x.gen[i])&&count[j]==0)
            {
                if (record[j][num2[j]+1].time!=-1)
                {
                    num2[j]++;
                    if (record[j][num2[j]].time>retime[i])
                    {
                        retime[i]=record[j][num2[j]].time;
                    }
                }
                else
                {
                    if (record[j][num2[j]].time>retime[i])
                    {
                        retime[i]=record[j][num2[j]].time;
                        count[j]++;
                    }
                }
            }
            if (num[j]!=0&&j==(x.gen[i])&&count[j]==0&&num[x.gen[i]]>=num2[x.gen[i]])
            {
                num2[x.gen[i]]++;
                if (record[j][num2[j]].time>retime[i])
                {
                    retime[i]=record[j][num2[j]].time;
                }
            }
        }
    }
    for (int i=0; i<job; i++)
    {
        x.makespan=retime[i]+x.makespan;
    }
}


void decode(Chrome & x)
{
    int MachineGen[100];
    int Record[100];
    int Record2[100];
    int Record3[100];
    int Record4[100];
    int Record5[100];
    int ElementGen[100];
    int retime[100];
    int m=0;
    int c=0;
    int count1=0;
    int maxtime=0;
    for (int i=0; i<=element; i++)
    {
        Record5[i]=0;
    }
    for (int i=0; i<=machine; i++)
    {
        Record3[i]=0;
    }
    for (int i=0; i<job; i++)
    {
        ElementGen[i]=x.gen[i];
    }
    for (int i=0; i<job; i++)
    {
        Record3[x.gen[i]]++;
        MachineGen[i]=record[x.gen[i]][Record3[x.gen[i]]].line;
    }
    for (int i=0; i<=machine; i++)
    {
        Record[i]=0;
        Record3[i]=0;
    }
    for(int i=0; i<=element;i++)
    {
        Record2[i]=0;
        Record4[i]=0;
    }
    while (count1<job)
    {
        int j;
        for (j=0; j<job; j++)
        {
            if (MachineGen[j]!=0&&ElementGen[j]!=0)
            {
                Record[MachineGen[j]]++;
                Record2[ElementGen[j]]++;
                Record3[0]=MachineGen[j];
                Record4[0]=ElementGen[j];
                retime[0]=record[ElementGen[j]][Record2[ElementGen[j]]].time;
                MachineGen[j]=0;
                ElementGen[j]=0;
                count1++;
                break;
            }
        }
        for (int k=1+j; k<job; k++)
        {
            for (int n=0; n<maxproduce; n++)
            {
                if (ElementGen[k]==Record4[n]||MachineGen[k]==0||ElementGen[k]==0||Record5[ElementGen[k]]==1)
                {
                    m=1;
                    break;
                }
                if(MachineGen[k]==Record3[n])
                {
                    m=1;
                    Record5[ElementGen[k]]++;
                    break;
                }
            }
            if (m==1)
            {
                m=0;
            }
            else
            {
                Record3[c++]=MachineGen[k];
                Record4[c]=ElementGen[k];
                Record[MachineGen[k]]++;
                Record2[ElementGen[k]]++;
                retime[c]=record[ElementGen[k]][Record2[ElementGen[k]]].time;
                MachineGen[k]=0;
                ElementGen[k]=0;
                count1++;
            }
        }
        for (int a=0; a<=machine; a++)
        {
            Record3[a]=0;
        }
        for(int b=0; b<=element;b++)
        {
            Record4[b]=0;
        }
        for (int aa=0; aa<=element; aa++)
        {
            Record5[aa]=0;
        }
        c=0;
        for (int ii=1; ii<job; ii++)
        {
            if (retime[ii]>retime[0])
            {
                retime[0]=retime[ii];
            }
        }
        maxtime=maxtime+retime[0];
        for (int bb=0; bb<=job; bb++)
        {
            retime[bb]=0;
        }
    }
    x.makespan=maxtime;
}


void chioce () //赌盘选择
{
    sort(island, island+gennumber, cmp);
    int total=0;
    float *chance;
    float x;
    srand(time(NULL));
    chance=new float [gennumber];
    for (int i=0; i<gennumber; i++)
    {
        total=island[i].makespan+total;
    }
    for (int i=0; i<gennumber; i++)
    {
        if (i==0)
        {
            chance[i]=island[i].makespan/total;
        }
        else
        {
            chance[i]=island[i].makespan/total+chance[i-1];
        }
    }
    for (int i=0; i<gennumber*chiocechance; i++)
    {
        x=(float)(rand()%10000)/10000;
        for (int j=0; j<gennumber; j++)
        {
            if(x<chance[j])
            {
                island[gennumber-i]=island[j];
                break;
            }
        }
        
    }
    delete []chance;
    chance=NULL;
}

void chioce2()//选择适应力强的基因
 {
 sort(island, island+gennumber, cmp);
 for (int i=0; i<=gennumber*chiocechance; i++)
 {
 island[gennumber-i]=island[i];
 }
 sort(island, island+gennumber, cmp);
 }


void crossover() //交叉互换
{
    int gen2[job];
    int gen3[job];
    int x;
    int y;
    int m=0;
    int n=0;
    srand(time(NULL));
    for (int i=0; i<=gennumber*crosschance; i++)
    {
        y=min(Produce[i], Produce[gennumber-i-1]);
        for (int e=1; e<=y*exchangefactor; e++)
        {
            x=rand()%(y-1)+1;
            for (int k=0; k<job; k++)
            {
                if (island[i].gen[k]==x)
                {
                    island[i].gen[k]=0;
                    gen2[m]=x;
                    m++;
                }
                if (island[gennumber-1-i].gen[k]==x)
                {
                    island[gennumber-1-i].gen[k]=0;
                    gen3[n]=x;
                    n++;
                }
            }
        }
        n=0;
        m=0;
        for (int k=0; k<job; k++)
        {
            if (island[i].gen[k]==0)
            {
                island[i].gen[k]=gen3[n];
                n++;
            }
            if (island[gennumber-i-1].gen[k]==0)
            {
                island[gennumber-i-1].gen[k]=gen2[m];
                m++;
            }
        }
    }
}


void mutant() //变异
{
    int x;
    int temp;
    srand(time(NULL));
    for (int i=0; i<gennumber*mutantchance; i++)
    {
        for (int j=0; j<job*mutantchance; j++)
        {
            x=rand()%(job-1)+1;
            temp=island[i].gen[x];
            island[i].gen[x]=island[i].gen[x-1];
            island[i].gen[x-1]=temp;
        }
    }
}


int main(int argc, const char * argv[])
{
    ofstream out("data.text");
    if (!out)
    {
        cout<<"打开文件错误"<<endl;
    }
    int i=0;
    input();
    Code();
    while (i<Iteration)
    {
        for (int j=0; j<gennumber; j++)
        {
            decode(island[j]);
        }
        chioce();
        chioce2();
        crossover();
        mutant();
        i++;
        sort(island, island+gennumber, cmp);
        out<<island[0].makespan<<endl;
    }
    i=0;
    sort(island, island+gennumber, cmp);
    out<<island[0].makespan<<endl;
    for (int j=0; j<chrome; j++)
    {
        out<<island[0].gen[j];
    }
    return 0;
}
