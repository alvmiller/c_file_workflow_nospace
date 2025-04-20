#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <errno.h>
#include <fcntl.h>
#include <string.h>
#include <sys/stat.h>
#include <time.h>

int w05_lseek()
{
    printf("\n==================================\n");
    printf("==================================\n");
    printf("==================================\n");
    printf("\n==================================\n\n");
    printf("\n\t%s | %d | %s\n\n", __FILE__, __LINE__, __func__);
    
    sync();
    system("rm -rf /media/user1/3b6f63c6-3d92-45bc-8623-ed4645dbcb84/*");
    sync();
    sleep(3);
    system("df");

    printf("Opening create...\n");
    int fd = open("/media/user1/3b6f63c6-3d92-45bc-8623-ed4645dbcb84/test.txt",
                   O_RDWR | O_CREAT | O_TRUNC | O_SYNC,
                   S_IWUSR | S_IRUSR | S_IWGRP | S_IRGRP | S_IROTH);
    if (fd == -1) {
        perror("\tGot error");
        return -1;
    }
    
    ssize_t len = 1024 * 80000;
    char *buf = NULL;
    int ret = -1;
    
    printf("Writing 1B to file...\n");
    ret = write(fd, (void *)"0x42", 1);
    if (ret == -1) {
        printf("\tYoy!!!\n");
        perror("\tGot error");
    }
    
    printf("Seek file...\n");
    off_t ret_lseek = lseek(fd, len - 1, SEEK_SET);
    if (ret_lseek == -1) {
        perror("\tGot error");
        return -1;
    }
    
    printf("Writing 1B to file...\n");
    ret = write(fd, (void *)"0x42", 1);
    if (ret == -1) {
        printf("\tYoy!!!\n");
        perror("\tGot error");
    }
    
    printf("File should be %zu...\n", len);
    
    printf("Closing file...\n");
    if (close(fd) == -1) {
        perror("\tGot error");
        return -1;
    }
    sync();
    printf("Getting stat...\n");
    struct stat st0 = {}; 
    if (stat("/media/user1/3b6f63c6-3d92-45bc-8623-ed4645dbcb84/test.txt", &st0) == -1) {
        perror("\tGot error");
        return -1;
    }
    printf("file size: %lld\n", (long long int)st0.st_size);

    printf("\n");
    system("df");
    sync();
    printf("\n");
    sleep(5);
    
    printf("DD...\n");
    system("dd if=/dev/urandom of=/media/user1/3b6f63c6-3d92-45bc-8623-ed4645dbcb84/test_1.txt bs=500MB count=1");
    
    printf("\n");
    system("df");
    sync();
    printf("\n");
    sleep(5);
    
    printf("Opening...\n");
    fd = open("/media/user1/3b6f63c6-3d92-45bc-8623-ed4645dbcb84/test.txt",
                   O_RDWR  | O_SYNC,
                   S_IWUSR | S_IRUSR | S_IWGRP | S_IRGRP | S_IROTH);
    if (fd == -1) {
        perror("\tGot error");
        return -1;
    }
    
    len = 1024 * 40000;
    printf("Writing data to file (%zu)...\n", len);
    buf = malloc(len);
    srand(time(NULL));
    for (size_t i = 0; i < len; ++i) {
        buf[i] = rand() % 100 + 1;
    }
    ret = write(fd, buf, len);
    if (ret == -1) {
        printf("\tYoy!!!\n");
        perror("\tGot error");
        //return -1;
    }
    printf("Closing file...\n");
    if (close(fd) == -1) {
        perror("\tGot error");
        return -1;
    }
    sync();
    free(buf);
    buf = NULL;
    printf("\n");
    printf("Getting stat...\n"); 
    if (stat("/media/user1/3b6f63c6-3d92-45bc-8623-ed4645dbcb84/test.txt", &st0) == -1) {
        perror("\tGot error");
        return -1;
    }
    printf("file size: %lld\n", (long long int)st0.st_size);
    printf("\n");
    system("df");
    printf("\n");
    sleep(5);
    
    printf("Getting truncate...\n"); 
    if (truncate("/media/user1/3b6f63c6-3d92-45bc-8623-ed4645dbcb84/test.txt", len) == -1) {
        perror("\tGot error");
        return -1;
    }
    sync();
    
    printf("\n");
    system("df");
    printf("\n");
    printf("Getting stat...\n"); 
    if (stat("/media/user1/3b6f63c6-3d92-45bc-8623-ed4645dbcb84/test.txt", &st0) == -1) {
        perror("\tGot error");
        return -1;
    }
    printf("file size: %lld\n", (long long int)st0.st_size);
    printf("\n");
    
    printf("\n");
    system("df");
    printf("\n");
    sleep(1);
    
    printf("\n==================================\n");
    printf("==================================\n");
    printf("==================================\n");
    printf("\n==================================\n\n");

    return 0;
}

