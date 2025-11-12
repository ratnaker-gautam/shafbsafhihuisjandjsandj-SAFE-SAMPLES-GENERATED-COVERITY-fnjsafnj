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
    char filename[MAX_FILENAME];
};

void initialize_analyzer(struct LogAnalyzer *analyzer, const char *filename) {
    if (analyzer == NULL || filename == NULL) return;
    
    analyzer->entry_count = 0;
    strncpy(analyzer->filename, filename, MAX_FILENAME - 1);
    analyzer->filename[MAX_FILENAME - 1] = '\0';
    
    analyzer->stats.total_entries = 0;
    analyzer->stats.error_count = 0;
    analyzer->stats.warning_count = 0;
    analyzer->stats.info_count = 0;
    analyzer->stats.debug_count = 0;
    strcpy(analyzer->stats.most_frequent_level, "UNKNOWN");
    analyzer->stats.max_level_count = 0;
}

int parse_log_line(const char *line, struct LogEntry *entry) {
    if (line == NULL || entry == NULL) return 0;
    
    char level_buf[16];
    char timestamp_buf[32];
    char message_buf[256];
    
    if (sscanf(line, "%31s %15s %255[^\n]", timestamp_buf, level_buf, message_buf) != 3) {
        return 0;
    }
    
    strncpy(entry->timestamp, timestamp_buf, 31);
    entry->timestamp[31] = '\0';
    strncpy(entry->level, level_buf, 15);
    entry->level[15] = '\0';
    strncpy(entry->message, message_buf, 255);
    entry->message[255] = '\0';
    
    for (char *p = entry->level; *p; p++) {
        *p = (char)toupper((unsigned char)*p);
    }
    
    return 1;
}

int add_log_entry(struct LogAnalyzer *analyzer, const struct LogEntry *entry) {
    if (analyzer == NULL || entry == NULL) return 0;
    if (analyzer->entry_count >= MAX_ENTRIES) return 0;
    
    analyzer->entries[analyzer->entry_count] = *entry;
    analyzer->entries[analyzer->entry_count].line_number = analyzer->entry_count + 1;
    analyzer->entry_count++;
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
    } else if (strcmp(level, "DEBUG") == 0) {
        analyzer->stats.debug_count++;
    }
    
    uint32_t current_count = 0;
    if (strcmp(level, "ERROR") == 0) {
        current_count = analyzer->stats.error_count;
    } else if (strcmp(level, "WARNING") == 0) {
        current_count = analyzer->stats.warning_count;
    } else if (strcmp(level, "INFO") == 0) {
        current_count = analyzer->stats.info_count;
    } else if (strcmp(level, "DEBUG") == 0) {
        current_count = analyzer->stats.debug_count;
    }
    
    if (current_count > analyzer->stats.max_level_count) {
        analyzer->stats.max_level_count = current_count;
        strncpy(analyzer->stats.most_frequent_level, level, 15);
        analyzer->stats.most_frequent_level[15] = '\0';
    }
}

int process_log_file(struct LogAnalyzer *analyzer) {
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
        
        if (len <= 1) continue;
        
        struct LogEntry entry;
        if (parse_log_line(line, &entry)) {
            if (add_log_entry(analyzer, &entry)) {
                update