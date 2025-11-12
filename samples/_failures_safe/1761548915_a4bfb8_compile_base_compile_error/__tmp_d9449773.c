//DeepSeek-V3 V2.5 Category: Safe ; Style: recursion ; Variation: log_analyzer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>

#define MAX_LINE_LEN 1024
#define MAX_ENTRIES 1000

typedef struct {
    char timestamp[32];
    char level[16];
    char message[MAX_LINE_LEN];
} LogEntry;

int parse_log_line(const char* line, LogEntry* entry) {
    if (!line || !entry) return 0;
    
    char temp[MAX_LINE_LEN];
    strncpy(temp, line, MAX_LINE_LEN - 1);
    temp[MAX_LINE_LEN - 1] = '\0';
    
    char* timestamp = strtok(temp, " ");
    char* level = strtok(NULL, " ");
    char* message = strtok(NULL, "\n");
    
    if (!timestamp || !level || !message) return 0;
    
    strncpy(entry->timestamp, timestamp, sizeof(entry->timestamp) - 1);
    entry->timestamp[sizeof(entry->timestamp) - 1] = '\0';
    
    strncpy(entry->level, level, sizeof(entry->level) - 1);
    entry->level[sizeof(entry->level) - 1] = '\0';
    
    strncpy(entry->message, message, sizeof(entry->message) - 1);
    entry->message[sizeof(entry->message) - 1] = '\0';
    
    return 1;
}

void analyze_logs_recursive(LogEntry* logs, int count, int index, int* error_count, int* warning_count, int* info_count) {
    if (index >= count) return;
    
    if (strcmp(logs[index].level, "ERROR") == 0) {
        (*error_count)++;
    } else if (strcmp(logs[index].level, "WARNING") == 0) {
        (*warning_count)++;
    } else if (strcmp(logs[index].level, "INFO") == 0) {
        (*info_count)++;
    }
    
    analyze_logs_recursive(logs, count, index + 1, error_count, warning_count, info_count);
}

void display_log_summary(int error_count, int warning_count, int info_count, int total_count) {
    printf("\n=== Log Analysis Summary ===\n");
    printf("Total entries: %d\n", total_count);
    printf("ERROR entries: %d (%.1f%%)\n", error_count, total_count > 0 ? (error_count * 100.0) / total_count : 0.0);
    printf("WARNING entries: %d (%.1f%%)\n", warning_count, total_count > 0 ? (warning_count * 100.0) / total_count : 0.0);
    printf("INFO entries: %d (%.1f%%)\n", info_count, total_count > 0 ? (info_count * 100.0) / total_count : 0.0);
}

int main() {
    LogEntry logs[MAX_ENTRIES];
    int log_count = 0;
    char line[MAX_LINE_LEN];
    
    printf("Log Analyzer - Enter log entries (format: TIMESTAMP LEVEL MESSAGE)\n");
    printf("Supported levels: ERROR, WARNING, INFO\n");
    printf("Enter 'END' on a new line to finish input\n");
    printf("Maximum %d entries allowed\n\n", MAX_ENTRIES);
    
    while (log_count < MAX_ENTRIES) {
        printf("Log entry %d: ", log_count + 1);
        
        if (fgets(line, sizeof(line), stdin) == NULL) {
            break;
        }
        
        line[strcspn(line, "\n")] = '\0';
        
        if (strcmp(line, "END") == 0) {
            break;
        }
        
        if (strlen(line) == 0) {
            continue;
        }
        
        LogEntry entry;
        if (parse_log_line(line, &entry)) {
            logs[log_count] = entry;
            log_count++;
        } else {
            printf("Invalid log format. Use: YYYY-MM-DD HH:MM:SS LEVEL MESSAGE\n");
        }
    }
    
    if (log_count == 0) {
        printf("No valid log entries provided.\n");
        return 1;
    }
    
    int error_count = 0;
    int warning_count = 0;
    int info_count = 0;
    
    analyze_logs_recursive(logs, log_count, 0, &error_count, &warning_count, &info_count);
    display_log_summary(error_count, warning_count, info_count, log_count);
    
    printf("\nSample entries by level:\n");
    
    int found_error = 0;
    int found_warning = 0;
    int found_info = 0;
    
    for (int i = 0; i < log_count && (!found_error || !found_warning || !found_info); i++) {
        if (!found_error && strcmp(logs[i].level, "ERROR") == 0) {
            printf("ERROR sample: %s - %s\n", logs[i].timestamp, logs[i].message);
            found_error = 1;
        } else if (!found_warning && strcmp(logs[i].level, "WARNING") == 0) {
            printf("WARNING sample: %s - %s\n", logs[i].timestamp, logs[i].message);
            found_warning =