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

struct LogFile {
    char filename[MAX_FILENAME];
    struct LogEntry entries[MAX_ENTRIES];
    uint32_t entry_count;
    struct LogStats stats;
};

int validate_filename(const char *filename) {
    if (filename == NULL || strlen(filename) == 0 || strlen(filename) >= MAX_FILENAME) {
        return 0;
    }
    for (size_t i = 0; i < strlen(filename); i++) {
        if (!isalnum(filename[i]) && filename[i] != '.' && filename[i] != '-' && filename[i] != '_') {
            return 0;
        }
    }
    return 1;
}

int parse_log_line(const char *line, struct LogEntry *entry) {
    if (line == NULL || entry == NULL || strlen(line) >= MAX_LINE_LENGTH) {
        return 0;
    }
    
    char temp_line[MAX_LINE_LENGTH];
    strncpy(temp_line, line, MAX_LINE_LENGTH - 1);
    temp_line[MAX_LINE_LENGTH - 1] = '\0';
    
    char *timestamp = strtok(temp_line, " ");
    char *level = strtok(NULL, " ");
    char *message = strtok(NULL, "\n");
    
    if (timestamp == NULL || level == NULL || message == NULL) {
        return 0;
    }
    
    if (strlen(timestamp) >= sizeof(entry->timestamp) || 
        strlen(level) >= sizeof(entry->level) || 
        strlen(message) >= sizeof(entry->message)) {
        return 0;
    }
    
    strncpy(entry->timestamp, timestamp, sizeof(entry->timestamp) - 1);
    entry->timestamp[sizeof(entry->timestamp) - 1] = '\0';
    
    strncpy(entry->level, level, sizeof(entry->level) - 1);
    entry->level[sizeof(entry->level) - 1] = '\0';
    
    strncpy(entry->message, message, sizeof(entry->message) - 1);
    entry->message[sizeof(entry->message) - 1] = '\0';
    
    return 1;
}

void calculate_stats(struct LogFile *logfile) {
    if (logfile == NULL) return;
    
    struct LogStats *stats = &logfile->stats;
    stats->total_entries = logfile->entry_count;
    stats->error_count = 0;
    stats->warning_count = 0;
    stats->info_count = 0;
    stats->max_message_length = 0;
    
    uint32_t level_counts[3] = {0};
    const char *levels[] = {"ERROR", "WARNING", "INFO"};
    
    for (uint32_t i = 0; i < logfile->entry_count; i++) {
        struct LogEntry *entry = &logfile->entries[i];
        size_t msg_len = strlen(entry->message);
        
        if (msg_len > stats->max_message_length) {
            stats->max_message_length = (uint32_t)msg_len;
        }
        
        if (strcmp(entry->level, "ERROR") == 0) {
            stats->error_count++;
            level_counts[0]++;
        } else if (strcmp(entry->level, "WARNING") == 0) {
            stats->warning_count++;
            level_counts[1]++;
        } else if (strcmp(entry->level, "INFO") == 0) {
            stats->info_count++;
            level_counts[2]++;
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
    
    strncpy(stats->most_common_level, levels[max_index], sizeof(stats->most_common_level) - 1);
    stats->most_common_level[sizeof(stats->most_common_level) - 1] = '\0';
}

int read_log_file(struct LogFile *logfile) {
    if (logfile == NULL || !validate_filename(logfile->filename)) {
        return 0;
    }
    
    FILE *file = fopen(logfile->filename, "r");
    if (file == NULL) {
        return 0;
    }
    
    char line[MAX_LINE_LENGTH];
    uint32_t line_num = 0;
    logfile->entry_count = 0;
    
    while (fgets(line, sizeof(line), file) != NULL && logfile->entry_count < MAX_ENTRIES) {
        line_num++;
        if (strlen(line) > 0 && line[strlen(line) - 1] == '\n') {
            line[strlen(line) - 1] = '\0';
        }