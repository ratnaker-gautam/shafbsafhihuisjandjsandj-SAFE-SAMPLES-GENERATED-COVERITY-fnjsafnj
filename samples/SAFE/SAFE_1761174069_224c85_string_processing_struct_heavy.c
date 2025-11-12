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
        analyzer->words[i].word[0] = '\0';
        analyzer->words[i].count = 0;
        analyzer->words[i].length = 0;
        analyzer->words[i].is_capitalized = 0;
    }
}

int is_valid_char(char c) {
    return isalpha((unsigned char)c) || c == ' ' || c == '\n' || c == '\t';
}

int add_word(struct TextAnalyzer *analyzer, const char *word, int length) {
    if (length <= 0 || length >= MAX_WORD_LEN) return 0;
    
    for (int i = 0; i < analyzer->word_count; i++) {
        if (strncmp(analyzer->words[i].word, word, length) == 0 && 
            analyzer->words[i].word[length] == '\0') {
            analyzer->words[i].count++;
            return 1;
        }
    }
    
    if (analyzer->word_count >= MAX_WORDS) return 0;
    
    strncpy(analyzer->words[analyzer->word_count].word, word, length);
    analyzer->words[analyzer->word_count].word[length] = '\0';
    analyzer->words[analyzer->word_count].count = 1;
    analyzer->words[analyzer->word_count].length = length;
    analyzer->words[analyzer->word_count].is_capitalized = isupper((unsigned char)word[0]);
    
    if (length > analyzer->longest_word_length) {
        analyzer->longest_word_length = length;
    }
    if (length < analyzer->shortest_word_length) {
        analyzer->shortest_word_length = length;
    }
    
    analyzer->word_count++;
    return 1;
}

void process_text(struct TextAnalyzer *analyzer, const char *text) {
    char buffer[MAX_TEXT_LEN];
    int text_len = strlen(text);
    
    if (text_len >= MAX_TEXT_LEN) return;
    
    strncpy(buffer, text, text_len);
    buffer[text_len] = '\0';
    
    char *token = strtok(buffer, " \t\n");
    while (token != NULL) {
        int valid = 1;
        int len = strlen(token);
        
        for (int i = 0; i < len; i++) {
            if (!is_valid_char(token[i])) {
                valid = 0;
                break;
            }
        }
        
        if (valid && len > 0) {
            analyzer->total_words++;
            add_word(analyzer, token, len);
        }
        
        token = strtok(NULL, " \t\n");
    }
}

void print_analysis(const struct TextAnalyzer *analyzer) {
    printf("Text Analysis Results:\n");
    printf("Total unique words: %d\n", analyzer->word_count);
    printf("Total words: %d\n", analyzer->total_words);
    printf("Longest word length: %d\n", analyzer->longest_word_length);
    printf("Shortest word length: %d\n", analyzer->shortest_word_length);
    printf("\nWord frequencies:\n");
    
    for (int i = 0; i < analyzer->word_count; i++) {
        printf("'%s': %d occurrences", analyzer->words[i].word, analyzer->words[i].count);
        if (analyzer->words[i].is_capitalized) {
            printf(" (capitalized)");
        }
        printf("\n");
    }
}

int main() {
    struct TextAnalyzer analyzer;
    char input_text[MAX_TEXT_LEN];
    
    init_analyzer(&analyzer);
    
    printf("Enter text to analyze (max %d characters):\n", MAX_TEXT_LEN - 1);
    
    if (fgets(input_text, sizeof(input_text), stdin) == NULL) {
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
    
    process_text(&analyzer, input_text);
    print_analysis(&analyzer);
    
    return 0;
}