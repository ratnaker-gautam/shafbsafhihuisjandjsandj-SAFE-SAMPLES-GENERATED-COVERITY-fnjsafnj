//DeepSeek-V3 V2.5 Category: Safe ; Style: loop_variants ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

enum State { START, READING_NUM, READING_WORD, ERROR, DONE };

int main(void) {
    char input[256];
    char buffer[256];
    int buffer_pos = 0;
    enum State current_state = START;
    int num_count = 0;
    int word_count = 0;
    int i = 0;
    
    printf("Enter text: ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        return 1;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len-1] == '\n') {
        input[len-1] = '\0';
        len--;
    }
    
    if (len >= sizeof(input) - 1) {
        return 1;
    }
    
    while (i <= len) {
        char c = input[i];
        
        switch (current_state) {
            case START:
                if (isdigit(c)) {
                    buffer[buffer_pos++] = c;
                    current_state = READING_NUM;
                } else if (isalpha(c)) {
                    buffer[buffer_pos++] = c;
                    current_state = READING_WORD;
                } else if (c == ' ' || c == '\t' || c == '\0') {
                    current_state = START;
                } else {
                    current_state = ERROR;
                }
                break;
                
            case READING_NUM:
                if (isdigit(c)) {
                    if (buffer_pos < sizeof(buffer) - 1) {
                        buffer[buffer_pos++] = c;
                    } else {
                        current_state = ERROR;
                    }
                } else if (c == ' ' || c == '\t' || c == '\0') {
                    buffer[buffer_pos] = '\0';
                    num_count++;
                    buffer_pos = 0;
                    current_state = START;
                } else {
                    current_state = ERROR;
                }
                break;
                
            case READING_WORD:
                if (isalpha(c)) {
                    if (buffer_pos < sizeof(buffer) - 1) {
                        buffer[buffer_pos++] = c;
                    } else {
                        current_state = ERROR;
                    }
                } else if (c == ' ' || c == '\t' || c == '\0') {
                    buffer[buffer_pos] = '\0';
                    word_count++;
                    buffer_pos = 0;
                    current_state = START;
                } else {
                    current_state = ERROR;
                }
                break;
                
            case ERROR:
                printf("Invalid input format\n");
                return 1;
                
            case DONE:
                break;
        }
        
        if (current_state == ERROR) {
            printf("Invalid input format\n");
            return 1;
        }
        
        i++;
        if (c == '\0') {
            break;
        }
    }
    
    if (current_state == READING_NUM) {
        buffer[buffer_pos] = '\0';
        num_count++;
    } else if (current_state == READING_WORD) {
        buffer[buffer_pos] = '\0';
        word_count++;
    }
    
    printf("Numbers found: %d\n", num_count);
    printf("Words found: %d\n", word_count);
    
    return 0;
}