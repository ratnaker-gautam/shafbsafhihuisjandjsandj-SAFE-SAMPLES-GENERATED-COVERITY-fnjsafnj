//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: struct_heavy ; Variation: log_analyzer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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
    uint32_t max_message_length;
};

struct LogAnalyzer {
    struct LogEntry entries[MAX_ENTRIES];
    struct LogStats stats;
    uint32_t entry_count;
};

void init_log_analyzer(struct LogAnalyzer *analyzer) {
    memset(analyzer, 0, sizeof(struct LogAnalyzer));
}

int parse_log_level(const char *level) {
    if (strcmp(level, "ERROR") == 0) return 3;
    if (strcmp(level, "WARN") == 0) return 2;
    if (strcmp(level, "INFO") == 0) return 1;
    return 0;
}

int parse_log_line(const char *line, struct LogEntry *entry) {
    if (!line || !entry) return 0;
    
    char timestamp[32] = {0};
    char level[16] = {0};
    char message[256] = {0};
    
    int result = sscanf(line, "%31s %15s %255[^\n]", timestamp, level, message);
    if (result != 3) return 0;
    
    if (strlen(timestamp) > 31 || strlen(level) > 15 || strlen(message) > 255) return 0;
    
    strcpy(entry->timestamp, timestamp);
    strcpy(entry->level, level);
    strcpy(entry->message, message);
    
    return 1;
}

int add_log_entry(struct LogAnalyzer *analyzer, const char *line, uint32_t line_num) {
    if (!analyzer || !line || analyzer->entry_count >= MAX_ENTRIES) return 0;
    
    struct LogEntry entry;
    entry.line_number = line_num;
    
    if (!parse_log_line(line, &entry)) return 0;
    
    analyzer->entries[analyzer->entry_count] = entry;
    analyzer->entry_count++;
    
    return 1;
}

void update_stats(struct LogAnalyzer *analyzer) {
    if (!analyzer) return;
    
    memset(&analyzer->stats, 0, sizeof(struct LogStats));
    analyzer->stats.total_entries = analyzer->entry_count;
    
    for (uint32_t i = 0; i < analyzer->entry_count; i++) {
        const struct LogEntry *entry = &analyzer->entries[i];
        size_t msg_len = strlen(entry->message);
        
        if (msg_len > analyzer->stats.max_message_length) {
            analyzer->stats.max_message_length = msg_len;
        }
        
        if (strcmp(entry->level, "ERROR") == 0) {
            analyzer->stats.error_count++;
        } else if (strcmp(entry->level, "WARN") == 0) {
            analyzer->stats.warning_count++;
        } else if (strcmp(entry->level, "INFO") == 0) {
            analyzer->stats.info_count++;
        }
    }
}

void print_stats(const struct LogAnalyzer *analyzer) {
    if (!analyzer) return;
    
    printf("Log Analysis Results:\n");
    printf("Total entries: %u\n", analyzer->stats.total_entries);
    printf("ERROR entries: %u\n", analyzer->stats.error_count);
    printf("WARN entries: %u\n", analyzer->stats.warning_count);
    printf("INFO entries: %u\n", analyzer->stats.info_count);
    printf("Max message length: %u\n", analyzer->stats.max_message_length);
}

void print_errors(const struct LogAnalyzer *analyzer) {
    if (!analyzer) return;
    
    printf("\nERROR entries:\n");
    for (uint32_t i = 0; i < analyzer->entry_count; i++) {
        const struct LogEntry *entry = &analyzer->entries[i];
        if (strcmp(entry->level, "ERROR") == 0) {
            printf("Line %u: %s %s %s\n", entry->line_number, 
                   entry->timestamp, entry->level, entry->message);
        }
    }
}

int main(void) {
    struct LogAnalyzer analyzer;
    init_log_analyzer(&analyzer);
    
    printf("Enter log entries (one per line, format: TIMESTAMP LEVEL MESSAGE):\n");
    printf("Example: 2024-01-15T10:30:00 INFO System started\n");
    printf("Enter 'END' on a separate line to finish input.\n");
    
    char line[MAX_LINE_LENGTH];
    uint32_t line_number = 1;
    
    while (fgets(line, sizeof(line), stdin)) {
        size_t len = strlen(line);
        if (len > 0 && line[len - 1] == '\n') {
            line[len - 1] = '\0';
        }
        
        if (strcmp(line, "END") == 0) {
            break;
        }
        
        if (strlen(line) > 0) {
            if (!add_log_entry(&analyzer, line, line_number)) {
                printf("Failed to parse line