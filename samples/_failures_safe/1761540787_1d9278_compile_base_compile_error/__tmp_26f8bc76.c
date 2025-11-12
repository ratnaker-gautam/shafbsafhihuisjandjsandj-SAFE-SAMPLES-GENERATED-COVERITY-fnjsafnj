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
        analyzer->words[i].word[0] = '\0';
        analyzer->words[i].count = 0;
        analyzer->words[i].length = 0;
        analyzer->words[i].is_capitalized = 0;
    }
}

int is_word_char(char c) {
    return isalnum((unsigned char)c) || c == '\'' || c == '-';
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
    
    char temp_word[MAX_WORD_LEN];
    if (strlen(word) >= MAX_WORD_LEN) {
        return -1;
    }
    strncpy(temp_word, word, MAX_WORD_LEN - 1);
    temp_word[MAX_WORD_LEN - 1] = '\0';
    to_lower_case(temp_word);
    
    int index = find_word_index(analyzer, temp_word);
    if (index != -1) {
        analyzer->words[index].count++;
        if (is_capitalized) {
            analyzer->words[index].is_capitalized = 1;
        }
        return index;
    }
    
    if (analyzer->word_count < MAX_WORDS) {
        struct WordStats *ws = &analyzer->words[analyzer->word_count];
        strncpy(ws->word, temp_word, MAX_WORD_LEN - 1);
        ws->word[MAX_WORD_LEN - 1] = '\0';
        ws->count = 1;
        ws->length = strlen(temp_word);
        ws->is_capitalized = is_capitalized;
        analyzer->word_count++;
        analyzer->unique_words++;
        return analyzer->word_count - 1;
    }
    
    return -1;
}

void process_text(struct TextAnalyzer *analyzer, const char *text) {
    if (text == NULL) {
        return;
    }
    
    char buffer[MAX_WORD_LEN];
    int buf_index = 0;
    int in_word = 0;
    
    for (int i = 0; text[i] != '\0'; i++) {
        if (is_word_char(text[i])) {
            if (buf_index < MAX_WORD_LEN - 1) {
                buffer[buf_index++] = text[i];
            }
            in_word = 1;
        } else {
            if (in_word && buf_index > 0) {
                buffer[buf_index] = '\0';
                int is_cap = isupper((unsigned char)buffer[0]);
                add_word(analyzer, buffer, is_cap);
                analyzer->total_words++;
                analyzer->total_chars += buf_index;
                buf_index = 0;
            }
            in_word = 0;
        }
    }
    
    if (in_word && buf_index > 0) {
        buffer[buf_index] = '\0';
        int is_cap = isupper((unsigned char)buffer[0]);
        add_word(analyzer, buffer, is_cap);
        analyzer->total_words++;
        analyzer->total_chars += buf_index;
    }
}

void print_analysis(struct TextAnalyzer *analyzer) {
    printf("Text Analysis Results:\n");
    printf("Total words: %d\n", analyzer->total_words);
    printf("Total characters in words: %d\n", analyzer->total_chars);
    printf("Unique words: %d\n", analyzer->unique_words);
    printf("\nWord frequency (appearing more than once):\n");
    
    int found_frequent = 0;
    for (int i = 0; i < analyzer->word_count; i++) {
        if (analyzer->words[i].count > 1) {
            printf("'%s': %d occurrences", analyzer->words[i].word, analyzer->words[i].count);
            if (analyzer->words[i].is_capitalized) {
                printf(" (capitalized)");
            }
            printf("\n");
            found_frequent = 1;
        }
    }
    
    if (!found_frequent) {
        printf("No words appear more than once.\n");
    }
    
    printf("\