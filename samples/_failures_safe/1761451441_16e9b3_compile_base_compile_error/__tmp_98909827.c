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
    uint32_t max_line_length;
    uint32_t min_line_length;
    double avg_line_length;
};

struct LogAnalyzer {
    struct LogEntry entries[MAX_ENTRIES];
    struct LogStats stats;
    uint32_t entry_count;
    char filename[MAX_FILENAME];
};

void init_log_analyzer(struct LogAnalyzer *analyzer, const char *filename) {
    if (analyzer == NULL || filename == NULL) return;
    
    analyzer->entry_count = 0;
    strncpy(analyzer->filename, filename, MAX_FILENAME - 1);
    analyzer->filename[MAX_FILENAME - 1] = '\0';
    
    analyzer->stats.total_entries = 0;
    analyzer->stats.error_count = 0;
    analyzer->stats.warning_count = 0;
    analyzer->stats.info_count = 0;
    analyzer->stats.max_line_length = 0;
    analyzer->stats.min_line_length = UINT32_MAX;
    analyzer->stats.avg_line_length = 0.0;
}

int parse_log_line(const char *line, struct LogEntry *entry, uint32_t line_num) {
    if (line == NULL || entry == NULL) return 0;
    
    char timestamp[32] = {0};
    char level[16] = {0};
    char message[256] = {0};
    
    int parsed = sscanf(line, "%31s %15s %255[^\n]", timestamp, level, message);
    
    if (parsed >= 2) {
        strncpy(entry->timestamp, timestamp, sizeof(entry->timestamp) - 1);
        entry->timestamp[sizeof(entry->timestamp) - 1] = '\0';
        
        strncpy(entry->level, level, sizeof(entry->level) - 1);
        entry->level[sizeof(entry->level) - 1] = '\0';
        
        if (parsed == 3) {
            strncpy(entry->message, message, sizeof(entry->message) - 1);
            entry->message[sizeof(entry->message) - 1] = '\0';
        } else {
            entry->message[0] = '\0';
        }
        
        entry->line_number = line_num;
        return 1;
    }
    
    return 0;
}

void update_stats(struct LogAnalyzer *analyzer, const char *line) {
    if (analyzer == NULL || line == NULL) return;
    
    size_t len = strlen(line);
    if (len > analyzer->stats.max_line_length) {
        analyzer->stats.max_line_length = len;
    }
    if (len < analyzer->stats.min_line_length) {
        analyzer->stats.min_line_length = len;
    }
    
    analyzer->stats.total_entries++;
}

void process_log_level(struct LogAnalyzer *analyzer, const char *level) {
    if (analyzer == NULL || level == NULL) return;
    
    if (strcmp(level, "ERROR") == 0) {
        analyzer->stats.error_count++;
    } else if (strcmp(level, "WARNING") == 0) {
        analyzer->stats.warning_count++;
    } else if (strcmp(level, "INFO") == 0) {
        analyzer->stats.info_count++;
    }
}

int load_log_file(struct LogAnalyzer *analyzer) {
    if (analyzer == NULL) return 0;
    
    FILE *file = fopen(analyzer->filename, "r");
    if (file == NULL) return 0;
    
    char line[MAX_LINE_LENGTH];
    uint32_t line_num = 0;
    
    while (fgets(line, sizeof(line), file) != NULL && analyzer->entry_count < MAX_ENTRIES) {
        line_num++;
        size_t len = strlen(line);
        if (len > 0 && line[len - 1] == '\n') {
            line[len - 1] = '\0';
        }
        
        update_stats(analyzer, line);
        
        struct LogEntry entry;
        if (parse_log_line(line, &entry, line_num)) {
            analyzer->entries[analyzer->entry_count] = entry;
            process_log_level(analyzer, entry.level);
            analyzer->entry_count++;
        }
    }
    
    fclose(file);
    
    if (analyzer->stats.total_entries > 0) {
        analyzer->stats.avg_line_length = (double)analyzer->stats.max_line_length / analyzer->stats.total_entries;
    }
    
    return 1;
}

void print_analysis(const struct LogAnalyzer *analyzer) {
    if (analyzer == NULL) return;
    
    printf("Log Analysis for: %s\n", analyzer->filename);
    printf("Total entries processed: %u\n", analyzer->stats.total_entries);
    printf("Valid log entries: %u\n", analyzer->entry_count);
    printf("Error count: %u\n", analyzer->stats.error_count);
    printf("Warning count: