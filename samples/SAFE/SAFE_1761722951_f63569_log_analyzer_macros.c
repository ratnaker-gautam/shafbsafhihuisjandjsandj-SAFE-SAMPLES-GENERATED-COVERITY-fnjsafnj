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

static const char* level_names[] = {"ERROR", "WARN", "INFO", "DEBUG"};
static int level_counts[LOG_LEVELS] = {0};
static struct log_entry entries[MAX_ENTRIES];
static int entry_count = 0;

static int parse_timestamp(const char* str) {
    struct tm tm = {0};
    return strptime(str, "%Y-%m-%d %H:%M:%S", &tm) != NULL;
}

static int parse_level(const char* str) {
    for (int i = 0; i < LOG_LEVELS; i++) {
        if (strcmp(str, level_names[i]) == 0) {
            return i;
        }
    }
    return -1;
}

static int read_log_file(const char* filename) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        fprintf(stderr, "Error: Cannot open file %s\n", filename);
        return 0;
    }

    char line[MAX_LINE_LEN];
    while (fgets(line, sizeof(line), file) && entry_count < MAX_ENTRIES) {
        size_t len = strlen(line);
        if (len > 0 && line[len-1] == '\n') {
            line[len-1] = '\0';
        }

        char timestamp[TIMESTAMP_LEN];
        char level_str[10];
        char message[MAX_LINE_LEN - TIMESTAMP_LEN - 10];

        if (sscanf(line, "%19s %9s %[^\n]", timestamp, level_str, message) == 3) {
            if (!parse_timestamp(timestamp)) {
                continue;
            }

            int level = parse_level(level_str);
            if (!IS_VALID_LEVEL(level)) {
                continue;
            }

            struct log_entry* entry = &entries[entry_count];
            strncpy(entry->timestamp, timestamp, TIMESTAMP_LEN - 1);
            entry->timestamp[TIMESTAMP_LEN - 1] = '\0';
            entry->level = level;
            strncpy(entry->message, message, sizeof(entry->message) - 1);
            entry->message[sizeof(entry->message) - 1] = '\0';

            level_counts[level]++;
            entry_count++;
        }
    }

    if (ferror(file)) {
        fprintf(stderr, "Error reading file\n");
        fclose(file);
        return 0;
    }

    fclose(file);
    return 1;
}

static void print_summary(void) {
    printf("Log Analysis Summary:\n");
    printf("Total entries: %d\n", entry_count);
    for (int i = 0; i < LOG_LEVELS; i++) {
        printf("%s: %d\n", level_names[i], level_counts[i]);
    }
    printf("\n");
}

static void print_entries_by_level(int level) {
    if (!IS_VALID_LEVEL(level)) {
        printf("Invalid log level\n");
        return;
    }

    printf("Entries with level %s:\n", level_names[level]);
    for (int i = 0; i < entry_count; i++) {
        if (entries[i].level == level) {
            printf("%s %s %s\n", entries[i].timestamp, level_names[entries[i].level], entries[i].message);
        }
    }
    printf("\n");
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <logfile>\n", argv[0]);
        return 1;
    }

    if (!read_log_file(argv[1])) {
        return 1;
    }

    if (entry_count == 0) {
        printf("No valid log entries found\n");
        return 0;
    }

    print_summary();

    for (int i = 0; i < LOG_LEVELS; i++) {
        if (level_counts[i] > 0) {
            print_entries_by_level(i);
        }
    }

    return 0;
}