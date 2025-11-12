//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: loop_variants ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

enum State { IDLE, READING, PROCESSING, FINISHED, ERROR };

int main(void) {
    enum State current_state = IDLE;
    char buffer[256];
    int buffer_pos = 0;
    int word_count = 0;
    int char_count = 0;
    int c;
    
    while (current_state != FINISHED && current_state != ERROR) {
        switch (current_state) {
            case IDLE:
                printf("Enter text (empty line to finish): ");
                buffer_pos = 0;
                current_state = READING;
                break;
                
            case READING:
                c = getchar();
                if (c == EOF) {
                    current_state = FINISHED;
                } else if (c == '\n') {
                    if (buffer_pos == 0) {
                        current_state = FINISHED;
                    } else {
                        buffer[buffer_pos] = '\0';
                        current_state = PROCESSING;
                    }
                } else if (buffer_pos < 255) {
                    buffer[buffer_pos++] = (char)c;
                } else {
                    current_state = ERROR;
                }
                break;
                
            case PROCESSING:
                {
                    int i = 0;
                    int in_word = 0;
                    
                    while (i < buffer_pos) {
                        if (isalpha((unsigned char)buffer[i])) {
                            char_count++;
                            if (!in_word) {
                                word_count++;
                                in_word = 1;
                            }
                        } else {
                            in_word = 0;
                        }
                        i++;
                    }
                    
                    printf("Line processed: %d words, %d letters\n", word_count, char_count);
                    word_count = 0;
                    char_count = 0;
                    current_state = IDLE;
                }
                break;
                
            case FINISHED:
                printf("Analysis complete.\n");
                break;
                
            case ERROR:
                printf("Error: Buffer overflow detected.\n");
                break;
        }
    }
    
    return current_state == ERROR ? EXIT_FAILURE : EXIT_SUCCESS;
}