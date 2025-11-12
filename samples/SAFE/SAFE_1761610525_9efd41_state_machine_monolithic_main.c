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
                    buffer_index = 0;
                    if (buffer_index < 255) {
                        buffer[buffer_index++] = c;
                    }
                    current_state = STATE_READING_NUMBER;
                } else if (isalpha(c)) {
                    buffer_index = 0;
                    if (buffer_index < 255) {
                        buffer[buffer_index++] = tolower(c);
                    }
                    current_state = STATE_READING_WORD;
                } else if (c != ' ' && c != '\t') {
                    current_state = STATE_ERROR;
                }
                break;
                
            case STATE_READING_NUMBER:
                if (isdigit(c)) {
                    if (buffer_index < 255) {
                        buffer[buffer_index++] = c;
                    }
                } else if (c == ' ' || c == '\t' || c == '\0') {
                    if (buffer_index > 0 && buffer_index < 256) {
                        buffer[buffer_index] = '\0';
                        long num = strtol(buffer, NULL, 10);
                        if (num >= 0 && num <= 1000000) {
                            number_count++;
                            printf("Found number: %ld\n", num);
                        }
                    }
                    buffer_index = 0;
                    current_state = (c == '\0') ? STATE_DONE : STATE_START;
                } else {
                    current_state = STATE_ERROR;
                }
                break;
                
            case STATE_READING_WORD:
                if (isalpha(c)) {
                    if (buffer_index < 255) {
                        buffer[buffer_index++] = tolower(c);
                    }
                } else if (c == ' ' || c == '\t' || c == '\0') {
                    if (buffer_index > 0 && buffer_index < 256) {
                        buffer[buffer_index] = '\0';
                        if (strlen(buffer) >= 2 && strlen(buffer) <= 20) {
                            word_count++;
                            printf("Found word: %s\n", buffer);
                        }
                    }
                    buffer_index = 0;
                    current_state = (c == '\0') ? STATE_DONE : STATE_START;
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
        if (i >= 255) break;
    }
    
    if (current_state == STATE_READING_NUMBER || current_state == STATE_READING_WORD) {
        if (buffer_index > 0 && buffer_index < 256) {
            buffer[buffer_index] = '\0';
            if (current_state == STATE_READING_NUMBER) {
                long num = strtol(buffer, NULL, 10);
                if (num >= 0 && num <= 1000000) {
                    number_count++;
                    printf("Found number: %ld\n", num);
                }
            } else {
                if (strlen(buffer) >= 2 && strlen(buffer) <= 20) {
                    word_count++;
                    printf("Found word: %s\n", buffer);
                }
            }
        }
    }
    
    if (current_state == STATE_ERROR) {
        printf("Error: Invalid input format\n");
        return 1;
    }
    
    printf("\nSummary: %d numbers, %d words found\n", number_count, word_count);
    return 0;
}