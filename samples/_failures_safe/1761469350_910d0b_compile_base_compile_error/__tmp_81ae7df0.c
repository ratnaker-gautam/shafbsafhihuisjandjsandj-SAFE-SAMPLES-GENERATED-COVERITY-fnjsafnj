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
    return isalpha(c) || c == ' ' || c == '\n' || c == '\t' || c == '\0';
}

int find_word_index(struct TextAnalyzer *analyzer, const char *word) {
    for (int i = 0; i < analyzer->word_count; i++) {
        if (strcmp(analyzer->words[i].word, word) == 0) {
            return i;
        }
    }
    return -1;
}

void add_word(struct TextAnalyzer *analyzer, const char *word, int length) {
    if (length <= 0 || length >= MAX_WORD_LEN) return;
    
    int index = find_word_index(analyzer, word);
    if (index != -1) {
        analyzer->words[index].count++;
        return;
    }
    
    if (analyzer->word_count >= MAX_WORDS) return;
    
    strncpy(analyzer->words[analyzer->word_count].word, word, MAX_WORD_LEN - 1);
    analyzer->words[analyzer->word_count].word[MAX_WORD_LEN - 1] = '\0';
    analyzer->words[analyzer->word_count].count = 1;
    analyzer->words[analyzer->word_count].length = length;
    analyzer->words[analyzer->word_count].is_capitalized = isupper(word[0]) ? 1 : 0;
    
    if (length > analyzer->longest_word_length) {
        analyzer->longest_word_length = length;
    }
    if (length < analyzer->shortest_word_length) {
        analyzer->shortest_word_length = length;
    }
    
    analyzer->word_count++;
}

void process_text(struct TextAnalyzer *analyzer, const char *text) {
    char buffer[MAX_TEXT_LEN];
    char word[MAX_WORD_LEN];
    int text_len = strlen(text);
    
    if (text_len >= MAX_TEXT_LEN - 1) return;
    
    strncpy(buffer, text, MAX_TEXT_LEN - 1);
    buffer[MAX_TEXT_LEN - 1] = '\0';
    
    int pos = 0;
    int word_start = -1;
    
    while (pos <= text_len) {
        if (!is_valid_char(buffer[pos])) {
            pos++;
            continue;
        }
        
        if (isalpha(buffer[pos])) {
            if (word_start == -1) {
                word_start = pos;
            }
        } else {
            if (word_start != -1) {
                int word_length = pos - word_start;
                if (word_length > 0 && word_length < MAX_WORD_LEN) {
                    strncpy(word, &buffer[word_start], word_length);
                    word[word_length] = '\0';
                    
                    for (int i = 0; i < word_length; i++) {
                        word[i] = tolower(word[i]);
                    }
                    
                    add_word(analyzer, word, word_length);
                    analyzer->total_words++;
                }
                word_start = -1;
            }
        }
        pos++;
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
        printf("%s: %d occurrences\n", analyzer->words[i].word, analyzer->words[i].count);
    }
    
    int capitalized_count = 0;
    for (int i = 0; i < analyzer->word_count; i++) {
        if (analyzer->words[i].is_capitalized) {
            capitalized_count++;
        }
    }
    printf("\nCapitalized words: %d\n", capitalized_count);
}

int main() {
    struct TextAnalyzer analyzer;
    char input_text[MAX_TEXT_LEN];
    
    init_analyzer(&analyzer);
    
    printf("Enter text to analyze (max %d characters):\n", MAX_TEXT_LEN - 1);
    
    if (fgets(input_text, sizeof(input_text), stdin) == NULL)