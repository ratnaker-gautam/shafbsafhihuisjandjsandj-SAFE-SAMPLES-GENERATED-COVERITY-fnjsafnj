//DeepSeek-V3 V2.5 Category: Safe ; Style: enum_switch ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

enum state {
    STATE_START,
    STATE_READING_WORD,
    STATE_READING_NUMBER,
    STATE_ERROR,
    STATE_END
};

int main(void) {
    char input[256];
    char buffer[256];
    int buf_index = 0;
    enum state current_state = STATE_START;
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
    
    if (input_len >= sizeof(input)) {
        fprintf(stderr, "Input too long\n");
        return 1;
    }
    
    for (size_t i = 0; i <= input_len; i++) {
        char c = input[i];
        
        switch (current_state) {
            case STATE_START:
                if (c == '\0') {
                    current_state = STATE_END;
                } else if (isalpha((unsigned char)c)) {
                    if (buf_index < sizeof(buffer) - 1) {
                        buffer[buf_index++] = c;
                    }
                    current_state = STATE_READING_WORD;
                } else if (isdigit((unsigned char)c)) {
                    if (buf_index < sizeof(buffer) - 1) {
                        buffer[buf_index++] = c;
                    }
                    current_state = STATE_READING_NUMBER;
                } else if (!isspace((unsigned char)c)) {
                    current_state = STATE_ERROR;
                }
                break;
                
            case STATE_READING_WORD:
                if (c == '\0' || isspace((unsigned char)c)) {
                    if (buf_index > 0 && buf_index < sizeof(buffer)) {
                        buffer[buf_index] = '\0';
                        word_count++;
                        printf("Found word: %s\n", buffer);
                    }
                    buf_index = 0;
                    current_state = (c == '\0') ? STATE_END : STATE_START;
                } else if (isalpha((unsigned char)c)) {
                    if (buf_index < sizeof(buffer) - 1) {
                        buffer[buf_index++] = c;
                    }
                } else {
                    current_state = STATE_ERROR;
                }
                break;
                
            case STATE_READING_NUMBER:
                if (c == '\0' || isspace((unsigned char)c)) {
                    if (buf_index > 0 && buf_index < sizeof(buffer)) {
                        buffer[buf_index] = '\0';
                        number_count++;
                        printf("Found number: %s\n", buffer);
                    }
                    buf_index = 0;
                    current_state = (c == '\0') ? STATE_END : STATE_START;
                } else if (isdigit((unsigned char)c)) {
                    if (buf_index < sizeof(buffer) - 1) {
                        buffer[buf_index++] = c;
                    }
                } else {
                    current_state = STATE_ERROR;
                }
                break;
                
            case STATE_ERROR:
                printf("Invalid character encountered\n");
                buf_index = 0;
                if (c == '\0') {
                    current_state = STATE_END;
                } else if (isspace((unsigned char)c)) {
                    current_state = STATE_START;
                }
                break;
                
            case STATE_END:
                break;
        }
        
        if (current_state == STATE_END) {
            break;
        }
    }
    
    printf("Summary: %d words, %d numbers\n", word_count, number_count);
    
    return 0;
}