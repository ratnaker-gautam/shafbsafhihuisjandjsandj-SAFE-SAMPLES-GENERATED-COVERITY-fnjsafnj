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
    char buffer[256];
    int number_sum = 0;
    int word_count = 0;
    enum State current_state = STATE_START;
    int valid_input = 1;
    
    printf("Enter text (max 255 chars): ");
    if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
        printf("Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(buffer);
    if (len > 0 && buffer[len - 1] == '\n') {
        buffer[len - 1] = '\0';
        len--;
    }
    
    if (len >= sizeof(buffer) - 1) {
        printf("Input too long\n");
        return 1;
    }
    
    int pos = 0;
    while (pos < len && current_state != STATE_ERROR && current_state != STATE_DONE) {
        char c = buffer[pos];
        
        switch (current_state) {
            case STATE_START:
                if (isspace(c)) {
                    pos++;
                } else if (isdigit(c)) {
                    current_state = STATE_READING_NUMBER;
                } else if (isalpha(c)) {
                    current_state = STATE_READING_WORD;
                } else {
                    current_state = STATE_ERROR;
                }
                break;
                
            case STATE_READING_NUMBER:
                if (isdigit(c)) {
                    pos++;
                } else if (isspace(c)) {
                    int num_start = pos - 1;
                    while (num_start >= 0 && isdigit(buffer[num_start])) {
                        num_start--;
                    }
                    num_start++;
                    
                    char num_str[32];
                    int num_len = pos - num_start;
                    if (num_len >= sizeof(num_str)) {
                        current_state = STATE_ERROR;
                        break;
                    }
                    
                    strncpy(num_str, &buffer[num_start], num_len);
                    num_str[num_len] = '\0';
                    
                    int num = atoi(num_str);
                    if (num > 0 || (num == 0 && num_len == 1 && buffer[num_start] == '0')) {
                        number_sum += num;
                    }
                    
                    current_state = STATE_START;
                    pos++;
                } else {
                    current_state = STATE_ERROR;
                }
                break;
                
            case STATE_READING_WORD:
                if (isalpha(c)) {
                    pos++;
                } else if (isspace(c)) {
                    word_count++;
                    current_state = STATE_START;
                    pos++;
                } else {
                    current_state = STATE_ERROR;
                }
                break;
                
            case STATE_ERROR:
                valid_input = 0;
                break;
                
            case STATE_DONE:
                break;
        }
    }
    
    if (current_state == STATE_READING_NUMBER) {
        int num_start = pos - 1;
        while (num_start >= 0 && isdigit(buffer[num_start])) {
            num_start--;
        }
        num_start++;
        
        char num_str[32];
        int num_len = pos - num_start;
        if (num_len < sizeof(num_str)) {
            strncpy(num_str, &buffer[num_start], num_len);
            num_str[num_len] = '\0';
            
            int num = atoi(num_str);
            if (num > 0 || (num == 0 && num_len == 1 && buffer[num_start] == '0')) {
                number_sum += num;
            }
        }
        current_state = STATE_DONE;
    } else if (current_state == STATE_READING_WORD) {
        word_count++;
        current_state = STATE_DONE;
    } else if (current_state == STATE_START) {
        current_state = STATE_DONE;
    }
    
    if (!valid_input || current_state == STATE_ERROR) {
        printf("Invalid input format\n");
        return 1;
    }
    
    printf("Sum of numbers: %d\n", number_sum);
    printf("Word count: %d\n", word_count);
    
    return 0;
}