//DeepSeek-V3 V2.5 Category: Safe ; Style: macros ; Variation: log_analyzer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>

#define MAX_LINE_LEN 1024
#define MAX_ENTRIES 1000
#define TIMESTAMP_LEN 20
#define LOG_LEVELS 4

#define LOG_LEVEL_ERROR 0
#define LOG_LEVEL_WARN 1
#define LOG_LEVEL_INFO 2
#define LOG_LEVEL_DEBUG 3

#define IS_VALID_LEVEL(l) ((l) >= LOG_LEVEL_ERROR && (l) <= LOG_LEVEL_DEBUG)

struct log_entry {
    char timestamp[TIMESTAMP_LEN + 1];
    int level;
    char message[MAX_LINE_LEN - TIMESTAMP_LEN - 10];
};

static const char *level_names[] = {"ERROR", "WARN", "INFO", "DEBUG"};
static struct log_entry entries[MAX_ENTRIES];
static int entry_count = 0;

static int parse_timestamp(const char *str) {
    struct tm tm = {0};
    return strptime(str, "%Y-%m-%d %H:%M:%S", &tm) != NULL;
}

static int parse_level(const char *str) {
    for (int i = 0; i < LOG_LEVELS; i++) {
        if (strcmp(str, level_names[i]) == 0) {
            return i;
        }
    }
    return -1;
}

static int read_log_file(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        return -1;
    }

    char line[MAX_LINE_LEN];
    entry_count = 0;

    while (fgets(line, sizeof(line), file) && entry_count < MAX_ENTRIES) {
        if (strlen(line) >= MAX_LINE_LEN - 1) {
            continue;
        }

        char timestamp[TIMESTAMP_LEN + 1] = {0};
        char level_str[10] = {0};
        char message[MAX_LINE_LEN - TIMESTAMP_LEN - 10] = {0};

        if (sscanf(line, "%20s %9s %[^\n]", timestamp, level_str, message) == 3) {
            if (!parse_timestamp(timestamp)) {
                continue;
            }

            int level = parse_level(level_str);
            if (!IS_VALID_LEVEL(level)) {
                continue;
            }

            struct log_entry *entry = &entries[entry_count];
            strncpy(entry->timestamp, timestamp, TIMESTAMP_LEN);
            entry->timestamp[TIMESTAMP_LEN] = '\0';
            entry->level = level;
            strncpy(entry->message, message, sizeof(entry->message) - 1);
            entry->message[sizeof(entry->message) - 1] = '\0';
            entry_count++;
        }
    }

    fclose(file);
    return entry_count;
}

static void print_summary(void) {
    int level_counts[LOG_LEVELS] = {0};

    for (int i = 0; i < entry_count; i++) {
        if (IS_VALID_LEVEL(entries[i].level)) {
            level_counts[entries[i].level]++;
        }
    }

    printf("Log Summary:\n");
    for (int i = 0; i < LOG_LEVELS; i++) {
        printf("%s: %d\n", level_names[i], level_counts[i]);
    }
    printf("Total entries: %d\n", entry_count);
}

static void filter_by_level(int level) {
    if (!IS_VALID_LEVEL(level)) {
        return;
    }

    printf("Entries with level %s:\n", level_names[level]);
    for (int i = 0; i < entry_count; i++) {
        if (entries[i].level == level) {
            printf("%s %s %s\n", entries[i].timestamp, level_names[entries[i].level], entries[i].message);
        }
    }
}

int main(void) {
    char filename[256];
    printf("Enter log file name: ");
    if (fgets(filename, sizeof(filename), stdin) == NULL) {
        return 1;
    }

    size_t len = strlen(filename);
    if (len > 0 && filename[len - 1] == '\n') {
        filename[len - 1] = '\0';
    }

    if (strlen(filename) == 0) {
        printf("Invalid filename\n");
        return 1;
    }

    int count = read_log_file(filename);
    if (count <= 0) {
        printf("Failed to read log file or no valid entries found\n");
        return 1;
    }

    printf("Successfully read %d log entries\n\n", count);

    print_summary();
    printf("\n");

    char input[10];
    while (1) {
        printf("Enter log level to filter (ERROR/WARN/INFO/DEBUG) or 'quit': ");
        if (fgets(input, sizeof(input), stdin) == NULL) {
            break;
        }

        size_t input_len = strlen(input);
        if (input_len > 0 && input[input_len - 1] == '\n') {
            input[input_len - 1] = '\0';
        }

        if (strcmp(input, "quit") == 0) {
            break;
        }

        int level = parse_level(input);
        if (IS_VALID_LEVEL(level)) {
            filter_by_level(level);
        } else {
            printf("Invalid log level\n