#include <stdio.h>
#include <string.h>
#include <stdlib.h>

//书
typedef struct book
{
    int number;      //编号
    char name[100];  //书名
    int totalnumber; //可借数量
} book;

//读者
typedef struct user
{
    int number;                 //编号
    char name[100];             //用户名
    char bookborrowed[10][100]; //在借图书<=10
    int bookborrowedsum;        //在借数量
} user;

//管理员
typedef struct admin
{
    int number;     //编号
    char name[100]; //用户名
} admin;

//馆藏图书数组，读者数组，管理员数组
book bookList[200000];
user userList[10000];
admin adminList[100];
int choice;         //用户选择选项
int adminsum = 0;   //管理员总数
int usersum = 0;    //读者总数
int booksum = 0;    //馆藏图书总数
FILE *userlist;     //读者文件指针
FILE *booklist;     //馆藏书目文件指针
FILE *adminlist;    //管理员文件指针
FILE *borrowedlist; //出借图书文件指针


void usermenu() //一行一行打方便对齐
{
    printf("————————————————————————————————————————————————————————————\n");
    printf("                        欢迎你读者！                         \n");
    printf("               0.退出                                       \n");
    printf("               1.借书                                       \n");
    printf("               2.还书                                       \n");
    printf("               3.获取图书信息                                \n");
    printf("               4.显示自己已借图书                                \n");
    printf("————————————————————————————————————————————————————————————\n");
}
void adminmenu()
{
    printf("————————————————————————————————————————————————————————————\n");
    printf("                          欢迎你管理员！                     \n");
    printf("                0.退出                                      \n");
    printf("                1.新书                                      \n");
    printf("                2.修改图书                                  \n");
    printf("                3.删除图书                                  \n");
    printf("                4.新读者                                    \n");
    printf("                5.修改读者                                  \n");
    printf("                6.删除读者                                  \n");
    printf("                7.获取馆藏图书信息                          \n");
    printf("                8.图书概览                                  \n");
    printf("                9.读者概览                                  \n");
    printf("————————————————————————————————————————————————————————————\n");
}

//保存管理员列表
void saveAdminlist()
{
    adminlist = fopen("adminlist.txt", "w");
    for (int i = 0; i < adminsum; i++)
    {
        fprintf(adminlist, "%d %s\n", adminList[i].number, adminList[i].name);
    }
    fclose(adminlist);
}
//保存读者列表
void saveUserlist()
{
    userlist = fopen("userlist.txt", "w");
    for (int i = 0; i < usersum; i++)
    {
        fprintf(userlist, "%d %s\n", userList[i].number, userList[i].name);
    }
    fclose(userlist);
}
//保存图书列表
void saveBooklist()
{
    booklist = fopen("booklist.txt", "w");
    for (int i = 0; i < booksum; i++)
    {
        fprintf(booklist, "%d %s %d\n", bookList[i].number, bookList[i].name, bookList[i].totalnumber);
    }
    fclose(booklist);
}
//保存借出列表
void saveBorrowedlist()
{
    borrowedlist = fopen("borrowedlist.txt", "w");
    for (int i = 0; i < usersum; i++)
    {
        for (int j = 0; j < userList[i].bookborrowedsum; j++)
        {
            fprintf(borrowedlist, "%s %s\n", userList[i].bookborrowed[j], userList[i].name);
        }
    }
    fclose(borrowedlist);
}
//保存主函数
void saveMain()
{
    saveAdminlist();
    saveBooklist();
    saveUserlist();
    saveUserlist();
}

//缓存管理员列表
int loadAdminlist()
{
    adminlist = fopen("adminlist.txt", "r");
    if (adminlist == NULL)
    {
        return 0;
    }
    else
    {
        while (fscanf(adminlist, "%d%s\n", &adminList[adminsum].number, adminList[adminsum].name) != EOF)
        {
            adminsum++;
        }
        fclose(adminlist);
        return 1;
    }
}
//缓存读者列表
int loadUserlist()
{
    userlist = fopen("userlist.txt", "r");
    if (userlist == NULL)
    {
        return 0;
    }
    else
    {
        while (fscanf(userlist, "%d%s\n", &userList[usersum].number, userList[usersum].name) != EOF)
        {
            usersum++;
        }
        fclose(userlist);
        return 1;
    }
}
//缓存图书列表
int loadBooklist()
{
    booklist = fopen("booklist.txt", "r");
    if (booklist == NULL)
    {
        return 0;
    }
    else
    {
        while (fscanf(booklist, "%d%s%d\n", &bookList[booksum].number, bookList[booksum].name, &bookList[booksum].totalnumber) != EOF)
        {
            booksum++;
        }
        fclose(booklist);
        return 1;
    }
}
//缓存借出图书列表
int loadBorrowed()
{ //读者借书信息由borrowedlist导出，先初始化
    for (int i = 0; i < usersum; i++)
    {
        userList[i].bookborrowedsum = 0;
        for (int j = 0; j < 10; j++)
        {
            strcpy(userList[i].bookborrowed[j], "\0");
        }
    }
    borrowedlist = fopen("borrowedlist.txt", "r");
    if (borrowedlist == NULL)
        return 0;
    else
    {
        char bookname[100];
        char username[100];
        while (fscanf(borrowedlist, "%s%s\n", bookname, username) != EOF)
        {
            for (int i = 0; i < usersum; i++)
            {
                if (strcmp(userList[i].name, username) == 0)
                {
                    strcpy(userList[i].bookborrowed[userList[i].bookborrowedsum], bookname);
                    userList[i].bookborrowedsum++;
                }
            }
        }
        fclose(borrowedlist);
        return 1;
    }
}

