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
    int longest_word_length;
    int shortest_word_length;
};

void init_analyzer(struct TextAnalyzer *analyzer) {
    analyzer->word_count = 0;
    analyzer->total_words = 0;
    analyzer->longest_word_length = 0;
    analyzer->shortest_word_length = MAX_WORD_LEN;
    for (int i = 0; i < MAX_WORDS; i++) {
        analyzer->words[i].count = 0;
        analyzer->words[i].length = 0;
        analyzer->words[i].is_capitalized = 0;
        analyzer->words[i].word[0] = '\0';
    }
}

int is_word_char(char c) {
    return isalpha((unsigned char)c) || c == '\'' || c == '-';
}

int add_word(struct TextAnalyzer *analyzer, const char *word, int length) {
    if (length <= 0 || length >= MAX_WORD_LEN || analyzer->word_count >= MAX_WORDS) {
        return 0;
    }

    for (int i = 0; i < analyzer->word_count; i++) {
        if (strncmp(analyzer->words[i].word, word, length) == 0 && 
            analyzer->words[i].word[length] == '\0') {
            analyzer->words[i].count++;
            return 1;
        }
    }

    if (analyzer->word_count < MAX_WORDS) {
        strncpy(analyzer->words[analyzer->word_count].word, word, length);
        analyzer->words[analyzer->word_count].word[length] = '\0';
        analyzer->words[analyzer->word_count].count = 1;
        analyzer->words[analyzer->word_count].length = length;
        analyzer->words[analyzer->word_count].is_capitalized = isupper((unsigned char)word[0]);
        analyzer->word_count++;
        return 1;
    }
    return 0;
}

void process_text(struct TextAnalyzer *analyzer, const char *text) {
    if (text == NULL) return;

    const char *start = NULL;
    int in_word = 0;
    int length = 0;

    for (const char *p = text; *p != '\0'; p++) {
        if (is_word_char(*p)) {
            if (!in_word) {
                start = p;
                in_word = 1;
                length = 1;
            } else {
                length++;
            }
        } else {
            if (in_word && length > 0) {
                add_word(analyzer, start, length);
                analyzer->total_words++;
                in_word = 0;
                length = 0;
            }
        }
    }

    if (in_word && length > 0) {
        add_word(analyzer, start, length);
        analyzer->total_words++;
    }
}

void calculate_statistics(struct TextAnalyzer *analyzer) {
    if (analyzer->word_count == 0) return;

    analyzer->longest_word_length = 0;
    analyzer->shortest_word_length = MAX_WORD_LEN;

    for (int i = 0; i < analyzer->word_count; i++) {
        int len = analyzer->words[i].length;
        if (len > analyzer->longest_word_length) {
            analyzer->longest_word_length = len;
        }
        if (len < analyzer->shortest_word_length) {
            analyzer->shortest_word_length = len;
        }
    }
}

void print_analysis(const struct TextAnalyzer *analyzer) {
    printf("Text Analysis Results:\n");
    printf("Total words: %d\n", analyzer->total_words);
    printf("Unique words: %d\n", analyzer->word_count);
    printf("Longest word length: %d\n", analyzer->longest_word_length);
    printf("Shortest word length: %d\n", analyzer->shortest_word_length);
    printf("\nWord frequencies:\n");

    for (int i = 0; i < analyzer->word_count; i++) {
        printf("%s: %d\n", analyzer->words[i].word, analyzer->words[i].count);
    }
}

int main() {
    struct TextAnalyzer analyzer;
    init_analyzer(&analyzer);

    char input_buffer[4096];
    printf("Enter text to analyze (max %zu characters):\n", sizeof(input_buffer) - 1);
    
    if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) {
        printf("Error reading input.\n");
        return 1;
    }

    size_t len = strlen(input_buffer);
    if (len > 0 && input_buffer[len - 1] == '\n') {
        input_buffer[len - 1] = '\0';
    }

    if (strlen(input_buffer) == 0) {
        printf("No input provided.\n");
        return 1;
    }

    process_text(&analyzer, input_buffer);
    calculate_statistics(&analyzer);
    print_analysis(&analyzer);

    return 0;
}