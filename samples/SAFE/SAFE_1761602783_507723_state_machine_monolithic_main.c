//DeepSeek-V3 V2.5 Category: Safe ; Style: monolithic_main ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

enum state {
    START,
    READ_CHAR,
    COUNT_WORD,
    SKIP_SPACE,
    FINISH
};

int main(void) {
    enum state current_state = START;
    int word_count = 0;
    int char_count = 0;
    int in_word = 0;
    int c;
    
    printf("Enter text (max 1000 characters, Ctrl+D to end): ");
    
    while (current_state != FINISH) {
        c = getchar();
        
        switch (current_state) {
            case START:
                if (c == EOF) {
                    current_state = FINISH;
                } else if (isalpha(c)) {
                    char_count = 1;
                    in_word = 1;
                    current_state = READ_CHAR;
                } else if (isspace(c)) {
                    current_state = SKIP_SPACE;
                } else {
                    current_state = READ_CHAR;
                }
                break;
                
            case READ_CHAR:
                if (c == EOF) {
                    if (in_word) {
                        word_count++;
                    }
                    current_state = FINISH;
                } else if (isalpha(c)) {
                    char_count++;
                    if (char_count > 1000) {
                        printf("Input too long\n");
                        return 1;
                    }
                    in_word = 1;
                    current_state = READ_CHAR;
                } else if (isspace(c)) {
                    if (in_word) {
                        word_count++;
                        in_word = 0;
                    }
                    current_state = SKIP_SPACE;
                } else {
                    char_count++;
                    if (char_count > 1000) {
                        printf("Input too long\n");
                        return 1;
                    }
                    current_state = READ_CHAR;
                }
                break;
                
            case SKIP_SPACE:
                if (c == EOF) {
                    current_state = FINISH;
                } else if (isalpha(c)) {
                    char_count = 1;
                    in_word = 1;
                    current_state = READ_CHAR;
                } else if (!isspace(c)) {
                    char_count++;
                    if (char_count > 1000) {
                        printf("Input too long\n");
                        return 1;
                    }
                    current_state = READ_CHAR;
                }
                break;
                
            case FINISH:
                break;
                
            case COUNT_WORD:
                break;
        }
    }
    
    printf("Words: %d\n", word_count);
    printf("Characters: %d\n", char_count);
    
    return 0;
}