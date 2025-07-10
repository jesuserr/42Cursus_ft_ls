#include <stdio.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>

int main(int argc, char **argv)
{
    DIR *dir = opendir(".");
    if (!dir)
        return 1;

    struct dirent *entity;
    struct stat st;
    
    entity = readdir(dir);
    while (entity)
    {
        printf("%d %s %d %ld\n", entity->d_type, entity->d_name, entity->d_reclen, entity->d_ino);
        stat(entity->d_name, &st);
        printf("Size: %ld bytes\n", st.st_size);
        entity = readdir(dir);
    }
    closedir(dir);
}