//DeepSeek-V3 V2.5 Category: Safe ; Style: monolithic_main ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

enum State {
    INIT,
    READING_NUMBER,
    READING_WORD,
    ERROR,
    DONE
};

int main(void) {
    char input[256];
    char buffer[256];
    int buffer_pos = 0;
    enum State state = INIT;
    int number_sum = 0;
    int word_count = 0;
    int i = 0;
    
    printf("Enter text (max 255 chars): ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    input[strcspn(input, "\n")] = '\0';
    
    while (input[i] != '\0' && state != ERROR && state != DONE) {
        char c = input[i];
        
        switch (state) {
            case INIT:
                if (isspace(c)) {
                    i++;
                } else if (isdigit(c)) {
                    state = READING_NUMBER;
                    buffer_pos = 0;
                    buffer[buffer_pos++] = c;
                    i++;
                } else if (isalpha(c)) {
                    state = READING_WORD;
                    buffer_pos = 0;
                    buffer[buffer_pos++] = c;
                    i++;
                } else {
                    state = ERROR;
                }
                break;
                
            case READING_NUMBER:
                if (isdigit(c)) {
                    if (buffer_pos < sizeof(buffer) - 1) {
                        buffer[buffer_pos++] = c;
                        i++;
                    } else {
                        state = ERROR;
                    }
                } else if (isspace(c) || c == '\0') {
                    buffer[buffer_pos] = '\0';
                    int num = atoi(buffer);
                    number_sum += num;
                    state = INIT;
                    i++;
                } else {
                    state = ERROR;
                }
                break;
                
            case READING_WORD:
                if (isalpha(c)) {
                    if (buffer_pos < sizeof(buffer) - 1) {
                        buffer[buffer_pos++] = c;
                        i++;
                    } else {
                        state = ERROR;
                    }
                } else if (isspace(c) || c == '\0') {
                    buffer[buffer_pos] = '\0';
                    word_count++;
                    state = INIT;
                    i++;
                } else {
                    state = ERROR;
                }
                break;
                
            case ERROR:
                break;
                
            case DONE:
                break;
        }
    }
    
    if (state == READING_NUMBER) {
        buffer[buffer_pos] = '\0';
        int num = atoi(buffer);
        number_sum += num;
        state = DONE;
    } else if (state == READING_WORD) {
        word_count++;
        state = DONE;
    }
    
    if (state == ERROR) {
        printf("Error: Invalid input format\n");
        return 1;
    }
    
    printf("Number sum: %d\n", number_sum);
    printf("Word count: %d\n", word_count);
    
    return 0;
}