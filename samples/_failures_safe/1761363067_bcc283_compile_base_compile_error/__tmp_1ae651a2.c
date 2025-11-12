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

struct LogFile {
    char filename[MAX_FILENAME];
    struct LogEntry entries[MAX_ENTRIES];
    size_t entry_count;
    uint32_t error_count;
    uint32_t warning_count;
    uint32_t info_count;
};

struct LogStats {
    uint32_t total_entries;
    uint32_t total_errors;
    uint32_t total_warnings;
    uint32_t total_info;
    time_t analysis_time;
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

int parse_log_level(const char *level, struct LogEntry *entry) {
    if (level == NULL || entry == NULL) return 0;
    
    if (strncmp(level, "ERROR", 5) == 0) {
        strncpy(entry->level, "ERROR", sizeof(entry->level) - 1);
        return 1;
    } else if (strncmp(level, "WARNING", 7) == 0) {
        strncpy(entry->level, "WARNING", sizeof(entry->level) - 1);
        return 1;
    } else if (strncmp(level, "INFO", 4) == 0) {
        strncpy(entry->level, "INFO", sizeof(entry->level) - 1);
        return 1;
    }
    return 0;
}

int parse_log_line(const char *line, uint32_t line_num, struct LogEntry *entry) {
    if (line == NULL || entry == NULL) return 0;
    
    char timestamp[32] = {0};
    char level[16] = {0};
    char message[256] = {0};
    
    int result = sscanf(line, "%31s %15s %255[^\n]", timestamp, level, message);
    if (result < 3) return 0;
    
    if (strlen(timestamp) > 0) {
        strncpy(entry->timestamp, timestamp, sizeof(entry->timestamp) - 1);
    }
    
    if (!parse_log_level(level, entry)) return 0;
    
    if (strlen(message) > 0) {
        strncpy(entry->message, message, sizeof(entry->message) - 1);
    }
    
    entry->line_number = line_num;
    return 1;
}

void initialize_log_file(struct LogFile *log_file, const char *filename) {
    if (log_file == NULL || filename == NULL) return;
    
    memset(log_file, 0, sizeof(struct LogFile));
    strncpy(log_file->filename, filename, sizeof(log_file->filename) - 1);
    log_file->entry_count = 0;
    log_file->error_count = 0;
    log_file->warning_count = 0;
    log_file->info_count = 0;
}

int process_log_file(struct LogFile *log_file) {
    if (log_file == NULL) return 0;
    
    FILE *file = fopen(log_file->filename, "r");
    if (file == NULL) return 0;
    
    char line[MAX_LINE_LENGTH];
    uint32_t line_number = 0;
    
    while (fgets(line, sizeof(line), file) != NULL && log_file->entry_count < MAX_ENTRIES) {
        line_number++;
        size_t len = strlen(line);
        if (len > 0 && line[len - 1] == '\n') {
            line[len - 1] = '\0';
        }
        
        if (strlen(line) == 0) continue;
        
        struct LogEntry entry;
        memset(&entry, 0, sizeof(entry));
        
        if (parse_log_line(line, line_number, &entry)) {
            log_file->entries[log_file->entry_count] = entry;
            
            if (strcmp(entry.level, "ERROR") == 0) {
                log_file->error_count++;
            } else if (strcmp(entry.level, "WARNING") == 0) {
                log_file->warning_count++;
            } else if (strcmp(entry.level, "INFO") == 0) {
                log_file->info_count++;
            }
            
            log_file->entry_count++;
        }
    }
    
    fclose(file);
    return 1;
}

void generate_stats(const struct LogFile *log_file, struct LogStats *stats) {
    if (log_file == NULL || stats == NULL) return;
    
    memset(stats, 0, sizeof(struct LogStats));
    stats->total_entries = log_file->entry_count;
    stats->total_errors = log_file->error_count;
    stats->total_warnings = log_file->warning_count;
    stats->total_info = log_file->info_count;
    stats