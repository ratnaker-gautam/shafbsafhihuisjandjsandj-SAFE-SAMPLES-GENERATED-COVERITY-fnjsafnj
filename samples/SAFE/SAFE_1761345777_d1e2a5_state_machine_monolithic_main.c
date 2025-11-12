//DeepSeek-V3 V2.5 Category: Safe ; Style: monolithic_main ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

enum State {
    START,
    READING_NUMBER,
    READING_WORD,
    SKIPPING_SPACE,
    FINISHED,
    ERROR
};

int main(void) {
    char input[1024];
    char buffer[1024];
    int buffer_index = 0;
    enum State current_state = START;
    int number_sum = 0;
    int word_count = 0;
    int number_count = 0;
    int i = 0;
    int input_len = 0;

    printf("Enter text (max 1023 chars): ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }

    input_len = strlen(input);
    if (input_len > 0 && input[input_len - 1] == '\n') {
        input[input_len - 1] = '\0';
        input_len--;
    }

    if (input_len >= sizeof(input)) {
        fprintf(stderr, "Input too long\n");
        return 1;
    }

    while (i <= input_len && current_state != FINISHED && current_state != ERROR) {
        char c = (i < input_len) ? input[i] : '\0';

        switch (current_state) {
            case START:
                if (c == '\0') {
                    current_state = FINISHED;
                } else if (isdigit((unsigned char)c)) {
                    buffer_index = 0;
                    if (buffer_index < sizeof(buffer) - 1) {
                        buffer[buffer_index++] = c;
                    }
                    current_state = READING_NUMBER;
                } else if (isalpha((unsigned char)c)) {
                    buffer_index = 0;
                    if (buffer_index < sizeof(buffer) - 1) {
                        buffer[buffer_index++] = c;
                    }
                    current_state = READING_WORD;
                } else if (isspace((unsigned char)c)) {
                    current_state = SKIPPING_SPACE;
                } else {
                    current_state = ERROR;
                }
                i++;
                break;

            case READING_NUMBER:
                if (isdigit((unsigned char)c)) {
                    if (buffer_index < sizeof(buffer) - 1) {
                        buffer[buffer_index++] = c;
                    }
                    i++;
                } else if (c == '\0' || isspace((unsigned char)c)) {
                    buffer[buffer_index] = '\0';
                    int num = atoi(buffer);
                    number_sum += num;
                    number_count++;
                    current_state = (c == '\0') ? FINISHED : SKIPPING_SPACE;
                    i++;
                } else {
                    current_state = ERROR;
                }
                break;

            case READING_WORD:
                if (isalnum((unsigned char)c)) {
                    if (buffer_index < sizeof(buffer) - 1) {
                        buffer[buffer_index++] = c;
                    }
                    i++;
                } else if (c == '\0' || isspace((unsigned char)c)) {
                    buffer[buffer_index] = '\0';
                    word_count++;
                    current_state = (c == '\0') ? FINISHED : SKIPPING_SPACE;
                    i++;
                } else {
                    current_state = ERROR;
                }
                break;

            case SKIPPING_SPACE:
                if (isspace((unsigned char)c)) {
                    i++;
                } else if (c == '\0') {
                    current_state = FINISHED;
                } else {
                    current_state = START;
                }
                break;

            case FINISHED:
                break;

            case ERROR:
                break;
        }
    }

    if (current_state == ERROR) {
        fprintf(stderr, "Error: Invalid input format\n");
        return 1;
    }

    printf("Words: %d\n", word_count);
    printf("Numbers: %d\n", number_count);
    if (number_count > 0) {
        printf("Number sum: %d\n", number_sum);
    }

    return 0;
}