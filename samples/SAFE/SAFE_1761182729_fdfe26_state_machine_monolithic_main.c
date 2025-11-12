//DeepSeek-V3 V2.5 Category: Safe ; Style: monolithic_main ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

enum state {
    INIT,
    READ_CHAR,
    PROCESS_WORD,
    COUNT_WORD,
    FINALIZE,
    ERROR
};

int main(void) {
    enum state current_state = INIT;
    char buffer[1024];
    int buffer_index = 0;
    int word_count = 0;
    int char_count = 0;
    int in_word = 0;
    int c;
    
    while (current_state != FINALIZE && current_state != ERROR) {
        switch (current_state) {
            case INIT:
                printf("Enter text (max 1023 chars, Ctrl+D to end): ");
                buffer_index = 0;
                word_count = 0;
                char_count = 0;
                in_word = 0;
                current_state = READ_CHAR;
                break;
                
            case READ_CHAR:
                c = getchar();
                if (c == EOF) {
                    current_state = FINALIZE;
                } else if (buffer_index < 1023) {
                    buffer[buffer_index++] = (char)c;
                    char_count++;
                    current_state = PROCESS_WORD;
                } else {
                    current_state = ERROR;
                }
                break;
                
            case PROCESS_WORD:
                if (isalpha(c)) {
                    if (!in_word) {
                        in_word = 1;
                        current_state = COUNT_WORD;
                    } else {
                        current_state = READ_CHAR;
                    }
                } else if (isspace(c)) {
                    in_word = 0;
                    current_state = READ_CHAR;
                } else {
                    current_state = READ_CHAR;
                }
                break;
                
            case COUNT_WORD:
                word_count++;
                in_word = 1;
                current_state = READ_CHAR;
                break;
                
            case FINALIZE:
                buffer[buffer_index] = '\0';
                printf("\nText entered: %s\n", buffer);
                printf("Character count: %d\n", char_count);
                printf("Word count: %d\n", word_count);
                break;
                
            case ERROR:
                printf("Error: Input too long or read error\n");
                return 1;
        }
    }
    
    return 0;
}