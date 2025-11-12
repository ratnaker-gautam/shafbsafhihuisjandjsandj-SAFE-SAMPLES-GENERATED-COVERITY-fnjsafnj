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
    
    char temp_word[MAX_WORD_LEN];
    if (strlen(word) >= MAX_WORD_LEN) {
        return -1;
    }
    strncpy(temp_word, word, MAX_WORD_LEN - 1);
    temp_word[MAX_WORD_LEN - 1] = '\0';
    to_lower_case(temp_word);
    
    int index = find_word(analyzer, temp_word);
    if (index != -1) {
        analyzer->words[index].count++;
        if (capitalized && !analyzer->words[index].is_capitalized) {
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
        ws->is_capitalized = capitalized ? 1 : 0;
        analyzer->word_count++;
        analyzer->unique_words++;
        return analyzer->word_count - 1;
    }
    
    return -1;
}

void process_text(struct TextAnalyzer *analyzer, const char *text) {
    if (!text) return;
    
    int len = strlen(text);
    if (len == 0) return;
    
    char buffer[MAX_WORD_LEN];
    int buf_pos = 0;
    int in_word = 0;
    int word_start_cap = 0;
    
    for (int i = 0; i < len && buf_pos < MAX_WORD_LEN - 1; i++) {
        char c = text[i];
        
        if (is_word_char(c)) {
            if (!in_word) {
                word_start_cap = isupper((unsigned char)c) ? 1 : 0;
                in_word = 1;
                buf_pos = 0;
            }
            buffer[buf_pos++] = c;
        } else {
            if (in_word && buf_pos > 0) {
                buffer[buf_pos] = '\0';
                add_word(analyzer, buffer, word_start_cap);
                analyzer->total_words++;
                analyzer->total_chars += buf_pos;
                in_word = 0;
                buf_pos = 0;
            }
        }
    }
    
    if (in_word && buf_pos > 0) {
        buffer[buf_pos] = '\0';
        add_word(analyzer, buffer, word_start_cap);
        analyzer->total_words++;
        analyzer->total_chars += buf_pos;
    }
}

void print_analysis(struct TextAnalyzer *analyzer) {
    printf("Text Analysis Results:\n");
    printf("Total words: %d\n", analyzer->total_words);
    printf("Total characters: %d\n", analyzer->total_chars);
    printf("Unique words: %d\n", analyzer->unique_words);
    printf("\nWord frequency (min 2 occurrences):\n");
    
    int printed = 0;
    for (int i = 0; i < analyzer->word_count; i++) {
        if (analyzer->words[i].count >= 2) {
            printf("%s: %d", analyzer->words[i].word, analyzer->words[i].count);
            if (analyzer->words[i].is_capitalized) {
                printf(" (capitalized)");
            }
            printf("\n");
            printed++;
        }
    }
    
    if (printed ==