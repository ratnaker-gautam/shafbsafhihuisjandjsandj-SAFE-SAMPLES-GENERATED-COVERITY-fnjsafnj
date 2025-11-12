//DeepSeek-V3 V2.5 Category: Safe ; Style: monolithic_main ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

enum State { START, READING_NUM, READING_WORD, SKIPPING, ERROR, DONE };

int main(void) {
    enum State current_state = START;
    char buffer[1024];
    int num_count = 0;
    int word_count = 0;
    int pos = 0;
    int ch;
    
    printf("Enter text (max 1023 chars): ");
    if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    buffer[strcspn(buffer, "\n")] = '\0';
    
    while (current_state != DONE && current_state != ERROR && pos < 1024) {
        ch = buffer[pos];
        
        switch (current_state) {
            case START:
                if (ch == '\0') {
                    current_state = DONE;
                } else if (isdigit(ch)) {
                    current_state = READING_NUM;
                    num_count++;
                } else if (isalpha(ch)) {
                    current_state = READING_WORD;
                    word_count++;
                } else if (isspace(ch)) {
                    current_state = SKIPPING;
                } else {
                    current_state = ERROR;
                }
                break;
                
            case READING_NUM:
                if (ch == '\0') {
                    current_state = DONE;
                } else if (isdigit(ch)) {
                    current_state = READING_NUM;
                } else if (isspace(ch)) {
                    current_state = SKIPPING;
                } else if (isalpha(ch)) {
                    current_state = ERROR;
                } else {
                    current_state = ERROR;
                }
                break;
                
            case READING_WORD:
                if (ch == '\0') {
                    current_state = DONE;
                } else if (isalpha(ch)) {
                    current_state = READING_WORD;
                } else if (isspace(ch)) {
                    current_state = SKIPPING;
                } else if (isdigit(ch)) {
                    current_state = ERROR;
                } else {
                    current_state = ERROR;
                }
                break;
                
            case SKIPPING:
                if (ch == '\0') {
                    current_state = DONE;
                } else if (isdigit(ch)) {
                    current_state = READING_NUM;
                    num_count++;
                } else if (isalpha(ch)) {
                    current_state = READING_WORD;
                    word_count++;
                } else if (isspace(ch)) {
                    current_state = SKIPPING;
                } else {
                    current_state = ERROR;
                }
                break;
                
            case ERROR:
                printf("Invalid input format at position %d\n", pos);
                return 1;
                
            case DONE:
                break;
        }
        
        pos++;
    }
    
    if (current_state == ERROR) {
        printf("Invalid input format at position %d\n", pos);
        return 1;
    }
    
    printf("Numbers: %d\n", num_count);
    printf("Words: %d\n", word_count);
    
    return 0;
}