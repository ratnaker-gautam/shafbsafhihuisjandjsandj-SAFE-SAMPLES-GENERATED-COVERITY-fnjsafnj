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
    int total_chars;
    int longest_word_length;
};

void init_analyzer(struct TextAnalyzer *analyzer) {
    analyzer->word_count = 0;
    analyzer->total_words = 0;
    analyzer->total_chars = 0;
    analyzer->longest_word_length = 0;
    for (int i = 0; i < MAX_WORDS; i++) {
        analyzer->words[i].word[0] = '\0';
        analyzer->words[i].count = 0;
        analyzer->words[i].length = 0;
        analyzer->words[i].is_capitalized = 0;
    }
}

int is_word_char(char c) {
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

void add_word(struct TextAnalyzer *analyzer, const char *word, int is_capitalized) {
    if (analyzer->word_count >= MAX_WORDS || strlen(word) >= MAX_WORD_LEN) {
        return;
    }
    
    int index = find_word_index(analyzer, word);
    if (index >= 0) {
        analyzer->words[index].count++;
        return;
    }
    
    strncpy(analyzer->words[analyzer->word_count].word, word, MAX_WORD_LEN - 1);
    analyzer->words[analyzer->word_count].word[MAX_WORD_LEN - 1] = '\0';
    analyzer->words[analyzer->word_count].count = 1;
    analyzer->words[analyzer->word_count].length = strlen(word);
    analyzer->words[analyzer->word_count].is_capitalized = is_capitalized;
    
    if (analyzer->words[analyzer->word_count].length > analyzer->longest_word_length) {
        analyzer->longest_word_length = analyzer->words[analyzer->word_count].length;
    }
    
    analyzer->word_count++;
}

void process_text(struct TextAnalyzer *analyzer, const char *text) {
    char buffer[MAX_TEXT_LEN];
    if (strlen(text) >= MAX_TEXT_LEN) {
        return;
    }
    strncpy(buffer, text, MAX_TEXT_LEN - 1);
    buffer[MAX_TEXT_LEN - 1] = '\0';
    
    char word[MAX_WORD_LEN];
    int word_pos = 0;
    int in_word = 0;
    
    for (int i = 0; buffer[i] != '\0'; i++) {
        if (is_word_char(buffer[i])) {
            if (!in_word) {
                in_word = 1;
                word_pos = 0;
            }
            if (word_pos < MAX_WORD_LEN - 1) {
                word[word_pos++] = buffer[i];
            }
        } else {
            if (in_word) {
                word[word_pos] = '\0';
                if (word_pos > 0) {
                    int is_cap = isupper((unsigned char)word[0]);
                    add_word(analyzer, word, is_cap);
                    analyzer->total_words++;
                    analyzer->total_chars += word_pos;
                }
                in_word = 0;
            }
        }
    }
    
    if (in_word && word_pos > 0) {
        word[word_pos] = '\0';
        int is_cap = isupper((unsigned char)word[0]);
        add_word(analyzer, word, is_cap);
        analyzer->total_words++;
        analyzer->total_chars += word_pos;
    }
}

void print_analysis(struct TextAnalyzer *analyzer) {
    printf("Text Analysis Results:\n");
    printf("Total words: %d\n", analyzer->total_words);
    printf("Total characters in words: %d\n", analyzer->total_chars);
    printf("Unique words: %d\n", analyzer->word_count);
    printf("Longest word length: %d\n", analyzer->longest_word_length);
    printf("\nWord frequency:\n");
    
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
    init_analyzer(&analyzer);
    
    char input_text[MAX_TEXT_LEN];
    printf("Enter text to analyze (max %d characters):\n", MAX_TEXT_LEN - 1);
    
    if (fgets(input_text, sizeof(input_text), stdin) == NULL) {
        printf("Error reading input.\n");
        return 1;
    }