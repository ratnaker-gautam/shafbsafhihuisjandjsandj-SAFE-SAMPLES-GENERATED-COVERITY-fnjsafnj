//DeepSeek-V3 V2.5 Category: Safe ; Style: struct_heavy ; Variation: string_processing
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_WORD_LEN 64
#define MAX_WORDS 1000
#define MAX_TEXT_LEN 10000

struct WordStats {
    char word[MAX_WORD_LEN];
    int count;
    int first_position;
    int last_position;
};

struct TextProcessor {
    char text[MAX_TEXT_LEN];
    struct WordStats words[MAX_WORDS];
    int word_count;
    int total_words;
};

void init_text_processor(struct TextProcessor *tp) {
    memset(tp->text, 0, sizeof(tp->text));
    memset(tp->words, 0, sizeof(tp->words));
    tp->word_count = 0;
    tp->total_words = 0;
}

int is_word_char(char c) {
    return isalnum((unsigned char)c) || c == '_' || c == '-';
}

int process_text(struct TextProcessor *tp, const char *input) {
    if (input == NULL || strlen(input) >= MAX_TEXT_LEN - 1) {
        return 0;
    }
    
    strncpy(tp->text, input, MAX_TEXT_LEN - 1);
    tp->text[MAX_TEXT_LEN - 1] = '\0';
    
    char buffer[MAX_WORD_LEN];
    int buf_pos = 0;
    int word_position = 0;
    
    for (int i = 0; tp->text[i] != '\0' && tp->word_count < MAX_WORDS; i++) {
        if (is_word_char(tp->text[i])) {
            if (buf_pos < MAX_WORD_LEN - 1) {
                buffer[buf_pos++] = tolower((unsigned char)tp->text[i]);
            }
        } else {
            if (buf_pos > 0) {
                buffer[buf_pos] = '\0';
                word_position++;
                tp->total_words++;
                
                int found = -1;
                for (int j = 0; j < tp->word_count; j++) {
                    if (strcmp(tp->words[j].word, buffer) == 0) {
                        found = j;
                        break;
                    }
                }
                
                if (found >= 0) {
                    tp->words[found].count++;
                    tp->words[found].last_position = word_position;
                } else {
                    if (tp->word_count < MAX_WORDS) {
                        strncpy(tp->words[tp->word_count].word, buffer, MAX_WORD_LEN - 1);
                        tp->words[tp->word_count].word[MAX_WORD_LEN - 1] = '\0';
                        tp->words[tp->word_count].count = 1;
                        tp->words[tp->word_count].first_position = word_position;
                        tp->words[tp->word_count].last_position = word_position;
                        tp->word_count++;
                    }
                }
                buf_pos = 0;
            }
        }
    }
    
    if (buf_pos > 0 && tp->word_count < MAX_WORDS) {
        buffer[buf_pos] = '\0';
        word_position++;
        tp->total_words++;
        
        int found = -1;
        for (int j = 0; j < tp->word_count; j++) {
            if (strcmp(tp->words[j].word, buffer) == 0) {
                found = j;
                break;
            }
        }
        
        if (found >= 0) {
            tp->words[found].count++;
            tp->words[found].last_position = word_position;
        } else {
            strncpy(tp->words[tp->word_count].word, buffer, MAX_WORD_LEN - 1);
            tp->words[tp->word_count].word[MAX_WORD_LEN - 1] = '\0';
            tp->words[tp->word_count].count = 1;
            tp->words[tp->word_count].first_position = word_position;
            tp->words[tp->word_count].last_position = word_position;
            tp->word_count++;
        }
    }
    
    return 1;
}

void print_word_stats(const struct TextProcessor *tp) {
    printf("Total words processed: %d\n", tp->total_words);
    printf("Unique words found: %d\n", tp->word_count);
    printf("\nWord statistics:\n");
    printf("%-20s %-8s %-12s %-12s\n", "Word", "Count", "First Pos", "Last Pos");
    printf("------------------------------------------------\n");
    
    for (int i = 0; i < tp->word_count; i++) {
        printf("%-20s %-8d %-12d %-12d\n", 
               tp->words[i].word, 
               tp->words[i].count, 
               tp->words[i].first_position, 
               tp->words[i].last_position);
    }
}

int main(void) {
    struct TextProcessor processor;
    char input_buffer[MAX_TEXT_LEN];
    
    init_text_processor(&processor);
    
    printf("Enter text to analyze (max %d characters):\n", MAX_TEXT_LEN - 1);
    
    if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) {
        printf("Error reading input.\n");
        return 1;
    }
    
    size_t len = strlen(input_buffer);
    if (len > 0 && input_buffer[len - 1] == '\n') {
        input_buffer[len -