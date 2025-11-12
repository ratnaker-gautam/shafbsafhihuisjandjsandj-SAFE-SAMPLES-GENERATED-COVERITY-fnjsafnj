//DeepSeek-V3 V2.5 Category: Safe ; Style: cpu_bound ; Variation: log_analyzer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>

#define MAX_LINE_LENGTH 1024
#define MAX_ENTRIES 10000

typedef struct {
    char timestamp[32];
    char level[16];
    char message[256];
    int error_count;
} LogEntry;

int parse_log_line(const char* line, LogEntry* entry) {
    if (line == NULL || entry == NULL) return 0;
    
    char buffer[MAX_LINE_LENGTH];
    strncpy(buffer, line, MAX_LINE_LENGTH - 1);
    buffer[MAX_LINE_LENGTH - 1] = '\0';
    
    char* timestamp = strtok(buffer, " ");
    char* level = strtok(NULL, " ");
    char* message = strtok(NULL, "\n");
    
    if (timestamp == NULL || level == NULL || message == NULL) return 0;
    
    if (strlen(timestamp) >= sizeof(entry->timestamp) ||
        strlen(level) >= sizeof(entry->level) ||
        strlen(message) >= sizeof(entry->message)) return 0;
    
    strcpy(entry->timestamp, timestamp);
    strcpy(entry->level, level);
    strcpy(entry->message, message);
    
    entry->error_count = (strcmp(level, "ERROR") == 0) ? 1 : 0;
    
    return 1;
}

int compare_entries(const void* a, const void* b) {
    const LogEntry* entry_a = (const LogEntry*)a;
    const LogEntry* entry_b = (const LogEntry*)b;
    return strcmp(entry_a->timestamp, entry_b->timestamp);
}

void generate_sample_logs(LogEntry* entries, int count) {
    const char* levels[] = {"INFO", "WARNING", "ERROR"};
    const char* messages[] = {
        "System started successfully",
        "User login detected",
        "Database connection established",
        "Memory usage high",
        "Network timeout occurred",
        "File not found",
        "Permission denied",
        "Configuration updated"
    };
    
    time_t base_time = time(NULL);
    
    for (int i = 0; i < count; i++) {
        time_t offset = base_time - (count - i) * 60;
        struct tm* timeinfo = localtime(&offset);
        
        strftime(entries[i].timestamp, sizeof(entries[i].timestamp), 
                "%Y-%m-%d %H:%M:%S", timeinfo);
        
        int level_idx = rand() % 3;
        int msg_idx = rand() % 8;
        
        strcpy(entries[i].level, levels[level_idx]);
        strcpy(entries[i].message, messages[msg_idx]);
        entries[i].error_count = (level_idx == 2) ? 1 : 0;
    }
}

int main(void) {
    LogEntry entries[MAX_ENTRIES];
    int entry_count = 5000;
    
    srand(42);
    generate_sample_logs(entries, entry_count);
    
    qsort(entries, entry_count, sizeof(LogEntry), compare_entries);
    
    int total_errors = 0;
    int info_count = 0;
    int warning_count = 0;
    int error_count = 0;
    
    for (int i = 0; i < entry_count; i++) {
        if (strcmp(entries[i].level, "INFO") == 0) info_count++;
        else if (strcmp(entries[i].level, "WARNING") == 0) warning_count++;
        else if (strcmp(entries[i].level, "ERROR") == 0) error_count++;
        
        total_errors += entries[i].error_count;
    }
    
    printf("Log Analysis Results:\n");
    printf("Total entries processed: %d\n", entry_count);
    printf("INFO entries: %d\n", info_count);
    printf("WARNING entries: %d\n", warning_count);
    printf("ERROR entries: %d\n", error_count);
    printf("Total errors: %d\n", total_errors);
    
    printf("\nFirst 5 entries:\n");
    for (int i = 0; i < 5 && i < entry_count; i++) {
        printf("%s [%s] %s\n", entries[i].timestamp, entries[i].level, entries[i].message);
    }
    
    printf("\nLast 5 entries:\n");
    for (int i = entry_count - 5; i < entry_count && i >= 0; i++) {
        printf("%s [%s] %s\n", entries[i].timestamp, entries[i].level, entries[i].message);
    }
    
    return 0;
}