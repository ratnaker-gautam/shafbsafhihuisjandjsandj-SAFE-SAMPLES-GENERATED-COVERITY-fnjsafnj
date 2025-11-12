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

#define IS_VALID_LEVEL(l) ((l) >= 0 && (l) < LOG_LEVELS)

struct log_entry {
    char timestamp[TIMESTAMP_LEN];
    int level;
    char message[MAX_LINE_LEN - TIMESTAMP_LEN - 10];
};

static const char *level_names[] = {"ERROR", "WARN", "INFO", "DEBUG"};
static int level_counts[LOG_LEVELS] = {0};
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
    while (fgets(line, sizeof(line), file) && entry_count < MAX_ENTRIES) {
        if (strlen(line) >= sizeof(line) - 1) {
            continue;
        }

        char timestamp[TIMESTAMP_LEN];
        char level_str[10];
        char message[MAX_LINE_LEN - TIMESTAMP_LEN - 10];

        int parsed = sscanf(line, "%19s %9s %[^\n]", timestamp, level_str, message);
        if (parsed != 3) {
            continue;
        }

        if (!parse_timestamp(timestamp)) {
            continue;
        }

        int level = parse_level(level_str);
        if (!IS_VALID_LEVEL(level)) {
            continue;
        }

        struct log_entry *entry = &entries[entry_count];
        strncpy(entry->timestamp, timestamp, sizeof(entry->timestamp) - 1);
        entry->timestamp[sizeof(entry->timestamp) - 1] = '\0';
        entry->level = level;
        strncpy(entry->message, message, sizeof(entry->message) - 1);
        entry->message[sizeof(entry->message) - 1] = '\0';

        level_counts[level]++;
        entry_count++;
    }

    fclose(file);
    return 0;
}

static void print_summary(void) {
    printf("Log Analysis Summary:\n");
    printf("Total entries: %d\n", entry_count);
    for (int i = 0; i < LOG_LEVELS; i++) {
        printf("%s: %d\n", level_names[i], level_counts[i]);
    }
}

static void print_entries_by_level(int level) {
    if (!IS_VALID_LEVEL(level)) {
        return;
    }

    printf("\n%s entries:\n", level_names[level]);
    for (int i = 0; i < entry_count; i++) {
        if (entries[i].level == level) {
            printf("%s - %s\n", entries[i].timestamp, entries[i].message);
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

    if (read_log_file(filename) != 0) {
        printf("Error reading file: %s\n", filename);
        return 1;
    }

    if (entry_count == 0) {
        printf("No valid log entries found\n");
        return 0;
    }

    print_summary();

    char input[10];
    printf("\nEnter level to view (ERROR/WARN/INFO/DEBUG) or 'quit': ");
    while (fgets(input, sizeof(input), stdin)) {
        size_t input_len = strlen(input);
        if (input_len > 0 && input[input_len - 1] == '\n') {
            input[input_len - 1] = '\0';
        }

        if (strcmp(input, "quit") == 0) {
            break;
        }

        int level = parse_level(input);
        if (IS_VALID_LEVEL(level)) {
            print_entries_by_level(level);
        } else {
            printf("Invalid level\n");
        }

        printf("\nEnter level to view (ERROR/WARN/INFO/DEBUG) or 'quit': ");
    }

    return 0;
}