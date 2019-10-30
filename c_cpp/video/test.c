// test.c
#include <stdio.h>
#include <stdlib.h>

typedef unsigned int   u32;
typedef          int   s32;
typedef unsigned short u16;
typedef          short s16;
typedef unsigned char  u8;
typedef          char  s8;

void stringconv(const char *orig, char *dest)
{
    char *p = (char *)orig;
    int k = 0;
    
    while (*p != '\0')
    {
        if (*p != '#')
        {
            dest[k] = *p;
            k++;
        }
        else
        {
            dest[k] = '%';
            dest[k + 1] = '2';
            dest[k + 2] = '0';
            k += 2;
        }
        p++;
    }
}



#define BSWAP_16(x) \
    (u16)((((u16)(x) & 0x00ff) << 8) | \
          (((u16)(x) & 0xff00) >> 8) \
        )  

#define BSWAP_32(x) \
    (u32)( (((u32)(x) & 0xff000000) >> 24) | \
            (((u32)(x) & 0x00ff0000) >> 8) | \
            (((u32)(x) & 0x0000ff00) << 8) | \
            (((u32)(x) & 0x000000ff) << 24) \
            )  
// ————————————————
// 版权声明：本文为CSDN博主「吕小猪不坏」的原创文章，遵循 CC 4.0 BY-SA 版权协议，转载请附上原文出处链接及本声明。
// 原文链接：https://blog.csdn.net/wokaowokaowokao12345/article/details/71534472


#define  FILE_PATH   "C:\\Users\\isuke\\wk\\media\\1004-2.flv"
int file_read(void)
{
    FILE* fp_src = fopen(FILE_PATH, "rb");
    if (NULL == fp_src)
    {
        return 10209;
    }
    int ret1 = fseek(fp_src, 0, SEEK_SET);
    int ret2 = fseek(fp_src, 0x19, SEEK_SET);
    int data = 0;
    int ret3 = fread(&data, 1, 2, fp_src);
    int big = BSWAP_16(data);
    printf("ret1:%d, ret2:%d, ret3:%d, data:%d[%x], big:%d[%x]\n", ret1, ret2, ret3, data, data, big, big);
    fclose(fp_src);
    return 0;
}

int tst_big_small_endian()
{
    short int x;
    char x0,x1;
    x=0x1122;
    x0=((char *)&x)[0];  //低地址单元
    x1=((char *)&x)[1];  //高地址单元
    printf("x0=0x%x,x1=0x%x, ",x0,x1);// 若x0=0x11,则是大端; 若x0=0x22,则是小端......

    unsigned int i=0x04030201;
    unsigned char* cp=(unsigned char*)&i;

    printf("%x,%x,%x,%x", *cp, *(cp+1), *(cp+2), *(cp+3));
    if(*cp==1)
        printf("little-endian\n");
    else if(*cp==4)
        printf("big-endian\n");
    else
        printf("who knows?\n");
    return 0;
}
// ————————————————
// 版权声明：本文为CSDN博主「吕小猪不坏」的原创文章，遵循 CC 4.0 BY-SA 版权协议，转载请附上原文出处链接及本声明。
// 原文链接：https://blog.csdn.net/wokaowokaowokao12345/article/details/71534472
// }

int main(void)
{
    const char *pch_src = "We#Love#YY";
    char ach_dst[256] = {0};
    // stringconv(pch_src, ach_dst);
    // tst_big_small_endian();
    file_read();
    printf("src:%s, dst:%s\n", pch_src, ach_dst);
    system("pause");
    return 0;
}


// 12 点此或手机扫描二维码查看代码编写过程 [平均分6.7分 | 2人正确/6人做题 
    // | 提交: 14 次 得分：0.0 / 20.0 标题：排序算法编程题 |
    //  时间限制：2秒 | 内存限制：32768K | 
    // 语言限制： 不限 【排序算法编程题】
    // 请实现一个程序，对数组进行排序，要求返回结果是奇数在前，
    // 偶数在后。奇数部分从小到大排列，偶数部分从大到小排列。 
    // 输入描述： 输出描述： 示例1： 输入输出
