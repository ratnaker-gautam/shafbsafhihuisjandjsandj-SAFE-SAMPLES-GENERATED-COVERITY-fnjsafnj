//DeepSeek-V3 V2.5 Category: Safe ; Style: monolithic_main ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

enum state {
    INIT,
    READ_CHAR,
    PROCESS_WORD,
    COUNT_WORD,
    FINALIZE,
    ERROR
};

int main(void) {
    enum state current_state = INIT;
    char buffer[256];
    int buffer_index = 0;
    int word_count = 0;
    int char_count = 0;
    int c;
    int valid_input = 1;

    while (current_state != FINALIZE && current_state != ERROR) {
        switch (current_state) {
            case INIT:
                printf("Enter text (max 255 chars, end with Ctrl+D): ");
                buffer_index = 0;
                word_count = 0;
                char_count = 0;
                current_state = READ_CHAR;
                break;

            case READ_CHAR:
                c = getchar();
                if (c == EOF) {
                    current_state = FINALIZE;
                } else if (buffer_index < 255) {
                    buffer[buffer_index++] = (char)c;
                    char_count++;
                    if (isspace(c)) {
                        current_state = PROCESS_WORD;
                    }
                } else {
                    current_state = ERROR;
                }
                break;

            case PROCESS_WORD:
                if (buffer_index > 0 && !isspace(buffer[buffer_index - 1])) {
                    current_state = COUNT_WORD;
                } else {
                    current_state = READ_CHAR;
                }
                break;

            case COUNT_WORD:
                word_count++;
                current_state = READ_CHAR;
                break;

            case FINALIZE:
                if (buffer_index > 0 && !isspace(buffer[buffer_index - 1])) {
                    word_count++;
                }
                if (valid_input && char_count <= 255) {
                    buffer[buffer_index] = '\0';
                    printf("Text: %s\n", buffer);
                    printf("Character count: %d\n", char_count);
                    printf("Word count: %d\n", word_count);
                } else {
                    printf("Error: Invalid input\n");
                }
                break;

            case ERROR:
                printf("Error: Input too long\n");
                valid_input = 0;
                current_state = FINALIZE;
                break;
        }
    }

    return valid_input ? 0 : 1;
}