#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>
#include <sys/stat.h>

int compare(const void *a, const void *b) {
    return strcmp(*(const char **)a, *(const char **)b);
}

void listDir(const char *dirName, int depth) {
    struct stat statbuf;
    if (stat(dirName, &statbuf) == -1) {
        perror("stat");
        return;
    }

    DIR *dir = opendir(dirName);
    if (!dir) {
        perror("opendir");
        return;
    }

    char **stuff = NULL;
    int numStuff = 0;

    struct dirent *dirStuff;
    while ((dirStuff = readdir(dir)) != NULL) {
        if (strcmp(dirStuff->d_name, ".") == 0 || strcmp(dirStuff->d_name, "..") == 0) {
            continue;
        }
        stuff = realloc(stuff, (numStuff + 1) * sizeof(char *));
        stuff[numStuff] = malloc(strlen(dirStuff->d_name) + 1);
        strcpy(stuff[numStuff], dirStuff->d_name);
        numStuff++;
    }

    closedir(dir);

    qsort(stuff, numStuff, sizeof(char *), compare);

    for (int i = 0; i < numStuff; ++i) {
        for (int j = 0; j < depth; ++j) {
            printf("|   ");
        }
        
        printf("|-- %s\n", stuff[i]);

        char path[1024];
        snprintf(path, sizeof(path), "%s/%s", dirName, stuff[i]);

        if (stat(path, &statbuf) == 0 && S_ISDIR(statbuf.st_mode)) {
            listDir(path, depth + 1);
        }

        free(stuff[i]);
    }

    free(stuff);
}

int main(int argc, char *argv[]) {
    const char *name;

    if (argc < 2) {
        name = "."; // If no argument provided, use the current directory
    } else {
        name = argv[1];
    }

    printf("%s\n", name);
    listDir(name, 0);
    return 0;
}
