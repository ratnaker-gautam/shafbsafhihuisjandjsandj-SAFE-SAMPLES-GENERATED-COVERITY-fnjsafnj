//DeepSeek-V3 V2.5 Category: Safe ; Style: enum_switch ; Variation: state_machine
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <ctype.h>

enum state {
    START,
    READING_NUMBER,
    READING_WORD,
    SKIPPING_SPACE,
    FINISHED,
    ERROR
};

int main(void) {
    char input[256];
    char buffer[256];
    size_t buf_pos = 0;
    enum state current_state = START;
    int number_count = 0;
    int word_count = 0;
    
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
                    buffer[buf_pos++] = c;
                    current_state = READING_NUMBER;
                } else if (isalpha((unsigned char)c)) {
                    buffer[buf_pos++] = c;
                    current_state = READING_WORD;
                } else if (isspace((unsigned char)c)) {
                    current_state = SKIPPING_SPACE;
                } else {
                    current_state = ERROR;
                }
                break;
                
            case READING_NUMBER:
                if (c == '\0') {
                    if (buf_pos > 0) {
                        buffer[buf_pos] = '\0';
                        printf("Found number: %s\n", buffer);
                        number_count++;
                        buf_pos = 0;
                    }
                    current_state = FINISHED;
                } else if (isdigit((unsigned char)c)) {
                    if (buf_pos < sizeof(buffer) - 1) {
                        buffer[buf_pos++] = c;
                    } else {
                        current_state = ERROR;
                    }
                } else if (isspace((unsigned char)c)) {
                    if (buf_pos > 0) {
                        buffer[buf_pos] = '\0';
                        printf("Found number: %s\n", buffer);
                        number_count++;
                        buf_pos = 0;
                    }
                    current_state = SKIPPING_SPACE;
                } else {
                    current_state = ERROR;
                }
                break;
                
            case READING_WORD:
                if (c == '\0') {
                    if (buf_pos > 0) {
                        buffer[buf_pos] = '\0';
                        printf("Found word: %s\n", buffer);
                        word_count++;
                        buf_pos = 0;
                    }
                    current_state = FINISHED;
                } else if (isalpha((unsigned char)c)) {
                    if (buf_pos < sizeof(buffer) - 1) {
                        buffer[buf_pos++] = c;
                    } else {
                        current_state = ERROR;
                    }
                } else if (isspace((unsigned char)c)) {
                    if (buf_pos > 0) {
                        buffer[buf_pos] = '\0';
                        printf("Found word: %s\n", buffer);
                        word_count++;
                        buf_pos = 0;
                    }
                    current_state = SKIPPING_SPACE;
                } else {
                    current_state = ERROR;
                }
                break;
                
            case SKIPPING_SPACE:
                if (c == '\0') {
                    current_state = FINISHED;
                } else if (isdigit((unsigned char)c)) {
                    buffer[buf_pos++] = c;
                    current_state = READING_NUMBER;
                } else if (isalpha((unsigned char)c)) {
                    buffer[buf_pos++] = c;
                    current_state = READING_WORD;
                } else if (!isspace((unsigned char)c)) {
                    current_state = ERROR;
                }
                break;
                
            case FINISHED:
                break;
                
            case ERROR:
                printf("Error: Invalid input format\n");
                return 1;
        }
        
        if (current_state == ERROR) {
            printf("Error: Invalid input format\n");
            return 1;
        }
    }
    
    printf("Summary: %d numbers, %d words\n", number_count, word_count);
    return 0;
}