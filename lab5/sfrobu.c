
//
//  sfrobu.c
//
//  Christopher Aziz
//  UID 304806012
//  Nov. 5, 2017
//
//  Homework: Encrypted sort revisited
//  --------------------------------
//  Takes frobnicated words from
//  stdin, sorts them, and prints
//  them to stdout without ever
//  storing unfrobnicated  words in
//  memory.
//

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <ctype.h>
#include <string.h>
#include <limits.h>

void checkIOError() {
    if(ferror(stdin)) {
        fprintf(stderr, "Error reading from stdin\n");
        exit(1);
    }
}

void checkMemError(void* ptr) {
    if(ptr == NULL) {
        fprintf(stderr, "Error in memory allocation\n");
        exit(1);
    }
}

// compare frobnicated c strings that end in a space
int frobcmp(char const *a, char const *b) {
    // compare words char by char or until space is reached
    while(1) {
        if(*a == ' ' && *b == ' ') return 0;
        if((*a ^ 42) > (*b ^ 42) || *b == ' ') return 1;
        if((*b ^ 42) > (*a ^ 42) || *a == ' ') return -1;
        a++;
        b++;
    }
}
// if f option enabled, do not check case
int ffrobcmp(char const *a, char const *b) {
    if ((*a ^ 42) > UCHAR_MAX || (*a ^ 42) < 0 || (*b ^ 42) > UCHAR_MAX || (*b ^ 42) < 0) {
        fprintf(stderr, "Invalid characters for use of '-f'\n");
        exit(1);
    }
    while(1) {
        if(*a == ' ' && *b == ' ') return 0;
        if(toupper(*a ^ 42) > toupper(*b ^ 42) || *b == ' ') return 1;
        if(toupper(*b ^ 42) > toupper(*a ^ 42) || *a == ' ') return -1;
        a++;
        b++;
    }
}

// frobcmp wrapper that takes pointers to c strings
int frobcmpWrapper(const void* a_ptr, const void* b_ptr) {
    const char* a = *(const char**)a_ptr;
    const char* b = *(const char**)b_ptr;
    return frobcmp(a,b);
}
// f option ffrobcmp wrapper
int ffrobcmpWrapper(const void* a_ptr, const void* b_ptr) {
    const char* a = *(const char**)a_ptr;
    const char* b = *(const char**)b_ptr;
    return ffrobcmp(a,b);
}

int main(int argc, const char * argv[]) {

    // if f option is enabled do not check case
    int f = 0;
    
    // parse and check arguments
    if(argc > 2) {
        fprintf(stderr, "Invalid arguments\n");
        exit(1);
    }
    if(argc == 2) {
        if(strcmp(argv[1], "-f") == 0) {
            f = 1;
        } else {
            fprintf(stderr, "Invalid arguments\n");
            exit(1);
        }
    }
    // set and check file status
    struct stat file_status;
    if(fstat(STDIN_FILENO, &file_status) < 0) {
        fprintf(stderr, "Error in getting file status\n");
        exit(1);
    }
    
    // buffer to hold raw input
    char* buffer;
    int buffer_size;
    int buffer_index;
    
    if(S_ISREG(file_status.st_mode)) {
        // for regular files, attempt to read entire file
        buffer_size = file_status.st_size;
        buffer_index = buffer_size;
        buffer = malloc(sizeof(char) * (buffer_size + 1));
        if(read(0, buffer, buffer_size) < 0) {
            checkIOError();
            fprintf(stderr, "Error in reading file\n");
            exit(1);
        }
    } else {
        // for irregular files, read file char by char
        buffer = malloc(sizeof(char));
        checkMemError(buffer);
        buffer_size = 1;
        buffer_index = 0;
        // char ptr used in reading to buffer
        char *curr_char = malloc(sizeof(char*));
        while(read(0, curr_char, 1) > 0) {
            checkIOError();
            buffer[buffer_index++] = *curr_char;
            // if buffer is full, realloc
            if(buffer_index == buffer_size) {
                buffer_size *= 2;
                buffer = realloc(buffer, sizeof(char) * buffer_size);
                checkMemError(buffer);
            }
        }
        free(curr_char);
    }
    // append a space to the end
    buffer[buffer_index] = ' ';
    
    // array of unsorted frobnicated words
    char** frob_words =(char**) malloc(sizeof(char*));
    checkMemError(frob_words);
    int words_size = 1;
    int words_index = 0;
    // keeps track if we have finished the last word
    int finishedWord = 1;
    for(int i = 0; i < buffer_index; i++) {
        if(buffer[i] == ' ') {
            // now next non space is new word
            finishedWord = 1;
        } else if (finishedWord){
            finishedWord = 0;
            // save address of word in frob_words
            frob_words[words_index] = buffer + (i * sizeof(char));
            words_index++;
            // if frob_words is full, realloc
            if(words_index == words_size) {
                words_size *= 2;
                frob_words = realloc(frob_words, sizeof(char*) * words_size);
                checkMemError(frob_words);
            }
        }
    }

    // sort frobnicated words from buffer
    if(f)
        qsort(frob_words, words_index, sizeof(char*), &ffrobcmpWrapper);
    else
        qsort(frob_words, words_index, sizeof(char*), &frobcmpWrapper);
    
    // print frobnicated words in sorted order
    for(int i = 0; i < words_index; i++) {
        int j;
        for(j = 0; frob_words[i][j] != ' '; j++);
        if (write(1, frob_words[i], j + 1) <= 0)
            fprintf(stderr, "Error in writing\n");
    }
    
    // free memory and exit
    free(frob_words);
    free(buffer);
    exit(0);
}
