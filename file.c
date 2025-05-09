#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <errno.h>
#include <fcntl.h>
#include <string.h>
#include <sys/stat.h>
#include <time.h>

const char *FILE_NAME = "/media/user1/355e9783-5dd5-4d08-a336-9c44bb422f48/test.txt";
const char *CREATE_TMP_FILE_COMMAND = "dd if=/dev/urandom of=/media/user1/355e9783-5dd5-4d08-a336-9c44bb422f48/test_1.txt bs=500MB count=1";
const char *DELETE_FILES_COMMAND = "rm -rf /media/user1/355e9783-5dd5-4d08-a336-9c44bb422f48/*";
const char *LS_LA_FILES = "ls -la /media/user1/355e9783-5dd5-4d08-a336-9c44bb422f48/";
// Partition size is 200MB
// reset; gcc file.c ; ./a.out

enum CMD_TYPE {
    CMD_TYPE_LSEEK_2FL,
    CMD_TYPE_LSEEK_1F,
    CMD_TYPE_LSEEK_1L,
    CMD_TYPE_LSEEK_0,
    CMD_TYPE_CALLOC,
    CMD_TYPE_RAND
};

int workflow(enum CMD_TYPE type)
{
    printf("\n==================================\n");
    printf("==================================\n");
    printf("==================================\n");
    printf("\n==================================\n");
    printf("\n\t%s | %d | %s\n\n", __FILE__, __LINE__, __func__);
    if (type == CMD_TYPE_LSEEK_2FL) printf("\tTYPE == CMD_TYPE_LSEEK_2FL\n\n");
    if (type == CMD_TYPE_LSEEK_1F) printf("\tTYPE == CMD_TYPE_LSEEK_1F\n\n");
    if (type == CMD_TYPE_LSEEK_1L) printf("\tTYPE == CMD_TYPE_LSEEK_1L\n\n");
    if (type == CMD_TYPE_LSEEK_0) printf("\tTYPE == CMD_TYPE_LSEEK_0\n\n");
    if (type == CMD_TYPE_CALLOC) printf("\tTYPE == CMD_TYPE_CALLOC\n\n");
    if (type == CMD_TYPE_RAND) printf("\tTYPE == CMD_TYPE_RAND\n\n");
    
    sync();
    system(DELETE_FILES_COMMAND);
    printf("\n");
    system(LS_LA_FILES);
    printf("\n");
    sync();
    sleep(3);
    system("df");
    
    srand(time(NULL));

    printf("\nOpening create...\n");
    int fd = open(FILE_NAME,
                   O_RDWR | O_CREAT | O_TRUNC | O_SYNC,
                   S_IWUSR | S_IRUSR | S_IWGRP | S_IRGRP | S_IROTH);
    if (fd == -1) {
        perror("\tGot error");
        return -1;
    }
    
    ssize_t len = -1;
    char *buf = NULL;
    int ret = -1;
    
    if (type == CMD_TYPE_LSEEK_2FL || type == CMD_TYPE_LSEEK_1F) {
        len = 1;
        printf("Writing data to file (%zu)...\n", len);
        ret = write(fd, (void *)"0x42", len);
        if (ret == -1) {
	   printf("\tYoy!!!\n");
	    perror("\tGot error");
        }
    }
    
    len = 1024 * 100000;
    
    if (type == CMD_TYPE_LSEEK_2FL || type == CMD_TYPE_LSEEK_1F || type == CMD_TYPE_LSEEK_1L || type == CMD_TYPE_LSEEK_0) {
        printf("Writing  lseek data to file (%zu)...\n", len);
        printf("Seek file...\n");
        off_t ret_lseek = lseek(fd, len - 1, SEEK_SET);
        if (ret_lseek == -1) {
            perror("\tGot error");
            return -1;
        }
    
        if (type == CMD_TYPE_LSEEK_2FL || type == CMD_TYPE_LSEEK_1L) {
            len = 1;
            printf("Writing data to file (%zu)...\n", len);
            ret = write(fd, (void *)"0x42", len);
            if (ret == -1) {
                printf("\tYoy!!!\n");
                perror("\tGot error");
            }
        }
    }
    if (type == CMD_TYPE_CALLOC) {
        printf("Writing calloc data to file (%zu)...\n", len);
        buf = calloc(1, len);
        ret = write(fd, buf, len);
        if (ret == -1) {
            printf("\tYoy!!!\n");
            perror("\tGot error");
        }
    }
    if (type == CMD_TYPE_RAND) {
        printf("Writing calloc data to file (%zu)...\n", len);
        buf = malloc(len);
        for (size_t i = 0; i < len; ++i) {
            buf[i] = rand() % 100 + 1;
        }
        ret = write(fd, buf, len);
        if (ret == -1) {
            printf("\tYoy!!!\n");
            perror("\tGot error");
        }
    }
    
    len = 1024 * 100000;
    printf("File should be %zu...\n", len);
    
    printf("Closing file...\n");
    if (close(fd) == -1) {
        perror("\tGot error");
        return -1;
    }
    free(buf);
    buf = NULL;
    len = -1;
    
    sync();
    sleep(3);
    printf("\n");
    system(LS_LA_FILES);
    printf("\n");
    
    printf("Getting stat...\n");
    struct stat st0 = {}; 
    if (stat(FILE_NAME, &st0) == -1) {
        perror("\tGot error");
        return -1;
    }
    printf("file size: %lld\n", (long long int)st0.st_size);

    printf("\n");
    system("df");
    sync();
    printf("\n");
    sleep(3);
    
    printf("DD...\n");
    system(CREATE_TMP_FILE_COMMAND);
    printf("\n");
    system("df");
    sync();
    printf("\n");
    sleep(3);
    printf("\n");
    system(LS_LA_FILES);
    printf("\n");
    
    printf("Opening...\n");
    fd = open(FILE_NAME,
                   O_RDWR  | O_SYNC,
                   S_IWUSR | S_IRUSR | S_IWGRP | S_IRGRP | S_IROTH);
    if (fd == -1) {
        perror("\tGot error");
        return -1;
    }
    
    len = 1024 * 40000;
    printf("Writing data to file (%zu)...\n", len);
    buf = malloc(len);
    for (size_t i = 0; i < len; ++i) {
        buf[i] = rand() % 100 + 1;
    }
    ret = write(fd, buf, len);
    if (ret == -1) {
        printf("\tYoy!!!\n");
        perror("\tGot error");
    }
    printf("Closing file...\n");
    if (close(fd) == -1) {
        perror("\tGot error");
        return -1;
    }
    free(buf);
    buf = NULL;
    
    sync();
    sleep(3);
    printf("\n");
    system(LS_LA_FILES);
    printf("\n");
    
    printf("Getting stat...\n"); 
    if (stat(FILE_NAME, &st0) == -1) {
        perror("\tGot error");
        return -1;
    }
    printf("file size: %lld\n", (long long int)st0.st_size);
    printf("\n");
    system("df");
    printf("\n");
    sleep(3);
    
    printf("Getting truncate (%zu)...\n", len); 
    printf("File Name: %s\n", FILE_NAME);
    if (truncate(FILE_NAME, (off_t)len) == -1) {
        perror("\tGot error");
        return -1;
    }
    sync();
    sleep(3);
    printf("\n");
    system(LS_LA_FILES);
    printf("\n");
    
    printf("\n");
    system("df");
    printf("\n");
    printf("Getting stat...\n"); 
    if (stat(FILE_NAME, &st0) == -1) {
        perror("\tGot error");
        return -1;
    }
    printf("file size: %lld\n", (long long int)st0.st_size);
    
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
    
    (void)workflow(CMD_TYPE_LSEEK_2FL);
    (void)workflow(CMD_TYPE_LSEEK_1F);
    (void)workflow(CMD_TYPE_LSEEK_1L);
    (void)workflow(CMD_TYPE_LSEEK_0);
    (void)workflow(CMD_TYPE_CALLOC);
    (void)workflow(CMD_TYPE_RAND);

    return 0;
}
