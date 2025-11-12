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
    int i = 0;
    
    printf("Enter text (max 255 chars): ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    input[strcspn(input, "\n")] = '\0';
    
    while (input[i] != '\0' && current_state != STATE_ERROR && current_state != STATE_DONE) {
        char c = input[i];
        
        switch (current_state) {
            case STATE_START:
                if (isdigit(c)) {
                    buffer[buffer_pos++] = c;
                    current_state = STATE_READING_NUMBER;
                } else if (isalpha(c)) {
                    buffer[buffer_pos++] = c;
                    current_state = STATE_READING_WORD;
                } else if (isspace(c)) {
                    i++;
                    continue;
                } else {
                    current_state = STATE_ERROR;
                }
                break;
                
            case STATE_READING_NUMBER:
                if (isdigit(c)) {
                    if (buffer_pos < 255) {
                        buffer[buffer_pos++] = c;
                    } else {
                        current_state = STATE_ERROR;
                    }
                } else if (isspace(c) || c == '\0') {
                    buffer[buffer_pos] = '\0';
                    number_count++;
                    printf("Found number: %s\n", buffer);
                    buffer_pos = 0;
                    current_state = STATE_START;
                } else {
                    current_state = STATE_ERROR;
                }
                break;
                
            case STATE_READING_WORD:
                if (isalpha(c)) {
                    if (buffer_pos < 255) {
                        buffer[buffer_pos++] = c;
                    } else {
                        current_state = STATE_ERROR;
                    }
                } else if (isspace(c) || c == '\0') {
                    buffer[buffer_pos] = '\0';
                    word_count++;
                    printf("Found word: %s\n", buffer);
                    buffer_pos = 0;
                    current_state = STATE_START;
                } else {
                    current_state = STATE_ERROR;
                }
                break;
                
            case STATE_ERROR:
                break;
                
            case STATE_DONE:
                break;
        }
        
        i++;
        
        if (current_state == STATE_START && c == '\0') {
            current_state = STATE_DONE;
        }
    }
    
    if (current_state == STATE_READING_NUMBER || current_state == STATE_READING_WORD) {
        if (buffer_pos > 0 && buffer_pos < 256) {
            buffer[buffer_pos] = '\0';
            if (current_state == STATE_READING_NUMBER) {
                number_count++;
                printf("Found number: %s\n", buffer);
            } else {
                word_count++;
                printf("Found word: %s\n", buffer);
            }
        }
        current_state = STATE_DONE;
    }
    
    if (current_state == STATE_ERROR) {
        fprintf(stderr, "Error: Invalid input format\n");
        return 1;
    }
    
    printf("Summary: %d numbers, %d words\n", number_count, word_count);
    
    return 0;
}