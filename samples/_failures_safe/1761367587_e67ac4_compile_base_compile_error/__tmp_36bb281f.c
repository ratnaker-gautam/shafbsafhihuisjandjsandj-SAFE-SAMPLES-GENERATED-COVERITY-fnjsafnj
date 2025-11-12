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
        analyzer->words[i].count = 0;
        analyzer->words[i].length = 0;
        analyzer->words[i].is_capitalized = 0;
        analyzer->words[i].word[0] = '\0';
    }
}

int is_word_character(char c) {
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
    
    char normalized[MAX_WORD_LEN];
    size_t word_len = strlen(word);
    if (word_len >= MAX_WORD_LEN) {
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
    analyzer->words[new_index].length = (int)word_len;
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
    int in_word = 0;
    int word_start_capitalized = 0;
    
    for (int i = 0; text[i] != '\0'; i++) {
        if (is_word_character(text[i])) {
            if (!in_word) {
                in_word = 1;
                buf_index = 0;
                word_start_capitalized = isupper((unsigned char)text[i]);
            }
            if (buf_index < MAX_WORD_LEN - 1) {
                buffer[buf_index++] = text[i];
            }
        } else {
            if (in_word) {
                buffer[buf_index] = '\0';
                if (buf_index > 0) {
                    add_word(analyzer, buffer, word_start_capitalized);
                    analyzer->total_words++;
                }
                in_word = 0;
            }
        }
    }
    
    if (in_word && buf_index > 0) {
        buffer[buf_index] = '\0';
        add_word(analyzer, buffer, word_start_capitalized);
        analyzer->total_words++;
    }
}

void print_analysis(struct TextAnalyzer *analyzer) {
    printf("Text Analysis Results:\n");
    printf("Total words: %d\n", analyzer->total_words);
    printf("Unique words: %d\n", analyzer->unique_words);
    printf("Longest word length: %d\n\n", analyzer->longest_word_length);
    
    printf("Word frequency (appearing more than once):\n");
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
}

int main() {
    struct TextAnalyzer analyzer;
    init_analyzer(&analyzer);
    
    char input_buffer