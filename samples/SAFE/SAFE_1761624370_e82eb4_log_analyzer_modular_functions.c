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
    
    char temp_line[MAX_LINE_LENGTH];
    if (strlen(line) >= MAX_LINE_LENGTH - 1) return 0;
    strcpy(temp_line, line);
    
    char* timestamp_start = strchr(temp_line, '[');
    if (timestamp_start == NULL) return 0;
    
    char* timestamp_end = strchr(timestamp_start, ']');
    if (timestamp_end == NULL) return 0;
    
    *timestamp_end = '\0';
    if (strlen(timestamp_start + 1) >= sizeof(entry->timestamp)) return 0;
    strcpy(entry->timestamp, timestamp_start + 1);
    
    char* level_start = timestamp_end + 1;
    while (*level_start == ' ') level_start++;
    
    char* level_end = strchr(level_start, ' ');
    if (level_end == NULL) return 0;
    
    *level_end = '\0';
    if (strlen(level_start) >= sizeof(entry->level)) return 0;
    strcpy(entry->level, level_start);
    
    char* message_start = level_end + 1;
    while (*message_start == ' ') message_start++;
    
    if (strlen(message_start) >= sizeof(entry->message)) return 0;
    strcpy(entry->message, message_start);
    
    return 1;
}

int count_log_levels(const struct LogEntry* entries, int count, const char* level) {
    int level_count = 0;
    for (int i = 0; i < count; i++) {
        if (strcmp(entries[i].level, level) == 0) {
            level_count++;
        }
    }
    return level_count;
}

void print_log_summary(const struct LogEntry* entries, int count) {
    printf("Log Analysis Summary:\n");
    printf("Total entries: %d\n", count);
    printf("INFO entries: %d\n", count_log_levels(entries, count, "INFO"));
    printf("WARN entries: %d\n", count_log_levels(entries, count, "WARN"));
    printf("ERROR entries: %d\n", count_log_levels(entries, count, "ERROR"));
    printf("DEBUG entries: %d\n", count_log_levels(entries, count, "DEBUG"));
}

int read_log_file(const char* filename, struct LogEntry* entries, int max_entries) {
    if (filename == NULL || entries == NULL || max_entries <= 0) return 0;
    
    FILE* file = fopen(filename, "r");
    if (file == NULL) return 0;
    
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
    struct LogEntry entries[MAX_ENTRIES];
    
    printf("Enter log file name: ");
    char filename[256];
    if (fgets(filename, sizeof(filename), stdin) == NULL) {
        printf("Error reading input\n");
        return 1;
    }
    
    filename[strcspn(filename, "\n")] = '\0';
    
    if (strlen(filename) == 0) {
        printf("Invalid filename\n");
        return 1;
    }
    
    int entry_count = read_log_file(filename, entries, MAX_ENTRIES);
    
    if (entry_count == 0) {
        printf("No valid log entries found or file cannot be read\n");
        return 1;
    }
    
    print_log_summary(entries, entry_count);
    
    printf("\nSample entries:\n");
    int samples = (entry_count < 3) ? entry_count : 3;
    for (int i = 0; i < samples; i++) {
        printf("[%s] %s %s\n", entries[i].timestamp, entries[i].level, entries[i].message);
    }
    
    return 0;
}