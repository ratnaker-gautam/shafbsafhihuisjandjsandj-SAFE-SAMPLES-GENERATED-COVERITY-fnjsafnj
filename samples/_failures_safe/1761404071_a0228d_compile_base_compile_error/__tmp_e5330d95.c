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

void init_log_analyzer(struct LogAnalyzer *analyzer) {
    if (!analyzer) return;
    memset(analyzer, 0, sizeof(*analyzer));
    analyzer->stats.total_entries = 0;
    analyzer->stats.error_count = 0;
    analyzer->stats.warning_count = 0;
    analyzer->stats.info_count = 0;
    analyzer->stats.debug_count = 0;
    analyzer->entry_count = 0;
    memset(analyzer->stats.earliest_timestamp, 0, 32);
    memset(analyzer->stats.latest_timestamp, 0, 32);
}

int parse_log_line(const char *line, struct LogEntry *entry) {
    if (!line || !entry) return 0;
    
    char temp_line[MAX_LINE_LENGTH];
    if (strlen(line) >= MAX_LINE_LENGTH) return 0;
    strncpy(temp_line, line, MAX_LINE_LENGTH - 1);
    temp_line[MAX_LINE_LENGTH - 1] = '\0';
    
    char *timestamp = strtok(temp_line, " ");
    char *level = strtok(NULL, " ");
    char *message = strtok(NULL, "\n");
    
    if (!timestamp || !level || !message) return 0;
    
    if (strlen(timestamp) >= 32 || strlen(level) >= 16 || strlen(message) >= 256) return 0;
    
    strncpy(entry->timestamp, timestamp, 31);
    entry->timestamp[31] = '\0';
    strncpy(entry->level, level, 15);
    entry->level[15] = '\0';
    strncpy(entry->message, message, 255);
    entry->message[255] = '\0';
    
    return 1;
}

void update_stats(struct LogAnalyzer *analyzer, const struct LogEntry *entry) {
    if (!analyzer || !entry) return;
    
    analyzer->stats.total_entries++;
    
    if (strcmp(entry->level, "ERROR") == 0) analyzer->stats.error_count++;
    else if (strcmp(entry->level, "WARNING") == 0) analyzer->stats.warning_count++;
    else if (strcmp(entry->level, "INFO") == 0) analyzer->stats.info_count++;
    else if (strcmp(entry->level, "DEBUG") == 0) analyzer->stats.debug_count++;
    
    if (analyzer->stats.total_entries == 1) {
        strncpy(analyzer->stats.earliest_timestamp, entry->timestamp, 31);
        strncpy(analyzer->stats.latest_timestamp, entry->timestamp, 31);
    } else {
        if (strcmp(entry->timestamp, analyzer->stats.earliest_timestamp) < 0) {
            strncpy(analyzer->stats.earliest_timestamp, entry->timestamp, 31);
        }
        if (strcmp(entry->timestamp, analyzer->stats.latest_timestamp) > 0) {
            strncpy(analyzer->stats.latest_timestamp, entry->timestamp, 31);
        }
    }
}

int load_log_file(struct LogAnalyzer *analyzer, const char *filename) {
    if (!analyzer || !filename) return 0;
    
    FILE *file = fopen(filename, "r");
    if (!file) return 0;
    
    char line[MAX_LINE_LENGTH];
    uint32_t line_num = 0;
    
    while (fgets(line, sizeof(line), file) && analyzer->entry_count < MAX_ENTRIES) {
        line_num++;
        size_t len = strlen(line);
        if (len > 0 && line[len - 1] == '\n') {
            line[len - 1] = '\0';
        }
        
        struct LogEntry entry;
        if (parse_log_line(line, &entry)) {
            entry.line_number = line_num;
            analyzer->entries[analyzer->entry_count] = entry;
            update_stats(analyzer, &entry);
            analyzer->entry_count++;
        }
    }
    
    fclose(file);
    return 1;
}

void print_statistics(const struct LogAnalyzer *analyzer) {
    if (!analyzer) return;
    
    printf("Log Analysis Statistics:\n");
    printf("Total entries: %u\n", analyzer->stats.total_entries);
    printf("Errors: %u\n", analyzer->stats.error_count);
    printf("Warnings: %u\n", analyzer->stats.warning_count);
    printf("Info: %u\n", analyzer->stats.info_count);
    printf("Debug: %u\n", analyzer->stats.debug_count);
    printf("Time range: %