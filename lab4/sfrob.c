//
//  sfrob.c
//
//  Christopher Aziz
//  UID 304806012
//  Oct. 27, 2017
//
//  Homework: Sorting encrypted text
//  --------------------------------
//  Takes frobnicated words from
//  stdin, sorts them, and prints
//  them to stdout without ever
//  storing unfrobnicated  words in
//  memory.
//

#include <stdio.h>
#include <stdlib.h>

void checkIOError() {
    if(ferror(stdin)) {
        fprintf(stderr, "Error reading from stdin");
        exit(1);
    }
}

void checkMemError(void* ptr) {
    if(ptr == NULL) {
        fprintf(stderr, "Error in memory allocation");
        exit(1);
    }
}

// compare frobnicated c strings that end in a space
int frobcmp(char const *a, char const *b)
{
    // compare words char by char or until space is reached
    while(1) {
        if(*a == ' ' && *b == ' ') return 0;
        if((*a ^ 42) > (*b ^ 42) || *b == ' ') return 1;
        if((*b ^ 42) > (*a ^ 42) || *a == ' ') return -1;
        a++;
        b++;
    }
}

// wrapper function for qsort that takes pointers to c strings
int frobcmpWrapper(const void* a_ptr, const void* b_ptr) {
    const char* a = *(const char**)a_ptr;
    const char* b = *(const char**)b_ptr;
    return frobcmp(a,b);
}

int main(int argc, const char * argv[]) {
    // buffer to build each word
    char* buffer = (char*) malloc(sizeof(char));
    int buffer_size = 1;
    int buffer_index = 0;
    
    // array of unsorted frobnicated words
    char** frob_words = (char**) malloc(sizeof(char*));;
    int words_size = 1;
    int words_index = 0;
    
    // current character
    char cur_char;
    // boolean if reached EOF
    int end_of_file = 0;
    
    // continuously get characters until EOF
    while(!end_of_file) {
        cur_char = getchar();
        checkIOError();

        // if EOF, append a space to the last word
        if(cur_char == EOF) {
            end_of_file = 1;
            cur_char = ' ';
        }
        
        // if buffer is too small, double buffer size
        if(buffer_size <= buffer_index) {
            buffer_size *= 2;
            buffer = (char*)
                realloc(buffer, sizeof(char) * buffer_size);
            checkMemError(buffer);
        }
        
        // add current char to buffer
        buffer[buffer_index++] = cur_char;
        
        // if the character was a space,
        // add buffer to frob_words and reset buffer
        if(cur_char == ' '){
            // if frob_words is too small, double frob_words size
            if(words_size <= words_index) {
                words_size *= 2;
                frob_words = (char**)
                    realloc(frob_words, sizeof(char*) * words_size);
                checkMemError(frob_words);
            }
            // add word from buffer to frob_words
            frob_words[words_index++] = buffer;
            
            // reset buffer for next word
            buffer = (char*) malloc(sizeof(char));
            checkMemError(buffer);
            buffer_size = 1;
            buffer_index = 0;
        }
    }
    
    // sort frobnicated words from buffer
    qsort(frob_words, words_index, sizeof(char*), &frobcmpWrapper);
    
    // print frobnicated words in unfrobnicated sorted order
    for(int i = 0; i < words_index; i++) {
        for(int j = 0;;j++) {
            putchar(frob_words[i][j]);
            checkIOError();
            if(frob_words[i][j] == ' ') break;
        }
    }
    
    // free memory and exit
    free(buffer);
    for(int i = 0; i < words_index; i++)
        free(frob_words[i]);
    free(frob_words);
    exit(0);
}
