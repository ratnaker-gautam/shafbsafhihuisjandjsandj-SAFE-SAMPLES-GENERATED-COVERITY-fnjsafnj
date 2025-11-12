//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: monolithic_main ; Variation: state_machine
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
    char buffer[256];
    char current_token[256];
    int token_pos = 0;
    enum State current_state = STATE_START;
    int number_sum = 0;
    int word_count = 0;
    int number_count = 0;
    int input_len;
    int i;
    
    printf("Enter text (max 255 chars): ");
    if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
        printf("Error reading input\n");
        return 1;
    }
    
    input_len = strlen(buffer);
    if (input_len > 0 && buffer[input_len - 1] == '\n') {
        buffer[input_len - 1] = '\0';
        input_len--;
    }
    
    if (input_len >= sizeof(buffer) - 1) {
        printf("Input too long\n");
        return 1;
    }
    
    for (i = 0; i <= input_len; i++) {
        char c = buffer[i];
        
        switch (current_state) {
            case STATE_START:
                if (c == '\0') {
                    current_state = STATE_END;
                } else if (isdigit((unsigned char)c)) {
                    current_state = STATE_READING_NUMBER;
                    token_pos = 0;
                    if (token_pos < sizeof(current_token) - 1) {
                        current_token[token_pos++] = c;
                    }
                } else if (isalpha((unsigned char)c)) {
                    current_state = STATE_READING_WORD;
                    token_pos = 0;
                    if (token_pos < sizeof(current_token) - 1) {
                        current_token[token_pos++] = c;
                    }
                } else if (c != ' ' && c != '\t') {
                    current_state = STATE_ERROR;
                }
                break;
                
            case STATE_READING_NUMBER:
                if (c == '\0' || c == ' ' || c == '\t') {
                    current_token[token_pos] = '\0';
                    if (token_pos > 0) {
                        int num = atoi(current_token);
                        number_sum += num;
                        number_count++;
                    }
                    token_pos = 0;
                    current_state = (c == '\0') ? STATE_END : STATE_START;
                } else if (isdigit((unsigned char)c)) {
                    if (token_pos < sizeof(current_token) - 1) {
                        current_token[token_pos++] = c;
                    } else {
                        current_state = STATE_ERROR;
                    }
                } else {
                    current_state = STATE_ERROR;
                }
                break;
                
            case STATE_READING_WORD:
                if (c == '\0' || c == ' ' || c == '\t') {
                    current_token[token_pos] = '\0';
                    if (token_pos > 0) {
                        word_count++;
                    }
                    token_pos = 0;
                    current_state = (c == '\0') ? STATE_END : STATE_START;
                } else if (isalpha((unsigned char)c)) {
                    if (token_pos < sizeof(current_token) - 1) {
                        current_token[token_pos++] = c;
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