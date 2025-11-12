//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: monolithic_main ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

enum State { START, READING_NUMBER, READING_WORD, ERROR, DONE };

int main(void) {
    enum State current_state = START;
    char input[256];
    char buffer[256];
    int buf_index = 0;
    int number_count = 0;
    int word_count = 0;
    
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
            case START:
                if (c == '\0') {
                    current_state = DONE;
                } else if (isdigit((unsigned char)c)) {
                    buffer[buf_index++] = c;
                    current_state = READING_NUMBER;
                } else if (isalpha((unsigned char)c)) {
                    buffer[buf_index++] = c;
                    current_state = READING_WORD;
                } else if (isspace((unsigned char)c)) {
                    continue;
                } else {
                    current_state = ERROR;
                }
                break;
                
            case READING_NUMBER:
                if (c == '\0' || isspace((unsigned char)c)) {
                    if (buf_index > 0) {
                        buffer[buf_index] = '\0';
                        number_count++;
                        printf("Found number: %s\n", buffer);
                        buf_index = 0;
                    }
                    current_state = (c == '\0') ? DONE : START;
                } else if (isdigit((unsigned char)c)) {
                    if (buf_index < sizeof(buffer) - 1) {
                        buffer[buf_index++] = c;
                    } else {
                        current_state = ERROR;
                    }
                } else {
                    current_state = ERROR;
                }
                break;
                
            case READING_WORD:
                if (c == '\0' || isspace((unsigned char)c)) {
                    if (buf_index > 0) {
                        buffer[buf_index] = '\0';
                        word_count++;
                        printf("Found word: %s\n", buffer);
                        buf_index = 0;
                    }
                    current_state = (c == '\0') ? DONE : START;
                } else if (isalpha((unsigned char)c)) {
                    if (buf_index < sizeof(buffer) - 1) {
                        buffer[buf_index++] = c;
                    } else {
                        current_state = ERROR;
                    }
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
        
        if (current_state == ERROR) {
            fprintf(stderr, "Invalid input format\n");
            return 1;
        }
    }
    
    printf("Summary: %d numbers, %d words\n", number_count, word_count);
    return 0;
}