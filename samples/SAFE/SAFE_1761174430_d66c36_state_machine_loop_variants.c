//DeepSeek-V3 V2.5 Category: Safe ; Style: loop_variants ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

enum State { START, READING_NUMBER, READING_WORD, ERROR, DONE };

int main(void) {
    char buffer[256];
    int number_count = 0;
    int word_count = 0;
    enum State current_state = START;
    int pos = 0;
    int ch;
    
    printf("Enter text (max 255 chars): ");
    if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    buffer[strcspn(buffer, "\n")] = '\0';
    
    while (current_state != DONE && current_state != ERROR) {
        ch = buffer[pos];
        
        switch (current_state) {
            case START:
                if (ch == '\0') {
                    current_state = DONE;
                } else if (isdigit(ch)) {
                    current_state = READING_NUMBER;
                    number_count++;
                } else if (isalpha(ch)) {
                    current_state = READING_WORD;
                    word_count++;
                } else if (isspace(ch)) {
                    pos++;
                } else {
                    current_state = ERROR;
                }
                break;
                
            case READING_NUMBER:
                if (ch == '\0') {
                    current_state = DONE;
                } else if (isdigit(ch)) {
                    pos++;
                } else if (isspace(ch)) {
                    current_state = START;
                    pos++;
                } else {
                    current_state = ERROR;
                }
                break;
                
            case READING_WORD:
                if (ch == '\0') {
                    current_state = DONE;
                } else if (isalpha(ch)) {
                    pos++;
                } else if (isspace(ch)) {
                    current_state = START;
                    pos++;
                } else {
                    current_state = ERROR;
                }
                break;
                
            case ERROR:
                break;
                
            case DONE:
                break;
        }
    }
    
    if (current_state == ERROR) {
        printf("Invalid input format at position %d\n", pos);
        return 1;
    }
    
    printf("Numbers found: %d\n", number_count);
    printf("Words found: %d\n", word_count);
    
    return 0;
}