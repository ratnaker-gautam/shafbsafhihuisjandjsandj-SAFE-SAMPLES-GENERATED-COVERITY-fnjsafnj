//DeepSeek-V3 V2.5 Category: Safe ; Style: struct_heavy ; Variation: log_analyzer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdint.h>
#include <ctype.h>

#define MAX_LINE_LENGTH 1024
#define MAX_ENTRIES 1000

struct LogEntry {
    char timestamp[32];
    char level[16];
    char message[256];
    uint32_t line_number;
};

struct LogStats {
    uint32_t total_entries;
    uint32_t error_count;
    uint32_t warning_count;
    uint32_t info_count;
    uint32_t debug_count;
    char earliest_timestamp[32];
    char latest_timestamp[32];
};

struct LogAnalyzer {
    struct LogEntry entries[MAX_ENTRIES];
    struct LogStats stats;
    uint32_t entry_count;
};

void initialize_analyzer(struct LogAnalyzer *analyzer) {
    memset(analyzer, 0, sizeof(struct LogAnalyzer));
    strcpy(analyzer->stats.earliest_timestamp, "9999-12-31 23:59:59");
    strcpy(analyzer->stats.latest_timestamp, "0000-01-01 00:00:00");
}

int parse_log_line(const char *line, struct LogEntry *entry) {
    if (!line || !entry) return 0;
    
    char temp_line[MAX_LINE_LENGTH];
    if (strlen(line) >= MAX_LINE_LENGTH - 1) return 0;
    strcpy(temp_line, line);
    
    char *timestamp = strtok(temp_line, " ");
    char *level = strtok(NULL, " ");
    char *message = strtok(NULL, "\n");
    
    if (!timestamp || !level || !message) return 0;
    
    if (strlen(timestamp) >= sizeof(entry->timestamp) - 1) return 0;
    if (strlen(level) >= sizeof(entry->level) - 1) return 0;
    if (strlen(message) >= sizeof(entry->message) - 1) return 0;
    
    strcpy(entry->timestamp, timestamp);
    strcpy(entry->level, level);
    strcpy(entry->message, message);
    
    return 1;
}

void update_stats(struct LogAnalyzer *analyzer, const struct LogEntry *entry) {
    if (!analyzer || !entry) return;
    
    analyzer->stats.total_entries++;
    
    if (strcmp(entry->level, "ERROR") == 0) {
        analyzer->stats.error_count++;
    } else if (strcmp(entry->level, "WARNING") == 0) {
        analyzer->stats.warning_count++;
    } else if (strcmp(entry->level, "INFO") == 0) {
        analyzer->stats.info_count++;
    } else if (strcmp(entry->level, "DEBUG") == 0) {
        analyzer->stats.debug_count++;
    }
    
    if (strcmp(entry->timestamp, analyzer->stats.earliest_timestamp) < 0) {
        strcpy(analyzer->stats.earliest_timestamp, entry->timestamp);
    }
    if (strcmp(entry->timestamp, analyzer->stats.latest_timestamp) > 0) {
        strcpy(analyzer->stats.latest_timestamp, entry->timestamp);
    }
}

int add_log_entry(struct LogAnalyzer *analyzer, const char *line) {
    if (!analyzer || !line || analyzer->entry_count >= MAX_ENTRIES) return 0;
    
    struct LogEntry entry;
    if (!parse_log_line(line, &entry)) return 0;
    
    entry.line_number = analyzer->entry_count + 1;
    analyzer->entries[analyzer->entry_count] = entry;
    update_stats(analyzer, &entry);
    analyzer->entry_count++;
    
    return 1;
}

void print_statistics(const struct LogAnalyzer *analyzer) {
    if (!analyzer) return;
    
    printf("Log Analysis Results:\n");
    printf("Total entries: %u\n", analyzer->stats.total_entries);
    printf("ERROR entries: %u\n", analyzer->stats.error_count);
    printf("WARNING entries: %u\n", analyzer->stats.warning_count);
    printf("INFO entries: %u\n", analyzer->stats.info_count);
    printf("DEBUG entries: %u\n", analyzer->stats.debug_count);
    printf("Earliest timestamp: %s\n", analyzer->stats.earliest_timestamp);
    printf("Latest timestamp: %s\n", analyzer->stats.latest_timestamp);
}

void print_entries_by_level(const struct LogAnalyzer *analyzer, const char *level) {
    if (!analyzer || !level) return;
    
    printf("Entries with level '%s':\n", level);
    for (uint32_t i = 0; i < analyzer->entry_count; i++) {
        if (strcmp(analyzer->entries[i].level, level) == 0) {
            printf("[%u] %s %s %s\n", 
                   analyzer->entries[i].line_number,
                   analyzer->entries[i].timestamp,
                   analyzer->entries[i].level,
                   analyzer->entries[i].message);
        }
    }
}

int main(void) {
    struct LogAnalyzer analyzer;
    initialize_analyzer(&analyzer);
    
    const char *sample_logs[] = {
        "2024-01-15 10:30:00 INFO Application started",
        "2024-01-15 10:31:15 DEBUG Loading configuration",
        "