//打印馆藏书目
void printBooklist()
{
    for (int i = 0; i < booksum; i++)
    {
        printf("编号:%d 书名:%s 剩余数量:%d\n\n", bookList[i].number, bookList[i].name, bookList[i].totalnumber);
    }
    printf("请按回车键继续");
    getchar();
    getchar();
    system("cls"); //新学的清屏，不然太乱了。
}
//显示读者信息
void userInfo()
{
    for (int i = 0; i < usersum; i++)
    {
        printf("读者编号:%d 读者姓名:%s 在借数量:%d\n", userList[i].number, userList[i].name, userList[i].bookborrowedsum);
        if (userList[i].bookborrowedsum == 0)
        {
            printf("该读者没有在借图书\n");
        }
        else
        {
            printf("该读者在借图书为:\n");
            for (int j = 0; j < userList[i].bookborrowedsum; j++)
            {
                printf("%s\n", userList[i].bookborrowed[j]);
            }
        }
        printf("\n");
    }
    printf("请按回车继续");
    getchar();
    getchar();
    system("cls");
    adminmenu();
}
//已借数目
void userBook(char *user)
{
    for (int i = 0; i < usersum; i++)
    {
        if (strcmp(userList[i].name, user) == 0)
        {
            if (userList[i].bookborrowedsum == 0)
            {
                printf("你没有在借图书哦\n\n");
            }
            else
            {
                printf("你的在借图书如下:\n");
                for (int j = 0; j < userList[i].bookborrowedsum; j++)
                {
                    printf("%s\n", userList[i].bookborrowed[j]);
                }
            }
        }
    }
    printf("请按回车继续");
    getchar();
    getchar();
    saveMain();
}
//借书
void borrowBook(char *user)
{
    char bufname[100];
    printf("请输入书名\n");
    scanf("%s", bufname);
    for (int i = 0; i < usersum; i++)
    {
        if (strcmp(userList[i].name, user) == 0)
        {
            if (userList[i].bookborrowedsum == 10) //借阅数量限制
            {
                printf("借阅数量达到上限!\n");
                return;
            }
            for (int j = 0; j < userList[i].bookborrowedsum; j++) //是否重复借阅
            {
                if (strcmp(userList[i].bookborrowed[j], bufname) == 0)
                {
                    printf("你已经持有此书！请不要重复借阅\n");
                    return;
                }
            }
        }
    }
    int sign = 0;
    for (int i = 0; i < booksum; i++)
    {
        if (strcmp(bookList[i].name, bufname) == 0)
        {
            sign = 1;
            if (bookList[i].totalnumber == 0)
            {
                printf("抱歉，馆内没有此书剩余\n");
            }
            else
            {
                bookList[i].totalnumber--;
                for (int j = 0; j < usersum; j++)
                {
                    if (strcmp(userList[j].name, user) == 0)
                    {
                        strcpy(userList[j].bookborrowed[userList[j].bookborrowedsum], bufname);
                        userList[j].bookborrowedsum++;
                    }
                }
                printf("借阅成功！\n");
            }
        }
    }
    if (sign == 0)
    {
        printf("馆内还未收藏此书\n");
    }
    userBook(user);
}
//还书
void returnBook(char *user)
{
    char bufname[30];
    printf("请输入你想归还图书的书名\n");
    scanf("%s", bufname);
    int sign = 0;
    for (int i = 0; i < booksum; i++)
    {
        if (strcmp(bookList[i].name, bufname) == 0)
        {
            sign = 1;
            bookList[i].totalnumber++;
            break;
        }
    }
    if (sign == 0)
    {
        printf("请正确输入书名!\n");
        return;
    }
    for (int i = 0; i < usersum; i++)
    {
        if (strcmp(userList[i].name, user) == 0)
        {
            for (int j = 0; j < userList[i].bookborrowedsum; j++)
            {
                if (strcmp(userList[i].bookborrowed[j], bufname) == 0)
                {
                    for (int k = j; k < userList[i].bookborrowedsum; k++)
                    {
                        strcpy(userList[i].bookborrowed[k], userList[i].bookborrowed[k + 1]);
                    }
                    userList[i].bookborrowedsum--;
                }
            }
        }
    }
    printf("归还成功！\n");
    userBook(user);
}
//获取图书信息
void bookInfo()
{
    int n, sign = 0;
    while (1)
    {
        printf("请输入查找方式\n1.书名\n2.编号\n3.退出\n");
        scanf("%d", &n);
        if (n == 1)
        {
            char bufname[100];
            printf("请输入书名\n");
            scanf("%s", bufname);
            for (int i = 0; i < booksum; i++)
            {
                if (strcmp(bookList[i].name, bufname) == 0)
                {
                    sign = 1;
                    printf("剩余馆藏数量:%d\n", bookList[i].totalnumber);
                }
            }
            if (sign == 0)
            {
                printf("查无此书\n");
                return;
            }
            for (int i = 0; i < usersum; i++)
            {
                for (int j = 0; j < userList[i].bookborrowedsum; j++)
                {
                    if (strcmp(userList[i].bookborrowed[j], bufname) == 0)
                    {
                        printf("%s 持有此书\n", userList[i].name);
                    }
                }
            }
        }
        else if (n == 2)
        {
            int bookinquiry2;
            printf("请输入编号\n");
            scanf("%d", &bookinquiry2);

            char bookinquiry2name[30];
            for (int i = 0; i < booksum; i++)
            {
                if (bookList[i].number == bookinquiry2)
                {
                    sign = 1;
                    strcpy(bookinquiry2name, bookList[i].name);
                    printf("此书书名为: %s\n",bookList[i].name);
                    printf("剩余馆藏数量:%d.\n", bookList[i].totalnumber);
                }
            }
            if (sign == 0)
            {
                printf("抱歉，本图书馆暂未收藏此书\n");
                return;
            }
            for (int i = 0; i < usersum; i++)
            {
                for (int j = 0; j < userList[i].bookborrowedsum; j++)
                {
                    if (strcmp(userList[i].bookborrowed[j], bookinquiry2name) == 0)
                    {
                        printf("%s 借阅了本书.\n", userList[i].name);
                    }
                }
            }
        }
        else if (n == 3)
        {
            break;
        }
        else
        {
            printf("请正确输入命令\n");
        }
    }
}

