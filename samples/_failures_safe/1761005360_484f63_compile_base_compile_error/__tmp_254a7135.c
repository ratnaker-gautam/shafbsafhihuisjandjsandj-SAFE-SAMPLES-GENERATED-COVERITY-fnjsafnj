//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: struct_heavy ; Variation: log_analyzer
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

void init_analyzer(struct LogAnalyzer *analyzer) {
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

int parse_log_line(const char *line, struct LogEntry *entry) {
    char timestamp[32] = {0};
    char level[MAX_LEVEL_LENGTH] = {0};
    char message[256] = {0};
    
    if (sscanf(line, "%31s %15s %255[^\n]", timestamp, level, message) != 3) {
        return 0;
    }
    
    if (!validate_level(level)) {
        return 0;
    }
    
    time_t ts;
    if (!parse_timestamp(timestamp, &ts)) {
        return 0;
    }
    
    strncpy(entry->timestamp, timestamp, sizeof(entry->timestamp) - 1);
    entry->timestamp[sizeof(entry->timestamp) - 1] = '\0';
    strncpy(entry->level, level, sizeof(entry->level) - 1);
    entry->level[sizeof(entry->level) - 1] = '\0';
    strncpy(entry->message, message, sizeof(entry->message) - 1);
    entry->message[sizeof(entry->message) - 1] = '\0';
    
    if (strcmp(level, "ERROR") == 0) {
        entry->severity = 3;
    } else if (strcmp(level, "WARNING") == 0) {
        entry->severity = 2;
    } else {
        entry->severity = 1;
    }
    
    return 1;
}

int add_entry(struct LogAnalyzer *analyzer, const struct LogEntry *entry) {
    if (analyzer->count >= MAX_ENTRIES) {
        return 0;
    }
    
    analyzer->entries[analyzer->count] = *entry;
    
    if (strcmp(entry->level, "ERROR") == 0) {
        analyzer->error_count++;
    } else if (strcmp(entry->level, "WARNING") == 0) {
        analyzer->warning_count++;
    } else {
        analyzer->info_count++;
    }
    
    time_t ts;
    if (parse_timestamp(entry->timestamp, &ts)) {
        if (analyzer->start_time == 0 || ts < analyzer->start_time) {
            analyzer->start_time = ts;
        }
        if (ts > analyzer->end_time) {
            analyzer->end_time = ts;
        }
    }
    
    analyzer->count++;
    return 1;
}

void print_statistics(const struct LogAnalyzer *analyzer) {
    printf("Log Analysis Results:\n");
    printf("Total entries: %d\n", analyzer->count);
    printf("Errors: %d\n", analyzer->error_count);
    printf("Warnings: %d\n", analyzer->warning_count);
    printf("Info: %d\n", analyzer->info_count);
    
    if (analyzer->start_time != 0 && analyzer->end_time != 0) {
        double duration = difftime(analyzer->end_time, analyzer->start_time);
        printf("Time span: %.0f seconds\n", duration);
    }
    
    printf("\n");
}

void print_entries_by_level(const struct LogAnalyzer *analyzer, const char *level) {
    printf("%s entries:\n", level);
    for (int i = 0; i < analyzer->count; i++) {
        if (strcmp(analyzer->entries[i].level, level) == 0) {
            printf("%s %s %s\n", analyzer->entries[i].timestamp, 
                   analyzer->entries[i].level, analyzer->entries[i].message);
        }
    }
    printf("\n");
}

int main(void) {
    struct LogAnalyzer analyzer;
    init_analyzer(&analyzer);
    
    char line[MAX_LINE_LENGTH];
    int line_count = 0;
    
    printf("Enter log entries (format: YYYY-MM-DD HH:MM:SS LEVEL message):