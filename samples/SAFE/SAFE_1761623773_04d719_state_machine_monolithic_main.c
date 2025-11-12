//DeepSeek-V3 V2.5 Category: Safe ; Style: monolithic_main ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

enum State { START, READING_NUMBER, READING_WORD, ERROR, DONE };

int main(void) {
    enum State current_state = START;
    char input[256];
    int number_sum = 0;
    int word_count = 0;
    int pos = 0;
    char current_token[64];
    int token_pos = 0;
    
    printf("Enter text (max 255 chars): ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    input[strcspn(input, "\n")] = '\0';
    
    while (current_state != DONE && current_state != ERROR && pos < 255) {
        char c = input[pos];
        
        switch (current_state) {
            case START:
                if (c == '\0') {
                    current_state = DONE;
                } else if (isdigit(c)) {
                    current_state = READING_NUMBER;
                    token_pos = 0;
                    current_token[token_pos++] = c;
                } else if (isalpha(c)) {
                    current_state = READING_WORD;
                    token_pos = 0;
                    current_token[token_pos++] = tolower(c);
                } else if (isspace(c)) {
                    pos++;
                } else {
                    current_state = ERROR;
                }
                break;
                
            case READING_NUMBER:
                if (isdigit(c)) {
                    if (token_pos < 63) {
                        current_token[token_pos++] = c;
                    }
                    pos++;
                } else if (isspace(c) || c == '\0') {
                    current_token[token_pos] = '\0';
                    int num = atoi(current_token);
                    number_sum += num;
                    current_state = START;
                    pos++;
                } else {
                    current_state = ERROR;
                }
                break;
                
            case READING_WORD:
                if (isalpha(c)) {
                    if (token_pos < 63) {
                        current_token[token_pos++] = tolower(c);
                    }
                    pos++;
                } else if (isspace(c) || c == '\0') {
                    current_token[token_pos] = '\0';
                    if (strlen(current_token) > 0) {
                        word_count++;
                    }
                    current_state = START;
                    pos++;
                } else {
                    current_state = ERROR;
                }
                break;
                
            case ERROR:
                fprintf(stderr, "Invalid input format at position %d\n", pos);
                return 1;
                
            case DONE:
                break;
        }
        
        if (c == '\0') {
            current_state = DONE;
        }
    }
    
    if (current_state == READING_NUMBER || current_state == READING_WORD) {
        if (token_pos > 0) {
            current_token[token_pos] = '\0';
            if (current_state == READING_NUMBER) {
                int num = atoi(current_token);
                number_sum += num;
            } else {
                word_count++;
            }
        }
    }
    
    if (current_state == ERROR) {
        fprintf(stderr, "Invalid input format\n");
        return 1;
    }
    
    printf("Sum of numbers: %d\n", number_sum);
    printf("Word count: %d\n", word_count);
    
    return 0;
}