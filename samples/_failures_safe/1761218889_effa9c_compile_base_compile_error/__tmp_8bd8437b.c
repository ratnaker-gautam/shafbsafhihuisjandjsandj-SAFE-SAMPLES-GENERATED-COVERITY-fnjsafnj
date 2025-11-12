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
    uint32_t max_line_length;
    uint32_t min_line_length;
    double avg_line_length;
};

struct LogAnalyzer {
    struct LogEntry entries[MAX_ENTRIES];
    struct LogStats stats;
    uint32_t entry_count;
};

void initialize_analyzer(struct LogAnalyzer *analyzer) {
    if (analyzer == NULL) return;
    memset(analyzer, 0, sizeof(struct LogAnalyzer));
    analyzer->stats.min_line_length = UINT32_MAX;
}

int parse_log_level(const char *level, struct LogEntry *entry) {
    if (level == NULL || entry == NULL) return 0;
    
    size_t len = strlen(level);
    if (len == 0 || len >= MAX_LEVEL_LENGTH) return 0;
    
    for (size_t i = 0; i < len; i++) {
        if (!isalpha(level[i])) return 0;
    }
    
    strncpy(entry->level, level, MAX_LEVEL_LENGTH - 1);
    entry->level[MAX_LEVEL_LENGTH - 1] = '\0';
    return 1;
}

int parse_timestamp(const char *timestamp, struct LogEntry *entry) {
    if (timestamp == NULL || entry == NULL) return 0;
    
    size_t len = strlen(timestamp);
    if (len == 0 || len >= 31) return 0;
    
    strncpy(entry->timestamp, timestamp, 31);
    entry->timestamp[31] = '\0';
    return 1;
}

int parse_message(const char *message, struct LogEntry *entry) {
    if (message == NULL || entry == NULL) return 0;
    
    size_t len = strlen(message);
    if (len == 0 || len >= 255) return 0;
    
    strncpy(entry->message, message, 255);
    entry->message[255] = '\0';
    return 1;
}

void update_stats(struct LogAnalyzer *analyzer, const struct LogEntry *entry) {
    if (analyzer == NULL || entry == NULL) return;
    
    struct LogStats *stats = &analyzer->stats;
    stats->total_entries++;
    
    size_t line_length = strlen(entry->message);
    if (line_length > stats->max_line_length) {
        stats->max_line_length = line_length;
    }
    if (line_length < stats->min_line_length) {
        stats->min_line_length = line_length;
    }
    
    if (strcmp(entry->level, "ERROR") == 0) {
        stats->error_count++;
    } else if (strcmp(entry->level, "WARNING") == 0) {
        stats->warning_count++;
    } else if (strcmp(entry->level, "INFO") == 0) {
        stats->info_count++;
    }
}

int add_log_entry(struct LogAnalyzer *analyzer, const char *timestamp, 
                  const char *level, const char *message, uint32_t line_num) {
    if (analyzer == NULL || timestamp == NULL || level == NULL || message == NULL) {
        return 0;
    }
    
    if (analyzer->entry_count >= MAX_ENTRIES) {
        return 0;
    }
    
    struct LogEntry *entry = &analyzer->entries[analyzer->entry_count];
    memset(entry, 0, sizeof(struct LogEntry));
    
    if (!parse_timestamp(timestamp, entry)) return 0;
    if (!parse_log_level(level, entry)) return 0;
    if (!parse_message(message, entry)) return 0;
    
    entry->line_number = line_num;
    update_stats(analyzer, entry);
    analyzer->entry_count++;
    
    return 1;
}

void calculate_final_stats(struct LogAnalyzer *analyzer) {
    if (analyzer == NULL || analyzer->stats.total_entries == 0) return;
    
    struct LogStats *stats = &analyzer->stats;
    uint64_t total_length = 0;
    
    for (uint32_t i = 0; i < analyzer->entry_count; i++) {
        total_length += strlen(analyzer->entries[i].message);
    }
    
    if (stats->total_entries > 0) {
        stats->avg_line_length = (double)total_length / stats->total_entries;
    }
    
    if (stats->min_line_length == UINT32_MAX) {
        stats->min_line_length = 0;
    }
}

void print_analysis(const struct LogAnalyzer *analyzer) {
    if (analyzer == NULL) return;
    
    printf("Log Analysis Results:\n");
    printf("Total entries: %u\n", analyzer->stats.total_entries);
    printf("Error entries: %u\n", analyzer->stats.error_count);
    printf("Warning entries: %u\n", analyzer->stats.warning_count);
    printf("Info entries: %u\n", analyzer->stats