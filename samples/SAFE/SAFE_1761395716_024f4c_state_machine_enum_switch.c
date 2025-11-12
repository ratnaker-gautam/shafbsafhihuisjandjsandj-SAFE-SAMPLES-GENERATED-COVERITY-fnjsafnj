//DeepSeek-V3 V2.5 Category: Safe ; Style: enum_switch ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

enum state {
    INIT,
    READ_CHAR,
    PROCESS_WORD,
    COUNT_WORD,
    FINISH,
    ERROR
};

int main(void) {
    enum state current_state = INIT;
    char buffer[256];
    int buffer_index = 0;
    int word_count = 0;
    int char_count = 0;
    int c;
    
    while (current_state != FINISH && current_state != ERROR) {
        switch (current_state) {
            case INIT:
                printf("Enter text (max 255 chars, Ctrl+D to end): ");
                buffer_index = 0;
                current_state = READ_CHAR;
                break;
                
            case READ_CHAR:
                c = getchar();
                if (c == EOF) {
                    current_state = FINISH;
                } else if (buffer_index < 255) {
                    buffer[buffer_index++] = (char)c;
                    if (isspace(c)) {
                        current_state = PROCESS_WORD;
                    }
                } else {
                    current_state = FINISH;
                }
                break;
                
            case PROCESS_WORD:
                if (buffer_index > 0) {
                    int i = buffer_index - 1;
                    while (i >= 0 && isspace(buffer[i])) {
                        i--;
                    }
                    if (i >= 0) {
                        current_state = COUNT_WORD;
                    } else {
                        buffer_index = 0;
                        current_state = READ_CHAR;
                    }
                } else {
                    current_state = READ_CHAR;
                }
                break;
                
            case COUNT_WORD:
                word_count++;
                char_count += buffer_index;
                buffer_index = 0;
                current_state = READ_CHAR;
                break;
                
            case FINISH:
                if (buffer_index > 0) {
                    int has_word = 0;
                    for (int i = 0; i < buffer_index; i++) {
                        if (!isspace(buffer[i])) {
                            has_word = 1;
                            break;
                        }
                    }
                    if (has_word) {
                        word_count++;
                        char_count += buffer_index;
                    }
                }
                printf("Words: %d\n", word_count);
                printf("Characters: %d\n", char_count);
                break;
                
            case ERROR:
                printf("Error occurred\n");
                return 1;
        }
    }
    
    return 0;
}