//新书
void newBook()
{
    int booknumber;
    char bookname[100];
    int booktotalnumber;

    printf("请输入书名\n");
    getchar();
    gets(bookname);
    printf("请输入编号\n");
    scanf("%d", &booknumber);

    for (int i = 0; i < booksum; i++)
    {
        if (strcmp(bookList[i].name, bookname) == 0 || bookList[i].number == booknumber)
        {
            printf("这本书已经在书库里了，请使用修改图书信息功能！\n");
            return;
        }
    }

    printf("请输入数量\n");
    scanf("%d", &booktotalnumber);

    //新书放在最后一本,如果是链表按照字母顺序排检索更快
    bookList[booksum].number = booknumber;
    strcpy(bookList[booksum].name, bookname);
    bookList[booksum].totalnumber = booktotalnumber;
    booksum++;
    printf("添加新书成功！\n");
    saveMain();
}
//修改图书信息
void modiBook()
{
    printf("请输入图书编号\n");
    int bufnumber;
    scanf("%d", &bufnumber);
    int sign = 0, n = 0;
    for (int i = 0; i < booksum; i++)
    {
        if (bookList[i].number == bufnumber)
        {
            sign = 1;
            printf("本书当前信息为：\n 编号:%d 书名:%s 可借数量:%d\n确认修改此书吗？‘1’确认，‘0’返回\n", bookList[i].number, bookList[i].name, bookList[i].totalnumber);
            scanf("%d", &n);
            if (n != 1)
            {
                printf("已返回\n");
                return;
            }
            printf("请输入更改后的数量\n");
            scanf("%d", &bookList[i].totalnumber);
            printf("更改成功!结果为：\n 编号:%d 书名:%s 可借数量:%d\n\n\n", bookList[i].number, bookList[i].name, bookList[i].totalnumber);
        }
    }
    if (sign == 0)
    {
        printf("查无此书，请检查图书编号！\n");
    }
    saveMain();
}
//删除图书
void delBook()
{
    printf("如何删除图书?\n1.编号\n2.书名\n");
    int bufnumber, sign = 0, n = 0, p = 0;
    char bufname[100];
    scanf("%d", &p);
    if (p == 1)
    {
        printf("请输入编号\n");
        scanf("%d", &bufnumber);
        for (int i = 0; i < booksum; i++)
        {
            if (bookList[i].number == bufnumber)
            {
                printf("该书书名为:%s.\n确定是这本书吗？\n输入‘1’继续，‘0’返回\n", bookList[i].name);
                scanf("%d", &n);
                if (n != 1)
                {
                    printf("已返回\n");
                    return;
                }
                sign = 1;
                //用链表更方便，这种方法效率很低
                for (; i < booksum; i++)
                {
                    bookList[i] = bookList[i + 1];
                }
                booksum--;
                printf("删除成功！\n");
            }
        }
    }
    else if(p==2)
    {
        printf("请输入书名\n");
        scanf("%s", bufname);
        for (int i = 0; i < booksum; i++)
        {
            if (strcpy(bookList[i].name, bufname)==0)
            {
                printf("该书书名为:%s.\n确定是这本书吗？\n输入‘1’继续，‘0’返回\n", bookList[i].name);
                scanf("%d", &n);
                if (n != 1)
                {
                    printf("已返回\n");
                    return;
                }
                sign = 1;
                //用链表更方便，这种方法效率很低
                for (; i < booksum; i++)
                {
                    bookList[i] = bookList[i + 1];
                    bookList[i].number--;
                }
                booksum--;
                printf("删除成功！\n");
            }
    }
    }
    if (sign == 0)
    {
        printf("查无此书，请检查图书信息！\n");
    }
    saveMain();
}
//新读者
void newUser(){
    int usernumber;
    char username[100];
    printf("请输入读者用户名\n");
    getchar();
    gets(username);
    printf("请输入读者编号\n");
    scanf("%d", &usernumber);
    //查找读者是否已经存在
    for (int i = 0; i < usersum; i++)
    {
        if (userList[i].number == usernumber || strcmp(userList[i].name, username) == 0)
        {
            printf("该读者已经注册！\n");
            return;
        }
    }
    userList[usersum].number = usernumber;
    strcpy(userList[usersum].name, username);
    userList[usersum].bookborrowedsum = 0;
    usersum++;
    printf("注册新读者%s成功！\n\n", username);
    saveMain();
}
//修改读者信息,不知道有啥可改的……
void modiUser()
{
    printf("请输入需要修改的读者编号\n");
    int bufnumber, n = 0, sign = 0;
    scanf("%d", &bufnumber);
    for (int i = 0; i < usersum; i++)
    {
        if (userList[i].number == bufnumber)
        {
            sign = 1;
            printf("该读者用户名为:%s，确认更改他的信息吗？\n‘1’确认，‘0’返回\n", userList[i].name);
            scanf("%d", &n);
            if (n != 0)
            {
                printf("已返回\n");
                return;
            }
            printf("更改成功！\n");
        }
    }
    if (sign == 0)
        printf("该读者未注册!\n");
    saveMain();
    return;
}
//删除读者信息
void delUser()
{
    printf("如何删除该读者?\n1.编号\n2.用户名\n");
    int bufnumber, n = 0, p = 0, sign = 0;
    char bufname[100];
    scanf("%d", &n);
    if (n == 1)
    {
        scanf("%d", &bufnumber);
        for (int i = 0; i < usersum; i++)
        {
            if (userList[i].number == bufnumber)
            {
                sign = 1;
                if (userList[i].bookborrowedsum > 0)
                {
                    printf("请督促该读者还书！！\n");
                    break;
                }
                printf("读者用户名为:%s\n，确认删除他吗?\n‘1’确认，‘0’返回\n", userList[i].name);
                scanf("%d", &p);
                if (p != 1)
                {
                    printf("已返回\n");
                    return;
                }
                for (int j = i; j < usersum; j++)
                {
                    userList[j] = userList[j + 1];
                }
                usersum--;
                printf("删除成功！\n");
            }
        }
    }
    else if (n == 2)
    {
        scanf("%s", bufname);
        for (int i = 0; i < usersum; i++)
        {
            if (strcmp(userList[i].name, bufname)==0)
            {
                sign = 1;
                if (userList[i].bookborrowedsum > 0)
                {
                    printf("请督促该读者还书！！\n");
                    break;
                }
                printf("读者用户名为:%s\n，确认删除他吗?\n‘1’确认，‘0’返回\n", userList[i].name);
                scanf("%d", &p);
                if (p != 1)
                {
                    printf("已返回\n");
                    return;
                }
                for (int j = i; j < usersum; j++)
                {
                    userList[j] = userList[j + 1];
                    userList[j].number--;
                }
                usersum--;
                printf("删除成功！\n");
            }
        }
    }

    if (sign == 0)
    {
        printf("未找到该读者，请检查读者信息！\n");
    }
    saveMain();
}


