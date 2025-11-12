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

int is_word_character(char c) {
    return isalnum((unsigned char)c) || c == '\'' || c == '-';
}

int find_word_index(struct TextAnalyzer *analyzer, const char *word) {
    for (int i = 0; i < analyzer->word_count; i++) {
        if (strcmp(analyzer->words[i].word, word) == 0) {
            return i;
        }
    }
    return -1;
}

int add_word(struct TextAnalyzer *analyzer, const char *word, int length, int capitalized) {
    if (analyzer->word_count >= MAX_WORDS || length >= MAX_WORD_LEN || length <= 0) {
        return 0;
    }
    
    int index = find_word_index(analyzer, word);
    if (index >= 0) {
        analyzer->words[index].count++;
        return 1;
    }
    
    if (analyzer->word_count < MAX_WORDS) {
        struct WordStats *ws = &analyzer->words[analyzer->word_count];
        strncpy(ws->word, word, MAX_WORD_LEN - 1);
        ws->word[MAX_WORD_LEN - 1] = '\0';
        ws->count = 1;
        ws->length = length;
        ws->is_capitalized = capitalized;
        analyzer->word_count++;
        analyzer->unique_words++;
        
        if (length > analyzer->longest_word_length) {
            analyzer->longest_word_length = length;
        }
        return 1;
    }
    return 0;
}

void process_text(struct TextAnalyzer *analyzer, const char *text) {
    if (text == NULL) return;
    
    int len = strlen(text);
    if (len <= 0) return;
    
    char buffer[MAX_WORD_LEN];
    int buf_pos = 0;
    int in_word = 0;
    
    for (int i = 0; i < len && buf_pos < MAX_WORD_LEN - 1; i++) {
        char c = text[i];
        
        if (is_word_character(c)) {
            if (!in_word) {
                in_word = 1;
                buf_pos = 0;
            }
            buffer[buf_pos++] = c;
        } else {
            if (in_word && buf_pos > 0) {
                buffer[buf_pos] = '\0';
                int capitalized = isupper((unsigned char)buffer[0]) ? 1 : 0;
                if (add_word(analyzer, buffer, buf_pos, capitalized)) {
                    analyzer->total_words++;
                }
                in_word = 0;
                buf_pos = 0;
            }
        }
    }
    
    if (in_word && buf_pos > 0) {
        buffer[buf_pos] = '\0';
        int capitalized = isupper((unsigned char)buffer[0]) ? 1 : 0;
        if (add_word(analyzer, buffer, buf_pos, capitalized)) {
            analyzer->total_words++;
        }
    }
}

void print_analysis(struct TextAnalyzer *analyzer) {
    printf("Text Analysis Results:\n");
    printf("Total words: %d\n", analyzer->total_words);
    printf("Unique words: %d\n", analyzer->unique_words);
    printf("Longest word length: %d\n", analyzer->longest_word_length);
    printf("\nWord frequency (appearing more than once):\n");
    
    int printed = 0;
    for (int i = 0; i < analyzer->word_count; i++) {
        if (analyzer->words[i].count > 1) {
            printf("'%s': %d times\n", analyzer->words[i].word, analyzer->words[i].count);
            printed++;
        }
    }
    
    if (printed == 0) {
        printf("No repeated words found.\n");
    }
    
    printf("\nCapitalized words:\n");
    printed = 0;
    for (int i = 0; i < analyzer->word_count; i++) {
        if (analyzer->words[i].is_capitalized) {
            printf("'%s'\n", analyzer->words[i].word);
            printed++;
        }
    }
    
    if (printed == 0) {
        printf("No capitalized words found.\n");
    }
}

int main(void) {
    struct TextAnalyzer analyzer;
    init_analyzer(&analyzer);
    
    char input_buffer[4096];