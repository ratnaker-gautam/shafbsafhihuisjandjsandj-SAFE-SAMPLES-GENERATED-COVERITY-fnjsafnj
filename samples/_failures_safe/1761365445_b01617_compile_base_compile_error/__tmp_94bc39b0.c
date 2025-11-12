//DeepSeek-V3 V2.5 Category: Safe ; Style: struct_heavy ; Variation: log_analyzer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>

#define MAX_LINE_LENGTH 1024
#define MAX_ENTRIES 1000
#define MAX_LEVEL_LENGTH 16

struct LogEntry {
    char timestamp[32];
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
    analyzer->count = 0;
    analyzer->error_count = 0;
    analyzer->warning_count = 0;
    analyzer->info_count = 0;
    analyzer->start_time = 0;
    analyzer->end_time = 0;
}

int parse_timestamp(const char *str, time_t *result) {
    struct tm tm = {0};
    if (strptime(str, "%Y-%m-%d %H:%M:%S", &tm) == NULL) {
        return 0;
    }
    *result = mktime(&tm);
    return 1;
}

int validate_level(const char *level) {
    if (strcmp(level, "ERROR") == 0) return 1;
    if (strcmp(level, "WARNING") == 0) return 1;
    if (strcmp(level, "INFO") == 0) return 1;
    return 0;
}

int add_log_entry(struct LogAnalyzer *analyzer, const char *timestamp, 
                  const char *level, const char *message) {
    if (analyzer->count >= MAX_ENTRIES) {
        return 0;
    }
    
    if (strlen(timestamp) >= 32 || strlen(level) >= MAX_LEVEL_LENGTH || 
        strlen(message) >= 256) {
        return 0;
    }
    
    if (!validate_level(level)) {
        return 0;
    }
    
    time_t entry_time;
    if (!parse_timestamp(timestamp, &entry_time)) {
        return 0;
    }
    
    struct LogEntry *entry = &analyzer->entries[analyzer->count];
    strncpy(entry->timestamp, timestamp, 31);
    entry->timestamp[31] = '\0';
    strncpy(entry->level, level, MAX_LEVEL_LENGTH - 1);
    entry->level[MAX_LEVEL_LENGTH - 1] = '\0';
    strncpy(entry->message, message, 255);
    entry->message[255] = '\0';
    
    if (strcmp(level, "ERROR") == 0) {
        entry->severity = 3;
        analyzer->error_count++;
    } else if (strcmp(level, "WARNING") == 0) {
        entry->severity = 2;
        analyzer->warning_count++;
    } else {
        entry->severity = 1;
        analyzer->info_count++;
    }
    
    if (analyzer->count == 0 || entry_time < analyzer->start_time) {
        analyzer->start_time = entry_time;
    }
    if (entry_time > analyzer->end_time) {
        analyzer->end_time = entry_time;
    }
    
    analyzer->count++;
    return 1;
}

void generate_report(const struct LogAnalyzer *analyzer) {
    printf("Log Analysis Report\n");
    printf("==================\n");
    printf("Total entries: %d\n", analyzer->count);
    printf("Time range: ");
    if (analyzer->count > 0) {
        char start_buf[32], end_buf[32];
        struct tm *tm_info = localtime(&analyzer->start_time);
        strftime(start_buf, 32, "%Y-%m-%d %H:%M:%S", tm_info);
        tm_info = localtime(&analyzer->end_time);
        strftime(end_buf, 32, "%Y-%m-%d %H:%M:%S", tm_info);
        printf("%s to %s\n", start_buf, end_buf);
    } else {
        printf("No entries\n");
    }
    printf("Error count: %d\n", analyzer->error_count);
    printf("Warning count: %d\n", analyzer->warning_count);
    printf("Info count: %d\n", analyzer->info_count);
    
    if (analyzer->count > 0) {
        printf("\nRecent entries:\n");
        int start = analyzer->count > 5 ? analyzer->count - 5 : 0;
        for (int i = start; i < analyzer->count; i++) {
            const struct LogEntry *entry = &analyzer->entries[i];
            printf("[%s] %s: %s\n", entry->timestamp, entry->level, entry->message);
        }
    }
}

int main() {
    struct LogAnalyzer analyzer;
    init_log_analyzer(&analyzer);
    
    printf("Log Analyzer - Enter log entries (timestamp level message)\n");
    printf("Format: YYYY-MM-DD HH:MM:SS LEVEL message text\n");
    printf("Supported levels: ERROR, WARNING, INFO\n");
    printf("Enter 'quit' to finish input\n\n");
    
    char line[MAX_LINE_LENGTH];
    while (fgets(line, sizeof(line), stdin) != NULL) {
        if (str