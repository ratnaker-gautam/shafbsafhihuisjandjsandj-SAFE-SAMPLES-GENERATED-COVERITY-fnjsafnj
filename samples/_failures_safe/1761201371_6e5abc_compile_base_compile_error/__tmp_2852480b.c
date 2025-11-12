//DeepSeek-V3 V2.5 Category: Safe ; Style: modular_functions ; Variation: log_analyzer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>

#define MAX_LINE_LENGTH 1024
#define MAX_ENTRIES 1000

struct LogEntry {
    char timestamp[64];
    char level[16];
    char message[256];
};

int parse_log_line(const char* line, struct LogEntry* entry) {
    if (line == NULL || entry == NULL) return 0;
    
    char buffer[MAX_LINE_LENGTH];
    if (strlen(line) >= MAX_LINE_LENGTH - 1) return 0;
    strncpy(buffer, line, MAX_LINE_LENGTH - 1);
    buffer[MAX_LINE_LENGTH - 1] = '\0';
    
    char* parts[3];
    parts[0] = buffer;
    int part_count = 0;
    
    for (char* p = buffer; *p && part_count < 3; p++) {
        if (*p == '|') {
            *p = '\0';
            parts[part_count++] = p + 1;
        }
    }
    
    if (part_count != 2) return 0;
    
    if (strlen(parts[0]) >= sizeof(entry->timestamp) - 1) return 0;
    if (strlen(parts[1]) >= sizeof(entry->level) - 1) return 0;
    if (strlen(parts[2]) >= sizeof(entry->message) - 1) return 0;
    
    strncpy(entry->timestamp, parts[0], sizeof(entry->timestamp) - 1);
    entry->timestamp[sizeof(entry->timestamp) - 1] = '\0';
    
    strncpy(entry->level, parts[1], sizeof(entry->level) - 1);
    entry->level[sizeof(entry->level) - 1] = '\0';
    
    strncpy(entry->message, parts[2], sizeof(entry->message) - 1);
    entry->message[sizeof(entry->message) - 1] = '\0';
    
    return 1;
}

int validate_log_entry(const struct LogEntry* entry) {
    if (entry == NULL) return 0;
    if (strlen(entry->timestamp) == 0) return 0;
    if (strlen(entry->level) == 0) return 0;
    if (strlen(entry->message) == 0) return 0;
    
    const char* valid_levels[] = {"INFO", "WARNING", "ERROR", "DEBUG"};
    int valid = 0;
    for (int i = 0; i < 4; i++) {
        if (strcmp(entry->level, valid_levels[i]) == 0) {
            valid = 1;
            break;
        }
    }
    return valid;
}

void analyze_logs(struct LogEntry* entries, int count) {
    if (entries == NULL || count <= 0) return;
    
    int level_counts[4] = {0};
    const char* level_names[] = {"INFO", "WARNING", "ERROR", "DEBUG"};
    
    for (int i = 0; i < count; i++) {
        for (int j = 0; j < 4; j++) {
            if (strcmp(entries[i].level, level_names[j]) == 0) {
                level_counts[j]++;
                break;
            }
        }
    }
    
    printf("Log Analysis Results:\n");
    printf("====================\n");
    printf("Total entries processed: %d\n", count);
    printf("INFO: %d entries\n", level_counts[0]);
    printf("WARNING: %d entries\n", level_counts[1]);
    printf("ERROR: %d entries\n", level_counts[2]);
    printf("DEBUG: %d entries\n", level_counts[3]);
    
    int error_index = -1;
    for (int i = 0; i < count; i++) {
        if (strcmp(entries[i].level, "ERROR") == 0) {
            error_index = i;
            break;
        }
    }
    
    if (error_index != -1) {
        printf("\nFirst ERROR entry:\n");
        printf("Timestamp: %s\n", entries[error_index].timestamp);
        printf("Message: %s\n", entries[error_index].message);
    }
}

int read_log_file(const char* filename, struct LogEntry* entries, int max_entries) {
    if (filename == NULL || entries == NULL || max_entries <= 0) return 0;
    
    FILE* file = fopen(filename, "r");
    if (file == NULL) return 0;
    
    char line[MAX_LINE_LENGTH];
    int entry_count = 0;
    
    while (fgets(line, sizeof(line), file) != NULL && entry_count < max_entries) {
        size_t len = strlen(line);
        if (len > 0 && line[len - 1] == '\n') {
            line[len - 1] = '\0';
        }
        
        if (strlen(line) == 0) continue;
        
        struct LogEntry entry;
        if (parse_log_line(line, &entry) && validate_log_entry(&entry)) {
            entries[entry_count++] = entry;
        }
    }
    
    fclose(file);
    return entry_count;
}

int main(void) {
    struct LogEntry entries[MAX_ENTRIES];
    
    printf("Log Analyzer\n");