//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: enum_switch ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

enum state {
    STATE_START,
    STATE_READING_WORD,
    STATE_READING_NUMBER,
    STATE_ERROR,
    STATE_DONE
};

int main(void) {
    char buffer[256];
    char word_buf[256];
    char num_buf[256];
    int word_len = 0;
    int num_len = 0;
    int word_count = 0;
    int num_count = 0;
    enum state current_state = STATE_START;
    
    printf("Enter text (max 255 chars): ");
    if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t input_len = strlen(buffer);
    if (input_len > 0 && buffer[input_len - 1] == '\n') {
        buffer[input_len - 1] = '\0';
        input_len--;
    }
    
    for (size_t i = 0; i <= input_len; i++) {
        char c = buffer[i];
        
        switch (current_state) {
            case STATE_START:
                if (c == '\0') {
                    current_state = STATE_DONE;
                } else if (isalpha((unsigned char)c)) {
                    if (word_len < 255) {
                        word_buf[word_len++] = c;
                    }
                    current_state = STATE_READING_WORD;
                } else if (isdigit((unsigned char)c)) {
                    if (num_len < 255) {
                        num_buf[num_len++] = c;
                    }
                    current_state = STATE_READING_NUMBER;
                } else if (!isspace((unsigned char)c)) {
                    current_state = STATE_ERROR;
                }
                break;
                
            case STATE_READING_WORD:
                if (c == '\0' || isspace((unsigned char)c)) {
                    if (word_len > 0) {
                        word_buf[word_len] = '\0';
                        word_count++;
                        printf("Word %d: %s\n", word_count, word_buf);
                        word_len = 0;
                    }
                    current_state = (c == '\0') ? STATE_DONE : STATE_START;
                } else if (isalpha((unsigned char)c)) {
                    if (word_len < 255) {
                        word_buf[word_len++] = c;
                    }
                } else {
                    current_state = STATE_ERROR;
                }
                break;
                
            case STATE_READING_NUMBER:
                if (c == '\0' || isspace((unsigned char)c)) {
                    if (num_len > 0) {
                        num_buf[num_len] = '\0';
                        num_count++;
                        printf("Number %d: %s\n", num_count, num_buf);
                        num_len = 0;
                    }
                    current_state = (c == '\0') ? STATE_DONE : STATE_START;
                } else if (isdigit((unsigned char)c)) {
                    if (num_len < 255) {
                        num_buf[num_len++] = c;
                    }
                } else {
                    current_state = STATE_ERROR;
                }
                break;
                
            case STATE_ERROR:
                printf("Error: Invalid character at position %zu\n", i);
                return 1;
                
            case STATE_DONE:
                break;
        }
        
        if (current_state == STATE_ERROR) {
            printf("Error: Invalid character at position %zu\n", i);
            return 1;
        }
    }
    
    printf("Total words: %d\n", word_count);
    printf("Total numbers: %d\n", num_count);
    
    return 0;
}