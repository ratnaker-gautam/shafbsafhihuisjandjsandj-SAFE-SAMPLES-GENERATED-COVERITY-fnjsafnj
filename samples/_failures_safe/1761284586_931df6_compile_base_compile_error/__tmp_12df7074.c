//DeepSeek-V3 V2.5 Category: Safe ; Style: struct_heavy ; Variation: string_processing
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_WORD_LEN 64
#define MAX_WORDS 100
#define MAX_TEXT_LEN 1024

struct WordStats {
    char word[MAX_WORD_LEN];
    int count;
    int length;
    int is_capitalized;
};

struct TextAnalyzer {
    struct WordStats words[MAX_WORDS];
    int word_count;
    int total_chars;
    int total_words;
    char processed_text[MAX_TEXT_LEN];
};

void init_analyzer(struct TextAnalyzer *analyzer) {
    if (analyzer == NULL) return;
    analyzer->word_count = 0;
    analyzer->total_chars = 0;
    analyzer->total_words = 0;
    memset(analyzer->processed_text, 0, MAX_TEXT_LEN);
    for (int i = 0; i < MAX_WORDS; i++) {
        memset(analyzer->words[i].word, 0, MAX_WORD_LEN);
        analyzer->words[i].count = 0;
        analyzer->words[i].length = 0;
        analyzer->words[i].is_capitalized = 0;
    }
}

int is_word_char(char c) {
    return isalpha((unsigned char)c) || c == '\'' || c == '-';
}

int add_word(struct TextAnalyzer *analyzer, const char *word, int len) {
    if (analyzer == NULL || word == NULL || len <= 0 || len >= MAX_WORD_LEN || analyzer->word_count >= MAX_WORDS) {
        return 0;
    }
    
    char temp[MAX_WORD_LEN];
    if (len >= MAX_WORD_LEN) {
        return 0;
    }
    strncpy(temp, word, len);
    temp[len] = '\0';
    
    for (int i = 0; i < analyzer->word_count; i++) {
        if (strcmp(analyzer->words[i].word, temp) == 0) {
            analyzer->words[i].count++;
            return 1;
        }
    }
    
    if (analyzer->word_count < MAX_WORDS) {
        strncpy(analyzer->words[analyzer->word_count].word, temp, MAX_WORD_LEN - 1);
        analyzer->words[analyzer->word_count].word[MAX_WORD_LEN - 1] = '\0';
        analyzer->words[analyzer->word_count].count = 1;
        analyzer->words[analyzer->word_count].length = len;
        analyzer->words[analyzer->word_count].is_capitalized = isupper((unsigned char)word[0]) ? 1 : 0;
        analyzer->word_count++;
        return 1;
    }
    
    return 0;
}

void process_text(struct TextAnalyzer *analyzer, const char *text) {
    if (analyzer == NULL || text == NULL) {
        return;
    }
    
    size_t text_len = strlen(text);
    if (text_len >= MAX_TEXT_LEN) {
        return;
    }
    
    strncpy(analyzer->processed_text, text, MAX_TEXT_LEN - 1);
    analyzer->processed_text[MAX_TEXT_LEN - 1] = '\0';
    
    int in_word = 0;
    int word_start = 0;
    
    for (size_t i = 0; i <= text_len; i++) {
        if (is_word_char(text[i]) && !in_word) {
            in_word = 1;
            word_start = i;
        } else if (!is_word_char(text[i]) && in_word) {
            int word_len = i - word_start;
            if (word_len > 0 && word_len < MAX_WORD_LEN) {
                add_word(analyzer, text + word_start, word_len);
                analyzer->total_words++;
                analyzer->total_chars += word_len;
            }
            in_word = 0;
        }
    }
}

void print_analysis(const struct TextAnalyzer *analyzer) {
    if (analyzer == NULL) return;
    printf("Text Analysis Results:\n");
    printf("Processed text: %s\n", analyzer->processed_text);
    printf("Total words: %d\n", analyzer->total_words);
    printf("Total characters in words: %d\n", analyzer->total_chars);
    printf("Unique words: %d\n", analyzer->word_count);
    printf("\nWord frequency:\n");
    
    for (int i = 0; i < analyzer->word_count; i++) {
        printf("'%s': %d occurrences (length: %d, %s)\n",
               analyzer->words[i].word,
               analyzer->words[i].count,
               analyzer->words[i].length,
               analyzer->words[i].is_capitalized ? "capitalized" : "lowercase");
    }
}

int main() {
    struct TextAnalyzer analyzer;
    init_analyzer(&analyzer);
    
    char input_text[MAX_TEXT_LEN];
    printf("Enter text to analyze (max %d characters): ", MAX_TEXT_LEN - 1);
    
    if (fgets(input_text, MAX_TEXT_LEN, stdin) == NULL) {
        printf("Error reading input.\n");
        return 1;
    }
    
    size_t len = strlen(input_text);
    if (len > 0 && input_text[len - 1] == '\n') {
        input_text[len - 1] = '\0';
    }
    
    if (strlen