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
    int buffer_pos = 0;
    enum State current_state = STATE_START;
    int number_count = 0;
    int word_count = 0;
    int input_pos = 0;
    
    printf("Enter text (max 255 chars): ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    input[strcspn(input, "\n")] = '\0';
    
    while (input[input_pos] != '\0' && current_state != STATE_ERROR && current_state != STATE_DONE) {
        char c = input[input_pos];
        
        switch (current_state) {
            case STATE_START:
                if (isspace(c)) {
                    input_pos++;
                } else if (isdigit(c)) {
                    current_state = STATE_READING_NUMBER;
                    buffer_pos = 0;
                    if (buffer_pos < sizeof(buffer) - 1) {
                        buffer[buffer_pos++] = c;
                    }
                    input_pos++;
                } else if (isalpha(c)) {
                    current_state = STATE_READING_WORD;
                    buffer_pos = 0;
                    if (buffer_pos < sizeof(buffer) - 1) {
                        buffer[buffer_pos++] = c;
                    }
                    input_pos++;
                } else {
                    current_state = STATE_ERROR;
                }
                break;
                
            case STATE_READING_NUMBER:
                if (isdigit(c)) {
                    if (buffer_pos < sizeof(buffer) - 1) {
                        buffer[buffer_pos++] = c;
                    }
                    input_pos++;
                } else if (isspace(c) || c == '\0') {
                    buffer[buffer_pos] = '\0';
                    number_count++;
                    printf("Found number: %s\n", buffer);
                    current_state = STATE_START;
                    input_pos++;
                } else {
                    current_state = STATE_ERROR;
                }
                break;
                
            case STATE_READING_WORD:
                if (isalpha(c)) {
                    if (buffer_pos < sizeof(buffer) - 1) {
                        buffer[buffer_pos++] = c;
                    }
                    input_pos++;
                } else if (isspace(c) || c == '\0') {
                    buffer[buffer_pos] = '\0';
                    word_count++;
                    printf("Found word: %s\n", buffer);
                    current_state = STATE_START;
                    input_pos++;
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
    
    if (current_state == STATE_READING_NUMBER || current_state == STATE_READING_WORD) {
        if (buffer_pos > 0 && buffer_pos < sizeof(buffer)) {
            buffer[buffer_pos] = '\0';
            if (current_state == STATE_READING_NUMBER) {
                number_count++;
                printf("Found number: %s\n", buffer);
            } else {
                word_count++;
                printf("Found word: %s\n", buffer);
            }
        }
    }
    
    if (current_state == STATE_ERROR) {
        fprintf(stderr, "Error: Invalid input format\n");
        return 1;
    }
    
    printf("Summary: %d numbers, %d words\n", number_count, word_count);
    
    return 0;
}