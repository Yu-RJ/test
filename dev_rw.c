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

    FILE *fp = NULL;
    char filename[] = "/dev/nvme0n1";
    printf("begin test ... ... ...\n");
    fp = fopen(filename, "wb");
    if (!fp) {
        printf("%s file open fail\n", filename);
        return -1;
    } else
        printf("%s [file open success]  ", filename);
    char write_b[128];
    for (int i = 0; i < 128; i++)
        write_b[i] = i;
    for (int i = 0; i < 5000000; i++)
        fwrite(write_b, 128, 1, fp);
    fclose(fp);
    printf("[write over]\n");


    printf("\n [dev_rw]: ... ... ... \n");
    fp = fopen(filename, "rb");
    if (!fp) {
        printf("%s file open fail\n", filename);
        return -1;
    } else
        printf("%s [file open success]  ", filename);
    char com_c;
    for (int i = 0; i < 5000000; i++)
        for (int j = 0; j < 128; j++) {
            fread(&com_c, 1, 1, fp);
            if (com_c != j % 128) {
                printf("CMP ERROR: i=%d , j=%d , read:BEGIN-%d-END\n", i, j,
                       com_c);
            }
        }
    fclose(fp);
    printf("[dev_rw]: PSASS \n");
    return 0;
}