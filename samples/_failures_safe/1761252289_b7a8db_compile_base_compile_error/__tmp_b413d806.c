//DeepSeek-V3 V2.5 Category: Safe ; Style: struct_heavy ; Variation: log_analyzer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>

#define MAX_LINE_LENGTH 1024
#define MAX_ENTRIES 1000

struct LogEntry {
    char timestamp[20];
    char level[10];
    char message[MAX_LINE_LENGTH - 30];
};

struct LogStats {
    int total_entries;
    int error_count;
    int warning_count;
    int info_count;
    char earliest[20];
    char latest[20];
};

struct LogAnalyzer {
    struct LogEntry entries[MAX_ENTRIES];
    int entry_count;
    struct LogStats stats;
};

int validate_timestamp(const char* timestamp) {
    if (strlen(timestamp) != 19) return 0;
    for (int i = 0; i < 19; i++) {
        if (i == 4 || i == 7) {
            if (timestamp[i] != '-') return 0;
        } else if (i == 13 || i == 16) {
            if (timestamp[i] != ':') return 0;
        } else {
            if (!isdigit(timestamp[i])) return 0;
        }
    }
    return 1;
}

int validate_level(const char* level) {
    return (strcmp(level, "ERROR") == 0 || 
            strcmp(level, "WARNING") == 0 || 
            strcmp(level, "INFO") == 0);
}

void init_analyzer(struct LogAnalyzer* analyzer) {
    analyzer->entry_count = 0;
    analyzer->stats.total_entries = 0;
    analyzer->stats.error_count = 0;
    analyzer->stats.warning_count = 0;
    analyzer->stats.info_count = 0;
    memset(analyzer->stats.earliest, 0, 20);
    memset(analyzer->stats.latest, 0, 20);
}

int parse_log_line(const char* line, struct LogEntry* entry) {
    if (strlen(line) >= MAX_LINE_LENGTH) return 0;
    
    char temp_line[MAX_LINE_LENGTH];
    strncpy(temp_line, line, MAX_LINE_LENGTH - 1);
    temp_line[MAX_LINE_LENGTH - 1] = '\0';
    
    char* timestamp = strtok(temp_line, " ");
    char* level = strtok(NULL, " ");
    char* message = strtok(NULL, "\n");
    
    if (!timestamp || !level || !message) return 0;
    if (!validate_timestamp(timestamp) || !validate_level(level)) return 0;
    
    strncpy(entry->timestamp, timestamp, 19);
    entry->timestamp[19] = '\0';
    strncpy(entry->level, level, 9);
    entry->level[9] = '\0';
    strncpy(entry->message, message, MAX_LINE_LENGTH - 30);
    entry->message[MAX_LINE_LENGTH - 31] = '\0';
    
    return 1;
}

void update_stats(struct LogAnalyzer* analyzer, const struct LogEntry* entry) {
    analyzer->stats.total_entries++;
    
    if (strcmp(entry->level, "ERROR") == 0) {
        analyzer->stats.error_count++;
    } else if (strcmp(entry->level, "WARNING") == 0) {
        analyzer->stats.warning_count++;
    } else if (strcmp(entry->level, "INFO") == 0) {
        analyzer->stats.info_count++;
    }
    
    if (analyzer->stats.earliest[0] == '\0' || 
        strcmp(entry->timestamp, analyzer->stats.earliest) < 0) {
        strncpy(analyzer->stats.earliest, entry->timestamp, 19);
        analyzer->stats.earliest[19] = '\0';
    }
    
    if (analyzer->stats.latest[0] == '\0' || 
        strcmp(entry->timestamp, analyzer->stats.latest) > 0) {
        strncpy(analyzer->stats.latest, entry->timestamp, 19);
        analyzer->stats.latest[19] = '\0';
    }
}

int add_log_entry(struct LogAnalyzer* analyzer, const char* line) {
    if (analyzer->entry_count >= MAX_ENTRIES) return 0;
    
    struct LogEntry* entry = &analyzer->entries[analyzer->entry_count];
    if (!parse_log_line(line, entry)) return 0;
    
    update_stats(analyzer, entry);
    analyzer->entry_count++;
    return 1;
}

void generate_sample_logs(struct LogAnalyzer* analyzer) {
    const char* sample_logs[] = {
        "2024-01-15 10:30:15 INFO System started successfully",
        "2024-01-15 10:35:22 WARNING High memory usage detected",
        "2024-01-15 10:40:05 ERROR Database connection failed",
        "2024-01-15 10:45:33 INFO Backup completed",
        "2024-01-15 10:50:12 WARNING CPU temperature rising",
        "2024-01-15 10:55:47 INFO User login successful",
        "2024-01-15 11:00:03 ERROR File not found",
        "2024-01-15 11:05:28 INFO Cache cleared",
        "2024-01-15