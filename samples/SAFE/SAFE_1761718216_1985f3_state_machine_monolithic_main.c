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
        switch (current_state) {
            case INIT:
                buffer_index = 0;
                word_count = 0;
                char_count = 0;
                memset(buffer, 0, sizeof(buffer));
                current_state = READ_CHAR;
                break;
                
            case READ_CHAR:
                c = getchar();
                
                if (c == EOF) {
                    current_state = FINISHED;
                } else if (buffer_index < 255) {
                    buffer[buffer_index++] = (char)c;
                    char_count++;
                    
                    if (isspace(c)) {
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
                while ((c = getchar()) != EOF) {
                    if (buffer_index < 255) {
                        buffer[buffer_index++] = (char)c;
                        char_count++;
                        
                        if (isspace(c)) {
                            current_state = SKIP_SPACE;
                            break;
                        }
                    } else {
                        current_state = ERROR;
                        break;
                    }
                }
                if (c == EOF) {
                    current_state = FINISHED;
                }
                break;
                
            case SKIP_SPACE:
                while ((c = getchar()) != EOF) {
                    if (buffer_index < 255) {
                        buffer[buffer_index++] = (char)c;
                        char_count++;
                        
                        if (!isspace(c)) {
                            current_state = PROCESS_WORD;
                            break;
                        }
                    } else {
                        current_state = ERROR;
                        break;
                    }
                }
                if (c == EOF) {
                    current_state = FINISHED;
                }
                break;
                
            case FINISHED:
                break;
                
            case ERROR:
                printf("Error: Input too long\n");
                return 1;
        }
    }
    
    if (current_state == FINISHED) {
        printf("\nInput processed successfully.\n");
        printf("Characters read: %d\n", char_count);
        printf("Words counted: %d\n", word_count);
        printf("Buffer content: %s\n", buffer);
    }
    
    return 0;
}