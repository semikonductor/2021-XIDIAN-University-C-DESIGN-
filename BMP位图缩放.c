//BMP位图缩放
//记得提前学一下BMP图片的文件结构，和双线性插值……

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <windows.h>

// 文件信息头结构体
typedef struct BITMAPFILEHEADER
{
    unsigned short bfType;      // 19778，必须是BM字符串，对应的十六进制为0x4d42,十进制为19778，否则不是bmp格式文件
    unsigned int bfSize;        // 文件大小(byte)
    unsigned short bfReserved1; // 0
    unsigned short bfReserved2; // 0
    unsigned int bfOffBits;     // 从文件头到像素数据的偏移
} BITMAPFILEHEADER;

//图像信息头结构体
typedef struct BITMAPINFOHEADER
{
    unsigned int biSize;         // 此结构体的大小
    long biWidth;                // 图像的宽
    long biHeight;               // 图像的高
    unsigned short biPlanes;     // 1
    unsigned short biBitCount;   // 一般是24
    unsigned int biCompression;  // 说明图象数据压缩的类型，0为不压缩。
    unsigned int biSizeImage;    // 像素数据所占大小
    long biXPelsPerMeter;        // 0
    long biYPelsPerMeter;        // 0
    unsigned int biClrUsed;      // 0
    unsigned int biClrImportant; // 0

} BITMAPINFOHEADER;

long wid;                     //原来图片的宽度
long height;                  //原来图片的高度
long biBitCount;              //每一个像素占的位数
long nwid;                    //新图宽度
long nheight;                 //新图高度
long pbyte;                   //原图一行占的字节
long nbyte;                   //新图一行占的字节
int zoomnumin = 100;          //输入放大比例（默认100%）
double zoomnumber = 1;        //去掉百分号放大比例（有可能出现不是整数，用double）
char *buf;                    //指向旧图像素的指针
char *nbuf;                   //指向新图像素的指针
BITMAPINFOHEADER srcinfohead; //原图信息头

//获取原图的头信息和像素信息
int loadhead(const char *src) //防止在函数中更改新图
{
    FILE *fp = fopen(src, "rb"); //只读打开原图
    if (fp == 0)
    {
        printf("打开失败，请检查文件名称及路径\n");
        return 0;
    }
    fseek(fp, sizeof(BITMAPFILEHEADER), 0); //将光标跳过文件头
    //BITMAPINFOHEADER srcinfohead;
    fread(&srcinfohead, sizeof(BITMAPINFOHEADER), 1, fp); //将信息头信息储存在变量中
    wid = srcinfohead.biWidth;
    height = srcinfohead.biHeight;
    biBitCount = srcinfohead.biBitCount;
    pbyte = (wid * biBitCount / 8 + 3) / 4 * 4;          //    （   +3）/4*4 可以确保lbyte不会因为补位而不是4的倍数    /8获取占用的字节数
    buf = (char *)malloc(sizeof(char) * pbyte * height); //储存像素信息
    fread(buf, 1, pbyte * height, fp);
    return 0;
}

//保存新图的头信息和像素信息
void savehead(const char *src, char *nbuf, int width, int height, int biBitCount)
{
    if (!nbuf)
    {
        return;
    }
    nbyte = (width * biBitCount / 8 + 3) / 4 * 4;
    FILE *fp = fopen(src, "wb"); //只读防止更改原图像
    if (fp == 0)
    {
        return;
    }
    BITMAPFILEHEADER filehead; //文件头的基本数据
    filehead.bfType = 0x4d42;
    filehead.bfSize = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + nbyte * height;
    filehead.bfReserved1 = 0;
    filehead.bfReserved2 = 0;
    filehead.bfOffBits = 54;
    fwrite(&filehead, sizeof(BITMAPFILEHEADER), 1, fp);
    BITMAPINFOHEADER infohead = srcinfohead; //更改与原图不同的数据
    infohead.biBitCount = biBitCount;
    infohead.biHeight = height;
    infohead.biSizeImage = pbyte * height;
    infohead.biWidth = width;

    fwrite(&infohead, sizeof(BITMAPINFOHEADER), 1, fp);
    fwrite(nbuf, height * nbyte, 1, fp);
    fclose(fp);
}
//放缩函数
void Zoom(const char *src, const char *dst)
{
    if (loadhead(src))
        return;
    nwid = (int)((wid * zoomnumber) + 0.5); //采用四舍五入而不是裁掉末尾
    nheight = (int)(height * zoomnumber + 0.5);
    pbyte = (wid * biBitCount / 8 + 3) / 4 * 4;
    nbyte = (nwid * biBitCount / 8 + 3) / 4 * 4;
    nbuf = (char *)malloc(sizeof(char) * nbyte * nheight);
    //以下双线性插值
    int i; //x坐标
    int j; //y坐标
    int k;
    long i0;
    long j0;

    for (i = 0; i < nheight; i++) //高
    {
        for (j = 0; j < nwid; j++) //宽
        {
            for (k = 0; k < 3; k++) //每个点三个字节
            {
                //新图中点对应的坐标
                i0 = i / zoomnumber;
                j0 = j / zoomnumber;
                //寻找四个参考点
                int i1 = (int)i0;
                int i2 = i1 + 1;
                int j1 = (int)j0;
                int j2 = j1 + 1;
                //插入的点与参考点坐标的差
                if ((j0 >= 0) && (j0 < wid - 1) && (i0 >= 0) && (i0 < height - 1)) //不能超出原图的大小
                {                                                                  //双线性插值
                    *(nbuf + i * nbyte + j * 3 + k) = *(buf + i2 * pbyte + j2 * 3 + k) * (i0 - i1) * (j0 - j1) +
                                                      *(buf + i1 * pbyte + j2 * 3 + k) * (1.0 - (i0 - i1)) * (j0 - j1) +
                                                      *(buf + i1 * pbyte + j1 * 3 + k) *(1.0 - (i0 - i1)) * (1.0 - (j0 - j1)) +
                                                      *(buf + i2 * pbyte + j1 * 3 + k) * (i0 - i1) * (1.0 - (j0 - j1));
                } //宽每增加一个，向右移动三个字节；高每增加一个，向右移动一整行大小的字节数
                else
                { //没有计算的点为255白色
                    *(nbuf + i * nbyte + j * 3 + k) = 255;
                }
            }
        }
    }
    savehead(dst, nbuf, nwid, nheight, biBitCount);
    free(nbuf);
}

int main(int argc, char *argv[])
{
    zoomnumin = atoi(argv[2]);
    zoomnumber = zoomnumin * 1.0 / 100.0; //防止吞掉小数
    Zoom(argv[1], argv[3]);
    return 0;
}
