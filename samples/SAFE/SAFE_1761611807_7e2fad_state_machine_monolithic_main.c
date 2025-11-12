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
    char buffer[256];
    int buffer_index = 0;
    int word_count = 0;
    int char_count = 0;
    int in_word = 0;
    int c;
    
    while (current_state != FINALIZE && current_state != ERROR) {
        switch (current_state) {
            case INIT:
                memset(buffer, 0, sizeof(buffer));
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
                } else if (buffer_index >= 255) {
                    current_state = ERROR;
                } else if (c == '\n') {
                    current_state = FINALIZE;
                } else {
                    buffer[buffer_index++] = (char)c;
                    char_count++;
                    current_state = PROCESS_WORD;
                }
                break;
                
            case PROCESS_WORD:
                if (isalpha(buffer[buffer_index - 1])) {
                    if (!in_word) {
                        in_word = 1;
                        current_state = COUNT_WORD;
                    } else {
                        current_state = READ_CHAR;
                    }
                } else {
                    in_word = 0;
                    current_state = READ_CHAR;
                }
                break;
                
            case COUNT_WORD:
                word_count++;
                in_word = 0;
                current_state = READ_CHAR;
                break;
                
            case FINALIZE:
                if (in_word) {
                    word_count++;
                }
                printf("Words: %d\n", word_count);
                printf("Characters: %d\n", char_count);
                break;
                
            case ERROR:
                fprintf(stderr, "Error: Buffer overflow\n");
                return 1;
        }
    }
    
    return 0;
}