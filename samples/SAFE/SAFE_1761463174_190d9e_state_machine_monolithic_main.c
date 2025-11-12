//DeepSeek-V3 V2.5 Category: Safe ; Style: monolithic_main ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

enum State { START, READING_NUM, READING_WORD, ERROR, DONE };

int main(void) {
    enum State current_state = START;
    int num_count = 0;
    int word_count = 0;
    char buffer[1024];
    int buf_pos = 0;
    int ch;
    
    printf("Enter text (max 1023 chars, Ctrl+D to end): ");
    
    while ((ch = getchar()) != EOF && buf_pos < 1023) {
        buffer[buf_pos++] = (char)ch;
    }
    buffer[buf_pos] = '\0';
    
    if (buf_pos == 0) {
        printf("No input provided.\n");
        return 1;
    }
    
    char *ptr = buffer;
    
    while (current_state != DONE && current_state != ERROR) {
        switch (current_state) {
            case START:
                if (*ptr == '\0') {
                    current_state = DONE;
                } else if (isdigit((unsigned char)*ptr)) {
                    current_state = READING_NUM;
                    num_count++;
                } else if (isalpha((unsigned char)*ptr)) {
                    current_state = READING_WORD;
                    word_count++;
                } else if (isspace((unsigned char)*ptr)) {
                    ptr++;
                    continue;
                } else {
                    current_state = ERROR;
                }
                break;
                
            case READING_NUM:
                if (*ptr == '\0') {
                    current_state = DONE;
                } else if (isdigit((unsigned char)*ptr)) {
                    ptr++;
                } else if (isspace((unsigned char)*ptr)) {
                    current_state = START;
                    ptr++;
                } else {
                    current_state = ERROR;
                }
                break;
                
            case READING_WORD:
                if (*ptr == '\0') {
                    current_state = DONE;
                } else if (isalpha((unsigned char)*ptr)) {
                    ptr++;
                } else if (isspace((unsigned char)*ptr)) {
                    current_state = START;
                    ptr++;
                } else {
                    current_state = ERROR;
                }
                break;
                
            case ERROR:
                printf("Invalid input format.\n");
                return 1;
                
            case DONE:
                break;
        }
    }
    
    if (current_state == ERROR) {
        printf("Invalid input format.\n");
        return 1;
    }
    
    printf("Numbers found: %d\n", num_count);
    printf("Words found: %d\n", word_count);
    
    return 0;
}