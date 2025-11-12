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
    FINISH,
    ERROR
};

int main(void) {
    enum state current_state = START;
    char buffer[256];
    int buffer_index = 0;
    int word_count = 0;
    int ch;
    
    printf("Enter text (max 255 chars): ");
    
    while (current_state != FINISH && current_state != ERROR) {
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
                    current_state = FINISH;
                } else if (buffer_index < 255) {
                    buffer[buffer_index++] = (char)ch;
                    if (isspace(ch)) {
                        current_state = SKIP_SPACE;
                    } else {
                        current_state = PROCESS_WORD;
                    }
                } else {
                    current_state = ERROR;
                }
                break;
                
            case PROCESS_WORD:
                word_count++;
                while ((ch = getchar()) != EOF && ch != '\n') {
                    if (buffer_index < 255) {
                        buffer[buffer_index++] = (char)ch;
                        if (isspace(ch)) {
                            current_state = SKIP_SPACE;
                            break;
                        }
                    } else {
                        current_state = ERROR;
                        break;
                    }
                }
                if (ch == EOF || ch == '\n') {
                    current_state = FINISH;
                }
                break;
                
            case SKIP_SPACE:
                ch = getchar();
                if (ch == EOF || ch == '\n') {
                    current_state = FINISH;
                } else if (buffer_index < 255) {
                    buffer[buffer_index++] = (char)ch;
                    if (!isspace(ch)) {
                        current_state = PROCESS_WORD;
                    }
                } else {
                    current_state = ERROR;
                }
                break;
                
            case FINISH:
                if (buffer_index > 0 && !isspace(buffer[buffer_index-1])) {
                    word_count++;
                }
                printf("Input: %s\n", buffer);
                printf("Word count: %d\n", word_count);
                break;
                
            case ERROR:
                printf("Error: Input too long\n");
                break;
        }
    }
    
    return 0;
}