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
    if (analyzer == NULL) return;
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

void process_word(struct TextAnalyzer *analyzer, const char *word, int len) {
    if (analyzer == NULL || word == NULL || len <= 0 || len >= MAX_WORD_LEN || analyzer->word_count >= MAX_WORDS) {
        return;
    }

    for (int i = 0; i < analyzer->word_count; i++) {
        if (strncmp(analyzer->words[i].word, word, len) == 0 && 
            analyzer->words[i].word[len] == '\0') {
            analyzer->words[i].count++;
            return;
        }
    }

    if (analyzer->word_count < MAX_WORDS) {
        struct WordStats *ws = &analyzer->words[analyzer->word_count];
        strncpy(ws->word, word, len);
        ws->word[len] = '\0';
        ws->count = 1;
        ws->length = len;
        ws->is_capitalized = isupper((unsigned char)word[0]) ? 1 : 0;
        analyzer->word_count++;
    }
}

void analyze_text(struct TextAnalyzer *analyzer, const char *text) {
    if (analyzer == NULL || text == NULL) return;

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
            process_word(analyzer, buffer, buf_pos);
            buf_pos = 0;
            in_word = 0;
        }
    }

    if (in_word && buf_pos > 0) {
        buffer[buf_pos] = '\0';
        process_word(analyzer, buffer, buf_pos);
    }
}

void calculate_statistics(struct TextAnalyzer *analyzer) {
    if (analyzer == NULL) return;
    
    analyzer->total_words = 0;
    analyzer->longest_word_length = 0;
    analyzer->shortest_word_length = MAX_WORD_LEN;

    for (int i = 0; i < analyzer->word_count; i++) {
        analyzer->total_words += analyzer->words[i].count;
        
        if (analyzer->words[i].length > analyzer->longest_word_length) {
            analyzer->longest_word_length = analyzer->words[i].length;
        }
        
        if (analyzer->words[i].length < analyzer->shortest_word_length) {
            analyzer->shortest_word_length = analyzer->words[i].length;
        }
    }
}

void print_analysis(const struct TextAnalyzer *analyzer) {
    if (analyzer == NULL) return;
    
    printf("Text Analysis Results:\n");
    printf("Total unique words: %d\n", analyzer->word_count);
    printf("Total word occurrences: %d\n", analyzer->total_words);
    printf("Longest word length: %d\n", analyzer->longest_word_length);
    printf("Shortest word length: %d\n", analyzer->shortest_word_length);
    printf("\nWord Frequency:\n");

    for (int i = 0; i < analyzer->word_count; i++) {
        printf("%-20s: %d occurrences\n", analyzer->words[i].word, analyzer->words[i].count);
    }
}

int main(void) {
    struct TextAnalyzer analyzer;
    init_analyzer(&analyzer);

    char input_text[4096];
    printf("Enter text to analyze (max 4095 characters): ");
    
    if (fgets(input_text, sizeof(input_text), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }

    size_t input_len = strlen(input_text);
    if (input_len > 0 && input_text[input_len - 1] == '\n') {
        input_text[input_len - 1] = '\0';
    }

    if (strlen(input_text) == 0) {
        printf("