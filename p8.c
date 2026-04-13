#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>

long long total_size = 0;

void process_dir(const char *dir_name, int depth) {
    DIR *dir = opendir(dir_name);
    if (dir == NULL) {
        return;
    }

    for (int i = 0; i < depth; i++) printf("  ");
    printf("dir %s\n", dir_name);

    struct dirent *entry;
    while ((entry = readdir(dir)) != NULL) {
        if (entry->d_name[0] == '.') {
            continue; 
        }

        char *full_path;
        asprintf(&full_path, "%s/%s", dir_name, entry->d_name);

        struct stat file_stat;
        if (lstat(full_path, &file_stat) == 0) {
            if (S_ISDIR(file_stat.st_mode)) {
                process_dir(full_path, depth + 1);
            } else if (S_ISREG(file_stat.st_mode)) {
                for (int i = 0; i <= depth; i++) printf("  ");
                printf("%8lld:%s\n", (long long)file_stat.st_size, entry->d_name);
                total_size += file_stat.st_size;
            }
        }

        free(full_path);
    }

    closedir(dir);
}

int main(int argc, char *argv[]) {
    char *start_dir;

    if (argc > 1) {
        start_dir = argv[1];
    } else {
        start_dir = ".";
    }

    process_dir(start_dir, 0);

    printf("\nTotal file space used:%lld\n", total_size);

    return 0;
}