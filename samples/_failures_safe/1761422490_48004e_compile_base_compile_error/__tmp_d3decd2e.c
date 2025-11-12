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

int parse_timestamp(const char* str) {
    size_t len = strlen(str);
    if (len != TIMESTAMP_LEN - 1) return 0;
    for (int i = 0; i < TIMESTAMP_LEN - 1; i++) {
        if (i == 4 || i == 7) {
            if (str[i] != '-') return 0;
        } else if (i == 10) {
            if (str[i] != ' ') return 0;
        } else if (i == 13 || i == 16) {
            if (str[i] != ':') return 0;
        } else {
            if (!isdigit((unsigned char)str[i])) return 0;
        }
    }
    return 1;
}

int parse_log_level(const char* str) {
    for (int i = 0; i < LOG_LEVELS; i++) {
        if (strcmp(str, level_names[i]) == 0) {
            return i;
        }
    }
    return -1;
}

int parse_log_line(const char* line, struct log_entry* entry) {
    if (line == NULL || entry == NULL) return 0;
    size_t len = strlen(line);
    if (len >= MAX_LINE_LEN) return 0;
    
    char timestamp[TIMESTAMP_LEN];
    char level_str[10];
    char message[MAX_LINE_LEN];
    
    if (sscanf(line, "%19s %9s %[^\n]", timestamp, level_str, message) != 3) {
        return 0;
    }
    
    if (!parse_timestamp(timestamp)) return 0;
    
    int level = parse_log_level(level_str);
    if (level == -1) return 0;
    
    strncpy(entry->timestamp, timestamp, TIMESTAMP_LEN - 1);
    entry->timestamp[TIMESTAMP_LEN - 1] = '\0';
    entry->level = level;
    strncpy(entry->message, message, sizeof(entry->message) - 1);
    entry->message[sizeof(entry->message) - 1] = '\0';
    
    return 1;
}

void generate_sample_logs(void) {
    const char* sample_logs[] = {
        "2024-01-15 10:30:25 INFO System started successfully",
        "2024-01-15 10:31:10 DEBUG Loading configuration file",
        "2024-01-15 10:32:45 WARN Low disk space on /var",
        "2024-01-15 10:33:20 ERROR Database connection failed",
        "2024-01-15 10:34:05 INFO User login: john_doe",
        "2024-01-15 10:35:30 DEBUG Processing request ID 12345",
        "2024-01-15 10:36:15 WARN High memory usage detected",
        "2024-01-15 10:37:00 INFO Backup completed",
        "2024-01-15 10:38:22 ERROR File not found: config.xml",
        "2024-01-15 10:39:10 DEBUG Cache cleared"
    };
    
    int sample_count = sizeof(sample_logs) / sizeof(sample_logs[0]);
    
    for (int i = 0; i < sample_count && entry_count < MAX_ENTRIES; i++) {
        if (parse_log_line(sample_logs[i], &entries[entry_count])) {
            level_counts[entries[entry_count].level]++;
            entry_count++;
        }
    }
}

void display_statistics(void) {
    printf("\nLog Analysis Summary:\n");
    printf("====================\n");
    printf("Total entries: %d\n", entry_count);
    
    for (int i = 0; i < LOG_LEVELS; i++) {
        printf("%s: %d (%.1f%%)\n", level_names[i], level_counts[i],
               entry_count > 0 ? (level_counts[i] * 100.0) / entry_count : 0.0);
    }
}

void display_entries_by_level(int level) {
    if (!IS_VALID_LEVEL(level)) return;
    
    printf("\n%s entries:\n", level_names[level]);
    printf("============\n");
    
    int count = 0;
    for (int i = 0; i < entry_count; i++) {
        if (entries[i].level == level) {
            printf("%s %s %s\n", entries[i