//DeepSeek-V3 V2.5 Category: Safe ; Style: struct_heavy ; Variation: log_analyzer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdint.h>
#include <ctype.h>

#define MAX_LINE_LENGTH 1024
#define MAX_ENTRIES 1000
#define MAX_FILENAME 256

struct LogEntry {
    time_t timestamp;
    char level[16];
    char message[256];
    uint32_t source_id;
};

struct LogStats {
    uint32_t total_entries;
    uint32_t error_count;
    uint32_t warning_count;
    uint32_t info_count;
    time_t earliest_time;
    time_t latest_time;
    uint32_t unique_sources;
    uint32_t sources[100];
};

struct LogAnalyzer {
    struct LogEntry entries[MAX_ENTRIES];
    struct LogStats stats;
    uint32_t entry_count;
};

void init_analyzer(struct LogAnalyzer *analyzer) {
    memset(analyzer, 0, sizeof(struct LogAnalyzer));
    analyzer->stats.earliest_time = 0;
    analyzer->stats.latest_time = 0;
}

int parse_timestamp(const char *str, time_t *timestamp) {
    struct tm tm = {0};
    if (strptime(str, "%Y-%m-%d %H:%M:%S", &tm) == NULL) {
        return 0;
    }
    *timestamp = mktime(&tm);
    return *timestamp != -1;
}

int validate_level(const char *level) {
    return (strcmp(level, "ERROR") == 0 || 
            strcmp(level, "WARNING") == 0 || 
            strcmp(level, "INFO") == 0);
}

int add_log_entry(struct LogAnalyzer *analyzer, const char *timestamp_str, 
                  const char *level, const char *message, uint32_t source_id) {
    if (analyzer->entry_count >= MAX_ENTRIES) {
        return 0;
    }
    
    if (!validate_level(level)) {
        return 0;
    }
    
    if (strlen(message) >= 256 || strlen(level) >= 16) {
        return 0;
    }
    
    struct LogEntry *entry = &analyzer->entries[analyzer->entry_count];
    
    if (!parse_timestamp(timestamp_str, &entry->timestamp)) {
        return 0;
    }
    
    strncpy(entry->level, level, 15);
    entry->level[15] = '\0';
    strncpy(entry->message, message, 255);
    entry->message[255] = '\0';
    entry->source_id = source_id;
    
    analyzer->entry_count++;
    return 1;
}

void calculate_stats(struct LogAnalyzer *analyzer) {
    if (analyzer->entry_count == 0) {
        return;
    }
    
    struct LogStats *stats = &analyzer->stats;
    memset(stats, 0, sizeof(struct LogStats));
    
    stats->total_entries = analyzer->entry_count;
    stats->earliest_time = analyzer->entries[0].timestamp;
    stats->latest_time = analyzer->entries[0].timestamp;
    
    for (uint32_t i = 0; i < analyzer->entry_count; i++) {
        struct LogEntry *entry = &analyzer->entries[i];
        
        if (strcmp(entry->level, "ERROR") == 0) {
            stats->error_count++;
        } else if (strcmp(entry->level, "WARNING") == 0) {
            stats->warning_count++;
        } else if (strcmp(entry->level, "INFO") == 0) {
            stats->info_count++;
        }
        
        if (entry->timestamp < stats->earliest_time) {
            stats->earliest_time = entry->timestamp;
        }
        if (entry->timestamp > stats->latest_time) {
            stats->latest_time = entry->timestamp;
        }
        
        int found = 0;
        for (uint32_t j = 0; j < stats->unique_sources; j++) {
            if (stats->sources[j] == entry->source_id) {
                found = 1;
                break;
            }
        }
        if (!found && stats->unique_sources < 100) {
            stats->sources[stats->unique_sources] = entry->source_id;
            stats->unique_sources++;
        }
    }
}

void print_stats(const struct LogAnalyzer *analyzer) {
    const struct LogStats *stats = &analyzer->stats;
    
    printf("Log Analysis Results:\n");
    printf("Total entries: %u\n", stats->total_entries);
    printf("Errors: %u\n", stats->error_count);
    printf("Warnings: %u\n", stats->warning_count);
    printf("Info: %u\n", stats->info_count);
    
    char buffer[64];
    struct tm *tm_info = localtime(&stats->earliest_time);
    if (tm_info != NULL) {
        strftime(buffer, 64, "%Y-%m-%d %H:%M:%S", tm_info);
        printf("Earliest: %s\n", buffer);
    }
    
    tm_info = localtime(&stats->latest_time);
    if (tm_info != NULL) {
        strftime(buffer, 64, "%Y-%m-%d %H:%M:%S", tm_info);
        printf("Latest: %s\n", buffer);
    }
    
    printf("Unique sources: %u\n", stats->unique_sources