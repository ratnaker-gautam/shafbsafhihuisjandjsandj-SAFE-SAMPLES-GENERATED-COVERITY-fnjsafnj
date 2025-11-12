//DeepSeek-V3 V2.5 Category: Safe ; Style: macros ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <ctype.h>

#define STATE_IDLE 0
#define STATE_READING 1
#define STATE_PROCESSING 2
#define STATE_DONE 3
#define STATE_ERROR 4

#define MAX_INPUT_LEN 100
#define MAX_TOKENS 10

#define TRANSITION(from, to) ((from) * 10 + (to))
#define IS_VALID_TRANSITION(t) ((t) >= TRANSITION(STATE_IDLE, STATE_READING) && (t) <= TRANSITION(STATE_PROCESSING, STATE_DONE))

int main(void) {
    char input[MAX_INPUT_LEN + 1];
    char tokens[MAX_TOKENS][MAX_INPUT_LEN + 1];
    int token_count = 0;
    int current_state = STATE_IDLE;
    int valid_transitions[] = {
        TRANSITION(STATE_IDLE, STATE_READING),
        TRANSITION(STATE_READING, STATE_PROCESSING),
        TRANSITION(STATE_PROCESSING, STATE_DONE),
        TRANSITION(STATE_PROCESSING, STATE_ERROR),
        TRANSITION(STATE_ERROR, STATE_IDLE),
        TRANSITION(STATE_DONE, STATE_IDLE)
    };
    int num_transitions = sizeof(valid_transitions) / sizeof(valid_transitions[0]);

    while (1) {
        switch (current_state) {
            case STATE_IDLE:
                printf("State: IDLE\nEnter text (or 'quit' to exit): ");
                if (fgets(input, sizeof(input), stdin) == NULL) {
                    return EXIT_FAILURE;
                }
                input[strcspn(input, "\n")] = '\0';
                if (strlen(input) == 0) {
                    continue;
                }
                if (strcmp(input, "quit") == 0) {
                    return EXIT_SUCCESS;
                }
                current_state = STATE_READING;
                break;

            case STATE_READING:
                printf("State: READING\n");
                token_count = 0;
                char *token = strtok(input, " ");
                while (token != NULL && token_count < MAX_TOKENS) {
                    if (strlen(token) > MAX_INPUT_LEN) {
                        current_state = STATE_ERROR;
                        break;
                    }
                    strncpy(tokens[token_count], token, MAX_INPUT_LEN);
                    tokens[token_count][MAX_INPUT_LEN] = '\0';
                    token_count++;
                    token = strtok(NULL, " ");
                }
                if (current_state != STATE_ERROR) {
                    current_state = STATE_PROCESSING;
                }
                break;

            case STATE_PROCESSING:
                printf("State: PROCESSING\nTokens: ");
                for (int i = 0; i < token_count; i++) {
                    int all_digits = 1;
                    for (size_t j = 0; j < strlen(tokens[i]); j++) {
                        if (!isdigit((unsigned char)tokens[i][j])) {
                            all_digits = 0;
                            break;
                        }
                    }
                    if (all_digits && strlen(tokens[i]) > 0) {
                        printf("[NUM:%s] ", tokens[i]);
                    } else {
                        printf("[TXT:%s] ", tokens[i]);
                    }
                }
                printf("\n");
                current_state = STATE_DONE;
                break;

            case STATE_DONE:
                printf("State: DONE\nProcessing complete.\n\n");
                current_state = STATE_IDLE;
                break;

            case STATE_ERROR:
                printf("State: ERROR\nInput too long or invalid.\n\n");
                current_state = STATE_IDLE;
                break;

            default:
                current_state = STATE_IDLE;
                break;
        }

        int transition_valid = 0;
        int current_transition;
        for (int i = 0; i < num_transitions; i++) {
            current_transition = valid_transitions[i];
            if (current_transition / 10 == current_state) {
                transition_valid = 1;
                break;
            }
        }
        if (!transition_valid) {
            current_state = STATE_IDLE;
        }
    }

    return EXIT_SUCCESS;
}