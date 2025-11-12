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
    int longest_word_len;
};

void init_analyzer(struct TextAnalyzer *analyzer) {
    analyzer->word_count = 0;
    analyzer->total_chars = 0;
    analyzer->total_words = 0;
    analyzer->longest_word_len = 0;
    for (int i = 0; i < MAX_WORDS; i++) {
        analyzer->words[i].count = 0;
        analyzer->words[i].length = 0;
        analyzer->words[i].is_capitalized = 0;
        analyzer->words[i].word[0] = '\0';
    }
}

int is_word_char(char c) {
    return isalpha((unsigned char)c) || c == '\'' || c == '-';
}

void normalize_word(char *word) {
    int len = strlen(word);
    while (len > 0 && !isalpha((unsigned char)word[len - 1])) {
        word[--len] = '\0';
    }
    for (int i = 0; word[i]; i++) {
        word[i] = tolower((unsigned char)word[i]);
    }
}

int add_word(struct TextAnalyzer *analyzer, const char *word) {
    if (analyzer->word_count >= MAX_WORDS) {
        return 0;
    }
    
    char normalized[MAX_WORD_LEN];
    strncpy(normalized, word, MAX_WORD_LEN - 1);
    normalized[MAX_WORD_LEN - 1] = '\0';
    normalize_word(normalized);
    
    if (strlen(normalized) == 0) {
        return 0;
    }
    
    for (int i = 0; i < analyzer->word_count; i++) {
        if (strcmp(analyzer->words[i].word, normalized) == 0) {
            analyzer->words[i].count++;
            return 1;
        }
    }
    
    struct WordStats *ws = &analyzer->words[analyzer->word_count];
    strncpy(ws->word, normalized, MAX_WORD_LEN - 1);
    ws->word[MAX_WORD_LEN - 1] = '\0';
    ws->count = 1;
    ws->length = strlen(normalized);
    ws->is_capitalized = isupper((unsigned char)word[0]) ? 1 : 0;
    
    if (ws->length > analyzer->longest_word_len) {
        analyzer->longest_word_len = ws->length;
    }
    
    analyzer->word_count++;
    return 1;
}

void process_text(struct TextAnalyzer *analyzer, const char *text) {
    char buffer[MAX_TEXT_LEN];
    strncpy(buffer, text, MAX_TEXT_LEN - 1);
    buffer[MAX_TEXT_LEN - 1] = '\0';
    
    char *ptr = buffer;
    char word[MAX_WORD_LEN];
    int word_len = 0;
    
    while (*ptr && analyzer->total_words < MAX_WORDS) {
        if (is_word_char(*ptr)) {
            if (word_len < MAX_WORD_LEN - 1) {
                word[word_len++] = *ptr;
            }
        } else {
            if (word_len > 0) {
                word[word_len] = '\0';
                if (add_word(analyzer, word)) {
                    analyzer->total_words++;
                    analyzer->total_chars += word_len;
                }
                word_len = 0;
            }
        }
        ptr++;
    }
    
    if (word_len > 0) {
        word[word_len] = '\0';
        if (add_word(analyzer, word)) {
            analyzer->total_words++;
            analyzer->total_chars += word_len;
        }
    }
}

void print_analysis(const struct TextAnalyzer *analyzer) {
    printf("Text Analysis Results:\n");
    printf("Total words: %d\n", analyzer->total_words);
    printf("Total characters: %d\n", analyzer->total_chars);
    printf("Unique words: %d\n", analyzer->word_count);
    printf("Longest word length: %d\n", analyzer->longest_word_len);
    printf("\nWord frequency:\n");
    
    for (int i = 0; i < analyzer->word_count; i++) {
        const struct WordStats *ws = &analyzer->words[i];
        printf("'%s': %d occurrences\n", ws->word, ws->count);
    }
}

int main() {
    struct TextAnalyzer analyzer;
    init_analyzer(&analyzer);
    
    char input_text[MAX_TEXT_LEN];
    printf("Enter text to analyze (max %d characters):\n", MAX_TEXT_LEN - 1);
    
    if (fgets(input_text, sizeof(input_text), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(input_text);
    if (len > 0 && input_text[len - 1] == '\n') {
        input_text[len -