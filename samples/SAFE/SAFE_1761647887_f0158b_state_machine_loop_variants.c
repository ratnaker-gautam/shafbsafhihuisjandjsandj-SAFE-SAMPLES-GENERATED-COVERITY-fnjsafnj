//DeepSeek-V3 V2.5 Category: Safe ; Style: loop_variants ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <ctype.h>

enum state {
    START,
    READ_CHAR,
    COUNT_WORDS,
    COUNT_LINES,
    FINISH,
    ERROR
};

int main(void) {
    enum state current_state = START;
    int word_count = 0;
    int line_count = 0;
    int char_count = 0;
    int in_word = 0;
    int c;
    
    while (current_state != FINISH && current_state != ERROR) {
        switch (current_state) {
            case START:
                current_state = READ_CHAR;
                break;
                
            case READ_CHAR:
                c = getchar();
                if (c == EOF) {
                    current_state = FINISH;
                } else if (c == '\n') {
                    char_count++;
                    line_count++;
                    if (in_word) {
                        in_word = 0;
                    }
                    current_state = COUNT_LINES;
                } else if (isspace(c)) {
                    char_count++;
                    if (in_word) {
                        in_word = 0;
                    }
                    current_state = READ_CHAR;
                } else if (isprint(c)) {
                    char_count++;
                    if (!in_word) {
                        in_word = 1;
                        word_count++;
                    }
                    current_state = COUNT_WORDS;
                } else {
                    current_state = ERROR;
                }
                break;
                
            case COUNT_WORDS:
                current_state = READ_CHAR;
                break;
                
            case COUNT_LINES:
                current_state = READ_CHAR;
                break;
                
            case FINISH:
                break;
                
            case ERROR:
                fprintf(stderr, "Error: Invalid character encountered\n");
                return 1;
        }
    }
    
    if (current_state == FINISH) {
        printf("Characters: %d\n", char_count);
        printf("Words: %d\n", word_count);
        printf("Lines: %d\n", line_count);
    }
    
    return 0;
}