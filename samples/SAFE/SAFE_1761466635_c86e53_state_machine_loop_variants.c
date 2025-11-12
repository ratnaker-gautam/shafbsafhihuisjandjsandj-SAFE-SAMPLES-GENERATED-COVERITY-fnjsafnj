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
                printf("Enter text (max 255 chars, empty to exit): ");
                current_state = READING;
                break;

            case READING:
                input_char = getchar();
                if (input_char == EOF) {
                    current_state = ERROR;
                } else if (input_char == '\n') {
                    if (buffer_pos == 0) {
                        printf("Exiting...\n");
                        return 0;
                    }
                    buffer[buffer_pos] = '\0';
                    current_state = PROCESSING;
                } else if (buffer_pos < sizeof(buffer) - 1) {
                    buffer[buffer_pos++] = (char)input_char;
                } else {
                    printf("Input too long\n");
                    while (getchar() != '\n');
                    current_state = IDLE;
                }
                break;

            case PROCESSING:
                {
                    size_t i = 0;
                    while (buffer[i] != '\0') {
                        if (buffer[i] >= 'a' && buffer[i] <= 'z') {
                            buffer[i] = buffer[i] - 'a' + 'A';
                        }
                        i++;
                    }
                    current_state = WRITING;
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

    printf("Processed maximum items: %d\n", max_processed);
    return 0;
}