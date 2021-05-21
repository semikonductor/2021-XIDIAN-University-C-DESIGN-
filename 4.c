#include <stdio.h>
#include <stdlib.h> //malloc
#include <string.h> //strcmp
#include<math.h>
#define MAX 65536000

//判断是否存在三个以上重复数据
int repj(unsigned char *src, int srcleft)
{
    if (srcleft < 3)
    {
        return 0;
    }
    if ((src[0] == src[1]) && (src[1] == src[2]))
    {
        return 1;
    }
    return 0;
}

//判断重复数据数目
int repnum(unsigned char *src, int srcleft)
{
    int repeatedbuf = src[0];
    int length = 1;
    while (length < srcleft && length < 127 && src[length] == repeatedbuf)//1.长度不能超过127和读取的实际长度     2.连续重复
    {
        length++;
    }
    return length;
}

//判断不重复数据数目
int nrepnum(unsigned char *src, int srcleft)
{
    if (srcleft < 3)
    {
        return srcleft;
    }
    int length = 2;
    int a = src[0], b = src[1];
    while (length < srcleft && length < 127 && ((a != b) || (b != src[length])))//1.长度不能超过127和读取的实际长度     2.连续不重复
    {
        a = b;
        b = src[length];
        length++;
    }
    return length;
}
//压缩编码
int code(unsigned char *inbuf, int length, unsigned char *outbuf, int maxSize)
{
    unsigned char *src = inbuf;//用src操作，就不用移动inbuf
    int i;
    int codesize = 0;
    while (length > 0)
    {
        int count = 0;
        if (repj(src, length))
        {
            if ((codesize + 2) > maxSize) //输出缓存不够了
            {
                return 0;
            }
            count = repnum(src, length);
            outbuf[codesize++] = count | 128; //第一位为1，解压时表示重复的数据块，其余为表示重复数据个数<=127
            outbuf[codesize++] = *src;        //重复的数据复制一个
            src += count;                     //指针向后移动，跳过重复的区域,重复数据压缩成功，节省空间
            length -= count;                  //剩余需压缩数据减小
        }
        else
        {
            count = nrepnum(src, length);
            if ((codesize + count + 1) > maxSize) //输出缓存不够
            {
                return 0;
            }
            outbuf[codesize++] = count; //不重复数据的数量
            for (i = 0; i < count; i++) //逐个复制不重复数据
            {
                outbuf[codesize++] = *src++;
            }
            length -= count; //剩余需压缩数据减小
        }
    }
    return codesize;
}
//解压编码
int decode(unsigned char *inbuf, int length, unsigned char *outbuf, int maxSize)
{
    unsigned char *src = inbuf;//用src操作，就不用移动inbuf
    int i;
    int decodesize = 0;
    int count = 0;
    while (src < (inbuf + length))
    {
        unsigned char sign = *src++;
        int count = sign & 127; //清除第一位标识位的数据，得到后续数据的数量（长度）
        if ((decodesize + count) > maxSize)
        {
            return 0;
        }
        if ((sign & 128) == 128) //第一位是1，重复数据，复制一个
        {
            for (i = 0; i < count; i++)
            {
                outbuf[decodesize++] = *src; //复制一个
            }
            src++;
        }
        else //非重复数据，逐个复制
        {
            for (i = 0; i < count; i++)
            {
                outbuf[decodesize++] = *src++; //逐个复制
            }
        }
    }
    return decodesize;
}
//压缩
int compress(char *fin, char *fout)
{
    FILE *in = fopen(fin, "rb");
    FILE *out = fopen(fout, "wb");
    if (in == NULL || out == NULL)
    {
        printf("文件打开失败，请检查文件名称!");
    }
    unsigned char *inbuf;  //输入缓存
    unsigned char *outbuf; //输出缓存
    inbuf = (unsigned char *)malloc((sizeof(unsigned char)) * MAX);
    outbuf = (unsigned char *)malloc((sizeof(unsigned char)) * MAX);
    int length; //读取的有效长度
    while ((length = fread(inbuf, sizeof(unsigned char),1024*1024, in)) != 0)
    {
        int buf = code(inbuf, length, outbuf, MAX);
        if (buf == 0)
        {
            printf("文件太大，压缩失败");
            return 0;
        }
        fwrite(outbuf, sizeof(unsigned char), buf, out);
    }
    fclose(in);
    fclose(out);
    printf("压缩成功");
    return 1;
}
//解压
int decompress(char *fin, char *fout)
{
    FILE *in = fopen(fin, "rb");
    FILE *out = fopen(fout, "wb");
    if (in == NULL || out == NULL)
    {
        printf("文件打开失败，请检查文件名称!");
    }
    unsigned char *inbuf;
    unsigned char *outbuf;
    inbuf = (unsigned char *)malloc((sizeof(unsigned char)) * MAX);
    outbuf = (unsigned char *)malloc((sizeof(unsigned char)) * MAX);
    int length;
    while ((length = fread(inbuf, sizeof(unsigned char), MAX, in)) != 0)
    {
        int buf = decode(inbuf, length, outbuf, MAX);
        if (buf == 0)
        {
            return 0;
            printf("原文件过大，解压失败");
        }
        fwrite(outbuf, sizeof(unsigned char), buf, out);
    }
    fclose(in);
    fclose(out);
    printf("解压成功");
    return 1;
}

int main(int argc, char *argv[])
{
    if (!strcmp(argv[2], "-c"))
    {
        compress(argv[1], argv[3]); //压缩
    }
    else if (!strcmp(argv[2], "-d"))
    {
        decompress(argv[1], argv[3]); //解压
    }
    return 0;
}