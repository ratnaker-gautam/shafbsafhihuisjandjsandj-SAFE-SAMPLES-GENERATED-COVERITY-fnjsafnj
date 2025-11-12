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
    STATE_END
};

int main(void) {
    char input[256];
    char buffer[256];
    int buffer_index = 0;
    enum State current_state = STATE_START;
    int number_sum = 0;
    int word_count = 0;
    int number_count = 0;
    
    printf("Enter text (max 255 chars): ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t input_len = strlen(input);
    if (input_len > 0 && input[input_len - 1] == '\n') {
        input[input_len - 1] = '\0';
        input_len--;
    }
    
    if (input_len >= sizeof(input) - 1) {
        fprintf(stderr, "Input too long\n");
        return 1;
    }
    
    for (size_t i = 0; i <= input_len; i++) {
        char c = input[i];
        
        switch (current_state) {
            case STATE_START:
                if (c == '\0') {
                    current_state = STATE_END;
                } else if (isdigit(c)) {
                    buffer_index = 0;
                    if (buffer_index < sizeof(buffer) - 1) {
                        buffer[buffer_index++] = c;
                    }
                    current_state = STATE_READING_NUMBER;
                } else if (isalpha(c)) {
                    buffer_index = 0;
                    if (buffer_index < sizeof(buffer) - 1) {
                        buffer[buffer_index++] = tolower(c);
                    }
                    current_state = STATE_READING_WORD;
                } else if (c != ' ' && c != '\t') {
                    current_state = STATE_ERROR;
                }
                break;
                
            case STATE_READING_NUMBER:
                if (c == '\0' || c == ' ' || c == '\t') {
                    buffer[buffer_index] = '\0';
                    if (buffer_index > 0) {
                        int num = atoi(buffer);
                        number_sum += num;
                        number_count++;
                    }
                    buffer_index = 0;
                    current_state = (c == '\0') ? STATE_END : STATE_START;
                } else if (isdigit(c)) {
                    if (buffer_index < sizeof(buffer) - 1) {
                        buffer[buffer_index++] = c;
                    } else {
                        current_state = STATE_ERROR;
                    }
                } else {
                    current_state = STATE_ERROR;
                }
                break;
                
            case STATE_READING_WORD:
                if (c == '\0' || c == ' ' || c == '\t') {
                    buffer[buffer_index] = '\0';
                    if (buffer_index > 0) {
                        word_count++;
                    }
                    buffer_index = 0;
                    current_state = (c == '\0') ? STATE_END : STATE_START;
                } else if (isalpha(c)) {
                    if (buffer_index < sizeof(buffer) - 1) {
                        buffer[buffer_index++] = tolower(c);
                    } else {
                        current_state = STATE_ERROR;
                    }
                } else {
                    current_state = STATE_ERROR;
                }
                break;
                
            case STATE_ERROR:
                printf("Invalid input format\n");
                return 1;
                
            case STATE_END:
                break;
        }
        
        if (current_state == STATE_ERROR) {
            printf("Invalid input format\n");
            return 1;
        }
    }
    
    printf("Words: %d\n", word_count);
    printf("Numbers: %d\n", number_count);
    if (number_count > 0) {
        printf("Number sum: %d\n", number_sum);
    }
    
    return 0;
}