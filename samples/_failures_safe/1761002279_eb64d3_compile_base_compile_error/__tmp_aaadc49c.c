//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: struct_heavy ; Variation: log_analyzer
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
    char most_frequent_level[16];
};

struct LogFile {
    char filename[MAX_FILENAME];
    struct LogEntry entries[MAX_ENTRIES];
    uint32_t entry_count;
    struct LogStats stats;
};

int validate_filename(const char *filename) {
    if (filename == NULL) return 0;
    size_t len = strlen(filename);
    if (len == 0 || len >= MAX_FILENAME) return 0;
    for (size_t i = 0; i < len; i++) {
        if (!isalnum(filename[i]) && filename[i] != '.' && filename[i] != '_' && filename[i] != '-') {
            return 0;
        }
    }
    return 1;
}

int parse_log_line(const char *line, struct LogEntry *entry) {
    if (line == NULL || entry == NULL) return 0;
    
    char temp_line[MAX_LINE_LENGTH];
    if (strlen(line) >= MAX_LINE_LENGTH - 1) return 0;
    strcpy(temp_line, line);
    
    char *timestamp = strtok(temp_line, " ");
    char *level = strtok(NULL, " ");
    char *message = strtok(NULL, "\n");
    
    if (timestamp == NULL || level == NULL || message == NULL) return 0;
    
    if (strlen(timestamp) >= sizeof(entry->timestamp) - 1) return 0;
    if (strlen(level) >= sizeof(entry->level) - 1) return 0;
    if (strlen(message) >= sizeof(entry->message) - 1) return 0;
    
    strcpy(entry->timestamp, timestamp);
    strcpy(entry->level, level);
    strcpy(entry->message, message);
    
    return 1;
}

void analyze_log_file(struct LogFile *log_file) {
    if (log_file == NULL) return;
    
    log_file->stats.total_entries = log_file->entry_count;
    log_file->stats.error_count = 0;
    log_file->stats.warning_count = 0;
    log_file->stats.info_count = 0;
    log_file->stats.max_message_length = 0;
    
    uint32_t level_counts[3] = {0};
    const char *levels[] = {"ERROR", "WARNING", "INFO"};
    
    for (uint32_t i = 0; i < log_file->entry_count; i++) {
        struct LogEntry *entry = &log_file->entries[i];
        
        if (strcmp(entry->level, "ERROR") == 0) {
            log_file->stats.error_count++;
            level_counts[0]++;
        } else if (strcmp(entry->level, "WARNING") == 0) {
            log_file->stats.warning_count++;
            level_counts[1]++;
        } else if (strcmp(entry->level, "INFO") == 0) {
            log_file->stats.info_count++;
            level_counts[2]++;
        }
        
        size_t msg_len = strlen(entry->message);
        if (msg_len > log_file->stats.max_message_length) {
            log_file->stats.max_message_length = (uint32_t)msg_len;
        }
    }
    
    uint32_t max_count = 0;
    uint32_t max_index = 0;
    for (int i = 0; i < 3; i++) {
        if (level_counts[i] > max_count) {
            max_count = level_counts[i];
            max_index = i;
        }
    }
    
    strcpy(log_file->stats.most_frequent_level, levels[max_index]);
}

int read_log_file(struct LogFile *log_file) {
    if (log_file == NULL) return 0;
    
    FILE *file = fopen(log_file->filename, "r");
    if (file == NULL) return 0;
    
    char line[MAX_LINE_LENGTH];
    uint32_t line_num = 0;
    
    while (fgets(line, sizeof(line), file) != NULL && line_num < MAX_ENTRIES) {
        line[strcspn(line, "\n")] = '\0';
        
        struct LogEntry entry;
        entry.line_number = line_num + 1;
        
        if (parse_log_line(line, &entry)) {
            log_file->entries[line_num] = entry;
            line_num++;
        }
    }
    
    log_file->entry_count = line_num;
    fclose(file);
    
    analyze_log_file(log_file);
    return 1;
}

void print_log_stats(const struct LogFile *log_file) {
    if (log_file == NULL) return;
    
    printf("Log Analysis for: %s\n", log_file->filename);
    printf("Total entries: %u\n", log_file->stats.total_entries);
    printf("ERROR entries: %u\n", log_file->stats.error_count);
    printf("WARNING entries: %u\n", log_file->stats.warning_count);