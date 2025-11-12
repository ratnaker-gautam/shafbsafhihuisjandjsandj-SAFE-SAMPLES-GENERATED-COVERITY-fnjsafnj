//DeepSeek-V3 V2.5 Category: Safe ; Style: function_pointers ; Variation: log_analyzer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_LINE_LENGTH 1024
#define MAX_ENTRIES 1000

typedef struct {
    char timestamp[64];
    char level[16];
    char message[256];
} LogEntry;

typedef void (*LogProcessor)(LogEntry*, int);

void count_by_level(LogEntry* entries, int count) {
    int info_count = 0, warn_count = 0, error_count = 0;
    
    for (int i = 0; i < count; i++) {
        if (strcmp(entries[i].level, "INFO") == 0) {
            info_count++;
        } else if (strcmp(entries[i].level, "WARN") == 0) {
            warn_count++;
        } else if (strcmp(entries[i].level, "ERROR") == 0) {
            error_count++;
        }
    }
    
    printf("INFO: %d\n", info_count);
    printf("WARN: %d\n", warn_count);
    printf("ERROR: %d\n", error_count);
}

void show_recent_entries(LogEntry* entries, int count) {
    int display_count = (count > 5) ? 5 : count;
    
    printf("Most recent %d entries:\n", display_count);
    for (int i = count - display_count; i < count; i++) {
        printf("[%s] %s: %s\n", entries[i].timestamp, entries[i].level, entries[i].message);
    }
}

void find_errors(LogEntry* entries, int count) {
    printf("Error entries:\n");
    for (int i = 0; i < count; i++) {
        if (strcmp(entries[i].level, "ERROR") == 0) {
            printf("[%s] %s\n", entries[i].timestamp, entries[i].message);
        }
    }
}

int parse_log_line(const char* line, LogEntry* entry) {
    if (!line || !entry) return 0;
    
    char temp_line[MAX_LINE_LENGTH];
    if (strlen(line) >= MAX_LINE_LENGTH - 1) return 0;
    strncpy(temp_line, line, MAX_LINE_LENGTH - 1);
    temp_line[MAX_LINE_LENGTH - 1] = '\0';
    
    char* timestamp_start = strchr(temp_line, '[');
    if (!timestamp_start) return 0;
    timestamp_start++;
    
    char* timestamp_end = strchr(timestamp_start, ']');
    if (!timestamp_end) return 0;
    
    size_t timestamp_len = timestamp_end - timestamp_start;
    if (timestamp_len >= sizeof(entry->timestamp)) return 0;
    strncpy(entry->timestamp, timestamp_start, timestamp_len);
    entry->timestamp[timestamp_len] = '\0';
    
    char* level_start = timestamp_end + 1;
    while (*level_start && isspace(*level_start)) level_start++;
    
    char* level_end = strchr(level_start, ':');
    if (!level_end) return 0;
    
    size_t level_len = level_end - level_start;
    if (level_len >= sizeof(entry->level)) return 0;
    strncpy(entry->level, level_start, level_len);
    entry->level[level_len] = '\0';
    
    char* message_start = level_end + 1;
    while (*message_start && isspace(*message_start)) message_start++;
    
    size_t message_len = strlen(message_start);
    if (message_len >= sizeof(entry->message)) return 0;
    strncpy(entry->message, message_start, sizeof(entry->message) - 1);
    entry->message[sizeof(entry->message) - 1] = '\0';
    
    return 1;
}

int read_log_file(LogEntry* entries, int max_entries) {
    char line[MAX_LINE_LENGTH];
    int count = 0;
    
    printf("Enter log entries (empty line to finish):\n");
    
    while (count < max_entries && fgets(line, sizeof(line), stdin)) {
        if (line[0] == '\n') break;
        
        size_t len = strlen(line);
        if (len > 0 && line[len - 1] == '\n') {
            line[len - 1] = '\0';
        }
        
        if (parse_log_line(line, &entries[count])) {
            count++;
        }
    }
    
    return count;
}

void process_logs(LogEntry* entries, int count, LogProcessor processor) {
    if (!entries || count <= 0 || !processor) return;
    processor(entries, count);
}

int main() {
    LogEntry entries[MAX_ENTRIES];
    LogProcessor processors[] = {count_by_level, show_recent_entries, find_errors};
    const char* options[] = {"Count by level", "Show recent entries", "Find errors"};
    int num_processors = sizeof(processors) / sizeof(processors[0]);
    
    int entry_count = read_log_file(entries, MAX_ENTRIES);
    if (entry_count == 0) {
        printf("No valid log entries found.\n");
        return 1;
    }
    
    printf("\nLog Analysis Options:\n");
    for (int i = 0; i < num_processors; i++) {
        printf("%d. %s\n", i + 1, options[i]);
    }