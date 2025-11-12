//DeepSeek-V3 V2.5 Category: Safe ; Style: enum_switch ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

enum state {
    START,
    READ_CHAR,
    PROCESS_WORD,
    SKIP_SPACE,
    FINISHED,
    ERROR
};

int main(void) {
    enum state current_state = START;
    char buffer[256];
    int buffer_index = 0;
    int word_count = 0;
    int ch;
    
    printf("Enter text (max 255 chars): ");
    
    while (current_state != FINISHED && current_state != ERROR) {
        switch (current_state) {
            case START:
                buffer_index = 0;
                word_count = 0;
                memset(buffer, 0, sizeof(buffer));
                current_state = READ_CHAR;
                break;
                
            case READ_CHAR:
                ch = getchar();
                if (ch == EOF || ch == '\n') {
                    current_state = FINISHED;
                } else if (buffer_index >= 255) {
                    current_state = ERROR;
                } else if (isspace(ch)) {
                    current_state = SKIP_SPACE;
                } else {
                    buffer[buffer_index++] = (char)ch;
                    current_state = PROCESS_WORD;
                }
                break;
                
            case PROCESS_WORD:
                ch = getchar();
                if (ch == EOF || ch == '\n') {
                    word_count++;
                    current_state = FINISHED;
                } else if (buffer_index >= 255) {
                    current_state = ERROR;
                } else if (isspace(ch)) {
                    word_count++;
                    current_state = SKIP_SPACE;
                } else {
                    buffer[buffer_index++] = (char)ch;
                }
                break;
                
            case SKIP_SPACE:
                ch = getchar();
                if (ch == EOF || ch == '\n') {
                    current_state = FINISHED;
                } else if (!isspace(ch)) {
                    if (buffer_index >= 255) {
                        current_state = ERROR;
                    } else {
                        buffer[buffer_index++] = (char)ch;
                        current_state = PROCESS_WORD;
                    }
                }
                break;
                
            case FINISHED:
                break;
                
            case ERROR:
                break;
        }
    }
    
    if (current_state == ERROR) {
        printf("Error: Input too long\n");
        return 1;
    }
    
    buffer[buffer_index] = '\0';
    printf("Processed text: %s\n", buffer);
    printf("Word count: %d\n", word_count);
    
    return 0;
}