//DeepSeek-V3 V2.5 Category: Safe ; Style: monolithic_main ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

enum state {
    START,
    READING_NUMBER,
    READING_WORD,
    ERROR,
    DONE
};

int main(void) {
    char input[256];
    char buffer[256];
    int buf_index = 0;
    enum state current_state = START;
    int number_sum = 0;
    int word_count = 0;
    int i = 0;
    
    printf("Enter text (max 255 chars): ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    input[strcspn(input, "\n")] = '\0';
    
    while (current_state != DONE && current_state != ERROR) {
        char c = input[i];
        
        switch (current_state) {
            case START:
                if (c == '\0') {
                    current_state = DONE;
                } else if (isdigit(c)) {
                    buffer[buf_index++] = c;
                    current_state = READING_NUMBER;
                } else if (isalpha(c)) {
                    buffer[buf_index++] = c;
                    current_state = READING_WORD;
                } else if (isspace(c)) {
                    i++;
                } else {
                    current_state = ERROR;
                }
                break;
                
            case READING_NUMBER:
                if (c == '\0') {
                    buffer[buf_index] = '\0';
                    number_sum += atoi(buffer);
                    buf_index = 0;
                    current_state = DONE;
                } else if (isdigit(c)) {
                    if (buf_index < 255) {
                        buffer[buf_index++] = c;
                    } else {
                        current_state = ERROR;
                    }
                    i++;
                } else if (isspace(c)) {
                    buffer[buf_index] = '\0';
                    number_sum += atoi(buffer);
                    buf_index = 0;
                    current_state = START;
                    i++;
                } else {
                    current_state = ERROR;
                }
                break;
                
            case READING_WORD:
                if (c == '\0') {
                    buffer[buf_index] = '\0';
                    if (buf_index > 0) {
                        word_count++;
                    }
                    buf_index = 0;
                    current_state = DONE;
                } else if (isalpha(c)) {
                    if (buf_index < 255) {
                        buffer[buf_index++] = c;
                    } else {
                        current_state = ERROR;
                    }
                    i++;
                } else if (isspace(c)) {
                    buffer[buf_index] = '\0';
                    if (buf_index > 0) {
                        word_count++;
                    }
                    buf_index = 0;
                    current_state = START;
                    i++;
                } else {
                    current_state = ERROR;
                }
                break;
                
            case ERROR:
                fprintf(stderr, "Invalid input format\n");
                return 1;
                
            case DONE:
                break;
        }
    }
    
    if (current_state == ERROR) {
        fprintf(stderr, "Invalid input format\n");
        return 1;
    }
    
    printf("Sum of numbers: %d\n", number_sum);
    printf("Word count: %d\n", word_count);
    
    return 0;
}