//DeepSeek-V3 V2.5 Category: Safe ; Style: struct_heavy ; Variation: string_processing
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_WORD_LEN 64
#define MAX_WORDS 1000

struct WordInfo {
    char word[MAX_WORD_LEN];
    int count;
    int length;
    int is_capitalized;
};

struct TextStats {
    struct WordInfo words[MAX_WORDS];
    int word_count;
    int total_chars;
    int unique_words;
    int longest_word_length;
};

struct TextProcessor {
    struct TextStats stats;
    char buffer[8192];
};

void init_text_processor(struct TextProcessor *tp) {
    tp->stats.word_count = 0;
    tp->stats.total_chars = 0;
    tp->stats.unique_words = 0;
    tp->stats.longest_word_length = 0;
    memset(tp->buffer, 0, sizeof(tp->buffer));
}

int is_word_char(char c) {
    return isalnum((unsigned char)c) || c == '\'' || c == '-';
}

int find_word_index(struct TextStats *stats, const char *word) {
    for (int i = 0; i < stats->word_count; i++) {
        if (strcmp(stats->words[i].word, word) == 0) {
            return i;
        }
    }
    return -1;
}

void add_word(struct TextStats *stats, const char *word, int length) {
    if (length <= 0 || length >= MAX_WORD_LEN || stats->word_count >= MAX_WORDS) {
        return;
    }

    int index = find_word_index(stats, word);
    if (index >= 0) {
        stats->words[index].count++;
        return;
    }

    if (stats->word_count < MAX_WORDS) {
        struct WordInfo *wi = &stats->words[stats->word_count];
        strncpy(wi->word, word, MAX_WORD_LEN - 1);
        wi->word[MAX_WORD_LEN - 1] = '\0';
        wi->count = 1;
        wi->length = length;
        wi->is_capitalized = isupper((unsigned char)word[0]) ? 1 : 0;
        stats->word_count++;
        stats->unique_words++;
        
        if (length > stats->longest_word_length) {
            stats->longest_word_length = length;
        }
    }
}

void process_text(struct TextProcessor *tp, const char *text) {
    if (!text || strlen(text) >= sizeof(tp->buffer)) {
        return;
    }

    strncpy(tp->buffer, text, sizeof(tp->buffer) - 1);
    tp->buffer[sizeof(tp->buffer) - 1] = '\0';

    char *ptr = tp->buffer;
    char word[MAX_WORD_LEN];
    int word_len = 0;

    while (*ptr) {
        if (is_word_char(*ptr)) {
            if (word_len < MAX_WORD_LEN - 1) {
                word[word_len++] = *ptr;
            }
        } else {
            if (word_len > 0) {
                word[word_len] = '\0';
                add_word(&tp->stats, word, word_len);
                tp->stats.total_chars += word_len;
                word_len = 0;
            }
        }
        ptr++;
    }

    if (word_len > 0) {
        word[word_len] = '\0';
        add_word(&tp->stats, word, word_len);
        tp->stats.total_chars += word_len;
    }
}

void print_stats(const struct TextProcessor *tp) {
    printf("Text Statistics:\n");
    printf("Total words: %d\n", tp->stats.word_count);
    printf("Unique words: %d\n", tp->stats.unique_words);
    printf("Total characters in words: %d\n", tp->stats.total_chars);
    printf("Longest word length: %d\n\n", tp->stats.longest_word_length);

    printf("Word frequency (top 10):\n");
    for (int i = 0; i < (tp->stats.word_count < 10 ? tp->stats.word_count : 10); i++) {
        printf("%s: %d occurrences\n", tp->stats.words[i].word, tp->stats.words[i].count);
    }
}

int main() {
    struct TextProcessor processor;
    init_text_processor(&processor);

    char input[4096];
    printf("Enter text to analyze (max 4095 characters):\n");
    
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

    process_text(&processor, input);
    print_stats(&processor);

    return 0;
}