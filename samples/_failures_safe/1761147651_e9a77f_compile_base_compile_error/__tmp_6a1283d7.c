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
    char start_time[20];
    char end_time[20];
};

void init_log_analyzer(struct LogAnalyzer *analyzer) {
    analyzer->count = 0;
    analyzer->error_count = 0;
    analyzer->warning_count = 0;
    analyzer->info_count = 0;
    memset(analyzer->start_time, 0, sizeof(analyzer->start_time));
    memset(analyzer->end_time, 0, sizeof(analyzer->end_time));
}

int validate_timestamp(const char *timestamp) {
    if (strlen(timestamp) != 19) return 0;
    for (int i = 0; i < 19; i++) {
        if (i == 4 || i == 7) {
            if (timestamp[i] != '-') return 0;
        } else if (i == 10) {
            if (timestamp[i] != ' ') return 0;
        } else if (i == 13 || i == 16) {
            if (timestamp[i] != ':') return 0;
        } else {
            if (!isdigit(timestamp[i])) return 0;
        }
    }
    return 1;
}

int parse_severity(const char *level) {
    if (strcmp(level, "ERROR") == 0) return 3;
    if (strcmp(level, "WARNING") == 0) return 2;
    if (strcmp(level, "INFO") == 0) return 1;
    return 0;
}

int add_log_entry(struct LogAnalyzer *analyzer, const char *timestamp, const char *level, const char *message) {
    if (analyzer->count >= MAX_ENTRIES) return 0;
    if (!validate_timestamp(timestamp)) return 0;
    if (strlen(level) >= MAX_LEVEL_LENGTH) return 0;
    if (strlen(message) >= 256) return 0;
    
    struct LogEntry *entry = &analyzer->entries[analyzer->count];
    strncpy(entry->timestamp, timestamp, sizeof(entry->timestamp) - 1);
    entry->timestamp[sizeof(entry->timestamp) - 1] = '\0';
    strncpy(entry->level, level, sizeof(entry->level) - 1);
    entry->level[sizeof(entry->level) - 1] = '\0';
    strncpy(entry->message, message, sizeof(entry->message) - 1);
    entry->message[sizeof(entry->message) - 1] = '\0';
    entry->severity = parse_severity(level);
    
    if (entry->severity == 3) analyzer->error_count++;
    else if (entry->severity == 2) analyzer->warning_count++;
    else if (entry->severity == 1) analyzer->info_count++;
    
    if (analyzer->count == 0) {
        strncpy(analyzer->start_time, timestamp, sizeof(analyzer->start_time) - 1);
        analyzer->start_time[sizeof(analyzer->start_time) - 1] = '\0';
    }
    strncpy(analyzer->end_time, timestamp, sizeof(analyzer->end_time) - 1);
    analyzer->end_time[sizeof(analyzer->end_time) - 1] = '\0';
    
    analyzer->count++;
    return 1;
}

void generate_report(const struct LogAnalyzer *analyzer) {
    printf("Log Analysis Report\n");
    printf("==================\n");
    printf("Time range: %s to %s\n", analyzer->start_time, analyzer->end_time);
    printf("Total entries: %d\n", analyzer->count);
    printf("ERROR count: %d\n", analyzer->error_count);
    printf("WARNING count: %d\n", analyzer->warning_count);
    printf("INFO count: %d\n", analyzer->info_count);
    printf("\nRecent entries:\n");
    
    int start_idx = analyzer->count - 5;
    if (start_idx < 0) start_idx = 0;
    
    for (int i = start_idx; i < analyzer->count; i++) {
        const struct LogEntry *entry = &analyzer->entries[i];
        printf("[%s] %s: %s\n", entry->timestamp, entry->level, entry->message);
    }
}

int main(void) {
    struct LogAnalyzer analyzer;
    init_log_analyzer(&analyzer);
    
    printf("Enter log entries (format: YYYY-MM-DD HH:MM:SS LEVEL message)\n");
    printf("Enter 'quit' to finish input\n");
    
    char line[MAX_LINE_LENGTH];
    while (fgets(line, sizeof(line), stdin) != NULL) {
        if (strncmp(line, "quit", 4) == 0) break;
        
        char timestamp[20], level[MAX_LEVEL_LENGTH], message[