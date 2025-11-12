//DeepSeek-V3 V2.5 Category: Safe ; Style: struct_heavy ; Variation: log_analyzer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>

#define MAX_LINE_LENGTH 1024
#define MAX_ENTRIES 1000
#define MAX_LEVEL_LENGTH 16
#define MAX_MESSAGE_LENGTH 256

struct LogEntry {
    char timestamp[32];
    char level[MAX_LEVEL_LENGTH];
    char message[MAX_MESSAGE_LENGTH];
    int severity;
};

struct LogStats {
    int total_entries;
    int error_count;
    int warning_count;
    int info_count;
    int debug_count;
    char earliest_timestamp[32];
    char latest_timestamp[32];
};

struct LogAnalyzer {
    struct LogEntry entries[MAX_ENTRIES];
    struct LogStats stats;
    int entry_count;
};

void init_log_analyzer(struct LogAnalyzer *analyzer) {
    if (!analyzer) return;
    memset(analyzer, 0, sizeof(*analyzer));
    analyzer->stats.total_entries = 0;
    analyzer->stats.error_count = 0;
    analyzer->stats.warning_count = 0;
    analyzer->stats.info_count = 0;
    analyzer->stats.debug_count = 0;
    analyzer->entry_count = 0;
}

int parse_timestamp(const char *str, char *output) {
    if (!str || !output) return 0;
    size_t len = strlen(str);
    if (len == 0 || len >= 32) return 0;
    
    strncpy(output, str, 31);
    output[31] = '\0';
    return 1;
}

int parse_log_level(const char *str, char *output) {
    if (!str || !output) return 0;
    
    const char *levels[] = {"ERROR", "WARNING", "INFO", "DEBUG"};
    for (int i = 0; i < 4; i++) {
        if (strcasecmp(str, levels[i]) == 0) {
            strncpy(output, levels[i], MAX_LEVEL_LENGTH - 1);
            output[MAX_LEVEL_LENGTH - 1] = '\0';
            return 1;
        }
    }
    return 0;
}

int add_log_entry(struct LogAnalyzer *analyzer, const char *timestamp, 
                  const char *level, const char *message) {
    if (!analyzer || !timestamp || !level || !message) return 0;
    if (analyzer->entry_count >= MAX_ENTRIES) return 0;
    
    struct LogEntry *entry = &analyzer->entries[analyzer->entry_count];
    
    if (!parse_timestamp(timestamp, entry->timestamp)) return 0;
    if (!parse_log_level(level, entry->level)) return 0;
    
    size_t msg_len = strlen(message);
    if (msg_len == 0 || msg_len >= MAX_MESSAGE_LENGTH) return 0;
    strncpy(entry->message, message, MAX_MESSAGE_LENGTH - 1);
    entry->message[MAX_MESSAGE_LENGTH - 1] = '\0';
    
    if (strcmp(entry->level, "ERROR") == 0) entry->severity = 4;
    else if (strcmp(entry->level, "WARNING") == 0) entry->severity = 3;
    else if (strcmp(entry->level, "INFO") == 0) entry->severity = 2;
    else if (strcmp(entry->level, "DEBUG") == 0) entry->severity = 1;
    
    analyzer->entry_count++;
    return 1;
}

void update_stats(struct LogAnalyzer *analyzer) {
    if (!analyzer || analyzer->entry_count == 0) return;
    
    struct LogStats *stats = &analyzer->stats;
    stats->total_entries = analyzer->entry_count;
    stats->error_count = 0;
    stats->warning_count = 0;
    stats->info_count = 0;
    stats->debug_count = 0;
    
    strcpy(stats->earliest_timestamp, analyzer->entries[0].timestamp);
    strcpy(stats->latest_timestamp, analyzer->entries[0].timestamp);
    
    for (int i = 0; i < analyzer->entry_count; i++) {
        struct LogEntry *entry = &analyzer->entries[i];
        
        if (strcmp(entry->level, "ERROR") == 0) stats->error_count++;
        else if (strcmp(entry->level, "WARNING") == 0) stats->warning_count++;
        else if (strcmp(entry->level, "INFO") == 0) stats->info_count++;
        else if (strcmp(entry->level, "DEBUG") == 0) stats->debug_count++;
        
        if (strcmp(entry->timestamp, stats->earliest_timestamp) < 0) {
            strcpy(stats->earliest_timestamp, entry->timestamp);
        }
        if (strcmp(entry->timestamp, stats->latest_timestamp) > 0) {
            strcpy(stats->latest_timestamp, entry->timestamp);
        }
    }
}

void print_stats(const struct LogAnalyzer *analyzer) {
    if (!analyzer) return;
    
    const struct LogStats *stats = &analyzer->stats;
    printf("Log Analysis Results:\n");
    printf("Total entries: %d\n", stats->total_entries);
    printf("Errors: %d\n", stats->error_count);
    printf("W