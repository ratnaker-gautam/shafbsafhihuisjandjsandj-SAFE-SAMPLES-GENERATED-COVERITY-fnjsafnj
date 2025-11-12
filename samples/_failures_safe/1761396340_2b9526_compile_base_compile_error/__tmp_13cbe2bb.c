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
    struct WordStats words[MAX_WORDS];
    int word_count;
    char processed_text[MAX_TEXT_LEN];
    int text_length;
    int total_words;
};

struct AnalysisResult {
    struct TextProcessor processor;
    int unique_words;
    int most_frequent_count;
    char most_frequent_word[MAX_WORD_LEN];
};

void init_text_processor(struct TextProcessor *tp) {
    tp->word_count = 0;
    tp->text_length = 0;
    tp->total_words = 0;
    memset(tp->processed_text, 0, sizeof(tp->processed_text));
    for (int i = 0; i < MAX_WORDS; i++) {
        memset(tp->words[i].word, 0, sizeof(tp->words[i].word));
        tp->words[i].count = 0;
        tp->words[i].first_position = -1;
        tp->words[i].last_position = -1;
    }
}

int is_valid_char(char c) {
    return isalnum((unsigned char)c) || c == ' ' || c == '\t' || c == '\n';
}

int safe_strncpy(char *dest, const char *src, size_t n) {
    if (dest == NULL || src == NULL || n == 0) return 0;
    size_t i;
    for (i = 0; i < n - 1 && src[i] != '\0'; i++) {
        dest[i] = src[i];
    }
    dest[i] = '\0';
    return 1;
}

int process_input_text(struct TextProcessor *tp, const char *input) {
    if (input == NULL || tp == NULL) return 0;
    
    size_t input_len = strlen(input);
    if (input_len == 0 || input_len >= MAX_TEXT_LEN) return 0;
    
    for (size_t i = 0; i < input_len; i++) {
        if (!is_valid_char(input[i])) return 0;
    }
    
    if (!safe_strncpy(tp->processed_text, input, sizeof(tp->processed_text))) return 0;
    tp->text_length = (int)input_len;
    
    char temp_text[MAX_TEXT_LEN];
    if (!safe_strncpy(temp_text, input, sizeof(temp_text))) return 0;
    
    char *token = strtok(temp_text, " \t\n");
    int position = 0;
    
    while (token != NULL && tp->word_count < MAX_WORDS) {
        size_t token_len = strlen(token);
        if (token_len > 0 && token_len < MAX_WORD_LEN) {
            int found = 0;
            for (int i = 0; i < tp->word_count; i++) {
                if (strcmp(tp->words[i].word, token) == 0) {
                    tp->words[i].count++;
                    tp->words[i].last_position = position;
                    found = 1;
                    break;
                }
            }
            
            if (!found) {
                if (!safe_strncpy(tp->words[tp->word_count].word, token, MAX_WORD_LEN)) {
                    return 0;
                }
                tp->words[tp->word_count].count = 1;
                tp->words[tp->word_count].first_position = position;
                tp->words[tp->word_count].last_position = position;
                tp->word_count++;
            }
            
            position++;
            tp->total_words++;
        }
        token = strtok(NULL, " \t\n");
    }
    
    return 1;
}

void analyze_text(struct TextProcessor *tp, struct AnalysisResult *result) {
    if (tp == NULL || result == NULL) return;
    
    result->processor = *tp;
    result->unique_words = tp->word_count;
    result->most_frequent_count = 0;
    memset(result->most_frequent_word, 0, sizeof(result->most_frequent_word));
    
    for (int i = 0; i < tp->word_count; i++) {
        if (tp->words[i].count > result->most_frequent_count) {
            result->most_frequent_count = tp->words[i].count;
            safe_strncpy(result->most_frequent_word, tp->words[i].word, MAX_WORD_LEN);
        }
    }
}

void print_analysis(const struct AnalysisResult *result) {
    if (result == NULL) return;
    
    printf("Text Analysis Results:\n");
    printf("Total words: %d\n", result->processor.total_words);
    printf("Unique words: %d\n", result->unique_words);
    printf("Most frequent word: '%s' (appears %d times)\n", 
           result->most_frequent_word, result->most_frequent_count);
    
    printf("\nWord details:\n");
    for (int i = 0; i < result->processor.word_count; i++) {
        printf("'%s': count=%d, first=%d, last=%d\n",
               result->processor.words[i].word,
               result->processor.words[i].count,
               result->processor.words[i].first_position,
               result->processor