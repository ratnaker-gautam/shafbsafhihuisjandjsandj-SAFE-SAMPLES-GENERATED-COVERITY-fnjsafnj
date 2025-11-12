//DeepSeek-V3 V2.5 Category: Safe ; Style: struct_heavy ; Variation: string_processing
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_WORD_LEN 64
#define MAX_WORDS 1000

struct WordInfo {
    char word[MAX_WORD_LEN];
    int count;
    int length;
    int is_capitalized;
};

struct TextStats {
    struct WordInfo words[MAX_WORDS];
    int word_count;
    int total_chars;
    int unique_words;
    int longest_word_length;
};

struct TextProcessor {
    struct TextStats stats;
    char buffer[1024];
};

void init_text_processor(struct TextProcessor *tp) {
    tp->stats.word_count = 0;
    tp->stats.total_chars = 0;
    tp->stats.unique_words = 0;
    tp->stats.longest_word_length = 0;
    memset(tp->buffer, 0, sizeof(tp->buffer));
    for (int i = 0; i < MAX_WORDS; i++) {
        memset(tp->stats.words[i].word, 0, MAX_WORD_LEN);
        tp->stats.words[i].count = 0;
        tp->stats.words[i].length = 0;
        tp->stats.words[i].is_capitalized = 0;
    }
}

int is_word_char(char c) {
    return isalpha((unsigned char)c) || c == '\'' || c == '-';
}

int find_word_index(struct TextStats *stats, const char *word) {
    for (int i = 0; i < stats->word_count; i++) {
        if (strcmp(stats->words[i].word, word) == 0) {
            return i;
        }
    }
    return -1;
}

void add_word(struct TextStats *stats, const char *word, int length) {
    if (length <= 0 || length >= MAX_WORD_LEN || stats->word_count >= MAX_WORDS) {
        return;
    }
    
    char normalized[MAX_WORD_LEN];
    strncpy(normalized, word, length);
    normalized[length] = '\0';
    
    for (int i = 0; i < length; i++) {
        normalized[i] = tolower((unsigned char)normalized[i]);
    }
    
    int index = find_word_index(stats, normalized);
    if (index >= 0) {
        stats->words[index].count++;
    } else {
        int new_index = stats->word_count;
        strncpy(stats->words[new_index].word, normalized, MAX_WORD_LEN - 1);
        stats->words[new_index].word[MAX_WORD_LEN - 1] = '\0';
        stats->words[new_index].count = 1;
        stats->words[new_index].length = length;
        stats->words[new_index].is_capitalized = isupper((unsigned char)word[0]) ? 1 : 0;
        stats->word_count++;
        stats->unique_words++;
    }
    
    if (length > stats->longest_word_length) {
        stats->longest_word_length = length;
    }
}

void process_text(struct TextProcessor *tp, const char *text) {
    if (text == NULL) return;
    
    int len = strlen(text);
    if (len <= 0 || len >= sizeof(tp->buffer)) return;
    
    strncpy(tp->buffer, text, sizeof(tp->buffer) - 1);
    tp->buffer[sizeof(tp->buffer) - 1] = '\0';
    
    int i = 0;
    while (tp->buffer[i] != '\0') {
        while (tp->buffer[i] != '\0' && !is_word_char(tp->buffer[i])) {
            i++;
        }
        
        if (tp->buffer[i] == '\0') break;
        
        int start = i;
        while (tp->buffer[i] != '\0' && is_word_char(tp->buffer[i])) {
            i++;
        }
        
        int word_length = i - start;
        if (word_length > 0 && word_length < MAX_WORD_LEN) {
            char temp[MAX_WORD_LEN];
            strncpy(temp, &tp->buffer[start], word_length);
            temp[word_length] = '\0';
            add_word(&tp->stats, temp, word_length);
            tp->stats.total_chars += word_length;
        }
    }
}

void print_stats(struct TextStats *stats) {
    printf("Text Statistics:\n");
    printf("Total words: %d\n", stats->word_count);
    printf("Unique words: %d\n", stats->unique_words);
    printf("Total characters in words: %d\n", stats->total_chars);
    printf("Longest word length: %d\n", stats->longest_word_length);
    printf("\nWord frequency:\n");
    
    for (int i = 0; i < stats->word_count; i++) {
        printf("%-20s: %d occurrences\n", stats->words[i].word, stats->words[i].count);
    }
}

int main(void) {
    struct TextProcessor processor;
    init_text_processor(&processor);
    
    char input[2048];
    printf("Enter text to analyze (max 2047 characters):\n");
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        printf("Error reading input.\n");
        return 1;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input