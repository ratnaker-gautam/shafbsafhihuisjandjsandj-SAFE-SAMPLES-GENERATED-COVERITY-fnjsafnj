//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: function_pointers ; Variation: log_analyzer
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

typedef void (*LogProcessor)(LogEntry*);

void count_errors(LogEntry* entry) {
    static int error_count = 0;
    if (strcmp(entry->level, "ERROR") == 0) {
        error_count++;
        printf("Error count: %d\n", error_count);
    }
}

void print_warnings(LogEntry* entry) {
    if (strcmp(entry->level, "WARNING") == 0) {
        printf("Warning: %s - %s\n", entry->timestamp, entry->message);
    }
}

void process_timestamps(LogEntry* entry) {
    static time_t first_time = 0;
    static time_t last_time = 0;
    
    struct tm tm = {0};
    if (strptime(entry->timestamp, "%Y-%m-%d %H:%M:%S", &tm) != NULL) {
        time_t current = mktime(&tm);
        if (current != -1) {
            if (first_time == 0) first_time = current;
            last_time = current;
            
            if (first_time != 0 && last_time != 0) {
                double duration = difftime(last_time, first_time);
                printf("Log duration: %.0f seconds\n", duration);
            }
        }
    }
}

int parse_log_line(const char* line, LogEntry* entry) {
    if (line == NULL || entry == NULL) return 0;
    
    size_t len = strlen(line);
    if (len == 0 || len >= MAX_LINE_LEN) return 0;
    
    char temp[MAX_LINE_LEN];
    strncpy(temp, line, sizeof(temp) - 1);
    temp[sizeof(temp) - 1] = '\0';
    
    char* timestamp = strtok(temp, " ");
    char* level = strtok(NULL, " ");
    char* message = strtok(NULL, "\n");
    
    if (timestamp == NULL || level == NULL || message == NULL) return 0;
    
    if (strlen(timestamp) >= sizeof(entry->timestamp)) return 0;
    if (strlen(level) >= sizeof(entry->level)) return 0;
    if (strlen(message) >= sizeof(entry->message)) return 0;
    
    strcpy(entry->timestamp, timestamp);
    strcpy(entry->level, level);
    strcpy(entry->message, message);
    
    return 1;
}

void process_logs(LogEntry* entries, int count, LogProcessor* processors, int proc_count) {
    if (entries == NULL || processors == NULL || count <= 0 || proc_count <= 0) return;
    
    for (int i = 0; i < count; i++) {
        for (int j = 0; j < proc_count; j++) {
            processors[j](&entries[i]);
        }
    }
}

int main() {
    LogEntry entries[MAX_ENTRIES];
    int entry_count = 0;
    
    printf("Enter log entries (format: YYYY-MM-DD HH:MM:SS LEVEL message):\n");
    printf("Enter 'END' on a separate line to finish input.\n");
    
    char line[MAX_LINE_LEN];
    while (entry_count < MAX_ENTRIES) {
        if (fgets(line, sizeof(line), stdin) == NULL) break;
        
        line[strcspn(line, "\n")] = '\0';
        
        if (strcmp(line, "END") == 0) break;
        
        if (strlen(line) == 0) continue;
        
        if (parse_log_line(line, &entries[entry_count])) {
            entry_count++;
        } else {
            printf("Invalid log format. Skipping: %s\n", line);
        }
    }
    
    if (entry_count == 0) {
        printf("No valid log entries to process.\n");
        return 1;
    }
    
    LogProcessor processors[] = {count_errors, print_warnings, process_timestamps};
    int proc_count = sizeof(processors) / sizeof(processors[0]);
    
    printf("\nProcessing logs...\n");
    process_logs(entries, entry_count, processors, proc_count);
    
    printf("\nTotal entries processed: %d\n", entry_count);
    
    return 0;
}