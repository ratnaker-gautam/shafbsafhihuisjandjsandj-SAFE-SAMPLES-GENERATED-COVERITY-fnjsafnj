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
    char most_frequent_level[16];
    uint32_t max_level_count;
};

struct LogAnalyzer {
    struct LogEntry entries[MAX_ENTRIES];
    struct LogStats stats;
    uint32_t entry_count;
};

void init_analyzer(struct LogAnalyzer *analyzer) {
    memset(analyzer, 0, sizeof(struct LogAnalyzer));
    strcpy(analyzer->stats.most_frequent_level, "UNKNOWN");
}

int parse_log_line(const char *line, struct LogEntry *entry) {
    if (!line || !entry) return 0;
    
    char level[16] = {0};
    char timestamp[32] = {0};
    char message[256] = {0};
    
    int parsed = sscanf(line, "%31s %15s %255[^\n]", timestamp, level, message);
    if (parsed < 3) return 0;
    
    if (strlen(timestamp) > 31 || strlen(level) > 15 || strlen(message) > 255) {
        return 0;
    }
    
    strcpy(entry->timestamp, timestamp);
    strcpy(entry->level, level);
    strcpy(entry->message, message);
    
    return 1;
}

void update_stats(struct LogAnalyzer *analyzer, const char *level) {
    if (!analyzer || !level) return;
    
    analyzer->stats.total_entries++;
    
    if (strcmp(level, "ERROR") == 0) {
        analyzer->stats.error_count++;
    } else if (strcmp(level, "WARNING") == 0) {
        analyzer->stats.warning_count++;
    } else if (strcmp(level, "INFO") == 0) {
        analyzer->stats.info_count++;
    }
    
    uint32_t current_count = 0;
    for (uint32_t i = 0; i < analyzer->entry_count; i++) {
        if (strcmp(analyzer->entries[i].level, level) == 0) {
            current_count++;
        }
    }
    
    if (current_count > analyzer->stats.max_level_count) {
        analyzer->stats.max_level_count = current_count;
        strcpy(analyzer->stats.most_frequent_level, level);
    }
}

int add_log_entry(struct LogAnalyzer *analyzer, const char *line) {
    if (!analyzer || !line || analyzer->entry_count >= MAX_ENTRIES) {
        return 0;
    }
    
    if (strlen(line) >= MAX_LINE_LENGTH) return 0;
    
    struct LogEntry *entry = &analyzer->entries[analyzer->entry_count];
    entry->line_number = analyzer->entry_count + 1;
    
    if (!parse_log_line(line, entry)) {
        return 0;
    }
    
    update_stats(analyzer, entry->level);
    analyzer->entry_count++;
    
    return 1;
}

void print_stats(const struct LogAnalyzer *analyzer) {
    if (!analyzer) return;
    
    printf("Log Analysis Results:\n");
    printf("Total entries: %u\n", analyzer->stats.total_entries);
    printf("ERROR entries: %u\n", analyzer->stats.error_count);
    printf("WARNING entries: %u\n", analyzer->stats.warning_count);
    printf("INFO entries: %u\n", analyzer->stats.info_count);
    printf("Most frequent level: %s (%u occurrences)\n", 
           analyzer->stats.most_frequent_level, analyzer->stats.max_level_count);
}

void print_entries_by_level(const struct LogAnalyzer *analyzer, const char *level) {
    if (!analyzer || !level) return;
    
    printf("\nEntries with level '%s':\n", level);
    uint32_t count = 0;
    
    for (uint32_t i = 0; i < analyzer->entry_count; i++) {
        if (strcmp(analyzer->entries[i].level, level) == 0) {
            printf("%u: [%s] %s - %s\n", 
                   analyzer->entries[i].line_number,
                   analyzer->entries[i].timestamp,
                   analyzer->entries[i].level,
                   analyzer->entries[i].message);
            count++;
        }
    }
    
    if (count == 0) {
        printf("No entries found with level '%s'\n", level);
    }
}

int main(void) {
    struct LogAnalyzer analyzer;
    init_analyzer(&analyzer);
    
    const char *sample_logs[] = {
        "2024-01-15T10:30:00 INFO System started successfully",
        "2024-01-15T10:31:15 WARNING High memory usage detected",
        "2024-01-15T10:32:00 INFO User login: john_doe",
        "2024-01-15T10:33:45 ERROR Database connection failed",
        "2024-01-15T10:34:10 INFO Backup process started",
        "202