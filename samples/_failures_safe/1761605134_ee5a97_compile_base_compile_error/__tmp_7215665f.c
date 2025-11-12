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
    int total_words;
    int longest_word_length;
    int shortest_word_length;
};

void initialize_analyzer(struct TextAnalyzer *analyzer) {
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

int is_word_character(char c) {
    return isalpha((unsigned char)c) || c == '\'' || c == '-';
}

int find_word_index(struct TextAnalyzer *analyzer, const char *word) {
    for (int i = 0; i < analyzer->word_count; i++) {
        if (strcmp(analyzer->words[i].word, word) == 0) {
            return i;
        }
    }
    return -1;
}

void add_word(struct TextAnalyzer *analyzer, const char *word, int length, int capitalized) {
    if (analyzer->word_count >= MAX_WORDS || length >= MAX_WORD_LEN) {
        return;
    }
    
    int index = find_word_index(analyzer, word);
    if (index != -1) {
        analyzer->words[index].count++;
        return;
    }
    
    strncpy(analyzer->words[analyzer->word_count].word, word, MAX_WORD_LEN - 1);
    analyzer->words[analyzer->word_count].word[MAX_WORD_LEN - 1] = '\0';
    analyzer->words[analyzer->word_count].count = 1;
    analyzer->words[analyzer->word_count].length = length;
    analyzer->words[analyzer->word_count].is_capitalized = capitalized;
    
    if (length > analyzer->longest_word_length) {
        analyzer->longest_word_length = length;
    }
    if (length < analyzer->shortest_word_length) {
        analyzer->shortest_word_length = length;
    }
    
    analyzer->word_count++;
}

void process_text(struct TextAnalyzer *analyzer, const char *text) {
    char buffer[MAX_WORD_LEN];
    int buf_index = 0;
    int text_len = strlen(text);
    
    for (int i = 0; i <= text_len; i++) {
        if (i < text_len && is_word_character(text[i])) {
            if (buf_index < MAX_WORD_LEN - 1) {
                buffer[buf_index++] = text[i];
            }
        } else {
            if (buf_index > 0) {
                buffer[buf_index] = '\0';
                int capitalized = isupper((unsigned char)buffer[0]) ? 1 : 0;
                
                char lowercase_word[MAX_WORD_LEN];
                for (int j = 0; j <= buf_index; j++) {
                    lowercase_word[j] = tolower((unsigned char)buffer[j]);
                }
                
                add_word(analyzer, lowercase_word, buf_index, capitalized);
                analyzer->total_words++;
                buf_index = 0;
            }
        }
    }
}

void print_analysis(struct TextAnalyzer *analyzer) {
    printf("Text Analysis Results:\n");
    printf("Total words: %d\n", analyzer->total_words);
    printf("Unique words: %d\n", analyzer->word_count);
    printf("Longest word length: %d\n", analyzer->longest_word_length);
    printf("Shortest word length: %d\n", analyzer->shortest_word_length);
    printf("\nWord frequency:\n");
    
    for (int i = 0; i < analyzer->word_count; i++) {
        printf("'%s': %d occurrence(s), length: %d", 
               analyzer->words[i].word, 
               analyzer->words[i].count,
               analyzer->words[i].length);
        if (analyzer->words[i].is_capitalized) {
            printf(" (capitalized)");
        }
        printf("\n");
    }
}

int main() {
    struct TextAnalyzer analyzer;
    char input_text[MAX_TEXT_LEN];
    
    initialize_analyzer(&analyzer);
    
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
    print_analysis(&analyzer