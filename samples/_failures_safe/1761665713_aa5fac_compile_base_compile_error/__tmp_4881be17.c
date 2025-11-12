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
    uint32_t max_message_length;
    char most_common_level[16];
};

struct LogAnalyzer {
    struct LogEntry entries[MAX_ENTRIES];
    struct LogStats stats;
    uint32_t entry_count;
};

void init_analyzer(struct LogAnalyzer *analyzer) {
    if (!analyzer) return;
    memset(analyzer, 0, sizeof(struct LogAnalyzer));
    strcpy(analyzer->stats.most_common_level, "UNKNOWN");
}

int parse_log_line(const char *line, struct LogEntry *entry) {
    if (!line || !entry) return 0;
    
    char temp_line[MAX_LINE_LENGTH];
    if (strlen(line) >= MAX_LINE_LENGTH - 1) return 0;
    strcpy(temp_line, line);
    
    char *timestamp = strtok(temp_line, " ");
    char *level = strtok(NULL, " ");
    char *message = strtok(NULL, "\n");
    
    if (!timestamp || !level || !message) return 0;
    
    if (strlen(timestamp) >= sizeof(entry->timestamp) - 1) return 0;
    if (strlen(level) >= sizeof(entry->level) - 1) return 0;
    if (strlen(message) >= sizeof(entry->message) - 1) return 0;
    
    strcpy(entry->timestamp, timestamp);
    strcpy(entry->level, level);
    strcpy(entry->message, message);
    
    return 1;
}

void update_stats(struct LogAnalyzer *analyzer, const struct LogEntry *entry) {
    if (!analyzer || !entry) return;
    
    analyzer->stats.total_entries++;
    
    size_t msg_len = strlen(entry->message);
    if (msg_len > analyzer->stats.max_message_length) {
        analyzer->stats.max_message_length = msg_len;
    }
    
    if (strcmp(entry->level, "ERROR") == 0) {
        analyzer->stats.error_count++;
    } else if (strcmp(entry->level, "WARNING") == 0) {
        analyzer->stats.warning_count++;
    } else if (strcmp(entry->level, "INFO") == 0) {
        analyzer->stats.info_count++;
    }
}

void calculate_common_level(struct LogAnalyzer *analyzer) {
    if (!analyzer) return;
    
    uint32_t error = analyzer->stats.error_count;
    uint32_t warning = analyzer->stats.warning_count;
    uint32_t info = analyzer->stats.info_count;
    
    if (error >= warning && error >= info) {
        strcpy(analyzer->stats.most_common_level, "ERROR");
    } else if (warning >= error && warning >= info) {
        strcpy(analyzer->stats.most_common_level, "WARNING");
    } else if (info >= error && info >= warning) {
        strcpy(analyzer->stats.most_common_level, "INFO");
    }
}

int read_log_file(struct LogAnalyzer *analyzer, const char *filename) {
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
        memset(&entry, 0, sizeof(entry));
        entry.line_number = line_num;
        
        if (parse_log_line(line, &entry)) {
            analyzer->entries[analyzer->entry_count] = entry;
            update_stats(analyzer, &entry);
            analyzer->entry_count++;
        }
    }
    
    fclose(file);
    calculate_common_level(analyzer);
    return 1;
}

void print_analysis(const struct LogAnalyzer *analyzer) {
    if (!analyzer) return;
    
    printf("Log Analysis Results:\n");
    printf("Total entries: %u\n", analyzer->stats.total_entries);
    printf("Error count: %u\n", analyzer->stats.error_count);
    printf("Warning count: %u\n", analyzer->stats.warning_count);
    printf("Info count: %u\n", analyzer->stats.info_count);
    printf("Max message length: %u\n", analyzer->stats.max_message_length);
    printf("Most common level: %s\n", analyzer->stats.most_common_level);
    
    printf("\nSample entries:\n");
    uint32_t samples = (analyzer->entry_count < 3) ? analyzer->entry_count : 3;
    for (uint32_t i = 0; i < samples; i++) {