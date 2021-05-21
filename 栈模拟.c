//栈模拟
//老师更喜欢用链表的同学（应该和可变长度有关吧）
//记得没有元素时的Pop和GetElement要考虑到。



#include<stdio.h>//实在是不会用函数把pop ，push等动作表示，结构体中还有指针很难操作。
#include <stdlib.h>
#define ADD 10//如果栈已有空间不够，每次加10个int
typedef struct stack{
    int* top;//栈顶指针
    int* bot;//栈底指针
    int size;//目前栈的大小
}stack;
int main(){
    stack s;
    int n=0,num;
    s.bot=(int*)malloc(10*sizeof(int));//栈初始长度为10个int
    if(s.bot==NULL)//分配失败
    {
        printf("分配失败");
        return 0;
    }
    s.top=s.bot;//统一栈顶栈底
    int size=0;//栈中元素数量
    while(1)
    {
        printf("请输入要进行的操作\n1.压栈\n2.出栈\n3.返回栈顶元素\n4.退出\n");
        scanf("%d",&n);
        if(n==1)//push
        {
            printf("请输入数字\n");
            scanf("%d",&num);//新加入的数字
            if(size==s.size)//如果栈满，重新分配更大的空间
            {
                s.bot=(int*)realloc(s.bot , (s.size+=ADD)*sizeof(int));//重新多分配10个int
                if(s.bot==NULL)
                {
                    printf("错误：上溢");
                    
                    return 0;
                }
                s.top=s.bot+size;//将栈顶重新分配
            }
            *s.top=num;
            s.top++;
        }
        else if (n==2)//pop
        {
            if(size>0)
            {
                s.top--;
                printf("出栈成功\n");//提示出栈成功了
            }
            else
            printf("栈中无元素，出栈失败\n");
        }
        else if(n==3)//return top element
        {
            if(size==0)
            printf("栈中无元素,无法返回栈顶元素\n");
            else
            printf("栈顶元素是：%d\n\n",*(s.top-1));//top指向下一个元素，-1
        }
           
        else//输入数字不是操作数
        {
            printf("ERROR");
            break;
        } 
        size=s.top-s.bot;
        if(size>0)
            printf("栈中元素为\n");
        else
            printf("栈中无元素");
        for(int i=size-1;i>=0;i--)
        {
            printf("%d\n",*(s.bot+i));
            if(i==0)
            printf("\n");
        }
    }
    free(s.bot);//释放空间（在当前文件没啥必要，但是养成习惯）
    return 0;
}
