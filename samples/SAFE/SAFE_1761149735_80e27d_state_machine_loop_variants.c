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
    
    buffer[strcspn(buffer, "\n")] = '\0';
    
    for (int i = 0; buffer[i] != '\0'; i++) {
        ch = buffer[i];
        
        switch (current_state) {
            case START:
                if (isspace(ch)) {
                    continue;
                } else if (isdigit(ch)) {
                    current_state = READING_NUM;
                    num_count++;
                } else if (isalpha(ch)) {
                    current_state = READING_WORD;
                    word_count++;
                } else {
                    current_state = ERROR;
                }
                break;
                
            case READING_NUM:
                if (isspace(ch)) {
                    current_state = START;
                } else if (!isdigit(ch)) {
                    current_state = ERROR;
                }
                break;
                
            case READING_WORD:
                if (isspace(ch)) {
                    current_state = START;
                } else if (!isalpha(ch)) {
                    current_state = ERROR;
                }
                break;
                
            case ERROR:
                break;
                
            case DONE:
                break;
        }
        
        if (current_state == ERROR) {
            break;
        }
    }
    
    if (current_state == ERROR) {
        printf("Invalid input format\n");
        return 1;
    }
    
    printf("Numbers: %d\n", num_count);
    printf("Words: %d\n", word_count);
    
    return 0;
}