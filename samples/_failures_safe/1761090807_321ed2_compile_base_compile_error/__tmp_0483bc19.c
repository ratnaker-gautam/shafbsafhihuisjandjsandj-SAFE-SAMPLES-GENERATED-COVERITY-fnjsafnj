//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: macros ; Variation: log_analyzer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>

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

int parse_timestamp(const char* str, char* timestamp) {
    if (str == NULL || timestamp == NULL) return 0;
    if (strlen(str) < TIMESTAMP_LEN - 1) return 0;
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
    strncpy(timestamp, str, TIMESTAMP_LEN - 1);
    timestamp[TIMESTAMP_LEN - 1] = '\0';
    return 1;
}

int parse_log_level(const char* str) {
    if (str == NULL) return -1;
    if (strncmp(str, "ERROR", 5) == 0) return LOG_LEVEL_ERROR;
    if (strncmp(str, "WARN", 4) == 0) return LOG_LEVEL_WARN;
    if (strncmp(str, "INFO", 4) == 0) return LOG_LEVEL_INFO;
    if (strncmp(str, "DEBUG", 5) == 0) return LOG_LEVEL_DEBUG;
    return -1;
}

int parse_log_line(const char* line, struct log_entry* entry) {
    if (line == NULL || entry == NULL) return 0;
    if (strlen(line) >= MAX_LINE_LEN) return 0;
    
    char temp[MAX_LINE_LEN];
    strncpy(temp, line, MAX_LINE_LEN - 1);
    temp[MAX_LINE_LEN - 1] = '\0';
    
    if (!parse_timestamp(temp, entry->timestamp)) return 0;
    
    char* level_start = temp + TIMESTAMP_LEN - 1;
    while (*level_start == ' ' && *level_start != '\0') level_start++;
    if (*level_start == '\0') return 0;
    
    int level = parse_log_level(level_start);
    if (!IS_VALID_LEVEL(level)) return 0;
    entry->level = level;
    
    char* message_start = level_start;
    while (*message_start != ' ' && *message_start != '\0') message_start++;
    while (*message_start == ' ' && *message_start != '\0') message_start++;
    if (*message_start == '\0') return 0;
    
    strncpy(entry->message, message_start, sizeof(entry->message) - 1);
    entry->message[sizeof(entry->message) - 1] = '\0';
    
    return 1;
}

void print_summary(const struct log_entry* entries, int count) {
    if (entries == NULL || count <= 0) return;
    
    int level_counts[LOG_LEVELS] = {0};
    
    for (int i = 0; i < count; i++) {
        if (IS_VALID_LEVEL(entries[i].level)) {
            level_counts[entries[i].level]++;
        }
    }
    
    printf("Log Summary:\n");
    printf("ERROR: %d\n", level_counts[LOG_LEVEL_ERROR]);
    printf("WARN:  %d\n", level_counts[LOG_LEVEL_WARN]);
    printf("INFO:  %d\n", level_counts[LOG_LEVEL_INFO]);
    printf("DEBUG: %d\n", level_counts[LOG_LEVEL_DEBUG]);
    printf("Total: %d\n", count);
}

void print_recent_entries(const struct log_entry* entries, int count) {
    if (entries == NULL || count <= 0) return;
    
    int start = (count > 5) ? count - 5 : 0;
    int display_count = count - start;
    
    printf("\nRecent entries:\n");
    for (int i = start; i < count; i++) {
        const char* level_str = "";
        switch (entries[i].level) {
            case LOG_LEVEL_ERROR: level_str = "ERROR"; break;
            case LOG_LEVEL_WARN: level_str = "WARN"; break;
            case LOG_LEVEL_INFO: level_str = "INFO"; break;
            case LOG_LEVEL_DEBUG: level_str = "DEBUG"; break;
        }
        printf("%s %s %s\n", entries[i].timestamp, level_str, entries[i].message);
    }
}

int main(void) {
    struct log_entry entries[MAX_ENTRIES];
    int entry_count = 0;
    char line