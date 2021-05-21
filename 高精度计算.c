//高精度计算（大数）
//只实现了正数的加减操作。


#include <stdio.h>
#include<string.h>
int scan(char s[])
{//得到字符串及对应长度
    gets(s);
    return strlen(s);
}
void print(char s[],int n[],int l)
{
    int i;
    for(i=0;i<l;i++)
    {
        n[i]=s[l-i-1]-'0';//将数组倒叙放入，方便利用循环计算，不必思考空几个位；
    }
}
void exchange(char s1[],char s2[],int *l1,int* l2)
{   //交换字符串，以及相对应得字符串长度
    char buf[100];
    int t;
    strcpy(buf,s1);
    strcpy(s1,s2);
    strcpy(s2,buf);
    t=*l1;
    *l1=*l2;
    *l2=t;
    return;
}
void write(int n1[],int n2[],int* l1,int*l2,char* sig,int* p,int * error)
{
    char s1[100],s2[100];
    int i;
    printf("请输入第一个正整数\n");
    *l1=scan(s1);
    printf("请输入运算符号    \n");
    *sig=getchar();
    getchar();
    printf("请输入第二个正整数\n");
    *l2=scan(s2);
    for(i=0;i<*l1;i++)//start
        {
            if(s1[i]>'9'||s1[i]<'0')
            {
                printf("\nerror");
                *error=1;
                return;
                
            }
        }
        for(i=0;i<*l2;i++)
        {
            if(s2[i]>'9'||s2[i]<'0')
            {
                printf("\nerror");
                *error=1;
                return;
            }
        }
        if(*sig!='-'&&*sig!='+')
        {
            printf("\nerror");
            *error=1;
            return;
        }
    //end 验证输入的数和运算符号格式是否正确
    if(*l1<*l2)
    {
        exchange(s1,s2,l1,l2);
        *p=1;
    }
    
    else if(*l1==*l2)
    {
        for(i=0;i<*l1;i++)
        {
            if(s1[i]>s2[i])//若s1中首先出现大数，则s1大于s2；
                break;
            if(s1[i]<s2[i])
            {
                exchange(s1,s2,l1,l2);
                *p=1;//标志已经发生转换，对减法产生影响
                break;
            }
            
        }

    }
    print(s1,n1,*l1);
    print(s2,n2,*l2);
    return;
}
void plus(int n1[],int n2[],int ans[],int l2,int l1)
{
    int k=0,i,buf;//k为进位标志，buf暂存计算结果；
    for(i=0;i<l2;i++)//加法从个位开始，加到小数得最高位；
    {   buf=0;
        if(k==0)
            buf=n1[i]+n2[i];
        else
            buf=n1[i]+n2[i]+1;
        k=0;
        if(buf>=10)//此位超过10，进一位；
        {
            buf-=10;
            k=1;//标志进位
        }
        ans[i]+=buf;
    }
    for(;i<l1;i++)
    {
        ans[i]=n1[i];//大数未计算的位数输入结果
    }
}
void minus(int n1[],int n2[],int ans[],int l2,int l1)
{
    int i,k,buf;//k为借位标志，buf暂存计算结果
    for(i=l2-1;i>=0;i--)//从小数最高位开始减，体现出倒序的重要性
    {
        k=0;
        if(n1[i]<n2[i])//被减数当前位小于减数，向前一位借1；
        {
            k=1;//向前一位借1
            int p=i;
            while(ans[i+1]==0)
            i++;
            ans[i+1]--;//直接在结果的前一位借，后面必须使用+=而不能直接用buf赋值；
            for(;i>p;i--)
                ans[i]=9;
        }
        if(k==1)//借位
            buf=n1[i]+10-n2[i];
        else//不借位
            buf=n1[i]-n2[i];
        ans[i]+=buf;
        
    }
    for(i=l2;i<l1;i++)
    ans[i]=n1[i];//将大数未计算的位数输入结果

}
int main (){
    int n1[100]={0},n2[100]={0},ans[100]={0};//n1,n2为两个数，ans 为结果；
    int l1,l2,p=0,i,q=1,error=0;//l1，l2为两个字符串长度，p为交换标识符，q为输出标识符，error检测数字是否输入正确；
    char sig;//运算符号；
    write(n1,n2,&l1,&l2,&sig,&p,&error);//读入两个数，大的数为n1，小的数为n2；
    if(error==1)
        return 0;
    if(sig=='+')
        plus(n1,n2,ans,l2,l1);//加法
    if(sig=='-')
        minus(n1,n2,ans,l2,l1);//减法
    printf("结果为：\n");
    if(sig=='-'&&p==1)
        {printf("-");}//若减数比被减数大，输出负数
        
    for(i=99;i>=0;i--)//倒叙输出
    {
        if(ans[i]!=0||q==0)//寻找第一个不为0数开始输出，后面出现的哦也输出
        {
            printf("%d",ans[i]);
            q=0;//保证后出现的0也输出；
        }
    }
    return 0;
}
