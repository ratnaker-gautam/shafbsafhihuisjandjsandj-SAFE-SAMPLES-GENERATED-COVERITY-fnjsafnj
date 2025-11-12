//DeepSeek-V3 V2.5 Category: Safe ; Style: struct_heavy ; Variation: log_analyzer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>

#define MAX_LINE_LENGTH 1024
#define MAX_ENTRIES 1000
#define MAX_LEVEL_LENGTH 10

struct LogEntry {
    char timestamp[20];
    char level[MAX_LEVEL_LENGTH];
    char message[256];
    int severity;
};

struct LogAnalyzer {
    struct LogEntry entries[MAX_ENTRIES];
    int count;
    int error_count;
    int warning_count;
    int info_count;
    time_t start_time;
    time_t end_time;
};

void init_log_analyzer(struct LogAnalyzer *analyzer) {
    if (analyzer == NULL) return;
    analyzer->count = 0;
    analyzer->error_count = 0;
    analyzer->warning_count = 0;
    analyzer->info_count = 0;
    analyzer->start_time = 0;
    analyzer->end_time = 0;
}

int parse_timestamp(const char *str, char *timestamp) {
    if (str == NULL || timestamp == NULL) return 0;
    if (strlen(str) < 19) return 0;
    
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
    
    strncpy(timestamp, str, 19);
    timestamp[19] = '\0';
    return 1;
}

int parse_log_level(const char *str, char *level, int *severity) {
    if (str == NULL || level == NULL || severity == NULL) return 0;
    
    if (strncmp(str, "ERROR", 5) == 0) {
        strncpy(level, "ERROR", MAX_LEVEL_LENGTH - 1);
        level[MAX_LEVEL_LENGTH - 1] = '\0';
        *severity = 3;
        return 1;
    } else if (strncmp(str, "WARNING", 7) == 0) {
        strncpy(level, "WARNING", MAX_LEVEL_LENGTH - 1);
        level[MAX_LEVEL_LENGTH - 1] = '\0';
        *severity = 2;
        return 1;
    } else if (strncmp(str, "INFO", 4) == 0) {
        strncpy(level, "INFO", MAX_LEVEL_LENGTH - 1);
        level[MAX_LEVEL_LENGTH - 1] = '\0';
        *severity = 1;
        return 1;
    }
    
    return 0;
}

int add_log_entry(struct LogAnalyzer *analyzer, const char *line) {
    if (analyzer == NULL || line == NULL) return 0;
    if (analyzer->count >= MAX_ENTRIES) return 0;
    
    char timestamp[20];
    char level[MAX_LEVEL_LENGTH];
    char message[256];
    int severity;
    
    if (!parse_timestamp(line, timestamp)) return 0;
    
    const char *level_start = line + 20;
    if (*level_start != '[') return 0;
    
    level_start++;
    const char *level_end = strchr(level_start, ']');
    if (level_end == NULL) return 0;
    
    size_t level_len = level_end - level_start;
    if (level_len >= MAX_LEVEL_LENGTH) return 0;
    
    char level_str[MAX_LEVEL_LENGTH];
    strncpy(level_str, level_start, level_len);
    level_str[level_len] = '\0';
    
    if (!parse_log_level(level_str, level, &severity)) return 0;
    
    const char *msg_start = level_end + 2;
    if (*msg_start != '-') return 0;
    
    msg_start += 2;
    size_t msg_len = strlen(msg_start);
    if (msg_len >= 256) return 0;
    
    strncpy(message, msg_start, 255);
    message[255] = '\0';
    
    struct LogEntry *entry = &analyzer->entries[analyzer->count];
    strncpy(entry->timestamp, timestamp, 19);
    entry->timestamp[19] = '\0';
    strncpy(entry->level, level, MAX_LEVEL_LENGTH - 1);
    entry->level[MAX_LEVEL_LENGTH - 1] = '\0';
    strncpy(entry->message, message, 255);
    entry->message[255] = '\0';
    entry->severity = severity;
    
    analyzer->count++;
    
    if (severity == 3) analyzer->error_count++;
    else if (severity == 2) analyzer->warning_count++;
    else if (severity == 1) analyzer->info_count++;
    
    return 1;
}

void generate_report(const struct LogAnalyzer *analyzer) {
    if (analyzer == NULL) return;
    
    printf("Log Analysis Report\n");
    printf("==================\n");
    printf("Total entries: %d\n", analyzer->count);
    printf("Errors