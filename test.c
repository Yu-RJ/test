#include <errno.h> //与全局变量errno相关的定义
#include <fcntl.h> //定义了open、creat等函数，以及表示文件权限的宏定义
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h> //定义了ioctl函数
#include <sys/types.h> //定义了一些常用数据类型，比如size_t
#include <unistd.h>    //定义了read、write、close、lseek等函数

int main(void) {

    FILE *fp = NULL;
    int res = 0;
    int num = 8000000; // 8000000 = 1GB

    char filename[] = "test_b";
    char write_dat[] = "abcdefghijklmnopqrstuvwxyz ABCDEFGHIJKLMNOPQRSTUVWXYZ "
                       "123456789 ~!@#$^&*()_+-=[]./',.?><:` Hello World!\n";
    char write_dat_miss_order_1[] = "abcdefghijklmnopqrstuvwxyz ABCDEFGHIJKLMN";
    char write_dat_miss_order_2[] =
        "OPQRSTUVWXYZ 123456789 ~!@#$^&*()_+-=[]./',.?><:` Hello "
        "World!\nabcdefghijklmnopqrstuvwxyz ABCDEFGHIJKLMN";
    char read_buf[256] = {0};

    fp = fopen(filename, "wb");
    if (!fp) {
        printf("%s file open fail", filename);
        return -1;
    }
    for (int i = 0; i < num; i++)
        fwrite(write_dat, sizeof(write_dat), 1, fp);
    printf("write over ,num=%d\n", num);
    fclose(fp);

    // test 1
    printf("\n [test 1]: ... ... ... \n");
    fp = fopen(filename, "rb");
    if (!fp) {
        printf("%s file open fail", filename);
        return -1;
    }
    for (int i = 0; i < num; i++) {
        for (int j = 0; j < 256; j++)
            read_buf[j] = '\0';
        fread(read_buf, sizeof(write_dat), 1, fp);
        if (strcmp(read_buf, write_dat) != 0) {
            printf("CMP ERROR: i= %d , read: %s \n", i, read_buf);
            fclose(fp);
            return 0;
        }
    }
    printf("read over ,num=%d\n", num);
    fclose(fp);
    printf("[test 1]: PSASS \n");

    // test 2
    printf("\n [test 2]: ... ... ... \n");
    fp = fopen(filename, "rb");
    if (!fp) {
        printf("%s file open fail", filename);
        return -1;
    }
    for (int j = 0; j < 256; j++)
        read_buf[j] = '\0';
    fread(read_buf, sizeof(write_dat_miss_order_1), 1, fp);        
    for (int i = 0; i < num - 1; i++) {
        for (int j = 0; j < 256; j++)
            read_buf[j] = '\0';
        fread(read_buf, sizeof(write_dat_miss_order_2), 1, fp); 
        if (strcmp(read_buf, write_dat_miss_order_2) != 0) {
            printf("CMP ERROR: i= %d , read: %s \n", i, read_buf);
            fclose(fp);
            return 0;
        }
    }
    printf("read over ,num=%d\n", num);
    fclose(fp);
    printf("[test 2]: PSASS \n");

    return 0;
}