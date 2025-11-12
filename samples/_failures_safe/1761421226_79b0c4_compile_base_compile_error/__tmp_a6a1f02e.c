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
        analyzer->words[i].word[0] = '\0';
    }
}

int is_word_char(char c) {
    return isalpha((unsigned char)c) || c == '\'' || c == '-';
}

void add_word(struct TextAnalyzer *analyzer, const char *word, int length) {
    if (length <= 0 || length >= MAX_WORD_LEN || analyzer->word_count >= MAX_WORDS) {
        return;
    }
    
    for (int i = 0; i < analyzer->word_count; i++) {
        if (strncmp(analyzer->words[i].word, word, length) == 0 && 
            analyzer->words[i].word[length] == '\0') {
            analyzer->words[i].count++;
            return;
        }
    }
    
    if (analyzer->word_count < MAX_WORDS) {
        strncpy(analyzer->words[analyzer->word_count].word, word, length);
        analyzer->words[analyzer->word_count].word[length] = '\0';
        analyzer->words[analyzer->word_count].count = 1;
        analyzer->words[analyzer->word_count].length = length;
        analyzer->words[analyzer->word_count].is_capitalized = isupper((unsigned char)word[0]);
        analyzer->word_count++;
    }
}

void process_text(struct TextAnalyzer *analyzer, const char *text) {
    if (text == NULL) return;
    
    int len = strlen(text);
    if (len <= 0) return;
    
    char buffer[MAX_WORD_LEN];
    int buf_pos = 0;
    int in_word = 0;
    
    for (int i = 0; i < len && buf_pos < MAX_WORD_LEN - 1; i++) {
        if (is_word_char(text[i])) {
            buffer[buf_pos++] = text[i];
            in_word = 1;
        } else if (in_word) {
            buffer[buf_pos] = '\0';
            add_word(analyzer, buffer, buf_pos);
            analyzer->total_words++;
            buf_pos = 0;
            in_word = 0;
        }
    }
    
    if (in_word && buf_pos > 0) {
        buffer[buf_pos] = '\0';
        add_word(analyzer, buffer, buf_pos);
        analyzer->total_words++;
    }
}

void calculate_statistics(struct TextAnalyzer *analyzer) {
    analyzer->longest_word_length = 0;
    analyzer->shortest_word_length = MAX_WORD_LEN;
    
    for (int i = 0; i < analyzer->word_count; i++) {
        int len = analyzer->words[i].length;
        if (len > analyzer->longest_word_length) {
            analyzer->longest_word_length = len;
        }
        if (len < analyzer->shortest_word_length && len > 0) {
            analyzer->shortest_word_length = len;
        }
    }
    
    if (analyzer->shortest_word_length == MAX_WORD_LEN) {
        analyzer->shortest_word_length = 0;
    }
}

void print_analysis(const struct TextAnalyzer *analyzer) {
    printf("Text Analysis Results:\n");
    printf("Total words processed: %d\n", analyzer->total_words);
    printf("Unique words found: %d\n", analyzer->word_count);
    printf("Longest word length: %d\n", analyzer->longest_word_length);
    printf("Shortest word length: %d\n", analyzer->shortest_word_length);
    printf("\nWord frequency (top 10):\n");
    
    struct WordStats sorted[MAX_WORDS];
    int count = analyzer->word_count > 10 ? 10 : analyzer->word_count;
    
    for (int i = 0; i < analyzer->word_count && i < MAX_WORDS; i++) {
        sorted[i] = analyzer->words[i];
    }
    
    for (int i = 0; i < analyzer->word_count - 1 && i < MAX_WORDS - 1; i++) {
        for (int j = i + 1; j < analyzer->word_count && j < MAX_WORDS; j++) {
            if (sorted[j].count > sorted[i].count) {
                struct WordStats temp = sorted[i];
                sorted[i] = sorted[j];
                sorted[j] = temp;
            }
        }
    }
    
    for (int i = 0; i < count; i++) {
        printf("%d.