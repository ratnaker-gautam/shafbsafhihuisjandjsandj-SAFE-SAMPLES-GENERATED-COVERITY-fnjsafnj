//DeepSeek-V3 V2.5 Category: Safe ; Style: function_pointers ; Variation: log_analyzer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdint.h>

#define MAX_LINE_LENGTH 1024
#define MAX_ENTRIES 1000

typedef struct {
    char timestamp[64];
    char level[16];
    char message[256];
} LogEntry;

typedef void (*LogProcessor)(LogEntry*, int);

void count_by_level(LogEntry* entries, int count) {
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

void find_errors(LogEntry* entries, int count) {
    printf("Error Logs:\n");
    
    for (int i = 0; i < count; i++) {
        if (strcmp(entries[i].level, "ERROR") == 0) {
            printf("[%s] %s: %s\n", entries[i].timestamp, entries[i].level, entries[i].message);
        }
    }
}

void display_all(LogEntry* entries, int count) {
    printf("All Log Entries:\n");
    
    for (int i = 0; i < count; i++) {
        printf("[%s] %s: %s\n", entries[i].timestamp, entries[i].level, entries[i].message);
    }
}

int parse_log_line(const char* line, LogEntry* entry) {
    char temp_line[MAX_LINE_LENGTH];
    
    if (strlen(line) >= MAX_LINE_LENGTH - 1) {
        return 0;
    }
    
    strncpy(temp_line, line, MAX_LINE_LENGTH - 1);
    temp_line[MAX_LINE_LENGTH - 1] = '\0';
    
    char* timestamp_start = strchr(temp_line, '[');
    if (!timestamp_start) return 0;
    
    char* timestamp_end = strchr(timestamp_start, ']');
    if (!timestamp_end) return 0;
    
    size_t timestamp_len = timestamp_end - timestamp_start - 1;
    if (timestamp_len >= sizeof(entry->timestamp)) {
        return 0;
    }
    
    strncpy(entry->timestamp, timestamp_start + 1, timestamp_len);
    entry->timestamp[timestamp_len] = '\0';
    
    char* level_start = timestamp_end + 1;
    while (*level_start && isspace(*level_start)) level_start++;
    
    char* level_end = strchr(level_start, ':');
    if (!level_end) return 0;
    
    size_t level_len = level_end - level_start;
    if (level_len >= sizeof(entry->level)) {
        return 0;
    }
    
    strncpy(entry->level, level_start, level_len);
    entry->level[level_len] = '\0';
    
    char* message_start = level_end + 1;
    while (*message_start && isspace(*message_start)) message_start++;
    
    size_t message_len = strlen(message_start);
    if (message_len >= sizeof(entry->message)) {
        return 0;
    }
    
    strncpy(entry->message, message_start, sizeof(entry->message) - 1);
    entry->message[sizeof(entry->message) - 1] = '\0';
    
    return 1;
}

int read_log_file(const char* filename, LogEntry* entries, int max_entries) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        printf("Error: Cannot open file %s\n", filename);
        return 0;
    }
    
    char line[MAX_LINE_LENGTH];
    int count = 0;
    
    while (fgets(line, sizeof(line), file) && count < max_entries) {
        line[strcspn(line, "\n")] = '\0';
        
        if (strlen(line) > 0) {
            if (parse_log_line(line, &entries[count])) {
                count++;
            }
        }
    }
    
    fclose(file);
    return count;
}

void show_menu(void) {
    printf("\nLog Analyzer Menu:\n");
    printf("1. Display all logs\n");
    printf("2. Count logs by level\n");
    printf("3. Show error logs only\n");
    printf("4. Exit\n");
    printf("Choose an option: ");
}

int main(void) {
    LogEntry entries[MAX_ENTRIES];
    LogProcessor processors[] = {display_all, count_by_level, find_errors};
    
    printf("Enter log filename: ");
    char filename[256];
    
    if (fgets(filename, sizeof(filename), stdin) == NULL) {
        printf("Error reading input\n");
        return 1;
    }
    
    filename[strcspn(filename, "\n")] = '\0';
    
    if (strlen(filename) == 0) {
        printf("Error: Empty filename\n");
        return 1;
    }