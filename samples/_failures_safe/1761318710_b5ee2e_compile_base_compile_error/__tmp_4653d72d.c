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

void initialize_analyzer(struct LogAnalyzer *analyzer) {
    memset(analyzer, 0, sizeof(struct LogAnalyzer));
    strcpy(analyzer->stats.most_frequent_level, "UNKNOWN");
}

int parse_log_line(const char *line, struct LogEntry *entry) {
    if (!line || !entry) return 0;
    
    char temp_line[MAX_LINE_LENGTH];
    strncpy(temp_line, line, MAX_LINE_LENGTH - 1);
    temp_line[MAX_LINE_LENGTH - 1] = '\0';
    
    char *timestamp = strtok(temp_line, " ");
    char *level = strtok(NULL, " ");
    char *message = strtok(NULL, "\n");
    
    if (!timestamp || !level || !message) return 0;
    
    if (strlen(timestamp) >= sizeof(entry->timestamp) ||
        strlen(level) >= sizeof(entry->level) ||
        strlen(message) >= sizeof(entry->message)) {
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
        strncpy(analyzer->stats.most_frequent_level, level, 
                sizeof(analyzer->stats.most_frequent_level) - 1);
        analyzer->stats.most_frequent_level[sizeof(analyzer->stats.most_frequent_level) - 1] = '\0';
    }
}

int add_log_entry(struct LogAnalyzer *analyzer, const char *line) {
    if (!analyzer || !line || analyzer->entry_count >= MAX_ENTRIES) {
        return 0;
    }
    
    struct LogEntry *entry = &analyzer->entries[analyzer->entry_count];
    
    if (!parse_log_line(line, entry)) {
        return 0;
    }
    
    entry->line_number = analyzer->entry_count + 1;
    update_stats(analyzer, entry->level);
    analyzer->entry_count++;
    
    return 1;
}

void display_stats(const struct LogAnalyzer *analyzer) {
    if (!analyzer) return;
    
    printf("Log Analysis Results:\n");
    printf("Total entries: %u\n", analyzer->stats.total_entries);
    printf("ERROR count: %u\n", analyzer->stats.error_count);
    printf("WARNING count: %u\n", analyzer->stats.warning_count);
    printf("INFO count: %u\n", analyzer->stats.info_count);
    printf("Most frequent level: %s (%u occurrences)\n", 
           analyzer->stats.most_frequent_level, analyzer->stats.max_level_count);
}

void display_entries_by_level(const struct LogAnalyzer *analyzer, const char *level) {
    if (!analyzer || !level) return;
    
    printf("Entries with level '%s':\n", level);
    int found = 0;
    
    for (uint32_t i = 0; i < analyzer->entry_count; i++) {
        if (strcmp(analyzer->entries[i].level, level) == 0) {
            printf("%u: [%s] %s - %s\n", 
                   analyzer->entries[i].line_number,
                   analyzer->entries[i].timestamp,
                   analyzer->entries[i].level,
                   analyzer->entries[i].message);
            found = 1;
        }
    }
    
    if (!found) {
        printf("No entries found with level '%s'\n", level);
    }
}

int main(void) {
    struct LogAnalyzer analyzer;
    initialize_analyzer(&analyzer);
    
    const char *sample_logs[] = {
        "2024-01-15T10:30:00 INFO System started successfully",
        "2024-01-15T10:31:15 WARNING High memory usage