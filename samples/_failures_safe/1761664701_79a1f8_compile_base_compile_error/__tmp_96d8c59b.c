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
    char processed_text[MAX_TEXT_LEN];
    int text_length;
};

struct AnalysisResult {
    struct TextAnalyzer analyzer;
    int unique_words;
    char most_frequent_word[MAX_WORD_LEN];
    int max_count;
    double avg_word_length;
};

void init_text_analyzer(struct TextAnalyzer *analyzer) {
    analyzer->word_count = 0;
    analyzer->total_words = 0;
    analyzer->text_length = 0;
    analyzer->processed_text[0] = '\0';
    for (int i = 0; i < MAX_WORDS; i++) {
        analyzer->words[i].word[0] = '\0';
        analyzer->words[i].count = 0;
        analyzer->words[i].length = 0;
        analyzer->words[i].frequency = 0.0;
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

int process_text(struct TextAnalyzer *analyzer, const char *input_text) {
    if (analyzer == NULL || input_text == NULL) return 0;
    
    size_t input_len = strlen(input_text);
    if (input_len >= MAX_TEXT_LEN) return 0;
    
    char cleaned_text[MAX_TEXT_LEN];
    size_t clean_index = 0;
    
    for (size_t i = 0; i < input_len && clean_index < MAX_TEXT_LEN - 1; i++) {
        if (is_valid_char(input_text[i])) {
            cleaned_text[clean_index++] = tolower((unsigned char)input_text[i]);
        } else if (clean_index > 0 && cleaned_text[clean_index-1] != ' ') {
            cleaned_text[clean_index++] = ' ';
        }
    }
    cleaned_text[clean_index] = '\0';
    
    if (!safe_strncpy(analyzer->processed_text, cleaned_text, MAX_TEXT_LEN)) return 0;
    analyzer->text_length = (int)strlen(analyzer->processed_text);
    
    char temp_text[MAX_TEXT_LEN];
    if (!safe_strncpy(temp_text, cleaned_text, MAX_TEXT_LEN)) return 0;
    
    char *token = strtok(temp_text, " \t\n");
    while (token != NULL && analyzer->word_count < MAX_WORDS) {
        size_t token_len = strlen(token);
        if (token_len > 0 && token_len < MAX_WORD_LEN) {
            int found = 0;
            for (int i = 0; i < analyzer->word_count; i++) {
                if (strcmp(analyzer->words[i].word, token) == 0) {
                    analyzer->words[i].count++;
                    found = 1;
                    break;
                }
            }
            
            if (!found) {
                if (!safe_strncpy(analyzer->words[analyzer->word_count].word, token, MAX_WORD_LEN)) {
                    return 0;
                }
                analyzer->words[analyzer->word_count].count = 1;
                analyzer->words[analyzer->word_count].length = (int)token_len;
                analyzer->word_count++;
            }
            analyzer->total_words++;
        }
        token = strtok(NULL, " \t\n");
    }
    
    for (int i = 0; i < analyzer->word_count; i++) {
        if (analyzer->total_words > 0) {
            analyzer->words[i].frequency = (double)analyzer->words[i].count / analyzer->total_words;
        }
    }
    
    return 1;
}

void analyze_results(struct TextAnalyzer *analyzer, struct AnalysisResult *result) {
    if (analyzer == NULL || result == NULL) return;
    
    result->analyzer = *analyzer;
    result->unique_words = analyzer->word_count;
    result->max_count = 0;
    result->avg_word_length = 0.0;
    result->most_frequent_word[0] = '\0';
    
    if (analyzer->word_count == 0) return;
    
    int total_length = 0;
    for (int i = 0; i < analyzer->word_count; i++) {
        total_length += analyzer->words[i].length * analyzer->words[i].count;
        
        if (analyzer->words[i].count > result->max_count) {
            result->max_count = analyzer->words[i].count;
            safe_strncpy(result->most_frequent_word, analyzer->words[i].word, MAX_WORD_LEN);
        }
    }
    
    if (analyzer->total_words > 0