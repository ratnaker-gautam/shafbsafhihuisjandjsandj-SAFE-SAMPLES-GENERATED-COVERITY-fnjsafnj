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
    int pos = 0;
    int ch;
    
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
    
    for (pos = 0; pos <= (int)len; pos++) {
        ch = (pos < (int)len) ? buffer[pos] : '\0';
        
        switch (current_state) {
            case START:
                if (isdigit(ch)) {
                    current_state = READING_NUM;
                    num_count++;
                } else if (isalpha(ch)) {
                    current_state = READING_WORD;
                    word_count++;
                } else if (ch == ' ' || ch == '\t' || ch == '\0') {
                    current_state = START;
                } else {
                    current_state = ERROR;
                }
                break;
                
            case READING_NUM:
                if (isdigit(ch)) {
                    current_state = READING_NUM;
                } else if (isalpha(ch)) {
                    current_state = ERROR;
                } else if (ch == ' ' || ch == '\t' || ch == '\0') {
                    current_state = START;
                } else {
                    current_state = ERROR;
                }
                break;
                
            case READING_WORD:
                if (isalpha(ch)) {
                    current_state = READING_WORD;
                } else if (isdigit(ch)) {
                    current_state = ERROR;
                } else if (ch == ' ' || ch == '\t' || ch == '\0') {
                    current_state = START;
                } else {
                    current_state = ERROR;
                }
                break;
                
            case ERROR:
                if (ch == ' ' || ch == '\t' || ch == '\0') {
                    current_state = START;
                }
                break;
                
            case DONE:
                break;
        }
        
        if (current_state == ERROR) {
            printf("Invalid input format at position %d\n", pos);
            return 1;
        }
    }
    
    printf("Numbers: %d\n", num_count);
    printf("Words: %d\n", word_count);
    
    return 0;
}