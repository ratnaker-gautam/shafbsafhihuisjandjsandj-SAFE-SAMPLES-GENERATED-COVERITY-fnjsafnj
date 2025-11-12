//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: struct_heavy ; Variation: string_processing
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
}

int is_valid_char(char c) {
    return isalpha(c) || c == ' ' || c == '\t' || c == '\n' || c == '\0';
}

int add_word(struct TextAnalyzer *analyzer, const char *word) {
    if (analyzer->word_count >= MAX_WORDS || word == NULL || strlen(word) == 0) {
        return 0;
    }
    
    int len = strlen(word);
    if (len >= MAX_WORD_LEN) {
        return 0;
    }
    
    for (int i = 0; i < analyzer->word_count; i++) {
        if (strcmp(analyzer->words[i].word, word) == 0) {
            analyzer->words[i].count++;
            analyzer->total_words++;
            return 1;
        }
    }
    
    if (analyzer->word_count < MAX_WORDS) {
        strncpy(analyzer->words[analyzer->word_count].word, word, MAX_WORD_LEN - 1);
        analyzer->words[analyzer->word_count].word[MAX_WORD_LEN - 1] = '\0';
        analyzer->words[analyzer->word_count].count = 1;
        analyzer->words[analyzer->word_count].length = len;
        analyzer->words[analyzer->word_count].is_capitalized = isupper(word[0]) ? 1 : 0;
        
        if (len > analyzer->longest_word_length) {
            analyzer->longest_word_length = len;
        }
        if (len < analyzer->shortest_word_length) {
            analyzer->shortest_word_length = len;
        }
        
        analyzer->word_count++;
        analyzer->total_words++;
        return 1;
    }
    
    return 0;
}

int process_text(struct TextAnalyzer *analyzer, const char *text) {
    if (text == NULL) {
        return 0;
    }
    
    char buffer[MAX_WORD_LEN];
    int buf_index = 0;
    int text_len = strlen(text);
    
    for (int i = 0; i <= text_len; i++) {
        if (i < text_len && !is_valid_char(text[i])) {
            return 0;
        }
        
        if (i == text_len || isspace(text[i])) {
            if (buf_index > 0) {
                buffer[buf_index] = '\0';
                if (!add_word(analyzer, buffer)) {
                    return 0;
                }
                buf_index = 0;
            }
        } else {
            if (buf_index < MAX_WORD_LEN - 1) {
                buffer[buf_index++] = tolower(text[i]);
            } else {
                return 0;
            }
        }
    }
    
    return 1;
}

void print_analysis(const struct TextAnalyzer *analyzer) {
    if (analyzer == NULL || analyzer->word_count == 0) {
        printf("No words to analyze.\n");
        return;
    }
    
    printf("Text Analysis Results:\n");
    printf("Total unique words: %d\n", analyzer->word_count);
    printf("Total word occurrences: %d\n", analyzer->total_words);
    printf("Longest word length: %d\n", analyzer->longest_word_length);
    printf("Shortest word length: %d\n", analyzer->shortest_word_length);
    
    printf("\nWord frequencies:\n");
    for (int i = 0; i < analyzer->word_count; i++) {
        printf("'%s': %d occurrences (length: %d, %s)\n",
               analyzer->words[i].word,
               analyzer->words[i].count,
               analyzer->words[i].length,
               analyzer->words[i].is_capitalized ? "capitalized" : "lowercase");
    }
}

int main(void) {
    struct TextAnalyzer analyzer;
    init_analyzer(&analyzer);
    
    char input_text[4096];
    printf("Enter text to analyze (max 4095 characters): ");
    
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
    
    if (!process_text(&analyzer, input_text)) {
        printf("Error processing text. Text may contain invalid characters or be too long.\n");
        return 1;
    }
    
    print_analysis(&analyzer);
    
    return 0;
}