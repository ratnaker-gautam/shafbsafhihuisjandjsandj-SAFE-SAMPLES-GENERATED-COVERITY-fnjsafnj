//DeepSeek-V3 V2.5 Category: Safe ; Style: enum_switch ; Variation: state_machine
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <ctype.h>
#include <limits.h>

enum state {
    START,
    READING_NUMBER,
    READING_WORD,
    SKIPPING_SPACE,
    FINISHED,
    ERROR
};

int main(void) {
    enum state current_state = START;
    char input[256];
    char buffer[256];
    size_t buf_index = 0;
    int number_sum = 0;
    int word_count = 0;
    int number_value = 0;
    int is_negative = 0;
    
    printf("Enter text (max 255 chars): ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        return 1;
    }
    
    size_t input_len = strlen(input);
    if (input_len > 0 && input[input_len - 1] == '\n') {
        input[input_len - 1] = '\0';
        input_len--;
    }
    
    for (size_t i = 0; i <= input_len; i++) {
        char c = input[i];
        
        switch (current_state) {
            case START:
                if (c == '\0') {
                    current_state = FINISHED;
                } else if (isdigit((unsigned char)c)) {
                    number_value = c - '0';
                    is_negative = 0;
                    buf_index = 0;
                    current_state = READING_NUMBER;
                } else if (isalpha((unsigned char)c)) {
                    if (buf_index < sizeof(buffer) - 1) {
                        buffer[buf_index++] = c;
                    }
                    current_state = READING_WORD;
                } else if (isspace((unsigned char)c)) {
                    current_state = SKIPPING_SPACE;
                } else if (c == '-') {
                    is_negative = 1;
                    buf_index = 0;
                    current_state = READING_NUMBER;
                } else {
                    current_state = ERROR;
                }
                break;
                
            case READING_NUMBER:
                if (isdigit((unsigned char)c)) {
                    int digit = c - '0';
                    if (number_value > (INT_MAX - digit) / 10) {
                        current_state = ERROR;
                        break;
                    }
                    number_value = number_value * 10 + digit;
                } else if (isspace((unsigned char)c) || c == '\0') {
                    if (is_negative) {
                        number_value = -number_value;
                    }
                    number_sum += number_value;
                    current_state = (c == '\0') ? FINISHED : SKIPPING_SPACE;
                } else {
                    current_state = ERROR;
                }
                break;
                
            case READING_WORD:
                if (isalpha((unsigned char)c)) {
                    if (buf_index < sizeof(buffer) - 1) {
                        buffer[buf_index++] = c;
                    }
                } else if (isspace((unsigned char)c) || c == '\0') {
                    if (buf_index > 0 && buf_index < sizeof(buffer)) {
                        buffer[buf_index] = '\0';
                        word_count++;
                    }
                    buf_index = 0;
                    current_state = (c == '\0') ? FINISHED : SKIPPING_SPACE;
                } else {
                    current_state = ERROR;
                }
                break;
                
            case SKIPPING_SPACE:
                if (c == '\0') {
                    current_state = FINISHED;
                } else if (!isspace((unsigned char)c)) {
                    if (isdigit((unsigned char)c)) {
                        number_value = c - '0';
                        is_negative = 0;
                        buf_index = 0;
                        current_state = READING_NUMBER;
                    } else if (isalpha((unsigned char)c)) {
                        if (buf_index < sizeof(buffer) - 1) {
                            buffer[buf_index++] = c;
                        }
                        current_state = READING_WORD;
                    } else if (c == '-') {
                        is_negative = 1;
                        buf_index = 0;
                        current_state = READING_NUMBER;
                    } else {
                        current_state = ERROR;
                    }
                }
                break;
                
            case FINISHED:
                break;
                
            case ERROR:
                printf("Error: Invalid input format\n");
                return 1;
        }
        
        if (current_state == ERROR || current_state == FINISHED) {
            break;
        }
    }
    
    if (current_state != FINISHED && current_state != ERROR) {
        printf("Error: Incomplete processing\n");
        return 1;
    }
    
    printf("Sum of numbers: %d\n", number_sum);
    printf("Word count: %d\n", word_count);
    
    return 0;
}