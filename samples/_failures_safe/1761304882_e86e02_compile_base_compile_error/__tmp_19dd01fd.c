//DeepSeek-V3 V2.5 Category: Safe ; Style: struct_heavy ; Variation: log_analyzer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdint.h>
#include <ctype.h>

#define MAX_LINE_LENGTH 1024
#define MAX_ENTRIES 1000
#define MAX_LEVEL_LENGTH 16

struct LogEntry {
    char timestamp[32];
    char level[MAX_LEVEL_LENGTH];
    char message[256];
    uint32_t line_number;
};

struct LogStats {
    uint32_t total_entries;
    uint32_t error_count;
    uint32_t warning_count;
    uint32_t info_count;
    char most_frequent_level[MAX_LEVEL_LENGTH];
    uint32_t max_level_count;
};

struct LogAnalyzer {
    struct LogEntry entries[MAX_ENTRIES];
    struct LogStats stats;
    uint32_t entry_count;
};

void initialize_analyzer(struct LogAnalyzer *analyzer) {
    if (analyzer == NULL) return;
    memset(analyzer, 0, sizeof(struct LogAnalyzer));
    strncpy(analyzer->stats.most_frequent_level, "NONE", MAX_LEVEL_LENGTH - 1);
    analyzer->stats.most_frequent_level[MAX_LEVEL_LENGTH - 1] = '\0';
}

int parse_log_line(const char *line, struct LogEntry *entry) {
    if (line == NULL || entry == NULL) return 0;
    
    char level_buf[MAX_LEVEL_LENGTH];
    char timestamp_buf[32];
    char message_buf[256];
    
    int parsed = sscanf(line, "%31s %15s %255[^\n]", timestamp_buf, level_buf, message_buf);
    
    if (parsed != 3) return 0;
    
    if (strlen(timestamp_buf) >= sizeof(entry->timestamp) ||
        strlen(level_buf) >= sizeof(entry->level) ||
        strlen(message_buf) >= sizeof(entry->message)) {
        return 0;
    }
    
    strncpy(entry->timestamp, timestamp_buf, sizeof(entry->timestamp) - 1);
    entry->timestamp[sizeof(entry->timestamp) - 1] = '\0';
    
    strncpy(entry->level, level_buf, sizeof(entry->level) - 1);
    entry->level[sizeof(entry->level) - 1] = '\0';
    
    strncpy(entry->message, message_buf, sizeof(entry->message) - 1);
    entry->message[sizeof(entry->message) - 1] = '\0';
    
    return 1;
}

void update_stats(struct LogAnalyzer *analyzer, const char *level) {
    if (analyzer == NULL || level == NULL) return;
    
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
        strncpy(analyzer->stats.most_frequent_level, level, MAX_LEVEL_LENGTH - 1);
        analyzer->stats.most_frequent_level[MAX_LEVEL_LENGTH - 1] = '\0';
    }
}

int add_log_entry(struct LogAnalyzer *analyzer, const char *line) {
    if (analyzer == NULL || line == NULL) return 0;
    if (analyzer->entry_count >= MAX_ENTRIES) return 0;
    
    struct LogEntry *entry = &analyzer->entries[analyzer->entry_count];
    
    if (!parse_log_line(line, entry)) return 0;
    
    entry->line_number = analyzer->entry_count + 1;
    update_stats(analyzer, entry->level);
    analyzer->entry_count++;
    
    return 1;
}

void print_analysis(const struct LogAnalyzer *analyzer) {
    if (analyzer == NULL) return;
    
    printf("Log Analysis Report\n");
    printf("===================\n");
    printf("Total entries: %u\n", analyzer->stats.total_entries);
    printf("ERROR count: %u\n", analyzer->stats.error_count);
    printf("WARNING count: %u\n", analyzer->stats.warning_count);
    printf("INFO count: %u\n", analyzer->stats.info_count);
    printf("Most frequent level: %s (%u occurrences)\n", 
           analyzer->stats.most_frequent_level, analyzer->stats.max_level_count);
    printf("\n");
    
    printf("Recent entries:\n");
    uint32_t start = (analyzer->entry_count > 5) ? analyzer->entry_count - 5 : 0;
    for (uint32_t i = start; i < analyzer->entry_count; i++) {
        const struct LogEntry *entry = &analyzer->entries[i];
        printf("[%u] %s %s: %s\n", entry->line_number, entry->timestamp, 
               entry->level, entry->message);