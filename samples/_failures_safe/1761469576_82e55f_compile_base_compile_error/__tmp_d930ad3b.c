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
    int length;
    double frequency;
};

struct TextAnalyzer {
    struct WordStats words[MAX_WORDS];
    int word_count;
    int total_words;
    char text[MAX_TEXT_LEN];
    int text_length;
};

struct AnalysisResult {
    struct TextAnalyzer analyzer;
    int unique_words;
    char most_frequent_word[MAX_WORD_LEN];
    int max_count;
    double avg_word_length;
};

void init_text_analyzer(struct TextAnalyzer *ta) {
    ta->word_count = 0;
    ta->total_words = 0;
    ta->text_length = 0;
    ta->text[0] = '\0';
    for (int i = 0; i < MAX_WORDS; i++) {
        ta->words[i].word[0] = '\0';
        ta->words[i].count = 0;
        ta->words[i].length = 0;
        ta->words[i].frequency = 0.0;
    }
}

int is_valid_char(char c) {
    return isalnum((unsigned char)c) || c == ' ' || c == '\t' || c == '\n' || c == '.' || c == ',' || c == '!' || c == '?';
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

int process_text(struct TextAnalyzer *ta, const char *input) {
    if (input == NULL) return 0;
    
    size_t input_len = strlen(input);
    if (input_len >= MAX_TEXT_LEN) return 0;
    
    if (!safe_strncpy(ta->text, input, MAX_TEXT_LEN)) return 0;
    ta->text_length = (int)input_len;
    
    char buffer[MAX_WORD_LEN];
    int buf_pos = 0;
    
    for (int i = 0; i <= ta->text_length; i++) {
        char c = ta->text[i];
        
        if (!is_valid_char(c)) continue;
        
        if (isalnum((unsigned char)c)) {
            if (buf_pos < MAX_WORD_LEN - 1) {
                buffer[buf_pos++] = (char)tolower((unsigned char)c);
            }
        } else if (buf_pos > 0) {
            buffer[buf_pos] = '\0';
            ta->total_words++;
            
            int found = 0;
            for (int j = 0; j < ta->word_count; j++) {
                if (strcmp(ta->words[j].word, buffer) == 0) {
                    ta->words[j].count++;
                    found = 1;
                    break;
                }
            }
            
            if (!found && ta->word_count < MAX_WORDS) {
                if (safe_strncpy(ta->words[ta->word_count].word, buffer, MAX_WORD_LEN)) {
                    ta->words[ta->word_count].count = 1;
                    ta->words[ta->word_count].length = buf_pos;
                    ta->word_count++;
                }
            }
            
            buf_pos = 0;
        }
    }
    
    for (int i = 0; i < ta->word_count; i++) {
        if (ta->total_words > 0) {
            ta->words[i].frequency = (double)ta->words[i].count / ta->total_words;
        }
    }
    
    return 1;
}

void analyze_results(struct TextAnalyzer *ta, struct AnalysisResult *result) {
    if (ta == NULL || result == NULL) return;
    
    result->analyzer = *ta;
    result->unique_words = ta->word_count;
    result->max_count = 0;
    result->avg_word_length = 0.0;
    result->most_frequent_word[0] = '\0';
    
    int total_length = 0;
    for (int i = 0; i < ta->word_count; i++) {
        total_length += ta->words[i].length * ta->words[i].count;
        
        if (ta->words[i].count > result->max_count) {
            result->max_count = ta->words[i].count;
            safe_strncpy(result->most_frequent_word, ta->words[i].word, MAX_WORD_LEN);
        }
    }
    
    if (ta->total_words > 0) {
        result->avg_word_length = (double)total_length / ta->total_words;
    }
}

void print_analysis(struct AnalysisResult *result) {
    if (result == NULL) return;
    
    printf("Text Analysis Results:\n");
    printf("Total words: %d\n", result->analyzer.total_words);
    printf("Unique words: %d\n", result->unique_words);
    printf("Most frequent word: '%s' (appears %d times)\n", 
           result->most_frequent_word, result->max_count);
    printf("Average