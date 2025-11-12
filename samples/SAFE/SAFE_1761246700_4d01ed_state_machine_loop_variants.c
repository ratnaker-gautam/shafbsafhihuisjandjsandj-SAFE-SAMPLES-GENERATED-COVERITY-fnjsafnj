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
    int ch;
    
    while (1) {
        switch (current_state) {
            case IDLE:
                printf("Enter text (empty line to exit): ");
                buffer_pos = 0;
                current_state = READING;
                break;
                
            case READING:
                ch = getchar();
                if (ch == EOF) {
                    current_state = ERROR;
                } else if (ch == '\n') {
                    if (buffer_pos == 0) {
                        printf("Total words processed: %d\n", word_count);
                        return 0;
                    }
                    buffer[buffer_pos] = '\0';
                    current_state = PROCESSING;
                } else if (buffer_pos < 255) {
                    buffer[buffer_pos++] = (char)ch;
                } else {
                    current_state = ERROR;
                }
                break;
                
            case PROCESSING:
                {
                    int in_word = 0;
                    for (int i = 0; buffer[i] != '\0'; i++) {
                        if (isalpha((unsigned char)buffer[i])) {
                            if (!in_word) {
                                word_count++;
                                in_word = 1;
                            }
                        } else {
                            in_word = 0;
                        }
                    }
                    current_state = WRITING;
                }
                break;
                
            case WRITING:
                printf("Processed: %s (words: %d)\n", buffer, word_count);
                current_state = IDLE;
                break;
                
            case ERROR:
                fprintf(stderr, "Error occurred during processing\n");
                return 1;
        }
    }
    
    return 0;
}