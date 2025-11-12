//DeepSeek-V3 V2.5 Category: Safe ; Style: monolithic_main ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

enum State {
    STATE_START,
    STATE_READING_NUMBER,
    STATE_READING_WORD,
    STATE_ERROR,
    STATE_DONE
};

int main(void) {
    char input[256];
    char buffer[256];
    int buffer_index = 0;
    enum State current_state = STATE_START;
    int number_sum = 0;
    int word_count = 0;
    int number_count = 0;
    int i = 0;
    
    printf("Enter text (max 255 chars): ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        printf("Error reading input\n");
        return 1;
    }
    
    input[strcspn(input, "\n")] = '\0';
    
    while (input[i] != '\0' && current_state != STATE_ERROR && current_state != STATE_DONE) {
        char c = input[i];
        
        switch (current_state) {
            case STATE_START:
                if (isspace(c)) {
                    i++;
                } else if (isdigit(c)) {
                    current_state = STATE_READING_NUMBER;
                    buffer_index = 0;
                    if (buffer_index < 255) {
                        buffer[buffer_index++] = c;
                    }
                    i++;
                } else if (isalpha(c)) {
                    current_state = STATE_READING_WORD;
                    buffer_index = 0;
                    if (buffer_index < 255) {
                        buffer[buffer_index++] = c;
                    }
                    i++;
                } else {
                    current_state = STATE_ERROR;
                }
                break;
                
            case STATE_READING_NUMBER:
                if (isdigit(c)) {
                    if (buffer_index < 255) {
                        buffer[buffer_index++] = c;
                    }
                    i++;
                } else if (isspace(c) || c == '\0') {
                    buffer[buffer_index] = '\0';
                    int num = atoi(buffer);
                    number_sum += num;
                    number_count++;
                    current_state = STATE_START;
                    i++;
                } else {
                    current_state = STATE_ERROR;
                }
                break;
                
            case STATE_READING_WORD:
                if (isalpha(c)) {
                    if (buffer_index < 255) {
                        buffer[buffer_index++] = c;
                    }
                    i++;
                } else if (isspace(c) || c == '\0') {
                    buffer[buffer_index] = '\0';
                    word_count++;
                    current_state = STATE_START;
                    i++;
                } else {
                    current_state = STATE_ERROR;
                }
                break;
                
            case STATE_ERROR:
                break;
                
            case STATE_DONE:
                break;
        }
    }
    
    if (current_state == STATE_READING_NUMBER) {
        buffer[buffer_index] = '\0';
        int num = atoi(buffer);
        number_sum += num;
        number_count++;
        current_state = STATE_DONE;
    } else if (current_state == STATE_READING_WORD) {
        buffer[buffer_index] = '\0';
        word_count++;
        current_state = STATE_DONE;
    } else if (current_state == STATE_START) {
        current_state = STATE_DONE;
    }
    
    if (current_state == STATE_ERROR) {
        printf("Error: Invalid input format\n");
        return 1;
    }
    
    printf("Words found: %d\n", word_count);
    printf("Numbers found: %d\n", number_count);
    if (number_count > 0) {
        printf("Sum of numbers: %d\n", number_sum);
        printf("Average of numbers: %.2f\n", (double)number_sum / number_count);
    } else {
        printf("No numbers to calculate average\n");
    }
    
    return 0;
}