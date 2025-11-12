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
    if (analyzer == NULL) return;
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
    if (analyzer == NULL || word == NULL) return -1;
    for (int i = 0; i < analyzer->word_count; i++) {
        if (strcmp(analyzer->words[i].word, word) == 0) {
            return i;
        }
    }
    return -1;
}

void add_word(struct TextAnalyzer *analyzer, const char *word, int length, int capitalized) {
    if (analyzer == NULL || word == NULL || length <= 0 || length >= MAX_WORD_LEN || analyzer->word_count >= MAX_WORDS) {
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
    analyzer->word_count++;
}

void process_text(struct TextAnalyzer *analyzer, const char *text) {
    if (analyzer == NULL || text == NULL) return;
    
    char buffer[MAX_TEXT_LEN];
    size_t text_len = strlen(text);
    if (text_len >= MAX_TEXT_LEN - 1) {
        return;
    }
    strncpy(buffer, text, MAX_TEXT_LEN - 1);
    buffer[MAX_TEXT_LEN - 1] = '\0';
    
    char *ptr = buffer;
    char word[MAX_WORD_LEN];
    int word_len = 0;
    int in_word = 0;
    
    while (*ptr != '\0') {
        if (is_word_char(*ptr)) {
            if (!in_word) {
                in_word = 1;
                word_len = 0;
            }
            if (word_len < MAX_WORD_LEN - 1) {
                word[word_len++] = *ptr;
            }
        } else {
            if (in_word && word_len > 0) {
                word[word_len] = '\0';
                int capitalized = isupper((unsigned char)word[0]) ? 1 : 0;
                for (int i = 0; i < word_len; i++) {
                    word[i] = tolower((unsigned char)word[i]);
                }
                add_word(analyzer, word, word_len, capitalized);
                analyzer->total_words++;
                analyzer->total_chars += word_len;
                if (word_len > analyzer->longest_word_length) {
                    analyzer->longest_word_length = word_len;
                }
                in_word = 0;
            }
        }
        ptr++;
    }
    
    if (in_word && word_len > 0) {
        word[word_len] = '\0';
        int capitalized = isupper((unsigned char)word[0]) ? 1 : 0;
        for (int i = 0; i < word_len; i++) {
            word[i] = tolower((unsigned char)word[i]);
        }
        add_word(analyzer, word, word_len, capitalized);
        analyzer->total_words++;
        analyzer->total_chars += word_len;
        if (word_len > analyzer->longest_word_length) {
            analyzer->longest_word_length = word_len;
        }
    }
}

void print_analysis(struct TextAnalyzer *analyzer) {
    if (analyzer == NULL) return;
    printf("Text Analysis Results:\n");
    printf("Total words: %d\n", analyzer->total_words);
    printf("Total characters (in words): %d\n", analyzer->total_chars);
    printf("Unique words: %d\n", analyzer->word_count);
    printf("Longest word length: %d\n", analyzer->longest_word_length);
    printf("\nWord frequency:\n");
    
    for (int i = 0; i < analyzer->word_count; i++) {