//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: loop_variants ; Variation: state_machine
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
    
    while (1) {
        switch (current_state) {
            case START:
                if (pos >= (int)strlen(buffer)) {
                    current_state = DONE;
                } else if (isdigit((unsigned char)buffer[pos])) {
                    current_state = READING_NUM;
                } else if (isalpha((unsigned char)buffer[pos])) {
                    current_state = READING_WORD;
                } else if (isspace((unsigned char)buffer[pos])) {
                    pos++;
                } else {
                    current_state = ERROR;
                }
                break;
                
            case READING_NUM:
                while (pos < (int)strlen(buffer) && isdigit((unsigned char)buffer[pos])) {
                    pos++;
                }
                num_count++;
                current_state = START;
                break;
                
            case READING_WORD:
                while (pos < (int)strlen(buffer) && isalpha((unsigned char)buffer[pos])) {
                    pos++;
                }
                word_count++;
                current_state = START;
                break;
                
            case ERROR:
                fprintf(stderr, "Invalid character at position %d\n", pos);
                return 1;
                
            case DONE:
                printf("Numbers: %d\n", num_count);
                printf("Words: %d\n", word_count);
                return 0;
        }
        
        if (pos > 256) {
            fprintf(stderr, "Position overflow\n");
            return 1;
        }
    }
    
    return 0;
}