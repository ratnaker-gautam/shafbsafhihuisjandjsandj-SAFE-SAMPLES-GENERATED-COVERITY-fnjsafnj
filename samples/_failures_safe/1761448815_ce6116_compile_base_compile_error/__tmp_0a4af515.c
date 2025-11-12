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
        analyzer->words[i].count = 0;
        analyzer->words[i].length = 0;
        analyzer->words[i].is_capitalized = 0;
        memset(analyzer->words[i].word, 0, MAX_WORD_LEN);
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

int add_word(struct TextAnalyzer *analyzer, const char *word, int capitalized) {
    if (analyzer->word_count >= MAX_WORDS) {
        return -1;
    }
    
    char temp[MAX_WORD_LEN];
    strncpy(temp, word, MAX_WORD_LEN - 1);
    temp[MAX_WORD_LEN - 1] = '\0';
    to_lower_case(temp);
    
    int index = find_word(analyzer, temp);
    if (index != -1) {
        analyzer->words[index].count++;
        if (capitalized && !analyzer->words[index].is_capitalized) {
            analyzer->words[index].is_capitalized = 1;
        }
        return index;
    }
    
    if (strlen(temp) >= MAX_WORD_LEN) {
        return -1;
    }
    
    strcpy(analyzer->words[analyzer->word_count].word, temp);
    analyzer->words[analyzer->word_count].count = 1;
    analyzer->words[analyzer->word_count].length = strlen(temp);
    analyzer->words[analyzer->word_count].is_capitalized = capitalized ? 1 : 0;
    analyzer->word_count++;
    analyzer->unique_words++;
    
    return analyzer->word_count - 1;
}

void process_text(struct TextAnalyzer *analyzer, const char *text) {
    if (text == NULL) {
        return;
    }
    
    char buffer[MAX_WORD_LEN];
    int buf_pos = 0;
    int in_word = 0;
    int word_start_cap = 0;
    
    for (int i = 0; text[i] != '\0'; i++) {
        analyzer->total_chars++;
        
        if (is_word_char(text[i])) {
            if (!in_word) {
                in_word = 1;
                buf_pos = 0;
                word_start_cap = isupper((unsigned char)text[i]) ? 1 : 0;
            }
            
            if (buf_pos < MAX_WORD_LEN - 1) {
                buffer[buf_pos++] = text[i];
            }
        } else {
            if (in_word) {
                buffer[buf_pos] = '\0';
                if (buf_pos > 0) {
                    add_word(analyzer, buffer, word_start_cap);
                    analyzer->total_words++;
                }
                in_word = 0;
            }
        }
    }
    
    if (in_word && buf_pos > 0) {
        buffer[buf_pos] = '\0';
        add_word(analyzer, buffer, word_start_cap);
        analyzer->total_words++;
    }
}

void print_analysis(struct TextAnalyzer *analyzer) {
    printf("Text Analysis Results:\n");
    printf("Total characters: %d\n", analyzer->total_chars);
    printf("Total words: %d\n", analyzer->total_words);
    printf("Unique words: %d\n", analyzer->unique_words);
    printf("\nWord frequency (appearing more than once):\n");
    
    int printed = 0;
    for (int i = 0; i < analyzer->word_count; i++) {
        if (analyzer->words[i].count > 1) {
            printf("'%s': %d occurrences", analyzer->words[i].word, analyzer->words[i].count);
            if (analyzer->words[i].is_capitalized) {
                printf(" (capitalized)");
            }
            printf("\n");
            printed++;
        }
    }
    
    if (printed == 0) {
        printf("No words appear more than once.\n");
    }
    
    printf("\nLongest words:\n");
    int max_len = 0;
    for (int i = 0; i < analyzer->word_count