//DeepSeek-V3 V2.5 Category: Safe ; Style: function_pointers ; Variation: log_analyzer
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
    if (entry != NULL && strcmp(entry->level, "ERROR") == 0) {
        error_count++;
        printf("Error count: %d\n", error_count);
    }
}

void print_warnings(LogEntry* entry) {
    if (entry != NULL && strcmp(entry->level, "WARNING") == 0) {
        printf("Warning: %s - %s\n", entry->timestamp, entry->message);
    }
}

void process_timestamps(LogEntry* entry) {
    if (entry == NULL) return;
    
    static time_t first_time = 0;
    static time_t last_time = 0;
    
    struct tm tm = {0};
    if (strptime(entry->timestamp, "%Y-%m-%d %H:%M:%S", &tm) != NULL) {
        time_t current = mktime(&tm);
        if (first_time == 0) {
            first_time = current;
        }
        last_time = current;
        
        if (last_time > first_time) {
            double diff = difftime(last_time, first_time);
            printf("Time span: %.0f seconds\n", diff);
        }
    }
}

int validate_log_entry(LogEntry* entry) {
    if (entry == NULL) return 0;
    
    if (strlen(entry->timestamp) == 0 || strlen(entry->level) == 0 || strlen(entry->message) == 0) {
        return 0;
    }
    
    if (strlen(entry->timestamp) >= 31 || strlen(entry->level) >= 15 || strlen(entry->message) >= MAX_LINE_LEN - 1) {
        return 0;
    }
    
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

int parse_log_line(const char* line, LogEntry* entry) {
    if (line == NULL || entry == NULL) {
        return 0;
    }
    
    size_t len = strlen(line);
    if (len >= MAX_LINE_LEN || len == 0) {
        return 0;
    }
    
    char buffer[MAX_LINE_LEN];
    strncpy(buffer, line, MAX_LINE_LEN - 1);
    buffer[MAX_LINE_LEN - 1] = '\0';
    
    char* timestamp = strtok(buffer, " ");
    char* level = strtok(NULL, " ");
    char* message = strtok(NULL, "\n");
    
    if (timestamp == NULL || level == NULL || message == NULL) {
        return 0;
    }
    
    strncpy(entry->timestamp, timestamp, 31);
    entry->timestamp[31] = '\0';
    strncpy(entry->level, level, 15);
    entry->level[15] = '\0';
    strncpy(entry->message, message, MAX_LINE_LEN - 1);
    entry->message[MAX_LINE_LEN - 1] = '\0';
    
    return validate_log_entry(entry);
}

void process_logs(LogEntry entries[], int count, LogProcessor processors[], int proc_count) {
    if (entries == NULL || processors == NULL || count <= 0 || proc_count <= 0) {
        return;
    }
    
    if (count > MAX_ENTRIES) {
        count = MAX_ENTRIES;
    }
    
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
    printf("Enter 'END' on a separate line to finish.\n");
    
    char line[MAX_LINE_LEN];
    while (entry_count < MAX_ENTRIES) {
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
            entries[entry_count] = entry;
            entry_count++;
        } else {
            printf("Invalid log format: %s\n", line);
        }
    }
    
    if (entry_count == 0) {
        printf("No valid log entries to process.\n");
        return 1;
    }
    
    LogProcessor processors[]