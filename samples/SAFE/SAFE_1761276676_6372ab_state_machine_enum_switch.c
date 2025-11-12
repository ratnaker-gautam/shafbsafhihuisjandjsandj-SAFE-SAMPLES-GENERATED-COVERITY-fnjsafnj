//DeepSeek-V3 V2.5 Category: Safe ; Style: enum_switch ; Variation: state_machine
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
    int ch;
    
    printf("Enter text (max 255 chars, Ctrl+D to end): ");
    
    while (current_state != FINISHED && current_state != ERROR) {
        switch (current_state) {
            case INIT:
                memset(buffer, 0, sizeof(buffer));
                buffer_index = 0;
                word_count = 0;
                current_state = READ_CHAR;
                break;
                
            case READ_CHAR:
                ch = getchar();
                if (ch == EOF) {
                    current_state = FINISHED;
                } else if (buffer_index >= 255) {
                    current_state = ERROR;
                } else if (isspace(ch)) {
                    current_state = SKIP_SPACE;
                } else if (isalpha(ch)) {
                    buffer[buffer_index++] = (char)ch;
                    current_state = PROCESS_WORD;
                } else {
                    current_state = ERROR;
                }
                break;
                
            case PROCESS_WORD:
                ch = getchar();
                if (ch == EOF) {
                    buffer[buffer_index] = '\0';
                    if (buffer_index > 0) {
                        word_count++;
                        printf("Word %d: %s\n", word_count, buffer);
                    }
                    current_state = FINISHED;
                } else if (buffer_index >= 255) {
                    current_state = ERROR;
                } else if (isspace(ch)) {
                    buffer[buffer_index] = '\0';
                    word_count++;
                    printf("Word %d: %s\n", word_count, buffer);
                    buffer_index = 0;
                    current_state = SKIP_SPACE;
                } else if (isalpha(ch)) {
                    buffer[buffer_index++] = (char)ch;
                    current_state = PROCESS_WORD;
                } else {
                    current_state = ERROR;
                }
                break;
                
            case SKIP_SPACE:
                ch = getchar();
                if (ch == EOF) {
                    current_state = FINISHED;
                } else if (isspace(ch)) {
                    current_state = SKIP_SPACE;
                } else if (isalpha(ch)) {
                    buffer[buffer_index++] = (char)ch;
                    current_state = PROCESS_WORD;
                } else {
                    current_state = ERROR;
                }
                break;
                
            case FINISHED:
                break;
                
            case ERROR:
                printf("Error: Invalid input or buffer overflow\n");
                current_state = FINISHED;
                break;
        }
    }
    
    if (word_count == 0 && current_state != ERROR) {
        printf("No words found in input.\n");
    } else if (word_count > 0) {
        printf("Total words processed: %d\n", word_count);
    }
    
    return current_state == ERROR ? EXIT_FAILURE : EXIT_SUCCESS;
}