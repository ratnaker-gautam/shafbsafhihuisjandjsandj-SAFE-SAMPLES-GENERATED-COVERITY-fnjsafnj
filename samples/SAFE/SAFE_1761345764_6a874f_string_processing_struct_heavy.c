//DeepSeek-V3 V2.5 Category: Safe ; Style: struct_heavy ; Variation: string_processing
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_WORD_LEN 64
#define MAX_WORDS 100
#define MAX_TEXT_LEN 1024

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
    int longest_word_length;
    int shortest_word_length;
};

void init_analyzer(struct TextAnalyzer *analyzer) {
    analyzer->word_count = 0;
    analyzer->total_words = 0;
    analyzer->longest_word_length = 0;
    analyzer->shortest_word_length = MAX_WORD_LEN;
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

void clean_word(char *word) {
    int len = strlen(word);
    while (len > 0 && !isalpha((unsigned char)word[len - 1])) {
        word[len - 1] = '\0';
        len--;
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

void add_word(struct TextAnalyzer *analyzer, const char *word) {
    if (analyzer->word_count >= MAX_WORDS) {
        return;
    }
    
    char clean[MAX_WORD_LEN];
    strncpy(clean, word, MAX_WORD_LEN - 1);
    clean[MAX_WORD_LEN - 1] = '\0';
    clean_word(clean);
    
    if (strlen(clean) == 0) {
        return;
    }
    
    int index = find_word_index(analyzer, clean);
    if (index >= 0) {
        analyzer->words[index].count++;
        return;
    }
    
    struct WordStats *ws = &analyzer->words[analyzer->word_count];
    strncpy(ws->word, clean, MAX_WORD_LEN - 1);
    ws->word[MAX_WORD_LEN - 1] = '\0';
    ws->count = 1;
    ws->length = strlen(clean);
    ws->is_capitalized = isupper((unsigned char)clean[0]) ? 1 : 0;
    
    if (ws->length > analyzer->longest_word_length) {
        analyzer->longest_word_length = ws->length;
    }
    if (ws->length < analyzer->shortest_word_length) {
        analyzer->shortest_word_length = ws->length;
    }
    
    analyzer->word_count++;
    analyzer->total_words++;
}

void process_text(struct TextAnalyzer *analyzer, const char *text) {
    char buffer[MAX_TEXT_LEN];
    strncpy(buffer, text, MAX_TEXT_LEN - 1);
    buffer[MAX_TEXT_LEN - 1] = '\0';
    
    char *token = strtok(buffer, " \t\n\r");
    while (token != NULL && analyzer->total_words < MAX_WORDS) {
        add_word(analyzer, token);
        token = strtok(NULL, " \t\n\r");
    }
}

void print_analysis(struct TextAnalyzer *analyzer) {
    printf("Text Analysis Results:\n");
    printf("Total unique words: %d\n", analyzer->word_count);
    printf("Total word occurrences: %d\n", analyzer->total_words);
    printf("Longest word length: %d\n", analyzer->longest_word_length);
    printf("Shortest word length: %d\n", analyzer->shortest_word_length);
    printf("\nWord Frequency:\n");
    
    for (int i = 0; i < analyzer->word_count; i++) {
        printf("%-20s: %d occurrences", analyzer->words[i].word, analyzer->words[i].count);
        if (analyzer->words[i].is_capitalized) {
            printf(" (capitalized)");
        }
        printf("\n");
    }
}

int main() {
    struct TextAnalyzer analyzer;
    init_analyzer(&analyzer);
    
    char input[MAX_TEXT_LEN];
    printf("Enter text to analyze (max %d characters):\n", MAX_TEXT_LEN - 1);
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        printf("Error reading input.\n");
        return 1;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
    }
    
    if (strlen(input) == 0) {
        printf("No input provided.\n");
        return 1;
    }
    
    process_text(&analyzer, input);
    print_analysis(&analyzer);
    
    return 0;
}