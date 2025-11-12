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
    int buffer_pos = 0;
    int word_count = 0;
    int char_count = 0;
    int in_word = 0;
    int c;
    
    while (current_state != FINALIZE && current_state != ERROR) {
        switch (current_state) {
            case INIT:
                printf("Enter text (max 1023 chars, Ctrl+D to end): ");
                buffer_pos = 0;
                current_state = READ_CHAR;
                break;
                
            case READ_CHAR:
                c = getchar();
                if (c == EOF) {
                    current_state = FINALIZE;
                } else if (buffer_pos < 1023) {
                    buffer[buffer_pos++] = (char)c;
                    current_state = PROCESS_WORD;
                } else {
                    current_state = ERROR;
                }
                break;
                
            case PROCESS_WORD:
                if (buffer_pos > 0) {
                    char last_char = buffer[buffer_pos - 1];
                    if (isalpha(last_char)) {
                        if (!in_word) {
                            in_word = 1;
                            word_count++;
                        }
                    } else if (isspace(last_char)) {
                        in_word = 0;
                    }
                    char_count++;
                    current_state = READ_CHAR;
                } else {
                    current_state = READ_CHAR;
                }
                break;
                
            case FINALIZE:
                if (buffer_pos > 0) {
                    buffer[buffer_pos] = '\0';
                    printf("\nInput text: %s\n", buffer);
                    printf("Character count: %d\n", char_count);
                    printf("Word count: %d\n", word_count);
                } else {
                    printf("\nNo input received.\n");
                }
                current_state = FINALIZE;
                break;
                
            case ERROR:
                printf("Error: Input too long or read error.\n");
                return 1;
                break;
                
            default:
                current_state = ERROR;
                break;
        }
    }
    
    return 0;
}