//DeepSeek-V3 V2.5 Category: Safe ; Style: loop_variants ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

enum State { IDLE, READING, PROCESSING, WRITING, ERROR };

int main(void) {
    enum State current_state = IDLE;
    char buffer[256];
    size_t buffer_pos = 0;
    int input_char;
    int processed_count = 0;
    const int max_processed = 10;
    
    while (processed_count < max_processed) {
        switch (current_state) {
            case IDLE:
                buffer_pos = 0;
                printf("Enter data (max 255 chars, empty to exit): ");
                current_state = READING;
                break;
                
            case READING:
                buffer_pos = 0;
                while (buffer_pos < sizeof(buffer) - 1) {
                    input_char = getchar();
                    if (input_char == EOF) {
                        current_state = ERROR;
                        break;
                    }
                    if (input_char == '\n') {
                        buffer[buffer_pos] = '\0';
                        if (buffer_pos == 0) {
                            current_state = IDLE;
                        } else {
                            current_state = PROCESSING;
                        }
                        break;
                    }
                    buffer[buffer_pos++] = (char)input_char;
                }
                if (buffer_pos >= sizeof(buffer) - 1) {
                    buffer[sizeof(buffer) - 1] = '\0';
                    while (getchar() != '\n' && !feof(stdin));
                    current_state = PROCESSING;
                }
                break;
                
            case PROCESSING:
                if (buffer_pos > 0) {
                    size_t i = 0;
                    while (i < buffer_pos) {
                        if (buffer[i] >= 'a' && buffer[i] <= 'z') {
                            buffer[i] = buffer[i] - 'a' + 'A';
                        }
                        i++;
                    }
                    current_state = WRITING;
                } else {
                    current_state = IDLE;
                }
                break;
                
            case WRITING:
                printf("Processed: %s\n", buffer);
                processed_count++;
                current_state = IDLE;
                break;
                
            case ERROR:
                printf("Input error occurred\n");
                return 1;
        }
    }
    
    printf("Processed %d items, exiting\n", processed_count);
    return 0;
}