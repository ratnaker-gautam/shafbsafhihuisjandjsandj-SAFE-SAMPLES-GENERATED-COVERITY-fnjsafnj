//DeepSeek-V3 V2.5 Category: Safe ; Style: monolithic_main ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

enum State { IDLE, READING, PROCESSING, WRITING, ERROR, DONE };

int main(void) {
    enum State current_state = IDLE;
    char buffer[256];
    int buffer_pos = 0;
    int word_count = 0;
    int ch;
    int valid_input = 1;

    printf("Enter text (max 255 chars, Ctrl+D to end):\n");

    while (current_state != DONE && current_state != ERROR) {
        switch (current_state) {
            case IDLE:
                buffer_pos = 0;
                word_count = 0;
                memset(buffer, 0, sizeof(buffer));
                current_state = READING;
                break;

            case READING:
                ch = getchar();
                if (ch == EOF) {
                    if (buffer_pos > 0) {
                        current_state = PROCESSING;
                    } else {
                        current_state = DONE;
                    }
                } else if (ch == '\n') {
                    current_state = PROCESSING;
                } else if (buffer_pos < 255 && isprint(ch)) {
                    buffer[buffer_pos++] = (char)ch;
                } else if (!isprint(ch) && ch != '\n' && ch != '\t' && ch != '\r') {
                    current_state = ERROR;
                }
                break;

            case PROCESSING:
                if (buffer_pos == 0) {
                    current_state = WRITING;
                    break;
                }
                buffer[buffer_pos] = '\0';
                char *ptr = buffer;
                while (*ptr) {
                    while (*ptr && isspace(*ptr)) ptr++;
                    if (*ptr) {
                        word_count++;
                        while (*ptr && !isspace(*ptr)) ptr++;
                    }
                }
                current_state = WRITING;
                break;

            case WRITING:
                if (word_count > 0) {
                    printf("Processed line: \"%s\"\n", buffer);
                    printf("Word count: %d\n", word_count);
                } else {
                    printf("Empty line processed\n");
                }
                current_state = IDLE;
                break;

            case ERROR:
                printf("Error: Invalid input character encountered\n");
                valid_input = 0;
                current_state = DONE;
                break;

            case DONE:
                break;
        }
    }

    if (valid_input) {
        printf("Processing completed successfully\n");
    } else {
        printf("Processing terminated with errors\n");
    }

    return valid_input ? EXIT_SUCCESS : EXIT_FAILURE;
}