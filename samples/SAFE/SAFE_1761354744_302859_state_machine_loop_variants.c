//DeepSeek-V3 V2.5 Category: Safe ; Style: loop_variants ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

enum State { START, READING_NUM, READING_ALPHA, ERROR, DONE };

int main(void) {
    char input[256];
    char buffer[256];
    int buf_index = 0;
    enum State current_state = START;
    int i = 0;
    int num_count = 0;
    int alpha_count = 0;

    printf("Enter text (max 255 chars): ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Input error\n");
        return 1;
    }

    input[strcspn(input, "\n")] = '\0';

    while (1) {
        switch (current_state) {
            case START:
                if (input[i] == '\0') {
                    current_state = DONE;
                } else if (isdigit((unsigned char)input[i])) {
                    buffer[buf_index++] = input[i];
                    current_state = READING_NUM;
                } else if (isalpha((unsigned char)input[i])) {
                    buffer[buf_index++] = input[i];
                    current_state = READING_ALPHA;
                } else if (isspace((unsigned char)input[i])) {
                    i++;
                } else {
                    current_state = ERROR;
                }
                break;

            case READING_NUM:
                if (input[i] == '\0') {
                    buffer[buf_index] = '\0';
                    printf("Found number: %s\n", buffer);
                    num_count++;
                    buf_index = 0;
                    current_state = DONE;
                } else if (isdigit((unsigned char)input[i])) {
                    if (buf_index < 255) {
                        buffer[buf_index++] = input[i];
                    }
                    i++;
                } else if (isspace((unsigned char)input[i])) {
                    buffer[buf_index] = '\0';
                    printf("Found number: %s\n", buffer);
                    num_count++;
                    buf_index = 0;
                    current_state = START;
                    i++;
                } else {
                    current_state = ERROR;
                }
                break;

            case READING_ALPHA:
                if (input[i] == '\0') {
                    buffer[buf_index] = '\0';
                    printf("Found word: %s\n", buffer);
                    alpha_count++;
                    buf_index = 0;
                    current_state = DONE;
                } else if (isalpha((unsigned char)input[i])) {
                    if (buf_index < 255) {
                        buffer[buf_index++] = input[i];
                    }
                    i++;
                } else if (isspace((unsigned char)input[i])) {
                    buffer[buf_index] = '\0';
                    printf("Found word: %s\n", buffer);
                    alpha_count++;
                    buf_index = 0;
                    current_state = START;
                    i++;
                } else {
                    current_state = ERROR;
                }
                break;

            case ERROR:
                fprintf(stderr, "Invalid character at position %d\n", i);
                return 1;

            case DONE:
                printf("Processing complete.\n");
                printf("Numbers found: %d\n", num_count);
                printf("Words found: %d\n", alpha_count);
                return 0;
        }

        if (buf_index >= 255) {
            fprintf(stderr, "Buffer overflow\n");
            return 1;
        }
    }

    return 0;
}