//
//  tr2u.c
//
//  Created by Christopher Aziz on 11/4/17.
//
//  Laboratory: Buffered versus unbuffered I/O
//  ------------------------------------------
//  Takes two operands from and to that are
//  byte strings of the same length, and that
//  copies standard input to standard output,
//  transliterating every byte in from to the
//  corresponding byte in to.
//

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

int main(int argc, const char * argv[]) {
    
    // check number of arguments
    if(argc != 3) {
        fprintf(stderr, "Invalid number of arguments\n");
        exit(1);
    }
    
    // declare 'from' and 'to' string, and their length
    const char *from = argv[1];
    const char *to = argv[2];
    unsigned long length = strlen(to);
    
    // check 'from' and 'to' lengths are equal
    if(strlen(from) != length) {
        fprintf(stderr, "Input 'from' and 'to' must be the same length\n");
        exit(1);
    }
    
    // check for duplicates in 'from'
    for(int i = 0; i < length; i++)
        for(int j = i + 1; j < length; j++)
            if(from[j] == from[i]) {
                fprintf(stderr, "Input 'from' cannot contain duplicate bytes\n");
                exit(1);
            }
    
    // for every char from stdin, print the corresponding char to stdout,
    // if there is none, print the same char
    char cur_char[1];
    for(ssize_t read_out = read(0, cur_char, 1); read_out > 0; read_out = read(0, cur_char, 1)) {
        for(int i = 0; i < length; i++)
            if(cur_char[0] == from[i]) {
                cur_char[0] = to[i];
                break;
            }
        write(1, cur_char, 1);
    }
}
