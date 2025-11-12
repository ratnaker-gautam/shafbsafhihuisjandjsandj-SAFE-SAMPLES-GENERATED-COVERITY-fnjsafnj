//DeepSeek-V3 V2.5 Category: Safe ; Style: monolithic_main ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

enum State { START, READING_NUMBER, READING_WORD, ERROR, DONE };

int main(void) {
    enum State current_state = START;
    char input[256];
    int number_count = 0;
    int word_count = 0;
    int pos = 0;
    char current_token[256];
    int token_pos = 0;
    
    printf("Enter text (max 255 characters): ");
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
                    current_token[token_pos++] = c;
                } else if (isspace(c)) {
                    pos++;
                } else {
                    current_state = ERROR;
                }
                break;
                
            case READING_NUMBER:
                if (c == '\0') {
                    current_token[token_pos] = '\0';
                    number_count++;
                    current_state = DONE;
                } else if (isdigit(c)) {
                    if (token_pos < 255) {
                        current_token[token_pos++] = c;
                        pos++;
                    } else {
                        current_state = ERROR;
                    }
                } else if (isspace(c)) {
                    current_token[token_pos] = '\0';
                    number_count++;
                    current_state = START;
                    pos++;
                } else {
                    current_state = ERROR;
                }
                break;
                
            case READING_WORD:
                if (c == '\0') {
                    current_token[token_pos] = '\0';
                    word_count++;
                    current_state = DONE;
                } else if (isalpha(c)) {
                    if (token_pos < 255) {
                        current_token[token_pos++] = c;
                        pos++;
                    } else {
                        current_state = ERROR;
                    }
                } else if (isspace(c)) {
                    current_token[token_pos] = '\0';
                    word_count++;
                    current_state = START;
                    pos++;
                } else {
                    current_state = ERROR;
                }
                break;
                
            case ERROR:
                fprintf(stderr, "Error: Invalid input format at position %d\n", pos);
                return 1;
                
            case DONE:
                break;
        }
    }
    
    if (current_state == READING_NUMBER) {
        number_count++;
    } else if (current_state == READING_WORD) {
        word_count++;
    }
    
    printf("Numbers found: %d\n", number_count);
    printf("Words found: %d\n", word_count);
    
    return 0;
}