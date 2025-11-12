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
    int total_characters;
    int total_words;
    char processed_text[MAX_TEXT_LEN];
};

void init_analyzer(struct TextAnalyzer *analyzer) {
    analyzer->word_count = 0;
    analyzer->total_characters = 0;
    analyzer->total_words = 0;
    memset(analyzer->processed_text, 0, sizeof(analyzer->processed_text));
    for (int i = 0; i < MAX_WORDS; i++) {
        memset(analyzer->words[i].word, 0, sizeof(analyzer->words[i].word));
        analyzer->words[i].count = 0;
        analyzer->words[i].length = 0;
        analyzer->words[i].is_capitalized = 0;
    }
}

int is_valid_char(char c) {
    return isalpha(c) || isspace(c) || c == '.' || c == ',' || c == '!' || c == '?';
}

int add_word(struct TextAnalyzer *analyzer, const char *word, int length, int capitalized) {
    if (length <= 0 || length >= MAX_WORD_LEN || analyzer->word_count >= MAX_WORDS) {
        return 0;
    }

    for (int i = 0; i < analyzer->word_count; i++) {
        if (strncmp(analyzer->words[i].word, word, length) == 0 && 
            analyzer->words[i].length == length) {
            analyzer->words[i].count++;
            return 1;
        }
    }

    if (analyzer->word_count < MAX_WORDS) {
        strncpy(analyzer->words[analyzer->word_count].word, word, length);
        analyzer->words[analyzer->word_count].word[length] = '\0';
        analyzer->words[analyzer->word_count].count = 1;
        analyzer->words[analyzer->word_count].length = length;
        analyzer->words[analyzer->word_count].is_capitalized = capitalized;
        analyzer->word_count++;
        return 1;
    }
    
    return 0;
}

int process_text(struct TextAnalyzer *analyzer, const char *input) {
    if (input == NULL || strlen(input) >= MAX_TEXT_LEN) {
        return 0;
    }

    strncpy(analyzer->processed_text, input, MAX_TEXT_LEN - 1);
    analyzer->processed_text[MAX_TEXT_LEN - 1] = '\0';
    
    char current_word[MAX_WORD_LEN];
    int word_len = 0;
    int in_word = 0;
    
    for (int i = 0; analyzer->processed_text[i] != '\0'; i++) {
        if (!is_valid_char(analyzer->processed_text[i])) {
            return 0;
        }
        
        analyzer->total_characters++;
        
        if (isalpha(analyzer->processed_text[i])) {
            if (!in_word) {
                in_word = 1;
                word_len = 0;
            }
            if (word_len < MAX_WORD_LEN - 1) {
                current_word[word_len++] = analyzer->processed_text[i];
            }
        } else {
            if (in_word && word_len > 0) {
                current_word[word_len] = '\0';
                int capitalized = isupper(current_word[0]) ? 1 : 0;
                if (!add_word(analyzer, current_word, word_len, capitalized)) {
                    return 0;
                }
                analyzer->total_words++;
                in_word = 0;
                word_len = 0;
            }
        }
    }
    
    if (in_word && word_len > 0) {
        current_word[word_len] = '\0';
        int capitalized = isupper(current_word[0]) ? 1 : 0;
        if (!add_word(analyzer, current_word, word_len, capitalized)) {
            return 0;
        }
        analyzer->total_words++;
    }
    
    return 1;
}

void print_analysis(const struct TextAnalyzer *analyzer) {
    printf("Text Analysis Results:\n");
    printf("Total characters: %d\n", analyzer->total_characters);
    printf("Total words: %d\n", analyzer->total_words);
    printf("Unique words: %d\n", analyzer->word_count);
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
    char input_text[MAX_TEXT_LEN];
    
    init_analyzer(&analyzer);
    
    printf("Enter text to analyze (max %d characters):\n", MAX_TEXT_LEN - 1);
    
    if (fgets(input_text, sizeof(input_text), stdin) == NULL) {
        printf("Error reading input.\n");