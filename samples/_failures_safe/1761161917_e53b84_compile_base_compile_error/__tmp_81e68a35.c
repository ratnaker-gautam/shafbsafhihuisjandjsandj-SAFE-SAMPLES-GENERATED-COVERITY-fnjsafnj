//DeepSeek-V3 V2.5 Category: Safe ; Style: struct_heavy ; Variation: string_processing
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
    char text[1024];
};

void init_analyzer(struct TextAnalyzer *analyzer) {
    analyzer->words = NULL;
    analyzer->word_count = 0;
    analyzer->capacity = 0;
    analyzer->text[0] = '\0';
}

void cleanup_analyzer(struct TextAnalyzer *analyzer) {
    if (analyzer->words != NULL) {
        free(analyzer->words);
        analyzer->words = NULL;
    }
    analyzer->word_count = 0;
    analyzer->capacity = 0;
}

int is_valid_char(char c) {
    return isalpha((unsigned char)c) || c == ' ' || c == '\0';
}

int count_vowels(const char *str) {
    int count = 0;
    for (int i = 0; str[i] != '\0'; i++) {
        char c = tolower((unsigned char)str[i]);
        if (c == 'a' || c == 'e' || c == 'i' || c == 'o' || c == 'u') {
            count++;
        }
    }
    return count;
}

int count_consonants(const char *str) {
    int count = 0;
    for (int i = 0; str[i] != '\0'; i++) {
        char c = tolower((unsigned char)str[i]);
        if (isalpha((unsigned char)c) && c != 'a' && c != 'e' && c != 'i' && c != 'o' && c != 'u') {
            count++;
        }
    }
    return count;
}

int add_word(struct TextAnalyzer *analyzer, const char *word) {
    if (analyzer->word_count >= analyzer->capacity) {
        int new_capacity = analyzer->capacity == 0 ? 8 : analyzer->capacity * 2;
        struct WordStats *new_words = realloc(analyzer->words, new_capacity * sizeof(struct WordStats));
        if (new_words == NULL) {
            return 0;
        }
        analyzer->words = new_words;
        analyzer->capacity = new_capacity;
    }
    
    struct WordStats *ws = &analyzer->words[analyzer->word_count];
    int len = strlen(word);
    if (len >= 64) {
        return 0;
    }
    
    strncpy(ws->word, word, 63);
    ws->word[63] = '\0';
    ws->length = len;
    ws->vowel_count = count_vowels(word);
    ws->consonant_count = count_consonants(word);
    analyzer->word_count++;
    return 1;
}

int process_text(struct TextAnalyzer *analyzer, const char *input) {
    if (input == NULL) {
        return 0;
    }
    
    size_t input_len = strlen(input);
    if (input_len >= 1024) {
        return 0;
    }
    
    for (size_t i = 0; i < input_len; i++) {
        if (!is_valid_char(input[i])) {
            return 0;
        }
    }
    
    strncpy(analyzer->text, input, 1023);
    analyzer->text[1023] = '\0';
    
    char buffer[1024];
    strncpy(buffer, input, 1023);
    buffer[1023] = '\0';
    
    char *token = strtok(buffer, " ");
    while (token != NULL) {
        if (strlen(token) > 0) {
            if (!add_word(analyzer, token)) {
                return 0;
            }
        }
        token = strtok(NULL, " ");
    }
    
    return 1;
}

void print_analysis(const struct TextAnalyzer *analyzer) {
    printf("Text: %s\n", analyzer->text);
    printf("Word count: %d\n", analyzer->word_count);
    printf("\nWord analysis:\n");
    
    for (int i = 0; i < analyzer->word_count; i++) {
        const struct WordStats *ws = &analyzer->words[i];
        printf("Word: %-15s Length: %2d  Vowels: %d  Consonants: %d\n", 
               ws->word, ws->length, ws->vowel_count, ws->consonant_count);
    }
}

int main(void) {
    struct TextAnalyzer analyzer;
    init_analyzer(&analyzer);
    
    char input[1024];
    printf("Enter text (letters and spaces only, max 1023 chars): ");
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        printf("Error reading input\n");
        cleanup_analyzer(&analyzer);
        return 1;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
    }
    
    if (strlen(input) == 0) {
        printf("Empty input\n");
        cleanup_analyzer(&analyzer);
        return 1;
    }
    
    if (!process_text(&analyzer, input)) {
        printf("Error processing text\n");
        cleanup_analyzer(&analy