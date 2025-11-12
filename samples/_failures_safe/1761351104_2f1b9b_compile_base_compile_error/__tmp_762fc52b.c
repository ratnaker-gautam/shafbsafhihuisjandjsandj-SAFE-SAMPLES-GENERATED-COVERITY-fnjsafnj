//DeepSeek-V3 V2.5 Category: Safe ; Style: struct_heavy ; Variation: log_analyzer
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

struct LogStats {
    int total_entries;
    int error_count;
    int warning_count;
    int info_count;
    int debug_count;
    char earliest[32];
    char latest[32];
};

struct LogAnalyzer {
    struct LogEntry entries[MAX_ENTRIES];
    struct LogStats stats;
    int entry_count;
};

void init_log_analyzer(struct LogAnalyzer *analyzer) {
    memset(analyzer, 0, sizeof(*analyzer));
    analyzer->stats.earliest[0] = '\0';
    analyzer->stats.latest[0] = '\0';
}

int parse_severity(const char *level) {
    if (strcmp(level, "ERROR") == 0) return 4;
    if (strcmp(level, "WARN") == 0) return 3;
    if (strcmp(level, "INFO") == 0) return 2;
    if (strcmp(level, "DEBUG") == 0) return 1;
    return 0;
}

int parse_log_line(const char *line, struct LogEntry *entry) {
    if (!line || !entry) return 0;
    
    char timestamp[32] = {0};
    char level[16] = {0};
    char message[256] = {0};
    
    int parsed = sscanf(line, "%31s %15s %255[^\n]", timestamp, level, message);
    if (parsed != 3) return 0;
    
    if (strlen(timestamp) > 31 || strlen(level) > 15 || strlen(message) > 255) return 0;
    
    strcpy(entry->timestamp, timestamp);
    strcpy(entry->level, level);
    strcpy(entry->message, message);
    entry->severity = parse_severity(level);
    
    return entry->severity > 0;
}

int compare_timestamps(const char *ts1, const char *ts2) {
    return strcmp(ts1, ts2);
}

void update_stats(struct LogAnalyzer *analyzer, const struct LogEntry *entry) {
    if (!analyzer || !entry) return;
    
    analyzer->stats.total_entries++;
    
    if (entry->severity == 4) analyzer->stats.error_count++;
    else if (entry->severity == 3) analyzer->stats.warning_count++;
    else if (entry->severity == 2) analyzer->stats.info_count++;
    else if (entry->severity == 1) analyzer->stats.debug_count++;
    
    if (analyzer->stats.earliest[0] == '\0' || 
        compare_timestamps(entry->timestamp, analyzer->stats.earliest) < 0) {
        strcpy(analyzer->stats.earliest, entry->timestamp);
    }
    
    if (analyzer->stats.latest[0] == '\0' || 
        compare_timestamps(entry->timestamp, analyzer->stats.latest) > 0) {
        strcpy(analyzer->stats.latest, entry->timestamp);
    }
}

int load_log_file(struct LogAnalyzer *analyzer) {
    if (!analyzer) return 0;
    
    printf("Enter log data (one entry per line, format: TIMESTAMP LEVEL MESSAGE):\n");
    printf("Example: 2024-01-15T10:30:00 INFO System started\n");
    printf("Enter 'END' on a separate line to finish input.\n");
    
    char line[MAX_LINE_LENGTH];
    int line_count = 0;
    
    while (fgets(line, sizeof(line), stdin)) {
        line_count++;
        if (line_count > MAX_ENTRIES) {
            printf("Warning: Maximum entries reached (%d)\n", MAX_ENTRIES);
            break;
        }
        
        size_t len = strlen(line);
        if (len > 0 && line[len - 1] == '\n') {
            line[len - 1] = '\0';
        }
        
        if (strcmp(line, "END") == 0) break;
        
        if (len == 0) continue;
        
        struct LogEntry entry;
        if (parse_log_line(line, &entry)) {
            if (analyzer->entry_count < MAX_ENTRIES) {
                analyzer->entries[analyzer->entry_count] = entry;
                update_stats(analyzer, &entry);
                analyzer->entry_count++;
            }
        } else {
            printf("Invalid log format on line %d: %s\n", line_count, line);
        }
    }
    
    return analyzer->entry_count > 0;
}

void display_stats(const struct LogAnalyzer *analyzer) {
    if (!analyzer) return;
    
    printf("\n=== Log Analysis Results ===\n");
    printf("Total entries: %d\n", analyzer->stats.total_entries);
    printf("Error count: %d\n", analyzer->stats.error_count);
    printf("Warning count: %d\n", analyzer->stats.warning_count);
    printf("Info count: %d\n", analyzer->stats.info_count);
    printf("Debug count: %d\n", analyzer->stats.debug_count);
    printf("Time range: %