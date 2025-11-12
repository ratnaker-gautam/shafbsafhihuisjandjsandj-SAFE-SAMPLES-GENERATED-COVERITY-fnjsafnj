//DeepSeek-V3 V2.5 Category: Safe ; Style: monolithic_main ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

enum state {
    INIT,
    READ_CHAR,
    PROCESS_WORD,
    SKIP_SPACE,
    FINISHED,
    ERROR
};

int main(void) {
    enum state current_state = INIT;
    char buffer[256];
    int buffer_index = 0;
    int word_count = 0;
    int char_count = 0;
    int c;
    
    printf("Enter text (max 255 chars, Ctrl+D to end): ");
    
    while (current_state != FINISHED && current_state != ERROR) {
        c = getchar();
        
        switch (current_state) {
            case INIT:
                if (c == EOF) {
                    current_state = FINISHED;
                } else if (isalpha(c)) {
                    buffer_index = 0;
                    if (buffer_index < 255) {
                        buffer[buffer_index++] = (char)c;
                        char_count++;
                    }
                    current_state = READ_CHAR;
                } else if (isspace(c)) {
                    current_state = SKIP_SPACE;
                } else {
                    current_state = ERROR;
                }
                break;
                
            case READ_CHAR:
                if (c == EOF) {
                    if (buffer_index > 0 && buffer_index < 256) {
                        buffer[buffer_index] = '\0';
                        word_count++;
                        printf("Word %d: %s\n", word_count, buffer);
                    }
                    current_state = FINISHED;
                } else if (isalpha(c)) {
                    if (buffer_index < 255) {
                        buffer[buffer_index++] = (char)c;
                        char_count++;
                    }
                    current_state = READ_CHAR;
                } else if (isspace(c)) {
                    if (buffer_index > 0 && buffer_index < 256) {
                        buffer[buffer_index] = '\0';
                        word_count++;
                        printf("Word %d: %s\n", word_count, buffer);
                    }
                    current_state = SKIP_SPACE;
                } else {
                    current_state = ERROR;
                }
                break;
                
            case SKIP_SPACE:
                if (c == EOF) {
                    current_state = FINISHED;
                } else if (isalpha(c)) {
                    buffer_index = 0;
                    if (buffer_index < 255) {
                        buffer[buffer_index++] = (char)c;
                        char_count++;
                    }
                    current_state = READ_CHAR;
                } else if (isspace(c)) {
                    current_state = SKIP_SPACE;
                } else {
                    current_state = ERROR;
                }
                break;
                
            case FINISHED:
                break;
                
            case ERROR:
                printf("Error: Invalid character encountered\n");
                current_state = FINISHED;
                break;
        }
    }
    
    if (current_state == FINISHED) {
        printf("\nStatistics:\n");
        printf("Words processed: %d\n", word_count);
        printf("Characters processed: %d\n", char_count);
    }
    
    if (ferror(stdin)) {
        fprintf(stderr, "Input error occurred\n");
        return EXIT_FAILURE;
    }
    
    return EXIT_SUCCESS;
}