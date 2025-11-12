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

int add_word(struct TextAnalyzer *analyzer, const char *word, int length, int capitalized) {
    if (length <= 0 || length >= MAX_WORD_LEN || analyzer->word_count >= MAX_WORDS) {
        return 0;
    }

    char normalized[MAX_WORD_LEN];
    if (length >= MAX_WORD_LEN) {
        return 0;
    }
    strncpy(normalized, word, length);
    normalized[length] = '\0';
    to_lower_case(normalized);

    for (int i = 0; i < analyzer->word_count; i++) {
        if (strcmp(analyzer->words[i].word, normalized) == 0) {
            analyzer->words[i].count++;
            if (capitalized) {
                analyzer->words[i].is_capitalized = 1;
            }
            return 1;
        }
    }

    if (analyzer->word_count < MAX_WORDS) {
        strncpy(analyzer->words[analyzer->word_count].word, normalized, MAX_WORD_LEN - 1);
        analyzer->words[analyzer->word_count].word[MAX_WORD_LEN - 1] = '\0';
        analyzer->words[analyzer->word_count].count = 1;
        analyzer->words[analyzer->word_count].length = length;
        analyzer->words[analyzer->word_count].is_capitalized = capitalized;
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
    if (text == NULL) {
        return;
    }

    int length = strlen(text);
    if (length <= 0) {
        return;
    }

    int start = -1;
    int capitalized = 0;

    for (int i = 0; i <= length; i++) {
        if (i < length && is_word_char(text[i])) {
            if (start == -1) {
                start = i;
                capitalized = isupper((unsigned char)text[i]) ? 1 : 0;
            }
        } else {
            if (start != -1) {
                int word_length = i - start;
                if (word_length > 0 && word_length < MAX_WORD_LEN) {
                    add_word(analyzer, text + start, word_length, capitalized);
                    analyzer->total_words++;
                }
                start = -1;
            }
        }
    }
}

void print_analysis(const struct TextAnalyzer *analyzer) {
    printf("Text Analysis Results:\n");
    printf("Total words: %d\n", analyzer->total_words);
    printf("Unique words: %d\n", analyzer->unique_words);
    printf("Longest word length: %d\n", analyzer->longest_word_length);
    printf("\nWord frequencies:\n");

    for (int i = 0; i < analyzer->word_count; i++) {
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

    char input_buffer[4096];
    printf("Enter text to analyze (max 4095 characters):\n");
    
    if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
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
    print_analysis(&analyzer