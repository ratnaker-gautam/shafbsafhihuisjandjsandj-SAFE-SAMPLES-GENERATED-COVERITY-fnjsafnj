//DeepSeek-V3 V2.5 Category: Safe ; Style: loop_variants ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

enum State { START, READING_NUMBER, READING_WORD, ERROR, DONE };

struct Counter {
    int numbers;
    int words;
};

int is_valid_char(char c) {
    return isalnum(c) || c == ' ' || c == '\t' || c == '\n';
}

int main(void) {
    struct Counter counter = {0, 0};
    enum State current_state = START;
    char buffer[1024];
    size_t pos = 0;
    int c;
    
    while (current_state != DONE && current_state != ERROR) {
        c = getchar();
        
        if (c == EOF) {
            switch (current_state) {
                case START:
                    current_state = DONE;
                    break;
                case READING_NUMBER:
                    counter.numbers++;
                    current_state = DONE;
                    break;
                case READING_WORD:
                    counter.words++;
                    current_state = DONE;
                    break;
                default:
                    current_state = DONE;
                    break;
            }
            continue;
        }
        
        if (!is_valid_char(c)) {
            current_state = ERROR;
            continue;
        }
        
        switch (current_state) {
            case START:
                if (isdigit(c)) {
                    current_state = READING_NUMBER;
                    buffer[pos++] = (char)c;
                } else if (isalpha(c)) {
                    current_state = READING_WORD;
                    buffer[pos++] = (char)c;
                } else if (c == ' ' || c == '\t' || c == '\n') {
                    current_state = START;
                } else {
                    current_state = ERROR;
                }
                break;
                
            case READING_NUMBER:
                if (isdigit(c)) {
                    if (pos < sizeof(buffer) - 1) {
                        buffer[pos++] = (char)c;
                    } else {
                        current_state = ERROR;
                    }
                } else if (c == ' ' || c == '\t' || c == '\n') {
                    buffer[pos] = '\0';
                    counter.numbers++;
                    pos = 0;
                    current_state = START;
                } else if (isalpha(c)) {
                    current_state = ERROR;
                } else {
                    current_state = ERROR;
                }
                break;
                
            case READING_WORD:
                if (isalnum(c)) {
                    if (pos < sizeof(buffer) - 1) {
                        buffer[pos++] = (char)c;
                    } else {
                        current_state = ERROR;
                    }
                } else if (c == ' ' || c == '\t' || c == '\n') {
                    buffer[pos] = '\0';
                    counter.words++;
                    pos = 0;
                    current_state = START;
                } else {
                    current_state = ERROR;
                }
                break;
                
            case ERROR:
                break;
                
            case DONE:
                break;
        }
    }
    
    if (current_state == ERROR) {
        fprintf(stderr, "Error: Invalid input encountered\n");
        return EXIT_FAILURE;
    }
    
    printf("Numbers: %d\n", counter.numbers);
    printf("Words: %d\n", counter.words);
    
    return EXIT_SUCCESS;
}