int w04_calloc()
{
    printf("\n==================================\n");
    printf("==================================\n");
    printf("==================================\n");
    printf("\n==================================\n\n");
    printf("\n\t%s | %d | %s\n\n", __FILE__, __LINE__, __func__);
    
    sync();
    system("rm -rf /media/user1/3b6f63c6-3d92-45bc-8623-ed4645dbcb84/*");
    sync();
    sleep(3);
    system("df");

    printf("Opening create...\n");
    int fd = open("/media/user1/3b6f63c6-3d92-45bc-8623-ed4645dbcb84/test.txt",
                   O_RDWR | O_CREAT | O_TRUNC | O_SYNC,
                   S_IWUSR | S_IRUSR | S_IWGRP | S_IRGRP | S_IROTH);
    if (fd == -1) {
        perror("\tGot error");
        return -1;
    }
    printf("Writing data to file...\n");
    ssize_t len = 1024 * 80000;
    char *buf = calloc(1, len);
    int ret = write(fd, buf, len);
    if (ret == -1) {
        printf("\tYoy!!!\n");
        perror("\tGot error");
        //return -1;
    }
    printf("Closing file...\n");
    if (close(fd) == -1) {
        perror("\tGot error");
        return -1;
    }
    free(buf);
    buf = NULL;
    sync();
    printf("Getting stat...\n");
    struct stat st0 = {}; 
    if (stat("/media/user1/3b6f63c6-3d92-45bc-8623-ed4645dbcb84/test.txt", &st0) == -1) {
        perror("\tGot error");
        return -1;
    }
    printf("file size: %lld\n", (long long int)st0.st_size);

    printf("\n");
    system("df");
    sync();
    printf("\n");
    sleep(5);
    
    printf("DD...\n");
    system("dd if=/dev/urandom of=/media/user1/3b6f63c6-3d92-45bc-8623-ed4645dbcb84/test_1.txt bs=300MB count=1");
    
    printf("\n");
    system("df");
    sync();
    printf("\n");
    sleep(5);
    
    printf("Opening...\n");
    fd = open("/media/user1/3b6f63c6-3d92-45bc-8623-ed4645dbcb84/test.txt",
                   O_RDWR  | O_SYNC,
                   S_IWUSR | S_IRUSR | S_IWGRP | S_IRGRP | S_IROTH);
    if (fd == -1) {
        perror("\tGot error");
        return -1;
    }
    printf("Writing data to file...\n");
    len = 1024 * 40000;
    buf = malloc(len);
    srand(time(NULL));
    for (size_t i = 0; i < len; ++i) {
        buf[i] = rand() % 100 + 1;
    }
    ret = write(fd, buf, len);
    if (ret == -1) {
        printf("\tYoy!!!\n");
        perror("\tGot error");
        //return -1;
    }
    printf("Closing file...\n");
    if (close(fd) == -1) {
        perror("\tGot error");
        return -1;
    }
    sync();
    free(buf);
    buf = NULL;
    printf("\n");
    printf("Getting stat...\n"); 
    if (stat("/media/user1/3b6f63c6-3d92-45bc-8623-ed4645dbcb84/test.txt", &st0) == -1) {
        perror("\tGot error");
        return -1;
    }
    printf("file size: %lld\n", (long long int)st0.st_size);
    printf("\n");
    system("df");
    printf("\n");
    sleep(5);
    
    printf("Getting truncate...\n"); 
    if (truncate("/media/user1/3b6f63c6-3d92-45bc-8623-ed4645dbcb84/test.txt", len) == -1) {
        perror("\tGot error");
        return -1;
    }
    sync();
    
    printf("\n");
    system("df");
    printf("\n");
    printf("Getting stat...\n"); 
    if (stat("/media/user1/3b6f63c6-3d92-45bc-8623-ed4645dbcb84/test.txt", &st0) == -1) {
        perror("\tGot error");
        return -1;
    }
    printf("file size: %lld\n", (long long int)st0.st_size);
    printf("\n");
    
    printf("\n");
    system("df");
    printf("\n");
    sleep(1);
    
    printf("\n==================================\n");
    printf("==================================\n");
    printf("==================================\n");
    printf("\n==================================\n\n");

    return 0;
}

