//DeepSeek-V3 V2.5 Category: Safe ; Style: modular_functions ; Variation: log_analyzer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>

#define MAX_LINE_LENGTH 1024
#define MAX_ENTRIES 1000

struct LogEntry {
    char timestamp[32];
    char level[16];
    char message[256];
};

int parse_log_line(const char* line, struct LogEntry* entry) {
    if (line == NULL || entry == NULL) return 0;
    
    char buffer[MAX_LINE_LENGTH];
    if (strlen(line) >= MAX_LINE_LENGTH) return 0;
    strncpy(buffer, line, MAX_LINE_LENGTH - 1);
    buffer[MAX_LINE_LENGTH - 1] = '\0';
    
    char* timestamp_start = strchr(buffer, '[');
    if (timestamp_start == NULL) return 0;
    
    char* timestamp_end = strchr(timestamp_start, ']');
    if (timestamp_end == NULL) return 0;
    
    *timestamp_end = '\0';
    size_t timestamp_len = timestamp_end - timestamp_start - 1;
    if (timestamp_len >= sizeof(entry->timestamp)) return 0;
    strncpy(entry->timestamp, timestamp_start + 1, timestamp_len);
    entry->timestamp[timestamp_len] = '\0';
    
    char* level_start = timestamp_end + 1;
    while (*level_start == ' ') level_start++;
    
    char* level_end = strchr(level_start, ' ');
    if (level_end == NULL) return 0;
    
    size_t level_len = level_end - level_start;
    if (level_len >= sizeof(entry->level)) return 0;
    strncpy(entry->level, level_start, level_len);
    entry->level[level_len] = '\0';
    
    char* message_start = level_end + 1;
    while (*message_start == ' ') message_start++;
    
    size_t message_len = strlen(message_start);
    if (message_len >= sizeof(entry->message)) return 0;
    strncpy(entry->message, message_start, message_len);
    entry->message[message_len] = '\0';
    
    return 1;
}

void count_log_levels(struct LogEntry* entries, int count, int* level_counts) {
    if (entries == NULL || level_counts == NULL || count <= 0) return;
    
    for (int i = 0; i < count; i++) {
        if (strcmp(entries[i].level, "INFO") == 0) {
            level_counts[0]++;
        } else if (strcmp(entries[i].level, "WARNING") == 0) {
            level_counts[1]++;
        } else if (strcmp(entries[i].level, "ERROR") == 0) {
            level_counts[2]++;
        } else {
            level_counts[3]++;
        }
    }
}

void display_statistics(int* level_counts) {
    if (level_counts == NULL) return;
    
    int total = level_counts[0] + level_counts[1] + level_counts[2] + level_counts[3];
    if (total == 0) {
        printf("No log entries found.\n");
        return;
    }
    
    printf("Log Statistics:\n");
    printf("INFO: %d (%.1f%%)\n", level_counts[0], (level_counts[0] * 100.0) / total);
    printf("WARNING: %d (%.1f%%)\n", level_counts[1], (level_counts[1] * 100.0) / total);
    printf("ERROR: %d (%.1f%%)\n", level_counts[2], (level_counts[2] * 100.0) / total);
    printf("OTHER: %d (%.1f%%)\n", level_counts[3], (level_counts[3] * 100.0) / total);
    printf("Total entries: %d\n", total);
}

int read_log_file(const char* filename, struct LogEntry* entries, int max_entries) {
    if (filename == NULL || entries == NULL || max_entries <= 0) return 0;
    
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        printf("Error: Cannot open file '%s'\n", filename);
        return 0;
    }
    
    char line[MAX_LINE_LENGTH];
    int entry_count = 0;
    
    while (fgets(line, sizeof(line), file) != NULL && entry_count < max_entries) {
        line[strcspn(line, "\n")] = '\0';
        
        if (strlen(line) > 0) {
            struct LogEntry entry;
            if (parse_log_line(line, &entry)) {
                entries[entry_count] = entry;
                entry_count++;
            }
        }
    }
    
    fclose(file);
    return entry_count;
}

int main(void) {
    char filename[256];
    printf("Enter log file name: ");
    
    if (fgets(filename, sizeof(filename), stdin) == NULL) {
        printf("Error reading input.\n");
        return 1;
    }
    
    filename[strcspn(filename, "\n")] = '\0';
    
    if (strlen(filename) == 0) {
        printf("Error: No filename provided.\n");
        return 1;
    }
    
    struct LogEntry entries[MAX_ENTRIES];
    int entry_count = read_log_file(filename, entries, MAX_ENTRIES);
    
    if