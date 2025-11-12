//DeepSeek-V3 V2.5 Category: Safe ; Style: struct_heavy ; Variation: string_processing
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_WORDS 100
#define MAX_WORD_LEN 50
#define MAX_TEXT_LEN 1000

struct WordStats {
    char word[MAX_WORD_LEN];
    int count;
    int length;
    int is_capitalized;
};

struct TextAnalyzer {
    struct WordStats words[MAX_WORDS];
    int word_count;
    int total_characters;
    int total_words;
    char processed_text[MAX_TEXT_LEN];
};

void init_analyzer(struct TextAnalyzer *analyzer) {
    analyzer->word_count = 0;
    analyzer->total_characters = 0;
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
    return isalpha(c) || c == ' ' || c == '.' || c == ',' || c == '!' || c == '?';
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

void process_word(struct TextAnalyzer *analyzer, const char *word, int len) {
    if (len <= 0 || len >= MAX_WORD_LEN) return;
    
    char clean_word[MAX_WORD_LEN];
    int clean_len = 0;
    int capitalized = isupper(word[0]) ? 1 : 0;
    
    for (int i = 0; i < len && clean_len < MAX_WORD_LEN - 1; i++) {
        if (isalpha(word[i])) {
            clean_word[clean_len++] = tolower(word[i]);
        }
    }
    clean_word[clean_len] = '\0';
    
    if (clean_len == 0) return;
    
    for (int i = 0; i < analyzer->word_count; i++) {
        if (strcmp(analyzer->words[i].word, clean_word) == 0) {
            analyzer->words[i].count++;
            if (capitalized) analyzer->words[i].is_capitalized = 1;
            return;
        }
    }
    
    if (analyzer->word_count < MAX_WORDS) {
        safe_strncpy(analyzer->words[analyzer->word_count].word, clean_word, MAX_WORD_LEN);
        analyzer->words[analyzer->word_count].count = 1;
        analyzer->words[analyzer->word_count].length = clean_len;
        analyzer->words[analyzer->word_count].is_capitalized = capitalized;
        analyzer->word_count++;
    }
}

int process_text(struct TextAnalyzer *analyzer, const char *text) {
    if (text == NULL) return 0;
    
    size_t text_len = strlen(text);
    if (text_len == 0 || text_len >= MAX_TEXT_LEN) return 0;
    
    for (size_t i = 0; i < text_len; i++) {
        if (!is_valid_char(text[i])) return 0;
    }
    
    safe_strncpy(analyzer->processed_text, text, MAX_TEXT_LEN);
    analyzer->total_characters = (int)text_len;
    
    char buffer[MAX_WORD_LEN];
    int buf_pos = 0;
    int in_word = 0;
    
    for (size_t i = 0; i <= text_len; i++) {
        char c = text[i];
        
        if (isalpha(c)) {
            if (buf_pos < MAX_WORD_LEN - 1) {
                buffer[buf_pos++] = c;
            }
            in_word = 1;
        } else {
            if (in_word) {
                buffer[buf_pos] = '\0';
                process_word(analyzer, buffer, buf_pos);
                analyzer->total_words++;
                buf_pos = 0;
                in_word = 0;
            }
        }
    }
    
    return 1;
}

void print_analysis(const struct TextAnalyzer *analyzer) {
    printf("Text Analysis Results:\n");
    printf("Total characters: %d\n", analyzer->total_characters);
    printf("Total words: %d\n", analyzer->total_words);
    printf("Unique words: %d\n", analyzer->word_count);
    printf("\nWord Statistics:\n");
    
    for (int i = 0; i < analyzer->word_count; i++) {
        printf("Word: %s", analyzer->words[i].word);
        if (analyzer->words[i].is_capitalized) printf(" (capitalized)");
        printf(" - Count: %d, Length: %d\n", 
               analyzer->words[i].count, analyzer->words[i].length);
    }
}

int main() {
    struct TextAnalyzer analyzer;
    init_analyzer(&analyzer);
    
    char input_text[MAX_TEXT_LEN