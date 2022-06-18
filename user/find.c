#include "kernel/types.h"
#include "kernel/stat.h"
#include "kernel/fs.h"
#include "user/user.h"

void find(char *dir, const char *filename) {
    char buf[512], *p;
    int fd;
    struct dirent de;
    struct stat st;

    if ((fd = open(dir, 0)) < 0) {
        fprintf(2, "find: cannot open %s\n", dir);
        return;
    }

    if (fstat(fd, &st) < 0) {
        fprintf(2, "find: cannot stat %s\n", dir);
        close(fd);
        return;
    }

    if (st.type != T_DIR) {
        fprintf(2, "usage: find *<dir>* <filename>\n"); 
        return;
    }

    if (strlen(dir) + 1 + DIRSIZ + 1 > sizeof buf) {
        fprintf(2, "find: dir is too long!\n");
        close(fd);
        return;
    }

    strcpy(buf, dir);
    p = buf + strlen(buf);
    *p++ = '/';
    while (read(fd, &de, sizeof de) == sizeof de) {
        if (de.inum == 0) 
            continue;
        memmove(p, de.name, DIRSIZ);
        p[DIRSIZ] == 0;
        if (stat(buf, &st) < 0) {
            printf("find: cannot stat %s\n", buf);
            continue;
        }
        if (st.type == T_DIR && strcmp(p, ".") != 0 && strcmp(p, "..") != 0) {
            find(buf, filename);
        } else if (strcmp(filename, p) == 0){
            printf("%s\n", buf);
        }
    }
    close (fd);
}



int main(int argc, char *argv[]) 
{
    if (argc < 3) {
        fprintf(2, "usage: find <dir> <filename>\n");
        exit(1);
    }
    find(argv[1], argv[2]);
    exit(0);
}