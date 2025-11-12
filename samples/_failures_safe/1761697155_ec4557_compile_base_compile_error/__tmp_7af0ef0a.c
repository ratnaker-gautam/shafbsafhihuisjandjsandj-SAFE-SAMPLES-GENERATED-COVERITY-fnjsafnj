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
    for (int i = 0; i < MAX_WORDS; i++) {
        analyzer->words[i].count = 0;
        analyzer->words[i].length = 0;
        analyzer->words[i].is_capitalized = 0;
        memset(analyzer->words[i].word, 0, MAX_WORD_LEN);
    }
}

int is_word_char(char c) {
    return isalpha((unsigned char)c) || c == '\'' || c == '-';
}

void normalize_word(char *dest, const char *src, size_t max_len) {
    size_t i = 0;
    size_t j = 0;
    
    while (src[i] != '\0' && j < max_len - 1) {
        if (isalpha((unsigned char)src[i])) {
            dest[j] = tolower((unsigned char)src[i]);
            j++;
        } else if (src[i] == '\'' || src[i] == '-') {
            dest[j] = src[i];
            j++;
        }
        i++;
    }
    dest[j] = '\0';
}

int find_word_index(struct TextAnalyzer *analyzer, const char *word) {
    for (int i = 0; i < analyzer->word_count; i++) {
        if (strcmp(analyzer->words[i].word, word) == 0) {
            return i;
        }
    }
    return -1;
}

void add_word(struct TextAnalyzer *analyzer, const char *original_word) {
    if (analyzer->word_count >= MAX_WORDS) return;
    
    char normalized[MAX_WORD_LEN];
    normalize_word(normalized, original_word, MAX_WORD_LEN);
    
    if (strlen(normalized) == 0) return;
    
    int index = find_word_index(analyzer, normalized);
    
    if (index == -1) {
        index = analyzer->word_count;
        strncpy(analyzer->words[index].word, normalized, MAX_WORD_LEN - 1);
        analyzer->words[index].word[MAX_WORD_LEN - 1] = '\0';
        analyzer->words[index].length = strlen(normalized);
        analyzer->words[index].is_capitalized = isupper((unsigned char)original_word[0]) ? 1 : 0;
        analyzer->word_count++;
    }
    
    analyzer->words[index].count++;
    analyzer->total_words++;
    
    int len = analyzer->words[index].length;
    if (len > analyzer->longest_word_length) {
        analyzer->longest_word_length = len;
    }
    if (len < analyzer->shortest_word_length) {
        analyzer->shortest_word_length = len;
    }
}

void process_text(struct TextAnalyzer *analyzer, const char *text) {
    if (text == NULL) return;
    
    char buffer[1024];
    size_t text_len = strlen(text);
    if (text_len >= sizeof(buffer)) return;
    
    strncpy(buffer, text, sizeof(buffer) - 1);
    buffer[sizeof(buffer) - 1] = '\0';
    
    char *ptr = buffer;
    char word[MAX_WORD_LEN];
    int word_pos = 0;
    
    while (*ptr != '\0') {
        if (is_word_char(*ptr)) {
            if (word_pos < MAX_WORD_LEN - 1) {
                word[word_pos++] = *ptr;
            }
        } else {
            if (word_pos > 0) {
                word[word_pos] = '\0';
                add_word(analyzer, word);
                word_pos = 0;
            }
        }
        ptr++;
    }
    
    if (word_pos > 0) {
        word[word_pos] = '\0';
        add_word(analyzer, word);
    }
}

void print_analysis(struct TextAnalyzer *analyzer) {
    printf("Text Analysis Results:\n");
    printf("Total unique words: %d\n", analyzer->word_count);
    printf("Total word occurrences: %d\n", analyzer->total_words);
    printf("Longest word length: %d\n", analyzer->longest_word_length);
    printf("Shortest word length: %d\n", analyzer->shortest_word_length);
    printf("\nWord frequency:\n");
    
    for (int i = 0; i < analyzer->word_count; i++) {
        printf("%s: %d occurrences", analyzer->words[i].word, analyzer->words[i].count);
        if (analyzer->words[i].is_capitalized) {
            printf(" (capitalized)");
        }
        printf("\n");
    }
}

int main() {
    struct TextAnalyzer analyzer;
    init_analyzer(&analyzer);
    
    char input_text[2048];
    printf("Enter