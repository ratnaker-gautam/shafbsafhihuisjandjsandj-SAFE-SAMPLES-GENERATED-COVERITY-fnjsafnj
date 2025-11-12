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
    analyzer->words = malloc(sizeof(struct WordStats) * 50);
    analyzer->capacity = 50;
    analyzer->word_count = 0;
    analyzer->text[0] = '\0';
}

void cleanup_analyzer(struct TextAnalyzer *analyzer) {
    free(analyzer->words);
}

int is_valid_char(char c) {
    return isalpha(c) || c == ' ' || c == '\0';
}

void analyze_word(struct WordStats *stats, const char *word) {
    strncpy(stats->word, word, 63);
    stats->word[63] = '\0';
    stats->length = 0;
    stats->vowel_count = 0;
    stats->consonant_count = 0;
    
    for (int i = 0; word[i] != '\0' && i < 63; i++) {
        char c = tolower(word[i]);
        if (isalpha(c)) {
            stats->length++;
            if (c == 'a' || c == 'e' || c == 'i' || c == 'o' || c == 'u') {
                stats->vowel_count++;
            } else {
                stats->consonant_count++;
            }
        }
    }
}

int process_text(struct TextAnalyzer *analyzer, const char *input) {
    if (input == NULL) return 0;
    
    size_t input_len = strlen(input);
    if (input_len == 0 || input_len >= 1023) return 0;
    
    for (size_t i = 0; i < input_len; i++) {
        if (!is_valid_char(input[i])) return 0;
    }
    
    strncpy(analyzer->text, input, 1023);
    analyzer->text[1023] = '\0';
    
    char buffer[1024];
    strncpy(buffer, input, 1023);
    buffer[1023] = '\0';
    
    char *token = strtok(buffer, " ");
    analyzer->word_count = 0;
    
    while (token != NULL && analyzer->word_count < analyzer->capacity) {
        if (strlen(token) > 0) {
            analyze_word(&analyzer->words[analyzer->word_count], token);
            analyzer->word_count++;
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

int main() {
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
    if (len > 0 && input[len-1] == '\n') {
        input[len-1] = '\0';
    }
    
    if (!process_text(&analyzer, input)) {
        printf("Invalid input\n");
        cleanup_analyzer(&analyzer);
        return 1;
    }
    
    print_analysis(&analyzer);
    cleanup_analyzer(&analyzer);
    
    return 0;
}