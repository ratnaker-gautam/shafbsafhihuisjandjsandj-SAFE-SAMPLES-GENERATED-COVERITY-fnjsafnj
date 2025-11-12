//DeepSeek-V3 V2.5 Category: Safe ; Style: struct_heavy ; Variation: string_processing
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_WORD_LEN 64
#define MAX_WORDS 1000

struct WordStats {
    char word[MAX_WORD_LEN];
    int count;
    int length;
    int is_capitalized;
};

struct TextAnalyzer {
    struct WordStats words[MAX_WORDS];
    int word_count;
    int total_words;
    int longest_word_length;
    int shortest_word_length;
};

void init_analyzer(struct TextAnalyzer *analyzer) {
    analyzer->word_count = 0;
    analyzer->total_words = 0;
    analyzer->longest_word_length = 0;
    analyzer->shortest_word_length = MAX_WORD_LEN;
}

int is_valid_char(char c) {
    return isalpha((unsigned char)c) || c == ' ' || c == '\t' || c == '\n';
}

int process_word(struct TextAnalyzer *analyzer, const char *word, int len) {
    if (len <= 0 || len >= MAX_WORD_LEN) return 0;
    
    for (int i = 0; i < len; i++) {
        if (!isalpha((unsigned char)word[i])) return 0;
    }
    
    char normalized[MAX_WORD_LEN];
    int is_cap = isupper((unsigned char)word[0]) ? 1 : 0;
    
    for (int i = 0; i < len; i++) {
        normalized[i] = tolower((unsigned char)word[i]);
    }
    normalized[len] = '\0';
    
    for (int i = 0; i < analyzer->word_count; i++) {
        if (strncmp(analyzer->words[i].word, normalized, MAX_WORD_LEN) == 0) {
            analyzer->words[i].count++;
            return 1;
        }
    }
    
    if (analyzer->word_count >= MAX_WORDS) return 0;
    
    strncpy(analyzer->words[analyzer->word_count].word, normalized, MAX_WORD_LEN - 1);
    analyzer->words[analyzer->word_count].word[MAX_WORD_LEN - 1] = '\0';
    analyzer->words[analyzer->word_count].count = 1;
    analyzer->words[analyzer->word_count].length = len;
    analyzer->words[analyzer->word_count].is_capitalized = is_cap;
    
    if (len > analyzer->longest_word_length) analyzer->longest_word_length = len;
    if (len < analyzer->shortest_word_length) analyzer->shortest_word_length = len;
    
    analyzer->word_count++;
    return 1;
}

int analyze_text(struct TextAnalyzer *analyzer, const char *text) {
    if (!text) return 0;
    
    size_t text_len = strlen(text);
    if (text_len == 0 || text_len > 10000) return 0;
    
    for (size_t i = 0; i < text_len; i++) {
        if (!is_valid_char(text[i])) return 0;
    }
    
    char buffer[MAX_WORD_LEN];
    int buf_pos = 0;
    
    for (size_t i = 0; i <= text_len; i++) {
        char c = text[i];
        
        if (isalpha((unsigned char)c)) {
            if (buf_pos < MAX_WORD_LEN - 1) {
                buffer[buf_pos++] = c;
            }
        } else if (buf_pos > 0) {
            buffer[buf_pos] = '\0';
            if (!process_word(analyzer, buffer, buf_pos)) return 0;
            analyzer->total_words++;
            buf_pos = 0;
        }
    }
    
    return 1;
}

void print_analysis(const struct TextAnalyzer *analyzer) {
    if (!analyzer || analyzer->word_count == 0) {
        printf("No words to analyze.\n");
        return;
    }
    
    printf("Text Analysis Results:\n");
    printf("Total unique words: %d\n", analyzer->word_count);
    printf("Total word occurrences: %d\n", analyzer->total_words);
    printf("Longest word length: %d\n", analyzer->longest_word_length);
    printf("Shortest word length: %d\n", analyzer->shortest_word_length);
    printf("\nWord frequencies:\n");
    
    for (int i = 0; i < analyzer->word_count; i++) {
        printf("%-15s: %d\n", analyzer->words[i].word, analyzer->words[i].count);
    }
}

int main(void) {
    struct TextAnalyzer analyzer;
    init_analyzer(&analyzer);
    
    char input_text[10001];
    printf("Enter text to analyze (max 10000 characters):\n");
    
    if (!fgets(input_text, sizeof(input_text), stdin)) {
        printf("Error reading input.\n");
        return 1;
    }
    
    size_t len = strlen(input_text);
    if (len > 0 && input_text[len - 1] == '\n') {
        input_text[len - 1] = '\0';
    }
    
    if (strlen(input_text) == 0) {
        printf("No input provided.\n");
        return 1;
    }
    
    if (!analyze_text(&analyzer, input_text)) {
        printf("Error analyzing text. Text may contain invalid characters.\n");
        return 1;