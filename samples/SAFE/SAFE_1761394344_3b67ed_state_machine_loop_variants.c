//DeepSeek-V3 V2.5 Category: Safe ; Style: loop_variants ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

enum State { START, READING_NUM, READING_ALPHA, ERROR, DONE };

int main(void) {
    char input[256];
    printf("Enter text (max 255 chars): ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Input error\n");
        return 1;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len-1] == '\n') {
        input[len-1] = '\0';
        len--;
    }
    
    if (len == 0) {
        printf("Empty input\n");
        return 0;
    }
    
    enum State state = START;
    int num_count = 0;
    int alpha_count = 0;
    int i = 0;
    
    while (1) {
        switch (state) {
            case START:
                if (i >= len) {
                    state = DONE;
                } else if (isdigit(input[i])) {
                    state = READING_NUM;
                    num_count++;
                } else if (isalpha(input[i])) {
                    state = READING_ALPHA;
                    alpha_count++;
                } else {
                    state = ERROR;
                }
                break;
                
            case READING_NUM:
                if (i >= len) {
                    state = DONE;
                } else if (isdigit(input[i])) {
                    i++;
                } else if (isalpha(input[i])) {
                    state = READING_ALPHA;
                    alpha_count++;
                } else {
                    state = ERROR;
                }
                break;
                
            case READING_ALPHA:
                if (i >= len) {
                    state = DONE;
                } else if (isalpha(input[i])) {
                    i++;
                } else if (isdigit(input[i])) {
                    state = READING_NUM;
                    num_count++;
                } else {
                    state = ERROR;
                }
                break;
                
            case ERROR:
                printf("Invalid character at position %d\n", i);
                return 1;
                
            case DONE:
                printf("Found %d number sequences and %d letter sequences\n", 
                       num_count, alpha_count);
                return 0;
        }
        
        if (state != READING_NUM && state != READING_ALPHA) {
            i++;
        }
        
        if (i > len) {
            state = ERROR;
        }
    }
    
    return 0;
}