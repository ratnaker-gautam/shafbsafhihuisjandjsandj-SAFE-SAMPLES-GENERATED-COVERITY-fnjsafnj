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

void initialize_analyzer(struct LogAnalyzer *analyzer) {
    analyzer->entry_count = 0;
    analyzer->stats.total_entries = 0;
    analyzer->stats.error_count = 0;
    analyzer->stats.warning_count = 0;
    analyzer->stats.info_count = 0;
    memset(analyzer->stats.earliest, 0, sizeof(analyzer->stats.earliest));
    memset(analyzer->stats.latest, 0, sizeof(analyzer->stats.latest));
}

int parse_timestamp(const char *str, char *timestamp) {
    if (strlen(str) < 19) return 0;
    for (int i = 0; i < 19; i++) {
        if (i == 4 || i == 7) {
            if (str[i] != '-') return 0;
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

int parse_log_level(const char *str) {
    if (strncmp(str, "ERROR", 5) == 0) return 1;
    if (strncmp(str, "WARNING", 7) == 0) return 2;
    if (strncmp(str, "INFO", 4) == 0) return 3;
    return 0;
}

int add_log_entry(struct LogAnalyzer *analyzer, const char *line) {
    if (analyzer->entry_count >= MAX_ENTRIES) return 0;
    if (strlen(line) >= MAX_LINE_LENGTH) return 0;
    
    struct LogEntry *entry = &analyzer->entries[analyzer->entry_count];
    
    char temp_line[MAX_LINE_LENGTH];
    strncpy(temp_line, line, sizeof(temp_line) - 1);
    temp_line[sizeof(temp_line) - 1] = '\0';
    
    char *timestamp_start = temp_line;
    char *level_start = strchr(temp_line, ' ');
    if (!level_start) return 0;
    *level_start = '\0';
    level_start++;
    
    if (!parse_timestamp(timestamp_start, entry->timestamp)) return 0;
    
    char *message_start = strchr(level_start, ' ');
    if (!message_start) return 0;
    *message_start = '\0';
    message_start++;
    
    int level_type = parse_log_level(level_start);
    if (level_type == 0) return 0;
    
    strncpy(entry->level, level_start, sizeof(entry->level) - 1);
    entry->level[sizeof(entry->level) - 1] = '\0';
    
    strncpy(entry->message, message_start, sizeof(entry->message) - 1);
    entry->message[sizeof(entry->message) - 1] = '\0';
    
    analyzer->entry_count++;
    return 1;
}

void update_stats(struct LogAnalyzer *analyzer) {
    if (analyzer->entry_count == 0) return;
    
    analyzer->stats.total_entries = analyzer->entry_count;
    analyzer->stats.error_count = 0;
    analyzer->stats.warning_count = 0;
    analyzer->stats.info_count = 0;
    
    strcpy(analyzer->stats.earliest, analyzer->entries[0].timestamp);
    strcpy(analyzer->stats.latest, analyzer->entries[0].timestamp);
    
    for (int i = 0; i < analyzer->entry_count; i++) {
        struct LogEntry *entry = &analyzer->entries[i];
        
        if (strcmp(entry->level, "ERROR") == 0) {
            analyzer->stats.error_count++;
        } else if (strcmp(entry->level, "WARNING") == 0) {
            analyzer->stats.warning_count++;
        } else if (strcmp(entry->level, "INFO") == 0) {
            analyzer->stats.info_count++;
        }
        
        if (strcmp(entry->timestamp, analyzer->stats.earliest) < 0) {
            strcpy(analyzer->stats.earliest, entry->timestamp);
        }
        if (strcmp(entry->timestamp, analyzer->stats.latest) > 0) {
            strcpy(analyzer->stats.latest, entry->timestamp);
        }
    }
}

void print_stats(const struct LogStats *stats) {
    printf("Log Analysis Results:\n");
    printf("Total entries: %d\n", stats->total_entries);
    printf("Errors: %d\n", stats->error_count);
    printf("Warnings: %d\n", stats->