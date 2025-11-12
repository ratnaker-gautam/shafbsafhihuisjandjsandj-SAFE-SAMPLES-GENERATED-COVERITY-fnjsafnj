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
    return isalpha((unsigned char)c) || c == '\'' || c == '-';
}

int add_word(struct TextProcessor *tp, const char *word, int len, int capitalized) {
    if (len <= 0 || len >= MAX_WORD_LEN || tp->word_count >= MAX_WORDS) {
        return 0;
    }

    char temp[MAX_WORD_LEN];
    if (len >= MAX_WORD_LEN) {
        return 0;
    }
    strncpy(temp, word, len);
    temp[len] = '\0';

    for (int i = 0; i < tp->word_count; i++) {
        if (strcmp(tp->words[i].word, temp) == 0) {
            tp->words[i].count++;
            return 1;
        }
    }

    if (tp->word_count < MAX_WORDS) {
        strcpy(tp->words[tp->word_count].word, temp);
        tp->words[tp->word_count].count = 1;
        tp->words[tp->word_count].length = len;
        tp->words[tp->word_count].is_capitalized = capitalized;
        tp->word_count++;
        return 1;
    }
    return 0;
}

void process_text(struct TextProcessor *tp, const char *text) {
    if (text == NULL) {
        return;
    }

    int len = strlen(text);
    if (len <= 0 || len >= MAX_TEXT_LEN) {
        return;
    }

    strcpy(tp->processed_text, text);
    tp->total_chars = len;

    int i = 0;
    while (i < len && tp->total_words < MAX_WORDS) {
        while (i < len && !is_word_char(text[i])) {
            i++;
        }

        if (i >= len) {
            break;
        }

        int start = i;
        int capitalized = isupper((unsigned char)text[i]) ? 1 : 0;

        while (i < len && is_word_char(text[i])) {
            i++;
        }

        int word_len = i - start;
        if (word_len > 0 && word_len < MAX_WORD_LEN) {
            if (add_word(tp, text + start, word_len, capitalized)) {
                tp->total_words++;
            }
        }
    }
}

void print_word_stats(const struct TextProcessor *tp) {
    printf("Text Analysis Results:\n");
    printf("Total characters: %d\n", tp->total_chars);
    printf("Total unique words: %d\n", tp->word_count);
    printf("Total word occurrences: %d\n", tp->total_words);
    printf("\nWord Statistics:\n");

    for (int i = 0; i < tp->word_count; i++) {
        printf("Word: %-20s Count: %3d Length: %2d %s\n",
               tp->words[i].word,
               tp->words[i].count,
               tp->words[i].length,
               tp->words[i].is_capitalized ? "(Capitalized)" : "");
    }
}

int main(void) {
    struct TextProcessor processor;
    init_text_processor(&processor);

    char input_text[MAX_TEXT_LEN];
    printf("Enter text to analyze (max %d characters): ", MAX_TEXT_LEN - 1);
    
    if (fgets(input_text, sizeof(input_text), stdin) == NULL) {
        printf("Error reading input.\n");
        return 1;
    }

    size_t input_len = strlen(input_text);
    if (input_len > 0 && input_text[input_len - 1] == '\n') {
        input_text[input_len - 1] = '\0';
    }

    if (strlen(input_text) == 0) {
        printf("No input provided.\n");
        return 1;
    }

    process_text(&processor, input_text);
    print_word_stats(&processor);

    return 0;
}