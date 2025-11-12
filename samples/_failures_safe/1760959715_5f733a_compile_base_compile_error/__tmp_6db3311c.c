//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: struct_heavy ; Variation: log_analyzer
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
    int debug_count;
};

void init_log_analyzer(struct LogAnalyzer *analyzer) {
    if (analyzer == NULL) return;
    analyzer->count = 0;
    analyzer->error_count = 0;
    analyzer->warning_count = 0;
    analyzer->info_count = 0;
    analyzer->debug_count = 0;
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
        timestamp[i] = str[i];
    }
    timestamp[19] = '\0';
    return 1;
}

int parse_level(const char *str, char *level, int *severity) {
    if (str == NULL || level == NULL || severity == NULL) return 0;
    
    const char *levels[] = {"DEBUG", "INFO", "WARNING", "ERROR"};
    int severities[] = {1, 2, 3, 4};
    
    for (int i = 0; i < 4; i++) {
        if (strncmp(str, levels[i], strlen(levels[i])) == 0) {
            size_t len = strlen(levels[i]);
            if (len >= MAX_LEVEL_LENGTH) return 0;
            strncpy(level, levels[i], MAX_LEVEL_LENGTH - 1);
            level[MAX_LEVEL_LENGTH - 1] = '\0';
            *severity = severities[i];
            return 1;
        }
    }
    return 0;
}

int add_log_entry(struct LogAnalyzer *analyzer, const char *line) {
    if (analyzer == NULL || line == NULL) return 0;
    if (analyzer->count >= MAX_ENTRIES) return 0;
    
    struct LogEntry *entry = &analyzer->entries[analyzer->count];
    
    if (!parse_timestamp(line, entry->timestamp)) return 0;
    
    const char *level_start = line + 20;
    if (*level_start != '[') return 0;
    
    const char *level_end = strchr(level_start + 1, ']');
    if (level_end == NULL) return 0;
    
    size_t level_len = level_end - level_start - 1;
    if (level_len >= MAX_LEVEL_LENGTH) return 0;
    
    char level_buf[MAX_LEVEL_LENGTH];
    strncpy(level_buf, level_start + 1, level_len);
    level_buf[level_len] = '\0';
    
    if (!parse_level(level_buf, entry->level, &entry->severity)) return 0;
    
    const char *message_start = level_end + 2;
    if (*message_start != '-') return 0;
    message_start += 2;
    
    size_t message_len = strlen(message_start);
    if (message_len >= sizeof(entry->message)) return 0;
    
    strncpy(entry->message, message_start, sizeof(entry->message) - 1);
    entry->message[sizeof(entry->message) - 1] = '\0';
    
    switch (entry->severity) {
        case 1: analyzer->debug_count++; break;
        case 2: analyzer->info_count++; break;
        case 3: analyzer->warning_count++; break;
        case 4: analyzer->error_count++; break;
    }
    
    analyzer->count++;
    return 1;
}

void generate_report(const struct LogAnalyzer *analyzer) {
    if (analyzer == NULL) return;
    
    printf("Log Analysis Report\n");
    printf("==================\n");
    printf("Total entries: %d\n", analyzer->count);
    printf("DEBUG: %d\n", analyzer->debug_count);
    printf("INFO: %d\n", analyzer->info_count);
    printf("WARNING: %d\n", analyzer->warning_count);
    printf("ERROR: %d\n", analyzer->error_count);
    printf("\n");
    
    if (analyzer->count > 0) {
        printf("Recent entries:\n");
        int start = analyzer->count > 5 ? analyzer->count - 5 : 0;
        for (int i = start; i < analyzer->count; i++) {
            const struct LogEntry *entry = &analyzer->entries[i];
            printf("%s [%s] - %s\n",