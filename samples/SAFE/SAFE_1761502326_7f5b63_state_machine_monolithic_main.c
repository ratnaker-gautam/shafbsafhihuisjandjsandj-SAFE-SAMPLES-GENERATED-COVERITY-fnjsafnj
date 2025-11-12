//DeepSeek-V3 V2.5 Category: Safe ; Style: monolithic_main ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

enum State {
    START,
    READING_NUMBER,
    READING_WORD,
    ERROR,
    DONE
};

int main(void) {
    char input[256];
    char buffer[256];
    int buffer_pos = 0;
    enum State current_state = START;
    int number_sum = 0;
    int word_count = 0;
    int i = 0;
    
    printf("Enter text (max 255 chars): ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    input[strcspn(input, "\n")] = '\0';
    
    while (current_state != DONE && current_state != ERROR && i < 255) {
        char c = input[i];
        
        switch (current_state) {
            case START:
                if (c == '\0') {
                    current_state = DONE;
                } else if (isdigit(c)) {
                    buffer[buffer_pos++] = c;
                    current_state = READING_NUMBER;
                } else if (isalpha(c)) {
                    buffer[buffer_pos++] = c;
                    current_state = READING_WORD;
                } else if (isspace(c)) {
                } else {
                    current_state = ERROR;
                }
                break;
                
            case READING_NUMBER:
                if (c == '\0') {
                    buffer[buffer_pos] = '\0';
                    int num = atoi(buffer);
                    if (num > 0) {
                        number_sum += num;
                    }
                    buffer_pos = 0;
                    current_state = DONE;
                } else if (isdigit(c)) {
                    if (buffer_pos < 255) {
                        buffer[buffer_pos++] = c;
                    } else {
                        current_state = ERROR;
                    }
                } else if (isspace(c)) {
                    buffer[buffer_pos] = '\0';
                    int num = atoi(buffer);
                    if (num > 0) {
                        number_sum += num;
                    }
                    buffer_pos = 0;
                    current_state = START;
                } else {
                    current_state = ERROR;
                }
                break;
                
            case READING_WORD:
                if (c == '\0') {
                    buffer[buffer_pos] = '\0';
                    if (strlen(buffer) > 0) {
                        word_count++;
                    }
                    buffer_pos = 0;
                    current_state = DONE;
                } else if (isalpha(c)) {
                    if (buffer_pos < 255) {
                        buffer[buffer_pos++] = c;
                    } else {
                        current_state = ERROR;
                    }
                } else if (isspace(c)) {
                    buffer[buffer_pos] = '\0';
                    if (strlen(buffer) > 0) {
                        word_count++;
                    }
                    buffer_pos = 0;
                    current_state = START;
                } else {
                    current_state = ERROR;
                }
                break;
                
            case ERROR:
                printf("Invalid input format\n");
                return 1;
                
            case DONE:
                break;
        }
        
        i++;
    }
    
    if (current_state == READING_NUMBER) {
        buffer[buffer_pos] = '\0';
        int num = atoi(buffer);
        if (num > 0) {
            number_sum += num;
        }
    } else if (current_state == READING_WORD) {
        buffer[buffer_pos] = '\0';
        if (strlen(buffer) > 0) {
            word_count++;
        }
    }
    
    printf("Sum of numbers: %d\n", number_sum);
    printf("Word count: %d\n", word_count);
    
    return 0;
}