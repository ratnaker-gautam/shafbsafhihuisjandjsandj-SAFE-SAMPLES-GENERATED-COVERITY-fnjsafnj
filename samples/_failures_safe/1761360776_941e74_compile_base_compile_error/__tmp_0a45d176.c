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
    char most_frequent_level[16];
    uint32_t max_level_count;
};

struct LogAnalyzer {
    struct LogEntry entries[MAX_ENTRIES];
    struct LogStats stats;
    uint32_t entry_count;
};

void initialize_analyzer(struct LogAnalyzer *analyzer) {
    if (!analyzer) return;
    memset(analyzer, 0, sizeof(struct LogAnalyzer));
    strncpy(analyzer->stats.most_frequent_level, "NONE", 15);
    analyzer->stats.most_frequent_level[15] = '\0';
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
    
    if (strlen(timestamp) >= 31 || strlen(level) >= 15 || strlen(message) >= 255) return 0;
    
    strncpy(entry->timestamp, timestamp, 31);
    entry->timestamp[31] = '\0';
    strncpy(entry->level, level, 15);
    entry->level[15] = '\0';
    strncpy(entry->message, message, 255);
    entry->message[255] = '\0';
    
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
    } else if (strcmp(level, "DEBUG") == 0) {
        analyzer->stats.debug_count++;
    }
    
    uint32_t current_count = 0;
    if (strcmp(level, "ERROR") == 0) current_count = analyzer->stats.error_count;
    else if (strcmp(level, "WARNING") == 0) current_count = analyzer->stats.warning_count;
    else if (strcmp(level, "INFO") == 0) current_count = analyzer->stats.info_count;
    else if (strcmp(level, "DEBUG") == 0) current_count = analyzer->stats.debug_count;
    
    if (current_count > analyzer->stats.max_level_count) {
        analyzer->stats.max_level_count = current_count;
        strncpy(analyzer->stats.most_frequent_level, level, 15);
        analyzer->stats.most_frequent_level[15] = '\0';
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
        entry.line_number = line_num;
        
        if (parse_log_line(line, &entry)) {
            analyzer->entries[analyzer->entry_count] = entry;
            update_stats(analyzer, entry.level);
            analyzer->entry_count++;
        }
    }
    
    fclose(file);
    return 1;
}

void print_stats(const struct LogAnalyzer *analyzer) {
    if (!analyzer) return;
    
    printf("Log Analysis Results:\n");
    printf("Total entries: %u\n", analyzer->stats.total_entries);
    printf("ERROR entries: %u\n", analyzer->stats.error_count);
    printf("WARNING entries: %u\n", analyzer->stats.warning_count);
    printf("INFO entries: %u\n", analyzer->stats.info_count);
    printf("DEBUG entries: %u\n", analyzer->stats.debug_count);
    printf("Most frequent level: %s (%u occurrences)\n", 
           analyzer->stats.most_frequent_level, analyzer->stats.max_level_count);
}

void print_errors(const struct LogAnalyzer *analyzer) {
    if (!analyzer) return;
    
    printf("