int w03_rand()
{
    printf("\n==================================\n");
    printf("==================================\n");
    printf("==================================\n");
    printf("\n==================================\n\n");
    printf("\n\t%s | %d | %s\n\n", __FILE__, __LINE__, __func__);
    
    sync();
    system("rm -rf /media/user1/3b6f63c6-3d92-45bc-8623-ed4645dbcb84/*");
    sync();
    sleep(3);
    system("df");

    printf("Opening create...\n");
    int fd = open("/media/user1/3b6f63c6-3d92-45bc-8623-ed4645dbcb84/test.txt",
                   O_RDWR | O_CREAT | O_TRUNC | O_SYNC,
                   S_IWUSR | S_IRUSR | S_IWGRP | S_IRGRP | S_IROTH);
    if (fd == -1) {
        perror("\tGot error");
        return -1;
    }
    printf("Writing data to file...\n");
    ssize_t len = 1024 * 80000;
    char *buf = calloc(1, len);
    srand(time(NULL));
    for (size_t i = 0; i < len; ++i) {
        buf[i] = rand() % 100 + 1;
    }
    int ret = write(fd, buf, len);
    if (ret == -1) {
        printf("\tYoy!!!\n");
        perror("\tGot error");
        //return -1;
    }
    printf("Closing file...\n");
    if (close(fd) == -1) {
        perror("\tGot error");
        return -1;
    }
    free(buf);
    buf = NULL;
    sync();
    printf("Getting stat...\n");
    struct stat st0 = {}; 
    if (stat("/media/user1/3b6f63c6-3d92-45bc-8623-ed4645dbcb84/test.txt", &st0) == -1) {
        perror("\tGot error");
        return -1;
    }
    printf("file size: %lld\n", (long long int)st0.st_size);

    printf("\n");
    system("df");
    sync();
    printf("\n");
    sleep(5);
    
    printf("DD...\n");
    system("dd if=/dev/urandom of=/media/user1/3b6f63c6-3d92-45bc-8623-ed4645dbcb84/test_1.txt bs=300MB count=1");
    
    printf("\n");
    system("df");
    sync();
    printf("\n");
    sleep(5);
    
    printf("Opening...\n");
    fd = open("/media/user1/3b6f63c6-3d92-45bc-8623-ed4645dbcb84/test.txt",
                   O_RDWR  | O_SYNC,
                   S_IWUSR | S_IRUSR | S_IWGRP | S_IRGRP | S_IROTH);
    if (fd == -1) {
        perror("\tGot error");
        return -1;
    }
    printf("Writing data to file...\n");
    len = 1024 * 40000;
    buf = malloc(len);
    for (size_t i = 0; i < len; ++i) {
        buf[i] = rand() % 100 + 1;
    }
    ret = write(fd, buf, len);
    if (ret == -1) {
        printf("\tYoy!!!\n");
        perror("\tGot error");
        //return -1;
    }
    printf("Closing file...\n");
    if (close(fd) == -1) {
        perror("\tGot error");
        return -1;
    }
    sync();
    free(buf);
    buf = NULL;
    printf("\n");
    printf("Getting stat...\n"); 
    if (stat("/media/user1/3b6f63c6-3d92-45bc-8623-ed4645dbcb84/test.txt", &st0) == -1) {
        perror("\tGot error");
        return -1;
    }
    printf("file size: %lld\n", (long long int)st0.st_size);
    printf("\n");
    system("df");
    printf("\n");
    sleep(5);
    
    printf("Getting truncate...\n"); 
    if (truncate("/media/user1/3b6f63c6-3d92-45bc-8623-ed4645dbcb84/test.txt", len) == -1) {
        perror("\tGot error");
        return -1;
    }
    sync();
    
    printf("\n");
    system("df");
    printf("\n");
    printf("Getting stat...\n"); 
    if (stat("/media/user1/3b6f63c6-3d92-45bc-8623-ed4645dbcb84/test.txt", &st0) == -1) {
        perror("\tGot error");
        return -1;
    }
    printf("file size: %lld\n", (long long int)st0.st_size);
    printf("\n");
    
    printf("\n");
    system("df");
    printf("\n");
    sleep(1);
    
    printf("\n==================================\n");
    printf("==================================\n");
    printf("==================================\n");
    printf("\n==================================\n\n");

    return 0;
}

int main()
{
    system("reset");
    
    (void)w05_lseek();
    (void)w04_calloc();
    (void)w03_rand();

    return 0;
}
