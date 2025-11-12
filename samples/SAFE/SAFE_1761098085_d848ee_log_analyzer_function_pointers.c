//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: function_pointers ; Variation: log_analyzer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>

#define MAX_LINE_LENGTH 1024
#define MAX_ENTRIES 1000

typedef struct {
    char timestamp[20];
    char level[10];
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
    
    printf("INFO: %d\nWARN: %d\nERROR: %d\n", info_count, warn_count, error_count);
}

void find_errors(LogEntry* entries, int count) {
    printf("Error entries:\n");
    for (int i = 0; i < count; i++) {
        if (strcmp(entries[i].level, "ERROR") == 0) {
            printf("%s - %s\n", entries[i].timestamp, entries[i].message);
        }
    }
}

void show_recent(LogEntry* entries, int count) {
    int show_count = (count > 5) ? 5 : count;
    printf("Most recent %d entries:\n", show_count);
    
    for (int i = count - show_count; i < count; i++) {
        if (i >= 0) {
            printf("%s [%s] %s\n", entries[i].timestamp, entries[i].level, entries[i].message);
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
    
    if (strlen(timestamp) >= 19 || strlen(level) >= 9 || strlen(message) >= 255) return 0;
    
    strcpy(entry->timestamp, timestamp);
    strcpy(entry->level, level);
    strcpy(entry->message, message);
    
    return 1;
}

int read_log_file(LogEntry* entries, int max_entries) {
    char line[MAX_LINE_LENGTH];
    int count = 0;
    
    printf("Enter log entries (format: TIMESTAMP LEVEL MESSAGE):\n");
    printf("Example: 2024-01-15T10:30:00 INFO System started\n");
    printf("Enter 'END' on a separate line to finish input.\n");
    
    while (count < max_entries && fgets(line, sizeof(line), stdin)) {
        if (strncmp(line, "END", 3) == 0 && (line[3] == '\n' || line[3] == '\0')) {
            break;
        }
        
        line[strcspn(line, "\n")] = 0;
        
        if (strlen(line) == 0) continue;
        
        if (parse_log_line(line, &entries[count])) {
            count++;
        } else {
            printf("Invalid log format: %s\n", line);
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
    LogProcessor processors[] = {count_by_level, find_errors, show_recent};
    const char* processor_names[] = {"Count by level", "Find errors", "Show recent"};
    int processor_count = 3;
    
    int entry_count = read_log_file(entries, MAX_ENTRIES);
    
    if (entry_count == 0) {
        printf("No valid log entries read.\n");
        return 1;
    }
    
    printf("\nRead %d log entries.\n", entry_count);
    printf("\nAvailable analysis options:\n");
    
    for (int i = 0; i < processor_count; i++) {
        printf("%d. %s\n", i + 1, processor_names[i]);
    }
    
    printf("Enter choice (1-%d): ", processor_count);
    
    int choice;
    if (scanf("%d", &choice) != 1) {
        printf("Invalid input.\n");
        return 1;
    }
    
    if (choice < 1 || choice > processor_count) {
        printf("Invalid choice.\n");
        return 1;
    }
    
    printf("\n");
    process_logs(entries, entry_count, processors[choice - 1]);
    
    return 0;
}