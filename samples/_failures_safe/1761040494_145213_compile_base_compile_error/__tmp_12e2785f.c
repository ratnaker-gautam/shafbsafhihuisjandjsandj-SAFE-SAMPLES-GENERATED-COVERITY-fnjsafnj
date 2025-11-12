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
    int total_chars;
    int total_words;
    int unique_words;
};

void init_analyzer(struct TextAnalyzer *analyzer) {
    analyzer->word_count = 0;
    analyzer->total_chars = 0;
    analyzer->total_words = 0;
    analyzer->unique_words = 0;
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

int find_word(struct TextAnalyzer *analyzer, const char *word) {
    for (int i = 0; i < analyzer->word_count; i++) {
        if (strcmp(analyzer->words[i].word, word) == 0) {
            return i;
        }
    }
    return -1;
}

int add_word(struct TextAnalyzer *analyzer, const char *word, int is_cap) {
    if (analyzer->word_count >= MAX_WORDS) {
        return -1;
    }
    
    char temp[MAX_WORD_LEN];
    if (strlen(word) >= MAX_WORD_LEN) {
        return -1;
    }
    strcpy(temp, word);
    to_lower_case(temp);
    
    int idx = find_word(analyzer, temp);
    if (idx >= 0) {
        analyzer->words[idx].count++;
        if (is_cap) {
            analyzer->words[idx].is_capitalized = 1;
        }
        return idx;
    }
    
    idx = analyzer->word_count;
    strcpy(analyzer->words[idx].word, temp);
    analyzer->words[idx].count = 1;
    analyzer->words[idx].length = strlen(temp);
    analyzer->words[idx].is_capitalized = is_cap;
    analyzer->word_count++;
    analyzer->unique_words++;
    
    return idx;
}

void process_text(struct TextAnalyzer *analyzer, const char *text) {
    if (text == NULL) {
        return;
    }
    
    char buffer[MAX_WORD_LEN];
    int buf_idx = 0;
    int in_word = 0;
    
    for (int i = 0; text[i] != '\0'; i++) {
        char c = text[i];
        analyzer->total_chars++;
        
        if (is_word_char(c)) {
            if (!in_word) {
                in_word = 1;
                buf_idx = 0;
            }
            if (buf_idx < MAX_WORD_LEN - 1) {
                buffer[buf_idx++] = c;
            }
        } else {
            if (in_word) {
                buffer[buf_idx] = '\0';
                if (buf_idx > 0) {
                    int is_cap = isupper((unsigned char)buffer[0]);
                    add_word(analyzer, buffer, is_cap);
                    analyzer->total_words++;
                }
                in_word = 0;
            }
        }
    }
    
    if (in_word && buf_idx > 0) {
        buffer[buf_idx] = '\0';
        int is_cap = isupper((unsigned char)buffer[0]);
        add_word(analyzer, buffer, is_cap);
        analyzer->total_words++;
    }
}

void print_analysis(struct TextAnalyzer *analyzer) {
    printf("Text Analysis Results:\n");
    printf("Total characters: %d\n", analyzer->total_chars);
    printf("Total words: %d\n", analyzer->total_words);
    printf("Unique words: %d\n", analyzer->unique_words);
    printf("\nWord frequency (top 10):\n");
    
    for (int i = 0; i < analyzer->word_count && i < 10; i++) {
        int max_idx = i;
        for (int j = i + 1; j < analyzer->word_count; j++) {
            if (analyzer->words[j].count > analyzer->words[max_idx].count) {
                max_idx = j;
            }
        }
        
        if (max_idx != i) {
            struct WordStats temp = analyzer->words[i];
            analyzer->words[i] = analyzer->words[max_idx];
            analyzer->words[max_idx] = temp;
        }
        
        printf("%s: %d", analyzer->words[i].word, analyzer->words[i].count);
        if (analyzer->words[i].is_capitalized) {
            printf(" (capitalized)");
        }
        printf("\n");
    }
}

int main(void) {
    struct TextAnalyzer analyzer;
    init_analyzer(&analyzer);
    
    char input[4096];
    printf("Enter text to analyze (max 409