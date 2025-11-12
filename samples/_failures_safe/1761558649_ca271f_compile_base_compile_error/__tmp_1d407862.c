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
};

struct AnalysisResult {
    struct TextAnalyzer analyzer;
    int unique_words;
    int longest_word_length;
    char most_frequent_word[MAX_WORD_LEN];
};

void initialize_analyzer(struct TextAnalyzer *analyzer) {
    analyzer->word_count = 0;
    analyzer->total_words = 0;
    memset(analyzer->processed_text, 0, sizeof(analyzer->processed_text));
    for (int i = 0; i < MAX_WORDS; i++) {
        memset(analyzer->words[i].word, 0, sizeof(analyzer->words[i].word));
        analyzer->words[i].count = 0;
        analyzer->words[i].length = 0;
        analyzer->words[i].frequency = 0.0;
    }
}

int is_valid_char(char c) {
    return isalnum((unsigned char)c) || c == ' ' || c == '\t' || c == '\n';
}

int sanitize_text(const char *input, char *output, size_t max_len) {
    if (input == NULL || output == NULL || max_len == 0) {
        return 0;
    }
    
    size_t input_len = strlen(input);
    if (input_len >= max_len) {
        return 0;
    }
    
    size_t j = 0;
    for (size_t i = 0; i < input_len && j < max_len - 1; i++) {
        if (is_valid_char(input[i])) {
            output[j++] = tolower((unsigned char)input[i]);
        }
    }
    output[j] = '\0';
    return 1;
}

int add_word(struct TextAnalyzer *analyzer, const char *word) {
    if (analyzer == NULL || word == NULL || strlen(word) == 0) {
        return 0;
    }
    
    if (analyzer->word_count >= MAX_WORDS) {
        return 0;
    }
    
    for (int i = 0; i < analyzer->word_count; i++) {
        if (strcmp(analyzer->words[i].word, word) == 0) {
            analyzer->words[i].count++;
            return 1;
        }
    }
    
    size_t word_len = strlen(word);
    if (word_len >= MAX_WORD_LEN) {
        return 0;
    }
    
    strncpy(analyzer->words[analyzer->word_count].word, word, MAX_WORD_LEN - 1);
    analyzer->words[analyzer->word_count].word[MAX_WORD_LEN - 1] = '\0';
    analyzer->words[analyzer->word_count].count = 1;
    analyzer->words[analyzer->word_count].length = (int)word_len;
    analyzer->word_count++;
    return 1;
}

int process_text(struct TextAnalyzer *analyzer, const char *text) {
    if (analyzer == NULL || text == NULL) {
        return 0;
    }
    
    if (!sanitize_text(text, analyzer->processed_text, sizeof(analyzer->processed_text))) {
        return 0;
    }
    
    char buffer[MAX_TEXT_LEN];
    strncpy(buffer, analyzer->processed_text, sizeof(buffer) - 1);
    buffer[sizeof(buffer) - 1] = '\0';
    
    char *token = strtok(buffer, " \t\n");
    while (token != NULL && analyzer->total_words < MAX_WORDS) {
        if (strlen(token) > 0) {
            if (add_word(analyzer, token)) {
                analyzer->total_words++;
            }
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
    if (analyzer == NULL || result == NULL) {
        return;
    }
    
    result->analyzer = *analyzer;
    result->unique_words = analyzer->word_count;
    result->longest_word_length = 0;
    memset(result->most_frequent_word, 0, sizeof(result->most_frequent_word));
    
    int max_count = 0;
    for (int i = 0; i < analyzer->word_count; i++) {
        if (analyzer->words[i].length > result->longest_word_length) {
            result->longest_word_length = analyzer->words[i].length;
        }
        
        if (analyzer->words[i].count > max_count) {
            max_count = analyzer->words[i].count;
            strncpy(result->most_frequent_word, analyzer->words[i].word, MAX_WORD_LEN - 1);
            result->most_frequent_word[MAX_WORD_LEN - 1] = '\