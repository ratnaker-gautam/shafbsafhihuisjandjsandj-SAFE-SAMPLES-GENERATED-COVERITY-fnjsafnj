//DeepSeek-V3 V2.5 Category: Safe ; Style: function_pointers ; Variation: log_analyzer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdint.h>

#define MAX_LINE_LENGTH 1024
#define MAX_LOG_ENTRIES 1000

typedef struct {
    char timestamp[32];
    char level[16];
    char message[256];
} LogEntry;

typedef void (*LogProcessor)(LogEntry*, int);

void count_log_levels(LogEntry* entries, int count) {
    int error_count = 0, warning_count = 0, info_count = 0;
    
    for (int i = 0; i < count; i++) {
        if (strcmp(entries[i].level, "ERROR") == 0) {
            error_count++;
        } else if (strcmp(entries[i].level, "WARNING") == 0) {
            warning_count++;
        } else if (strcmp(entries[i].level, "INFO") == 0) {
            info_count++;
        }
    }
    
    printf("Log Level Summary:\n");
    printf("ERROR: %d\n", error_count);
    printf("WARNING: %d\n", warning_count);
    printf("INFO: %d\n", info_count);
}

void find_longest_message(LogEntry* entries, int count) {
    int max_length = 0;
    int longest_index = -1;
    
    for (int i = 0; i < count; i++) {
        int length = strlen(entries[i].message);
        if (length > max_length) {
            max_length = length;
            longest_index = i;
        }
    }
    
    if (longest_index >= 0) {
        printf("Longest message (%d chars): %s\n", max_length, entries[longest_index].message);
    }
}

void show_recent_logs(LogEntry* entries, int count) {
    int show_count = (count > 5) ? 5 : count;
    
    printf("Most recent %d logs:\n", show_count);
    for (int i = count - show_count; i < count; i++) {
        if (i >= 0) {
            printf("[%s] %s: %s\n", entries[i].timestamp, entries[i].level, entries[i].message);
        }
    }
}

int parse_log_line(const char* line, LogEntry* entry) {
    if (!line || !entry) return 0;
    
    char temp_line[MAX_LINE_LENGTH];
    if (strlen(line) >= MAX_LINE_LENGTH - 1) return 0;
    strcpy(temp_line, line);
    
    char* timestamp = strtok(temp_line, " ");
    char* level = strtok(NULL, " ");
    char* message = strtok(NULL, "\n");
    
    if (!timestamp || !level || !message) return 0;
    
    if (strlen(timestamp) >= 31) return 0;
    if (strlen(level) >= 15) return 0;
    if (strlen(message) >= 255) return 0;
    
    strcpy(entry->timestamp, timestamp);
    strcpy(entry->level, level);
    strcpy(entry->message, message);
    
    return 1;
}

int read_log_file(const char* filename, LogEntry* entries, int max_entries) {
    if (!filename || !entries || max_entries <= 0) return 0;
    
    FILE* file = fopen(filename, "r");
    if (!file) return 0;
    
    char line[MAX_LINE_LENGTH];
    int count = 0;
    
    while (fgets(line, sizeof(line), file) && count < max_entries) {
        if (strlen(line) > 0 && line[strlen(line) - 1] == '\n') {
            line[strlen(line) - 1] = '\0';
        }
        
        if (parse_log_line(line, &entries[count])) {
            count++;
        }
    }
    
    fclose(file);
    return count;
}

void process_logs(LogEntry* entries, int count, LogProcessor processor) {
    if (!entries || count <= 0 || !processor) return;
    processor(entries, count);
}

int main() {
    LogEntry entries[MAX_LOG_ENTRIES];
    LogProcessor processors[] = {
        count_log_levels,
        find_longest_message,
        show_recent_logs
    };
    const char* processor_names[] = {
        "Count log levels",
        "Find longest message",
        "Show recent logs"
    };
    int num_processors = sizeof(processors) / sizeof(processors[0]);
    
    printf("Enter log filename: ");
    char filename[256];
    if (!fgets(filename, sizeof(filename), stdin)) {
        printf("Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(filename);
    if (len > 0 && filename[len - 1] == '\n') {
        filename[len - 1] = '\0';
    }
    
    if (strlen(filename) == 0) {
        printf("Invalid filename\n");
        return 1;
    }
    
    int entry_count = read_log_file(filename, entries, MAX_LOG_ENTRIES);
    if (entry_count <= 0) {
        printf("No valid log entries found or file not found\n");
        return 1;
    }
    
    printf("Found %d log entries\n\n", entry_count);
    
    for (int i = 0; i < num_processors; i++) {
        printf