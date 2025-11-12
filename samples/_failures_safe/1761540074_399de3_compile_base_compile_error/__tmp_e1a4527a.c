//DeepSeek-V3 V2.5 Category: Safe ; Style: struct_heavy ; Variation: string_processing
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_WORDS 100
#define MAX_WORD_LEN 50
#define MAX_TEXT_LEN 1000

struct WordInfo {
    char word[MAX_WORD_LEN];
    int count;
    int length;
    int is_capitalized;
};

struct TextAnalyzer {
    struct WordInfo words[MAX_WORDS];
    int word_count;
    int total_chars;
    int total_words;
    char processed_text[MAX_TEXT_LEN];
};

void init_analyzer(struct TextAnalyzer *analyzer) {
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

int is_valid_char(char c) {
    return isalpha(c) || isspace(c) || c == '.' || c == ',' || c == '!' || c == '?';
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

int add_word(struct TextAnalyzer *analyzer, const char *word, int length) {
    if (analyzer->word_count >= MAX_WORDS || length <= 0 || length >= MAX_WORD_LEN) return 0;
    
    for (int i = 0; i < analyzer->word_count; i++) {
        if (strncmp(analyzer->words[i].word, word, MAX_WORD_LEN) == 0) {
            analyzer->words[i].count++;
            return 1;
        }
    }
    
    if (safe_strncpy(analyzer->words[analyzer->word_count].word, word, MAX_WORD_LEN)) {
        analyzer->words[analyzer->word_count].count = 1;
        analyzer->words[analyzer->word_count].length = length;
        analyzer->words[analyzer->word_count].is_capitalized = isupper((unsigned char)word[0]) ? 1 : 0;
        analyzer->word_count++;
        return 1;
    }
    return 0;
}

void process_text(struct TextAnalyzer *analyzer, const char *input) {
    if (input == NULL) return;
    
    char buffer[MAX_TEXT_LEN];
    if (!safe_strncpy(buffer, input, MAX_TEXT_LEN)) return;
    
    char current_word[MAX_WORD_LEN];
    int word_len = 0;
    int text_pos = 0;
    
    for (int i = 0; buffer[i] != '\0' && i < MAX_TEXT_LEN - 1 && text_pos < MAX_TEXT_LEN - 1; i++) {
        if (!is_valid_char(buffer[i])) continue;
        
        analyzer->processed_text[text_pos++] = buffer[i];
        analyzer->total_chars++;
        
        if (isalpha((unsigned char)buffer[i])) {
            if (word_len < MAX_WORD_LEN - 1) {
                current_word[word_len++] = buffer[i];
            }
        } else {
            if (word_len > 0) {
                current_word[word_len] = '\0';
                add_word(analyzer, current_word, word_len);
                analyzer->total_words++;
                word_len = 0;
            }
        }
    }
    
    if (word_len > 0) {
        current_word[word_len] = '\0';
        add_word(analyzer, current_word, word_len);
        analyzer->total_words++;
    }
    
    analyzer->processed_text[text_pos] = '\0';
}

void print_analysis(const struct TextAnalyzer *analyzer) {
    printf("Processed text: %s\n", analyzer->processed_text);
    printf("Total characters: %d\n", analyzer->total_chars);
    printf("Total words: %d\n", analyzer->total_words);
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
    
    char input[MAX_TEXT_LEN];
    printf("Enter text to analyze (max %d characters): ", MAX_TEXT_LEN - 1);
    
    if (fgets(input, MAX_TEXT_LEN, stdin) == NULL) {
        printf("Error reading input.\n");
        return 1;
    }
    
    size_t len = strlen(input);
    if (len >