//管理员主系统
void adminMain(char adminname[])
{
    int sign = 0;
    for (int i = 0; i < adminsum; i++)
    {
        if (strcmp(adminname, adminList[i].name) == 0)
        {
            sign = 1;
            break;
        }
    }
    if (sign == 0)
    {
        printf("很抱歉，您没有管理员权限！\n");
        return;
    }
    //检验密码是否正确
    printf("欢迎你%s请输入密码\n", adminname);
    int inputpassword;

    for (int i = 0; i < adminsum; i++)
    {
        if (strcmp(adminname, adminList[i].name) == 0)
        {
            i = adminsum;
            int j = 5;
            while (j > 0)
            {
                scanf("%d", &inputpassword);
                if (inputpassword != (adminList[i].number%10))
                {
                    printf("密码错误！你还有%d次机会\n", j-1);
                    j--;
                    if (j == 0)
                    {
                        printf("程序已经退出\n\n");
                        return;
                    }
                }
                else
                    j = -1;
            }
        }
    }
    int p = 1;
    while (p == 1)
    {
        adminmenu();
        printf("\n\n\n");
        scanf("%d", &choice);
        if (choice == 0)
        {
            break;
        }
        switch (choice)
        {
        case (0):
            p = 0;
            printf("退出成功！\n");
            break;
        case (1):
            newBook();
            break;
        case (2):
            modiBook();
            break;
        case (3):
            delBook();
            break;
        case (4):
            newUser();
            break;
        case (5):
            modiUser();
            break;
        case (6):
            delUser();
            break;
        case (7):
            bookInfo();
            break;
        case (8):
            printBooklist();
            break;
        case (9):
            userInfo();
            break;
        default:
            printf("请检查控制指令！\n\n");
        }
    }

}
//读者主系统
void userMain(char username[])
{
    int sign = 0;
    for (int i = 0; i < usersum; i++)
    {
        if (strcmp(username, userList[i].name) == 0)
        {
            sign = 1;
            break;
        }
    }
    if (sign == 0)
    {
        printf("嘻嘻，您好像还没有注册哦\n");
        return;
    }
    printf("请输入密码\n\n");
    int inputpassword,j=5;
    scanf("%d", &inputpassword);
    for (int i = 0; i < usersum; i++)
    {
        if (strcmp(username, userList[i].name) == 0)
        {
            if (inputpassword != (userList[i].number%10))
                {
                    printf("密码错误！你还有%d次机会\n", j-1);
                    j--;
                    if (j == 0)
                    {
                        printf("程序已经退出\n\n");
                        return;
                    }
                }
        }
    }
    //读者菜单
    int p = 0;
    while (p == 0)
    {
        usermenu();
        printf("\n");
        scanf("%d", &choice);
        if (choice == 0)
        {
            break;
        }
        switch (choice)
        {
        case (0):
            p = 1;
            printf("退出成功！\n");
            break;
        case (1):
            borrowBook(username);
            break;
        case (2):
            returnBook(username);
            break;
        case (3):
            bookInfo();
            break;
        case (4):
            userBook(username);
            break;
        default:
            printf("请输入正确的指令！\n");
        }
    }
}
//缓冲信息
int loadMain()
{
    if (loadAdminlist() && loadUserlist() && loadBooklist() && loadBorrowed())
    {
        printf("欢迎使用XD UNIVERSITY图书管理系统\n\n");
        return 1;
    }
    else
    {
        printf("请检查管理文件位置及是否损坏！\n");
        return 0;
    }
    return 0;
}
int main(int argc, char *argv[])
{
    //开始前导入用户,读者,馆藏书目,出借书目对应txt文件的信息
    if (loadMain() == 0)
        return 0;

    //admin
    if (strcmp(argv[1], "-a") == 0)
    {
        adminMain(argv[2]);
    }

    //user
    else if (strcmp(argv[1], "-u") == 0)
    {
        userMain(argv[2]);
    }
    else
    {
        printf("请输入正确用户名及指令！\n\n");
        return 0;
    }
    return 0;
}