//DeepSeek-V3 V2.5 Category: Safe ; Style: enum_switch ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

enum state {
    START,
    READING_NUMBER,
    READING_WORD,
    SKIPPING_SPACE,
    END,
    ERROR
};

int main(void) {
    char input[1024];
    char buffer[1024];
    int buf_index = 0;
    enum state current_state = START;
    int number_sum = 0;
    int word_count = 0;
    int i = 0;
    
    printf("Enter text (max 1023 chars): ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    input[strcspn(input, "\n")] = '\0';
    
    while (current_state != END && current_state != ERROR && i < 1024) {
        char c = input[i];
        
        switch (current_state) {
            case START:
                if (c == '\0') {
                    current_state = END;
                } else if (isdigit((unsigned char)c)) {
                    buffer[buf_index++] = c;
                    current_state = READING_NUMBER;
                } else if (isalpha((unsigned char)c)) {
                    buffer[buf_index++] = c;
                    current_state = READING_WORD;
                } else if (isspace((unsigned char)c)) {
                    current_state = SKIPPING_SPACE;
                } else {
                    current_state = ERROR;
                }
                i++;
                break;
                
            case READING_NUMBER:
                if (c == '\0') {
                    if (buf_index > 0) {
                        buffer[buf_index] = '\0';
                        int num = atoi(buffer);
                        if (num > 0) number_sum += num;
                    }
                    current_state = END;
                } else if (isdigit((unsigned char)c)) {
                    if (buf_index < 1023) {
                        buffer[buf_index++] = c;
                    }
                    i++;
                } else if (isspace((unsigned char)c)) {
                    if (buf_index > 0) {
                        buffer[buf_index] = '\0';
                        int num = atoi(buffer);
                        if (num > 0) number_sum += num;
                        buf_index = 0;
                    }
                    current_state = SKIPPING_SPACE;
                    i++;
                } else {
                    current_state = ERROR;
                }
                break;
                
            case READING_WORD:
                if (c == '\0') {
                    if (buf_index > 0) {
                        word_count++;
                        buf_index = 0;
                    }
                    current_state = END;
                } else if (isalpha((unsigned char)c)) {
                    if (buf_index < 1023) {
                        buffer[buf_index++] = c;
                    }
                    i++;
                } else if (isspace((unsigned char)c)) {
                    if (buf_index > 0) {
                        word_count++;
                        buf_index = 0;
                    }
                    current_state = SKIPPING_SPACE;
                    i++;
                } else {
                    current_state = ERROR;
                }
                break;
                
            case SKIPPING_SPACE:
                if (c == '\0') {
                    current_state = END;
                } else if (isspace((unsigned char)c)) {
                    i++;
                } else if (isdigit((unsigned char)c)) {
                    buffer[buf_index++] = c;
                    current_state = READING_NUMBER;
                    i++;
                } else if (isalpha((unsigned char)c)) {
                    buffer[buf_index++] = c;
                    current_state = READING_WORD;
                    i++;
                } else {
                    current_state = ERROR;
                }
                break;
                
            case END:
                break;
                
            case ERROR:
                fprintf(stderr, "Error: Invalid character at position %d\n", i);
                return 1;
        }
    }
    
    if (current_state == ERROR) {
        fprintf(stderr, "Error: Invalid input format\n");
        return 1;
    }
    
    printf("Sum of numbers: %d\n", number_sum);
    printf("Word count: %d\n", word_count);
    
    return 0;
}