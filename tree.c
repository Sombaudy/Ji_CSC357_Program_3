#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>
#include <sys/stat.h>

void listDirectory(const char *dirName, int depth) {
    DIR *dir = opendir(dirName);
    if (!dir) {
        perror("opendir");
        return;
    }

    struct dirent *entry;

    while ((entry = readdir(dir)) != NULL) {
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
            continue;
        }

        for (int i = 0; i < depth; ++i) {
            printf("|   ");
        }
        
        printf("|-- %s\n", entry->d_name);

        // Check if it's a directory
        struct stat statbuf;
        char path[1024];
        snprintf(path, sizeof(path), "%s/%s", dirName, entry->d_name);

        if (stat(path, &statbuf) == 0 && S_ISDIR(statbuf.st_mode)) {
            listDirectory(path, depth + 1);
        }
    }

    closedir(dir);
}

int main(int argc, char *argv[]) {
    const char *directory = ".";

    if (argc > 1) {
        directory = argv[1];
    }

    listDirectory(directory, 0);
    return 0;
}
