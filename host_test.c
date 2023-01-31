#include <errno.h> //与全局变量errno相关的定义
#include <fcntl.h> //定义了open、creat等函数，以及表示文件权限的宏定义
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h> //定义了ioctl函数
#include <sys/types.h> //定义了一些常用数据类型，比如size_t
#include <unistd.h>    //定义了read、write、close、lseek等函数
int mycmp(void *arr1, void *arr2, int len);

int main(void) {
    // --------------------------------------------------------------------------------
    FILE *fp = NULL;
    int re = 0;
    char filename[] = "test_data";
    char com_c;

    printf("begin test ... ... ...\n");
    fp = fopen(filename, "wb");
    if (!fp) {
        printf("%s file open fail\n", filename);
        return -1;
    } else
        printf("%s file open success:  ", filename);
    char write_b[128];
    void *ptr = write_b;

    for (int i = 0; i < 128; i++)
        write_b[i] = i;
    for (int i = 0; i < 8 * 1024; i++)
        fwrite(ptr, 128, 1, fp);
    fclose(fp);
    printf("[write over]\n");

    // test 1
    // --------------------------------------------------------------------------------
    printf("\n [test 1]: ... ... ... \n");
    fp = fopen(filename, "rb");
    if (!fp) {
        printf("%s file open fail\n", filename);
        return -1;
    } else
        printf("%s file open success:  ", filename);
    // test 1.1 ----------------------// 测试普通读写
    for (int i = 0; i < 8 * 1024; i++)
        for (int j = 0; j < 128; j++) {
            fread(&com_c, 1, 1, fp);
            if (com_c != j % 128) {
                printf("1.1 CMP ERROR: i=%d , j=%d , read:BEGIN-%d-END\n", i, j,
                       com_c);
            }
        }
    printf("    [1.1 pass]\n");
    // test 1.2 ---------------------- // 测试 fseek 定位
    re = fseek(fp, 128 * 8, SEEK_SET);
    if (re != 0)
        printf("1.1 fseek fail,re=%d", re);
    for (int i = 0; i < 8 * 1024 - 8; i++)
        for (int j = 0; j < 128; j++) {
            fread(&com_c, 1, 1, fp);
            if (com_c != j % 128) {
                printf("1.1 CMP ERROR: i=%d , j=%d , read:BEGIN-%d-END\n", i, j,
                       com_c);
            }
        }
    printf("    [1.2 pass]\n");
    fclose(fp);

    // test 2   // 测试 wb+ / rb+ 的 定位写后读
    // --------------------------------------------------------------------------------
    printf("\n [test 2]: ... ... ... \n");
    fp = fopen(filename, "wb+");
    if (!fp) {
        printf("%s file open fail\n", filename);
        return -1;
    } else
        printf("%s file open success\n", filename);
    // 先写一般数据
    for (int i = 0; i < 128; i++)
        write_b[i] = i;
    for (int i = 0; i < 8; i++)
        fwrite(ptr, 128, 1, fp);
    // 再写特殊位置数据
    re = fseek(fp, 128, SEEK_SET);
    if (re != 0)
        printf("2 fseek fail 1,re=%d", re);
    for (int i = 127; i >= 0; i--)
        write_b[127 - i] = i;
    for (int i = 0; i < 3; i++)
        fwrite(ptr, 128, 1, fp);
    // 读取特殊位置数据
    re = fseek(fp, 128 * 2, SEEK_SET);
    if (re != 0)
        printf("2 fseek fail 2,re=%d", re);
    for (int i = 0; i < 1; i++)
        for (int j = 127; j >= 0; j--) {
            fread(&com_c, 1, 1, fp);
            if (com_c != j % 128) {
                printf("2 CMP ERROR: i=%d , j=%d , read:BEGIN-%d-END\n", i, j,
                       com_c);
            }
        }
    for (int i = 0; i < 8 * 128; i++) {
        fread(&com_c, 1, 1, fp);
        printf("%c", com_c);
        if (i % 128 == 0)
            printf(" [|] \n");
    }
    fclose(fp);
    printf("    [2 pass]\n");
    return 0;
}

int mycmp(void *arr1, void *arr2, int len) {
    char *str1 = arr1;
    char *str2 = arr2;
    for (int i = 0; i < len; i++)
        if (*str1++ != *str2++)
            return -1;
    return 0;
}
