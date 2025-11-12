//DeepSeek-V3 V2.5 Category: Safe ; Style: loop_variants ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

enum State { IDLE, READING, PROCESSING, WRITING, ERROR };

int main(void) {
    enum State current_state = IDLE;
    char buffer[256];
    int buffer_pos = 0;
    int word_count = 0;
    int char_count = 0;
    int c;
    
    while (1) {
        switch (current_state) {
            case IDLE:
                printf("Enter text (empty line to exit): ");
                buffer_pos = 0;
                current_state = READING;
                break;
                
            case READING:
                c = getchar();
                if (c == EOF) {
                    current_state = ERROR;
                } else if (c == '\n') {
                    if (buffer_pos == 0) {
                        printf("Total words: %d, Total characters: %d\n", word_count, char_count);
                        return 0;
                    }
                    buffer[buffer_pos] = '\0';
                    current_state = PROCESSING;
                } else if (buffer_pos < 255) {
                    buffer[buffer_pos++] = (char)c;
                    char_count++;
                } else {
                    current_state = ERROR;
                }
                break;
                
            case PROCESSING:
                {
                    int in_word = 0;
                    int i = 0;
                    while (buffer[i] != '\0') {
                        if (isalpha((unsigned char)buffer[i])) {
                            if (!in_word) {
                                word_count++;
                                in_word = 1;
                            }
                        } else {
                            in_word = 0;
                        }
                        i++;
                    }
                    current_state = WRITING;
                }
                break;
                
            case WRITING:
                printf("Line processed: %s\n", buffer);
                printf("Words so far: %d, Characters so far: %d\n", word_count, char_count);
                current_state = IDLE;
                break;
                
            case ERROR:
                fprintf(stderr, "Error: Input too long or read error\n");
                return 1;
        }
    }
    
    return 0;
}