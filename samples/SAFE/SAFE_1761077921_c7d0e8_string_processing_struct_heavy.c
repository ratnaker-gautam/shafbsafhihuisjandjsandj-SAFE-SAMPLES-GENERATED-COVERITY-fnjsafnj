//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: struct_heavy ; Variation: string_processing
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_WORD_LEN 64
#define MAX_WORDS 1000
#define MAX_TEXT_LEN 10000

struct WordStats {
    char word[MAX_WORD_LEN];
    int count;
    int length;
    int is_capitalized;
};

struct TextProcessor {
    struct WordStats words[MAX_WORDS];
    int word_count;
    char processed_text[MAX_TEXT_LEN];
    int total_chars;
    int total_words;
};

void init_text_processor(struct TextProcessor *tp) {
    tp->word_count = 0;
    tp->processed_text[0] = '\0';
    tp->total_chars = 0;
    tp->total_words = 0;
    for (int i = 0; i < MAX_WORDS; i++) {
        tp->words[i].word[0] = '\0';
        tp->words[i].count = 0;
        tp->words[i].length = 0;
        tp->words[i].is_capitalized = 0;
    }
}

int is_word_char(char c) {
    return isalnum((unsigned char)c) || c == '\'' || c == '-';
}

int add_word(struct TextProcessor *tp, const char *word, int len) {
    if (len <= 0 || len >= MAX_WORD_LEN || tp->word_count >= MAX_WORDS) {
        return 0;
    }

    for (int i = 0; i < tp->word_count; i++) {
        if (strncmp(tp->words[i].word, word, len) == 0 && tp->words[i].word[len] == '\0') {
            tp->words[i].count++;
            return 1;
        }
    }

    if (tp->word_count < MAX_WORDS) {
        strncpy(tp->words[tp->word_count].word, word, len);
        tp->words[tp->word_count].word[len] = '\0';
        tp->words[tp->word_count].count = 1;
        tp->words[tp->word_count].length = len;
        tp->words[tp->word_count].is_capitalized = isupper((unsigned char)word[0]) ? 1 : 0;
        tp->word_count++;
        return 1;
    }
    return 0;
}

int process_text(struct TextProcessor *tp, const char *text) {
    if (text == NULL) {
        return 0;
    }

    size_t text_len = strlen(text);
    if (text_len == 0 || text_len >= MAX_TEXT_LEN) {
        return 0;
    }

    init_text_processor(tp);
    strncpy(tp->processed_text, text, MAX_TEXT_LEN - 1);
    tp->processed_text[MAX_TEXT_LEN - 1] = '\0';
    tp->total_chars = (int)text_len;

    const char *ptr = text;
    while (*ptr) {
        while (*ptr && !is_word_char((unsigned char)*ptr)) {
            ptr++;
        }
        if (!*ptr) break;

        const char *start = ptr;
        while (*ptr && is_word_char((unsigned char)*ptr)) {
            ptr++;
        }
        int word_len = (int)(ptr - start);
        if (word_len > 0 && word_len < MAX_WORD_LEN) {
            add_word(tp, start, word_len);
            tp->total_words++;
        }
    }
    return 1;
}

void print_statistics(const struct TextProcessor *tp) {
    printf("Text Statistics:\n");
    printf("Total characters: %d\n", tp->total_chars);
    printf("Total words: %d\n", tp->total_words);
    printf("Unique words: %d\n", tp->word_count);
    printf("\nWord frequency:\n");

    for (int i = 0; i < tp->word_count; i++) {
        printf("%-20s: %d occurrences\n", tp->words[i].word, tp->words[i].count);
    }

    int capitalized_count = 0;
    for (int i = 0; i < tp->word_count; i++) {
        if (tp->words[i].is_capitalized) {
            capitalized_count += tp->words[i].count;
        }
    }
    printf("\nCapitalized words: %d\n", capitalized_count);
}

int main(void) {
    struct TextProcessor processor;
    char input_text[MAX_TEXT_LEN];

    printf("Enter text to analyze (max %d characters):\n", MAX_TEXT_LEN - 1);
    
    if (fgets(input_text, sizeof(input_text), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }

    size_t len = strlen(input_text);
    if (len > 0 && input_text[len - 1] == '\n') {
        input_text[len - 1] = '\0';
    }

    if (strlen(input_text) == 0) {
        fprintf(stderr, "Empty input text\n");
        return 1;
    }

    if (!process_text(&processor, input_text)) {
        fprintf(stderr, "Error processing text\n");
        return 1;
    }

    print_statistics(&processor);
    return 0;
}