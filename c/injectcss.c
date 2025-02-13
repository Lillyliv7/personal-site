#include <stdio.h>
#include <sys/stat.h>
#include <stdbool.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>

#include <libxml/HTMLparser.h>
#include "cvector.h"

bool endsWith(const char *str, const char *suffix) {
    if (!str || !suffix) {
        return false;
    }
    size_t str_len = strlen(str);
    size_t suffix_len = strlen(suffix);

    if (suffix_len > str_len) {
        return false;
    }
    return (strncmp(str + str_len - suffix_len, suffix, suffix_len) == 0);
}

bool isDirectory(const char* filepath) {
    struct stat statbuf;
    if (stat(filepath, &statbuf)) return false;
    return S_ISDIR(statbuf.st_mode);
}

cvector_vector_type(char*) getFilesInDir(const char* location, const char* filetype) {
    cvector_vector_type(char*) list;
    cvector_init(list, 15, NULL);

    DIR *d;
    struct dirent *dir;
    d = opendir(location);
    if(!d) return list;

    while((dir = readdir(d))) {
        if(endsWith(dir->d_name, filetype))
            cvector_push_back(list, dir->d_name);
    }
    closedir(d);

    return list;
}

void printErr(const char* string) {
    printf("\033[0;31mINJECTCSS: %s\033[0m\n", string);
}

int main(int argc, char** argv) {
    puts("injecting css into html files");
    if (argc < 4) {
        printErr("Error incorrect usage");
        printErr("Call with ./injectcss <html_dir> <css_dir> <out_dir>");
        return 1;
    }
    if (!(isDirectory(argv[1]) && isDirectory(argv[2]) && isDirectory(argv[3]))) {
        printErr("Error directory does not exist");
    }
    
    cvector_vector_type(char*) list = getFilesInDir("./css/", ".css");
    for (int i = 0; i < cvector_size(list); i++)
        puts(list[i]);

    return 0;
}
