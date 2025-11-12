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
    int longest_word_len;
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
    return isalnum((unsigned char)c) || c == ' ' || c == '\t' || c == '\n' || c == '.';
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
    if (input_text == NULL || strlen(input_text) >= MAX_TEXT_LEN) return 0;
    
    if (!safe_strncpy(analyzer->processed_text, input_text, sizeof(analyzer->processed_text))) {
        return 0;
    }
    
    char temp_text[MAX_TEXT_LEN];
    if (!safe_strncpy(temp_text, input_text, sizeof(temp_text))) return 0;
    
    char *token = strtok(temp_text, " \t\n.,");
    while (token != NULL && analyzer->word_count < MAX_WORDS) {
        int token_len = strlen(token);
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
                analyzer->words[analyzer->word_count].length = token_len;
                analyzer->word_count++;
            }
            analyzer->total_words++;
        }
        token = strtok(NULL, " \t\n.,");
    }
    
    for (int i = 0; i < analyzer->word_count; i++) {
        if (analyzer->total_words > 0) {
            analyzer->words[i].frequency = (double)analyzer->words[i].count / analyzer->total_words;
        }
    }
    
    return 1;
}

void analyze_results(struct AnalysisResult *result) {
    result->unique_words = result->analyzer.word_count;
    result->longest_word_len = 0;
    int max_count = 0;
    
    for (int i = 0; i < result->analyzer.word_count; i++) {
        if (result->analyzer.words[i].length > result->longest_word_len) {
            result->longest_word_len = result->analyzer.words[i].length;
        }
        if (result->analyzer.words[i].count > max_count) {
            max_count = result->analyzer.words[i].count;
            safe_strncpy(result->most_frequent_word, result->analyzer.words[i].word, MAX_WORD_LEN);
        }
    }
}

void print_analysis(const struct AnalysisResult *result) {
    printf("Text Analysis Results:\n");
    printf("Total words: %d\n", result->analyzer.total_words);
    printf("Unique words: %d\n", result->unique_words);
    printf("Longest word length: %d\n", result->longest_word_len);
    printf("Most frequent word: %s\n", result->most_frequent_word);
    printf("\nWord frequencies:\n");
    
    for (int i = 0; i < result->analyzer.word_count; i++) {
        printf("%-15s: count=%d, frequency=%.4f\n", 
               result->analyzer.words[i].word,
               result->analyzer.words[i].count,
               result->analyzer.words[i].frequency);
    }
}

int main() {
    struct TextAnalyzer analyzer;
    struct AnalysisResult result;
    
    initialize_analyzer(&analyzer);
    
    char input_text[MAX_TEXT_L