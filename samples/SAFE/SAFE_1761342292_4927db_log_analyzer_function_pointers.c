//DeepSeek-V3 V2.5 Category: Safe ; Style: function_pointers ; Variation: log_analyzer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>

#define MAX_LINE_LENGTH 1024
#define MAX_ENTRIES 1000

typedef struct {
    char timestamp[32];
    char level[16];
    char message[256];
} LogEntry;

typedef void (*LogProcessor)(LogEntry*);

void count_entries(LogEntry* entry) {
    static int count = 0;
    count++;
    printf("Processed %d entries\r", count);
    fflush(stdout);
}

void find_errors(LogEntry* entry) {
    static int error_count = 0;
    if (strstr(entry->level, "ERROR") != NULL) {
        error_count++;
        printf("Error #%d: %s - %s\n", error_count, entry->timestamp, entry->message);
    }
}

void show_warnings(LogEntry* entry) {
    static int warning_count = 0;
    if (strstr(entry->level, "WARN") != NULL) {
        warning_count++;
        printf("Warning #%d: %s - %s\n", warning_count, entry->timestamp, entry->message);
    }
}

void analyze_timestamps(LogEntry* entry) {
    static time_t earliest = 0;
    static time_t latest = 0;
    static int first = 1;
    
    struct tm tm = {0};
    if (strptime(entry->timestamp, "%Y-%m-%d %H:%M:%S", &tm) != NULL) {
        time_t current = mktime(&tm);
        if (current != -1) {
            if (first) {
                earliest = current;
                latest = current;
                first = 0;
            } else {
                if (current < earliest) earliest = current;
                if (current > latest) latest = current;
            }
        }
    }
    
    if (!first) {
        double span = difftime(latest, earliest);
        printf("Time span: %.0f seconds\r", span);
        fflush(stdout);
    }
}

int parse_log_line(const char* line, LogEntry* entry) {
    if (line == NULL || entry == NULL) return 0;
    
    char buffer[MAX_LINE_LENGTH];
    if (strlen(line) >= MAX_LINE_LENGTH - 1) return 0;
    strncpy(buffer, line, MAX_LINE_LENGTH - 1);
    buffer[MAX_LINE_LENGTH - 1] = '\0';
    
    char* timestamp = strtok(buffer, " ");
    char* level = strtok(NULL, " ");
    char* message = strtok(NULL, "\n");
    
    if (timestamp == NULL || level == NULL || message == NULL) return 0;
    
    if (strlen(timestamp) >= sizeof(entry->timestamp)) return 0;
    if (strlen(level) >= sizeof(entry->level)) return 0;
    if (strlen(message) >= sizeof(entry->message)) return 0;
    
    strncpy(entry->timestamp, timestamp, sizeof(entry->timestamp) - 1);
    strncpy(entry->level, level, sizeof(entry->level) - 1);
    strncpy(entry->message, message, sizeof(entry->message) - 1);
    
    entry->timestamp[sizeof(entry->timestamp) - 1] = '\0';
    entry->level[sizeof(entry->level) - 1] = '\0';
    entry->message[sizeof(entry->message) - 1] = '\0';
    
    return 1;
}

void process_logs(LogProcessor processor) {
    if (processor == NULL) return;
    
    char line[MAX_LINE_LENGTH];
    LogEntry entry;
    
    while (fgets(line, sizeof(line), stdin) != NULL) {
        if (line[0] == '\n') continue;
        
        line[strcspn(line, "\n")] = '\0';
        
        if (parse_log_line(line, &entry)) {
            processor(&entry);
        }
    }
}

int main() {
    printf("Log Analyzer - Choose analysis type:\n");
    printf("1. Count entries\n");
    printf("2. Find errors\n");
    printf("3. Show warnings\n");
    printf("4. Analyze timestamps\n");
    printf("Enter choice (1-4): ");
    
    char input[16];
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    input[strcspn(input, "\n")] = '\0';
    
    if (strlen(input) != 1 || input[0] < '1' || input[0] > '4') {
        fprintf(stderr, "Invalid choice\n");
        return 1;
    }
    
    LogProcessor processors[] = {
        count_entries,
        find_errors,
        show_warnings,
        analyze_timestamps
    };
    
    int choice = input[0] - '1';
    if (choice < 0 || choice > 3) {
        fprintf(stderr, "Invalid processor selection\n");
        return 1;
    }
    
    printf("Processing log entries...\n");
    printf("Enter log data (Ctrl+D to end):\n");
    
    process_logs(processors[choice]);
    
    printf("\nAnalysis complete.\n");
    return 0;
}