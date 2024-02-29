#include<stdio.h>
#include<sys/stat.h>
#include<pwd.h>

void getUserName(int uid){
    struct passwd* pwd_stat;
    pwd_stat = getpwuid(uid);
    printf("User Name: %s\n", pwd_stat->pw_name);
};


int main(int argc, char const *argv[])
{
    struct stat file_stat;

    stat(argv[1], &file_stat);

    printf("Device ID: %lu\n", file_stat.st_dev);
    printf("File Mode: %u\n", file_stat.st_mode);
    printf("User ID: %u\n", file_stat.st_uid);
    printf("Group ID: %u\n", file_stat.st_gid);
    printf("File Size: %lu\n", file_stat.st_size);
    getUserName(file_stat.st_uid);

    return 0;
}
