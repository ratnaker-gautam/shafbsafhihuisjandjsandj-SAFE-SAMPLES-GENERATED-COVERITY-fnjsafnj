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
};

struct TextProcessor {
    char text[MAX_TEXT_LEN];
    struct WordStats words[MAX_WORDS];
    int word_count;
    int total_words;
};

void init_text_processor(struct TextProcessor *tp) {
    memset(tp, 0, sizeof(struct TextProcessor));
    tp->word_count = 0;
    tp->total_words = 0;
}

int is_word_char(char c) {
    return isalnum((unsigned char)c) || c == '_' || c == '-';
}

int process_text(struct TextProcessor *tp, const char *input) {
    if (input == NULL || strlen(input) >= MAX_TEXT_LEN) {
        return 0;
    }
    
    strncpy(tp->text, input, MAX_TEXT_LEN - 1);
    tp->text[MAX_TEXT_LEN - 1] = '\0';
    
    int pos = 0;
    int in_word = 0;
    char current_word[MAX_WORD_LEN] = {0};
    int word_len = 0;
    
    for (int i = 0; tp->text[i] != '\0' && tp->word_count < MAX_WORDS; i++) {
        if (is_word_char(tp->text[i])) {
            if (!in_word) {
                in_word = 1;
                word_len = 0;
                pos = i;
            }
            
            if (word_len < MAX_WORD_LEN - 1) {
                current_word[word_len++] = tolower((unsigned char)tp->text[i]);
            }
        } else {
            if (in_word && word_len > 0) {
                current_word[word_len] = '\0';
                tp->total_words++;
                
                int found = 0;
                for (int j = 0; j < tp->word_count; j++) {
                    if (strcmp(tp->words[j].word, current_word) == 0) {
                        tp->words[j].count++;
                        found = 1;
                        break;
                    }
                }
                
                if (!found && tp->word_count < MAX_WORDS) {
                    strncpy(tp->words[tp->word_count].word, current_word, MAX_WORD_LEN - 1);
                    tp->words[tp->word_count].word[MAX_WORD_LEN - 1] = '\0';
                    tp->words[tp->word_count].count = 1;
                    tp->words[tp->word_count].first_position = pos;
                    tp->word_count++;
                }
                
                in_word = 0;
                word_len = 0;
            }
        }
    }
    
    if (in_word && word_len > 0 && tp->word_count < MAX_WORDS) {
        current_word[word_len] = '\0';
        tp->total_words++;
        
        int found = 0;
        for (int j = 0; j < tp->word_count; j++) {
            if (strcmp(tp->words[j].word, current_word) == 0) {
                tp->words[j].count++;
                found = 1;
                break;
            }
        }
        
        if (!found) {
            strncpy(tp->words[tp->word_count].word, current_word, MAX_WORD_LEN - 1);
            tp->words[tp->word_count].word[MAX_WORD_LEN - 1] = '\0';
            tp->words[tp->word_count].count = 1;
            tp->words[tp->word_count].first_position = pos;
            tp->word_count++;
        }
    }
    
    return 1;
}

void print_word_stats(const struct TextProcessor *tp) {
    printf("Total words processed: %d\n", tp->total_words);
    printf("Unique words: %d\n", tp->word_count);
    printf("\nWord frequency analysis:\n");
    printf("%-20s %-8s %-12s\n", "Word", "Count", "First Pos");
    printf("----------------------------------------\n");
    
    for (int i = 0; i < tp->word_count; i++) {
        printf("%-20s %-8d %-12d\n", 
               tp->words[i].word, 
               tp->words[i].count, 
               tp->words[i].first_position);
    }
}

int main(void) {
    struct TextProcessor processor;
    init_text_processor(&processor);
    
    char input_buffer[MAX_TEXT_LEN];
    
    printf("Enter text to analyze (max %d characters):\n", MAX_TEXT_LEN - 1);
    
    if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(input_buffer);
    if (len > 0 && input_buffer[len - 1] == '\n') {
        input_buffer[len - 1] = '\0';
    }
    
    if (strlen(input_buffer) == 0) {
        fprintf(stderr, "Empty input provided\n");
        return 1;
    }
    
    if (!process_text(&processor, input_buffer)) {
        fprintf(stderr, "Error processing text\n");
        return 1;
    }