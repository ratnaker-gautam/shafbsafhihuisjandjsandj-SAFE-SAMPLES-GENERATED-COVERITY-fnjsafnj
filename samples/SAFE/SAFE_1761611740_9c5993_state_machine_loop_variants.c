//DeepSeek-V3 V2.5 Category: Safe ; Style: loop_variants ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

enum State { START, READING_NUM, READING_ALPHA, ERROR, DONE };

int main(void) {
    enum State current_state = START;
    char input[256];
    int num_count = 0;
    int alpha_count = 0;
    int pos = 0;
    char ch;
    
    printf("Enter text (max 255 chars): ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    input[strcspn(input, "\n")] = '\0';
    
    while (1) {
        ch = input[pos];
        
        switch (current_state) {
            case START:
                if (ch == '\0') {
                    current_state = DONE;
                } else if (isdigit(ch)) {
                    current_state = READING_NUM;
                    num_count++;
                } else if (isalpha(ch)) {
                    current_state = READING_ALPHA;
                    alpha_count++;
                } else if (ch != ' ') {
                    current_state = ERROR;
                }
                break;
                
            case READING_NUM:
                if (ch == '\0') {
                    current_state = DONE;
                } else if (isdigit(ch)) {
                    num_count++;
                } else if (isalpha(ch)) {
                    current_state = READING_ALPHA;
                    alpha_count++;
                } else if (ch == ' ') {
                    current_state = START;
                } else {
                    current_state = ERROR;
                }
                break;
                
            case READING_ALPHA:
                if (ch == '\0') {
                    current_state = DONE;
                } else if (isalpha(ch)) {
                    alpha_count++;
                } else if (isdigit(ch)) {
                    current_state = READING_NUM;
                    num_count++;
                } else if (ch == ' ') {
                    current_state = START;
                } else {
                    current_state = ERROR;
                }
                break;
                
            case ERROR:
                printf("Invalid character '%c' at position %d\n", ch, pos);
                return 1;
                
            case DONE:
                goto done_loop;
        }
        
        pos++;
        if (pos >= 255) {
            current_state = DONE;
        }
    }
    
done_loop:
    
    printf("Numbers: %d\n", num_count);
    printf("Letters: %d\n", alpha_count);
    
    return 0;
}