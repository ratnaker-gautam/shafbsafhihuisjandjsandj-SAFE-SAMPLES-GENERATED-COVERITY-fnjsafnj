//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: struct_heavy ; Variation: log_analyzer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>

#define MAX_LINE_LENGTH 1024
#define MAX_ENTRIES 1000

struct LogEntry {
    char timestamp[32];
    char level[16];
    char message[256];
    int severity;
};

struct LogAnalyzer {
    struct LogEntry entries[MAX_ENTRIES];
    int count;
    int error_count;
    int warning_count;
    int info_count;
};

void init_analyzer(struct LogAnalyzer *analyzer) {
    if (analyzer == NULL) return;
    analyzer->count = 0;
    analyzer->error_count = 0;
    analyzer->warning_count = 0;
    analyzer->info_count = 0;
}

int parse_timestamp(const char *str, char *output) {
    if (str == NULL || output == NULL) return 0;
    size_t len = strlen(str);
    if (len < 19 || len >= 32) return 0;
    
    for (int i = 0; i < 19; i++) {
        if (i == 4 || i == 7) {
            if (str[i] != '-') return 0;
        } else if (i == 10) {
            if (str[i] != ' ') return 0;
        } else if (i == 13 || i == 16) {
            if (str[i] != ':') return 0;
        } else {
            if (!isdigit(str[i])) return 0;
        }
    }
    
    strncpy(output, str, 19);
    output[19] = '\0';
    return 1;
}

int parse_level(const char *str, char *output, int *severity) {
    if (str == NULL || output == NULL || severity == NULL) return 0;
    
    if (strncmp(str, "ERROR", 5) == 0) {
        strncpy(output, "ERROR", 16);
        output[15] = '\0';
        *severity = 3;
        return 1;
    } else if (strncmp(str, "WARNING", 7) == 0) {
        strncpy(output, "WARNING", 16);
        output[15] = '\0';
        *severity = 2;
        return 1;
    } else if (strncmp(str, "INFO", 4) == 0) {
        strncpy(output, "INFO", 16);
        output[15] = '\0';
        *severity = 1;
        return 1;
    }
    return 0;
}

int parse_log_line(const char *line, struct LogEntry *entry) {
    if (line == NULL || entry == NULL) return 0;
    
    char temp_line[MAX_LINE_LENGTH];
    if (strlen(line) >= MAX_LINE_LENGTH - 1) return 0;
    strncpy(temp_line, line, MAX_LINE_LENGTH - 1);
    temp_line[MAX_LINE_LENGTH - 1] = '\0';
    
    char *timestamp_start = temp_line;
    char *level_start = strchr(timestamp_start, ']');
    if (level_start == NULL) return 0;
    level_start++;
    while (*level_start == ' ') level_start++;
    
    char *message_start = strchr(level_start, ']');
    if (message_start == NULL) return 0;
    message_start++;
    while (*message_start == ' ') message_start++;
    
    char timestamp_part[32];
    size_t timestamp_len = level_start - timestamp_start - 2;
    if (timestamp_len >= sizeof(timestamp_part)) return 0;
    strncpy(timestamp_part, timestamp_start + 1, timestamp_len);
    timestamp_part[timestamp_len] = '\0';
    
    if (!parse_timestamp(timestamp_part, entry->timestamp)) return 0;
    
    char level_part[16];
    size_t level_len = message_start - level_start - 1;
    if (level_len >= sizeof(level_part)) return 0;
    strncpy(level_part, level_start, level_len);
    level_part[level_len] = '\0';
    
    if (!parse_level(level_part, entry->level, &entry->severity)) return 0;
    
    if (strlen(message_start) >= sizeof(entry->message)) return 0;
    strncpy(entry->message, message_start, sizeof(entry->message) - 1);
    entry->message[sizeof(entry->message) - 1] = '\0';
    
    return 1;
}

void add_entry(struct LogAnalyzer *analyzer, const struct LogEntry *entry) {
    if (analyzer == NULL || entry == NULL) return;
    if (analyzer->count >= MAX_ENTRIES) return;
    
    analyzer->entries[analyzer->count] = *entry;
    
    switch (entry->severity) {
        case 3: analyzer->error_count++; break;
        case 2: analyzer->warning_count++; break;
        case 1: analyzer->info_count++; break;
    }
    
    analyzer->count++;
}

void generate_report(const struct LogAnalyzer *analyzer) {
    if (analyzer == NULL) return;
    
    printf("Log Analysis Report\n");
    printf("==================\n");
    printf("Total entries: %d\n", analyzer->count);
    printf("ERROR entries: %d\n", analyzer->error_count);
    printf