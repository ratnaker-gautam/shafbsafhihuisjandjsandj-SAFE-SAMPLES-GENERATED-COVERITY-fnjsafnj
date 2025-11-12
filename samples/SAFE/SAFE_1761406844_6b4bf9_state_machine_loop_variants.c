//DeepSeek-V3 V2.5 Category: Safe ; Style: loop_variants ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

enum State { START, READING_NUM, READING_WORD, ERROR, DONE };

int main(void) {
    char buffer[256];
    int num_count = 0;
    int word_count = 0;
    enum State current_state = START;
    
    printf("Enter text (max 255 chars): ");
    if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
        fprintf(stderr, "Input error\n");
        return 1;
    }
    
    size_t len = strlen(buffer);
    if (len > 0 && buffer[len-1] == '\n') {
        buffer[len-1] = '\0';
        len--;
    }
    
    if (len >= sizeof(buffer) - 1) {
        fprintf(stderr, "Input too long\n");
        return 1;
    }
    
    for (size_t i = 0; i <= len; i++) {
        char c = buffer[i];
        
        switch (current_state) {
            case START:
                if (c == '\0') {
                    current_state = DONE;
                } else if (isdigit((unsigned char)c)) {
                    current_state = READING_NUM;
                    num_count++;
                } else if (isalpha((unsigned char)c)) {
                    current_state = READING_WORD;
                    word_count++;
                } else if (c != ' ' && c != '\t') {
                    current_state = ERROR;
                }
                break;
                
            case READING_NUM:
                if (c == '\0') {
                    current_state = DONE;
                } else if (isdigit((unsigned char)c)) {
                    continue;
                } else if (isalpha((unsigned char)c)) {
                    current_state = READING_WORD;
                    word_count++;
                } else if (c == ' ' || c == '\t') {
                    current_state = START;
                } else {
                    current_state = ERROR;
                }
                break;
                
            case READING_WORD:
                if (c == '\0') {
                    current_state = DONE;
                } else if (isalnum((unsigned char)c)) {
                    continue;
                } else if (c == ' ' || c == '\t') {
                    current_state = START;
                } else {
                    current_state = ERROR;
                }
                break;
                
            case ERROR:
                printf("Invalid character at position %zu\n", i);
                return 1;
                
            case DONE:
                break;
        }
        
        if (current_state == ERROR) {
            printf("Invalid character at position %zu\n", i);
            return 1;
        }
        
        if (current_state == DONE) {
            break;
        }
    }
    
    printf("Numbers found: %d\n", num_count);
    printf("Words found: %d\n", word_count);
    
    return 0;
}