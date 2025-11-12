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

void initialize_analyzer(struct TextAnalyzer *analyzer) {
    analyzer->word_count = 0;
    analyzer->total_words = 0;
    analyzer->text_length = 0;
    memset(analyzer->text, 0, MAX_TEXT_LEN);
    for (int i = 0; i < MAX_WORDS; i++) {
        memset(analyzer->words[i].word, 0, MAX_WORD_LEN);
        analyzer->words[i].count = 0;
        analyzer->words[i].length = 0;
        analyzer->words[i].frequency = 0.0;
    }
}

int is_valid_char(char c) {
    return isalnum((unsigned char)c) || c == ' ' || c == '\n' || c == '\t' || c == '.' || c == ',' || c == '!' || c == '?';
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

int process_text(struct TextAnalyzer *analyzer, const char *input) {
    if (input == NULL) return 0;
    
    size_t input_len = strlen(input);
    if (input_len >= MAX_TEXT_LEN - 1) return 0;
    
    if (!safe_strncpy(analyzer->text, input, MAX_TEXT_LEN)) return 0;
    analyzer->text_length = (int)input_len;
    
    char buffer[MAX_WORD_LEN];
    int buf_pos = 0;
    int in_word = 0;
    
    for (int i = 0; i <= analyzer->text_length; i++) {
        char c = analyzer->text[i];
        
        if (!is_valid_char(c)) continue;
        
        if (isalnum((unsigned char)c)) {
            if (buf_pos < MAX_WORD_LEN - 1) {
                buffer[buf_pos++] = (char)tolower((unsigned char)c);
                in_word = 1;
            }
        } else if (in_word) {
            buffer[buf_pos] = '\0';
            
            if (buf_pos > 0 && analyzer->word_count < MAX_WORDS) {
                int found = 0;
                for (int j = 0; j < analyzer->word_count; j++) {
                    if (strcmp(analyzer->words[j].word, buffer) == 0) {
                        analyzer->words[j].count++;
                        found = 1;
                        break;
                    }
                }
                
                if (!found) {
                    if (safe_strncpy(analyzer->words[analyzer->word_count].word, buffer, MAX_WORD_LEN)) {
                        analyzer->words[analyzer->word_count].count = 1;
                        analyzer->words[analyzer->word_count].length = buf_pos;
                        analyzer->word_count++;
                    }
                }
                
                analyzer->total_words++;
            }
            
            buf_pos = 0;
            in_word = 0;
        }
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
    
    int total_length = 0;
    for (int i = 0; i < analyzer->word_count; i++) {
        total_length += analyzer->words[i].length * analyzer->words[i].count;
        
        if (analyzer->words[i].count > result->max_count) {
            result->max_count = analyzer->words[i].count;
            safe_strncpy(result->most_frequent_word, analyzer->words[i].word, MAX_WORD_LEN);
        }
    }
    
    if (analyzer->total_words > 0) {
        result->avg_word_length = (double)total_length / analyzer->total_words;
    }
}

void print_analysis(const struct AnalysisResult *result) {
    if (result == NULL) return;
    
    printf("Text Analysis Results:\n");
    printf("Total words: %d\n", result->analyzer.total_words);
    printf("Unique words