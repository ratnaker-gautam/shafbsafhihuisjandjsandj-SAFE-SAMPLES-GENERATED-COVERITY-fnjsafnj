//DeepSeek-V3 V2.5 Category: Safe ; Style: monolithic_main ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

enum state {
    START,
    READING_NUMBER,
    READING_WORD,
    ERROR,
    DONE
};

int main(void) {
    char buffer[256];
    int buffer_pos = 0;
    enum state current_state = START;
    int number_count = 0;
    int word_count = 0;
    int ch;
    
    printf("Enter text (max 255 chars, Ctrl+D to end): ");
    
    while ((ch = getchar()) != EOF && buffer_pos < 255) {
        if (ch == '\n') {
            buffer[buffer_pos] = '\0';
            break;
        }
        buffer[buffer_pos++] = ch;
    }
    
    if (buffer_pos == 255) {
        buffer[255] = '\0';
    } else if (buffer_pos == 0) {
        printf("No input provided.\n");
        return 1;
    }
    
    buffer_pos = 0;
    
    while (current_state != DONE && current_state != ERROR) {
        ch = buffer[buffer_pos];
        
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
                    buffer_pos++;
                } else {
                    current_state = ERROR;
                }
                break;
                
            case READING_NUMBER:
                if (ch == '\0') {
                    current_state = DONE;
                } else if (isdigit(ch)) {
                    buffer_pos++;
                } else if (isspace(ch)) {
                    current_state = START;
                    buffer_pos++;
                } else {
                    current_state = ERROR;
                }
                break;
                
            case READING_WORD:
                if (ch == '\0') {
                    current_state = DONE;
                } else if (isalpha(ch)) {
                    buffer_pos++;
                } else if (isspace(ch)) {
                    current_state = START;
                    buffer_pos++;
                } else {
                    current_state = ERROR;
                }
                break;
                
            case ERROR:
                printf("Invalid character '%c' at position %d\n", ch, buffer_pos);
                return 1;
                
            case DONE:
                break;
        }
    }
    
    if (current_state == DONE) {
        printf("Found %d numbers and %d words\n", number_count, word_count);
    } else {
        printf("Processing failed.\n");
        return 1;
    }
    
    return 0;
}