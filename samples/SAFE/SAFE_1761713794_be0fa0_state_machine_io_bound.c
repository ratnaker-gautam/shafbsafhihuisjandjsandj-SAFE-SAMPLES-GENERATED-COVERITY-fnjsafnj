//DeepSeek-V3 V2.5 Category: Safe ; Style: io_bound ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_INPUT_LEN 100
#define MAX_STATES 10

typedef enum {
    STATE_START,
    STATE_READING_NUMBER,
    STATE_READING_WORD,
    STATE_ERROR,
    STATE_END
} State;

typedef enum {
    EVENT_DIGIT,
    EVENT_LETTER,
    EVENT_SPACE,
    EVENT_EOF,
    EVENT_INVALID
} Event;

State current_state;
int number_count;
int word_count;

Event get_next_event(FILE *stream) {
    int c = fgetc(stream);
    
    if (c == EOF) {
        return EVENT_EOF;
    }
    
    if (isspace(c)) {
        return EVENT_SPACE;
    }
    
    if (isdigit(c)) {
        return EVENT_DIGIT;
    }
    
    if (isalpha(c)) {
        return EVENT_LETTER;
    }
    
    return EVENT_INVALID;
}

void process_state_machine(FILE *input) {
    current_state = STATE_START;
    number_count = 0;
    word_count = 0;
    
    while (current_state != STATE_END && current_state != STATE_ERROR) {
        Event event = get_next_event(input);
        
        switch (current_state) {
            case STATE_START:
                switch (event) {
                    case EVENT_DIGIT:
                        current_state = STATE_READING_NUMBER;
                        number_count++;
                        break;
                    case EVENT_LETTER:
                        current_state = STATE_READING_WORD;
                        word_count++;
                        break;
                    case EVENT_SPACE:
                        current_state = STATE_START;
                        break;
                    case EVENT_EOF:
                        current_state = STATE_END;
                        break;
                    case EVENT_INVALID:
                        current_state = STATE_ERROR;
                        break;
                }
                break;
                
            case STATE_READING_NUMBER:
                switch (event) {
                    case EVENT_DIGIT:
                        current_state = STATE_READING_NUMBER;
                        break;
                    case EVENT_SPACE:
                        current_state = STATE_START;
                        break;
                    case EVENT_EOF:
                        current_state = STATE_END;
                        break;
                    default:
                        current_state = STATE_ERROR;
                        break;
                }
                break;
                
            case STATE_READING_WORD:
                switch (event) {
                    case EVENT_LETTER:
                        current_state = STATE_READING_WORD;
                        break;
                    case EVENT_SPACE:
                        current_state = STATE_START;
                        break;
                    case EVENT_EOF:
                        current_state = STATE_END;
                        break;
                    default:
                        current_state = STATE_ERROR;
                        break;
                }
                break;
                
            case STATE_ERROR:
                break;
                
            case STATE_END:
                break;
        }
    }
}

int main(void) {
    char input_line[MAX_INPUT_LEN + 1];
    
    printf("Enter text (max %d characters): ", MAX_INPUT_LEN);
    
    if (fgets(input_line, sizeof(input_line), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(input_line);
    if (len > 0 && input_line[len - 1] == '\n') {
        input_line[len - 1] = '\0';
    }
    
    if (len >= MAX_INPUT_LEN) {
        fprintf(stderr, "Input too long\n");
        return 1;
    }
    
    FILE *temp_input = tmpfile();
    if (temp_input == NULL) {
        fprintf(stderr, "Error creating temporary file\n");
        return 1;
    }
    
    if (fputs(input_line, temp_input) == EOF) {
        fprintf(stderr, "Error writing to temporary file\n");
        fclose(temp_input);
        return 1;
    }
    
    if (fseek(temp_input, 0, SEEK_SET) != 0) {
        fprintf(stderr, "Error seeking in temporary file\n");
        fclose(temp_input);
        return 1;
    }
    
    process_state_machine(temp_input);
    
    fclose(temp_input);
    
    if (current_state == STATE_ERROR) {
        printf("Invalid input format detected\n");
        return 1;
    }
    
    printf("Numbers found: %d\n", number_count);
    printf("Words found: %d\n", word_count);
    
    return 0;
}