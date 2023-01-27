#include <sys/types.h> //定义了一些常用数据类型，比如size_t
#include <fcntl.h>     //定义了open、creat等函数，以及表示文件权限的宏定义
#include <unistd.h>    //定义了read、write、close、lseek等函数
#include <errno.h>     //与全局变量errno相关的定义
#include <sys/ioctl.h> //定义了ioctl函数
#include <stdio.h>
#include <string.h>

int main(void)
{
   int fd = -1;
   int res = 0;
   int num = 10000;

   char filename[] = "test.txt";
   char write_dat[] = "abcdefghijklmnopqrstuvwxyz ABCDEFGHIJKLMNOPQRSTUVWXYZ 123456789 ~!@#$^&*()_+-=[]./',.?><:` Hello World!\n";
   char read_buf[256] = {0};

   fd = open(filename, O_RDWR | O_CREAT, 0664);
   if (fd < 0){
      printf("%s file open fail", filename);
      return -1;
   }
   for (int i = 0; i < num; i++)
      res = write(fd, write_dat, sizeof(write_dat));
   if (res < 0){
      printf("write dat failb");
      return -1;
   }else
      printf("write over ,num=%d\n", num);
   close(fd);

   /* 读取并测试准确性 */
   fd = open(filename, O_RDONLY);
   if (fd < 0){
      printf("%s file open fail", filename);
      return -1;
   }
   for (int i = 0; i < num; i++){
      for (int j = 0; j < 256; j++)
         read_buf[j] = '\0';
      res = read(fd, read_buf, sizeof(write_dat));
      if (strcmp(read_buf, write_dat) != 0){
         printf("Comparation ERROR: i= %d , read: %s \n", i, read_buf);
         close(fd);
         return 0;
      }
   }
   if (res < 0){
      printf("read dat fail");
      return -1;
   }else
      printf("read over ,num=%d\n", num);
   close(fd);

   return 0;
}