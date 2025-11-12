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
    int unique_words;
    int longest_word_length;
};

void init_analyzer(struct TextAnalyzer *analyzer) {
    analyzer->word_count = 0;
    analyzer->total_words = 0;
    analyzer->unique_words = 0;
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

void to_lower_case(char *str) {
    for (int i = 0; str[i] != '\0'; i++) {
        str[i] = tolower((unsigned char)str[i]);
    }
}

int find_word_index(struct TextAnalyzer *analyzer, const char *word) {
    for (int i = 0; i < analyzer->word_count; i++) {
        if (strcmp(analyzer->words[i].word, word) == 0) {
            return i;
        }
    }
    return -1;
}

int add_word(struct TextAnalyzer *analyzer, const char *word, int is_capitalized) {
    if (analyzer->word_count >= MAX_WORDS) {
        return -1;
    }
    
    char normalized[MAX_WORD_LEN];
    if (strlen(word) >= MAX_WORD_LEN) {
        return -1;
    }
    strcpy(normalized, word);
    to_lower_case(normalized);
    
    int index = find_word_index(analyzer, normalized);
    if (index >= 0) {
        analyzer->words[index].count++;
        if (is_capitalized) {
            analyzer->words[index].is_capitalized = 1;
        }
        return index;
    }
    
    int new_index = analyzer->word_count;
    strcpy(analyzer->words[new_index].word, normalized);
    analyzer->words[new_index].count = 1;
    analyzer->words[new_index].length = strlen(normalized);
    analyzer->words[new_index].is_capitalized = is_capitalized;
    analyzer->word_count++;
    analyzer->unique_words++;
    
    if (analyzer->words[new_index].length > analyzer->longest_word_length) {
        analyzer->longest_word_length = analyzer->words[new_index].length;
    }
    
    return new_index;
}

void process_text(struct TextAnalyzer *analyzer, const char *text) {
    if (text == NULL) {
        return;
    }
    
    char buffer[MAX_WORD_LEN];
    int buf_index = 0;
    int text_len = strlen(text);
    
    for (int i = 0; i <= text_len; i++) {
        char c = text[i];
        
        if (is_word_char(c) && buf_index < MAX_WORD_LEN - 1) {
            buffer[buf_index++] = c;
        } else if (buf_index > 0) {
            buffer[buf_index] = '\0';
            
            int is_cap = isupper((unsigned char)buffer[0]) ? 1 : 0;
            add_word(analyzer, buffer, is_cap);
            analyzer->total_words++;
            
            buf_index = 0;
        }
    }
}

void print_analysis(struct TextAnalyzer *analyzer) {
    printf("Text Analysis Results:\n");
    printf("Total words: %d\n", analyzer->total_words);
    printf("Unique words: %d\n", analyzer->unique_words);
    printf("Longest word length: %d\n", analyzer->longest_word_length);
    printf("\nWord frequency (min 2 occurrences):\n");
    
    for (int i = 0; i < analyzer->word_count; i++) {
        if (analyzer->words[i].count >= 2) {
            printf("%s: %d", analyzer->words[i].word, analyzer->words[i].count);
            if (analyzer->words[i].is_capitalized) {
                printf(" (capitalized)");
            }
            printf("\n");
        }
    }
    
    printf("\nAll unique words:\n");
    for (int i = 0; i < analyzer->word_count; i++) {
        printf("%s ", analyzer->words[i].word);
        if ((i + 1) % 8 == 0) {
            printf("\n");
        }
    }
    printf("\n");
}

int main() {
    struct TextAnalyzer analyzer;
    init_analyzer(&analyzer);
    
    char input_text[4096];
    printf("Enter text to analyze (max 4095 characters):\n");
    
    if (fgets(input_text, sizeof(input_text), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(input_text);
    if