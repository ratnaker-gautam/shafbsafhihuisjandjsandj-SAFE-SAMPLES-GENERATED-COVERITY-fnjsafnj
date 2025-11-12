//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: struct_heavy ; Variation: string_processing
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

struct WordStats {
    char word[64];
    int length;
    int vowel_count;
    int consonant_count;
};

struct TextAnalyzer {
    struct WordStats *words;
    int word_count;
    int capacity;
    char *text;
    int total_chars;
    int total_words;
};

void init_analyzer(struct TextAnalyzer *analyzer, int initial_capacity) {
    if (initial_capacity <= 0) initial_capacity = 10;
    analyzer->words = malloc(initial_capacity * sizeof(struct WordStats));
    if (!analyzer->words) exit(EXIT_FAILURE);
    analyzer->capacity = initial_capacity;
    analyzer->word_count = 0;
    analyzer->text = NULL;
    analyzer->total_chars = 0;
    analyzer->total_words = 0;
}

void free_analyzer(struct TextAnalyzer *analyzer) {
    if (analyzer->words) free(analyzer->words);
    if (analyzer->text) free(analyzer->text);
}

int is_valid_char(char c) {
    return isalpha(c) || isspace(c) || c == '.' || c == ',' || c == '!' || c == '?';
}

int count_vowels(const char *str, int len) {
    int count = 0;
    for (int i = 0; i < len; i++) {
        char c = tolower(str[i]);
        if (c == 'a' || c == 'e' || c == 'i' || c == 'o' || c == 'u') count++;
    }
    return count;
}

void add_word(struct TextAnalyzer *analyzer, const char *word, int len) {
    if (len <= 0 || len >= 64) return;
    if (analyzer->word_count >= analyzer->capacity) {
        int new_cap = analyzer->capacity * 2;
        struct WordStats *new_words = realloc(analyzer->words, new_cap * sizeof(struct WordStats));
        if (!new_words) return;
        analyzer->words = new_words;
        analyzer->capacity = new_cap;
    }
    struct WordStats *ws = &analyzer->words[analyzer->word_count];
    strncpy(ws->word, word, len);
    ws->word[len] = '\0';
    ws->length = len;
    ws->vowel_count = count_vowels(word, len);
    ws->consonant_count = len - ws->vowel_count;
    analyzer->word_count++;
}

void process_text(struct TextAnalyzer *analyzer, const char *input) {
    if (!input) return;
    int len = strlen(input);
    if (len <= 0) return;
    analyzer->total_chars = len;
    analyzer->text = malloc(len + 1);
    if (!analyzer->text) return;
    strncpy(analyzer->text, input, len);
    analyzer->text[len] = '\0';
    
    char buffer[64];
    int buf_idx = 0;
    for (int i = 0; i <= len; i++) {
        char c = input[i];
        if (isalpha(c)) {
            if (buf_idx < 63) {
                buffer[buf_idx++] = c;
            }
        } else {
            if (buf_idx > 0) {
                buffer[buf_idx] = '\0';
                add_word(analyzer, buffer, buf_idx);
                analyzer->total_words++;
                buf_idx = 0;
            }
        }
    }
}

void print_analysis(const struct TextAnalyzer *analyzer) {
    printf("Text Analysis Results:\n");
    printf("Total characters: %d\n", analyzer->total_chars);
    printf("Total words: %d\n", analyzer->total_words);
    printf("\nWord Details:\n");
    for (int i = 0; i < analyzer->word_count; i++) {
        const struct WordStats *ws = &analyzer->words[i];
        printf("Word: %-15s Length: %2d  Vowels: %d  Consonants: %d\n", 
               ws->word, ws->length, ws->vowel_count, ws->consonant_count);
    }
}

int main() {
    struct TextAnalyzer analyzer;
    init_analyzer(&analyzer, 20);
    
    char input[1024];
    printf("Enter text to analyze: ");
    if (!fgets(input, sizeof(input), stdin)) {
        printf("Error reading input\n");
        free_analyzer(&analyzer);
        return EXIT_FAILURE;
    }
    
    input[strcspn(input, "\n")] = '\0';
    
    int valid = 1;
    for (int i = 0; input[i] != '\0'; i++) {
        if (!is_valid_char(input[i])) {
            valid = 0;
            break;
        }
    }
    
    if (!valid) {
        printf("Input contains invalid characters\n");
        free_analyzer(&analyzer);
        return EXIT_FAILURE;
    }
    
    process_text(&analyzer, input);
    print_analysis(&analyzer);
    
    free_analyzer(&analyzer);
    return EXIT_SUCCESS;
}