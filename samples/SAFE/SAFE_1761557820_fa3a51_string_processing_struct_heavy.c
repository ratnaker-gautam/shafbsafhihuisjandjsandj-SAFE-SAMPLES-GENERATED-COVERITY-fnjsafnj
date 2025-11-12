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
};

struct TextAnalyzer {
    struct WordStats words[MAX_WORDS];
    int word_count;
    int total_words;
    int longest_word;
    int shortest_word;
};

void init_analyzer(struct TextAnalyzer *analyzer) {
    analyzer->word_count = 0;
    analyzer->total_words = 0;
    analyzer->longest_word = 0;
    analyzer->shortest_word = MAX_WORD_LEN;
}

int is_valid_char(char c) {
    return isalpha((unsigned char)c) || c == ' ' || c == '\n' || c == '\t';
}

int add_word(struct TextAnalyzer *analyzer, const char *word) {
    if (analyzer->word_count >= MAX_WORDS) return 0;
    
    int len = strlen(word);
    if (len == 0 || len >= MAX_WORD_LEN) return 0;
    
    for (int i = 0; i < analyzer->word_count; i++) {
        if (strcmp(analyzer->words[i].word, word) == 0) {
            analyzer->words[i].count++;
            return 1;
        }
    }
    
    strncpy(analyzer->words[analyzer->word_count].word, word, MAX_WORD_LEN - 1);
    analyzer->words[analyzer->word_count].word[MAX_WORD_LEN - 1] = '\0';
    analyzer->words[analyzer->word_count].count = 1;
    analyzer->words[analyzer->word_count].length = len;
    analyzer->word_count++;
    
    return 1;
}

void process_text(struct TextAnalyzer *analyzer, const char *text) {
    if (text == NULL) return;
    
    char buffer[MAX_WORD_LEN];
    int buf_index = 0;
    
    for (int i = 0; text[i] != '\0'; i++) {
        if (!is_valid_char(text[i])) continue;
        
        if (isalpha((unsigned char)text[i])) {
            if (buf_index < MAX_WORD_LEN - 1) {
                buffer[buf_index++] = tolower((unsigned char)text[i]);
            }
        } else if (buf_index > 0) {
            buffer[buf_index] = '\0';
            if (add_word(analyzer, buffer)) {
                analyzer->total_words++;
                int len = strlen(buffer);
                if (len > analyzer->longest_word) analyzer->longest_word = len;
                if (len < analyzer->shortest_word) analyzer->shortest_word = len;
            }
            buf_index = 0;
        }
    }
    
    if (buf_index > 0) {
        buffer[buf_index] = '\0';
        if (add_word(analyzer, buffer)) {
            analyzer->total_words++;
            int len = strlen(buffer);
            if (len > analyzer->longest_word) analyzer->longest_word = len;
            if (len < analyzer->shortest_word) analyzer->shortest_word = len;
        }
    }
}

void print_analysis(const struct TextAnalyzer *analyzer) {
    printf("Text Analysis Results:\n");
    printf("Total words: %d\n", analyzer->total_words);
    printf("Unique words: %d\n", analyzer->word_count);
    printf("Longest word length: %d\n", analyzer->longest_word);
    printf("Shortest word length: %d\n", analyzer->shortest_word);
    printf("\nWord frequencies:\n");
    
    for (int i = 0; i < analyzer->word_count; i++) {
        printf("%-15s: %d\n", analyzer->words[i].word, analyzer->words[i].count);
    }
}

int main(void) {
    struct TextAnalyzer analyzer;
    init_analyzer(&analyzer);
    
    char input_buffer[4096];
    printf("Enter text to analyze (max 4095 characters):\n");
    
    if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(input_buffer);
    if (len > 0 && input_buffer[len - 1] == '\n') {
        input_buffer[len - 1] = '\0';
    }
    
    if (strlen(input_buffer) == 0) {
        printf("No input provided\n");
        return 1;
    }
    
    process_text(&analyzer, input_buffer);
    print_analysis(&analyzer);
    
    return 0;
}