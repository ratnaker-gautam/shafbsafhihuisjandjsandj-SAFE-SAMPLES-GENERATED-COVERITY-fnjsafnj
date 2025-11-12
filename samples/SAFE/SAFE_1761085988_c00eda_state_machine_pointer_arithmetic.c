//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: pointer_arithmetic ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_INPUT 256
#define MAX_STATES 8

typedef enum {
    STATE_START,
    STATE_READING_WORD,
    STATE_READING_NUMBER,
    STATE_PUNCTUATION,
    STATE_END,
    STATE_ERROR
} parser_state_t;

typedef enum {
    EVENT_CHAR,
    EVENT_SPACE,
    EVENT_PUNCT,
    EVENT_EOF
} event_type_t;

typedef struct {
    int word_count;
    int number_count;
    int punct_count;
} parser_stats_t;

static void process_event(parser_state_t *state, event_type_t event, parser_stats_t *stats) {
    switch (*state) {
        case STATE_START:
            if (event == EVENT_CHAR) {
                *state = STATE_READING_WORD;
                stats->word_count++;
            } else if (event == EVENT_PUNCT) {
                *state = STATE_PUNCTUATION;
                stats->punct_count++;
            }
            break;
            
        case STATE_READING_WORD:
            if (event == EVENT_SPACE) {
                *state = STATE_START;
            } else if (event == EVENT_PUNCT) {
                *state = STATE_PUNCTUATION;
                stats->punct_count++;
            }
            break;
            
        case STATE_READING_NUMBER:
            if (event == EVENT_SPACE) {
                *state = STATE_START;
            } else if (event == EVENT_PUNCT) {
                *state = STATE_PUNCTUATION;
                stats->punct_count++;
            }
            break;
            
        case STATE_PUNCTUATION:
            if (event == EVENT_SPACE) {
                *state = STATE_START;
            } else if (event == EVENT_CHAR) {
                *state = STATE_READING_WORD;
                stats->word_count++;
            }
            break;
            
        default:
            *state = STATE_ERROR;
            break;
    }
}

static event_type_t classify_char(char c) {
    if (c == '\0') return EVENT_EOF;
    if (isspace((unsigned char)c)) return EVENT_SPACE;
    if (isdigit((unsigned char)c)) return EVENT_CHAR;
    if (isalpha((unsigned char)c)) return EVENT_CHAR;
    if (ispunct((unsigned char)c)) return EVENT_PUNCT;
    return EVENT_CHAR;
}

static void analyze_text(const char *text, parser_stats_t *stats) {
    parser_state_t state = STATE_START;
    const char *ptr = text;
    
    memset(stats, 0, sizeof(parser_stats_t));
    
    while (*ptr != '\0' && state != STATE_ERROR) {
        event_type_t event = classify_char(*ptr);
        
        if (isdigit((unsigned char)*ptr) && state == STATE_START) {
            stats->number_count++;
            state = STATE_READING_NUMBER;
        } else {
            process_event(&state, event, stats);
        }
        
        if (ptr - text < MAX_INPUT - 1) {
            ptr++;
        } else {
            break;
        }
    }
}

int main(void) {
    char input[MAX_INPUT + 1];
    parser_stats_t stats;
    
    printf("Enter text to analyze (max %d chars): ", MAX_INPUT);
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    char *newline = strchr(input, '\n');
    if (newline != NULL) {
        *newline = '\0';
    }
    
    if (strlen(input) == 0) {
        fprintf(stderr, "Empty input\n");
        return 1;
    }
    
    analyze_text(input, &stats);
    
    printf("Analysis results:\n");
    printf("Words: %d\n", stats.word_count);
    printf("Numbers: %d\n", stats.number_count);
    printf("Punctuation marks: %d\n", stats.punct_count);
    
    return 0;
}