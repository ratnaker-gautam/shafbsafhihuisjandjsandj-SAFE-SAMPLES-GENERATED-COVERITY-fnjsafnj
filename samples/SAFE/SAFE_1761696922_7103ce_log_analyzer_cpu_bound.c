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
} LogEntry;

int parse_log_line(const char* line, LogEntry* entry) {
    if (line == NULL || entry == NULL) return 0;
    
    char temp[MAX_LINE_LENGTH];
    strncpy(temp, line, MAX_LINE_LENGTH - 1);
    temp[MAX_LINE_LENGTH - 1] = '\0';
    
    char* parts[3];
    char* token = strtok(temp, "|");
    int count = 0;
    
    while (token != NULL && count < 3) {
        parts[count] = token;
        count++;
        token = strtok(NULL, "|");
    }
    
    if (count != 3) return 0;
    
    strncpy(entry->timestamp, parts[0], 31);
    entry->timestamp[31] = '\0';
    strncpy(entry->level, parts[1], 15);
    entry->level[15] = '\0';
    strncpy(entry->message, parts[2], 255);
    entry->message[255] = '\0';
    
    return 1;
}

int count_log_level(const LogEntry* entries, int count, const char* level) {
    int total = 0;
    for (int i = 0; i < count; i++) {
        if (strcmp(entries[i].level, level) == 0) {
            total++;
        }
    }
    return total;
}

void generate_sample_logs(LogEntry* entries, int* count) {
    const char* levels[] = {"INFO", "WARNING", "ERROR", "DEBUG"};
    const char* messages[] = {
        "User login successful",
        "Database connection established",
        "File not found",
        "Memory allocation failed",
        "Network timeout",
        "Configuration loaded",
        "Invalid input detected",
        "Backup completed",
        "Security alert triggered",
        "Service started successfully"
    };
    
    time_t now = time(NULL);
    *count = 0;
    
    for (int i = 0; i < 500 && *count < MAX_ENTRIES; i++) {
        LogEntry entry;
        time_t offset = now - (rand() % 86400);
        struct tm* tm_info = localtime(&offset);
        
        strftime(entry.timestamp, 31, "%Y-%m-%d %H:%M:%S", tm_info);
        strncpy(entry.level, levels[rand() % 4], 15);
        entry.level[15] = '\0';
        strncpy(entry.message, messages[rand() % 10], 255);
        entry.message[255] = '\0';
        
        entries[*count] = entry;
        (*count)++;
    }
}

int main(void) {
    LogEntry entries[MAX_ENTRIES];
    int entry_count = 0;
    
    srand(42);
    generate_sample_logs(entries, &entry_count);
    
    printf("Log Analysis Results:\n");
    printf("Total log entries: %d\n", entry_count);
    printf("INFO entries: %d\n", count_log_level(entries, entry_count, "INFO"));
    printf("WARNING entries: %d\n", count_log_level(entries, entry_count, "WARNING"));
    printf("ERROR entries: %d\n", count_log_level(entries, entry_count, "ERROR"));
    printf("DEBUG entries: %d\n", count_log_level(entries, entry_count, "DEBUG"));
    
    printf("\nSample entries:\n");
    for (int i = 0; i < 5 && i < entry_count; i++) {
        printf("%s | %s | %s\n", entries[i].timestamp, entries[i].level, entries[i].message);
    }
    
    int error_count = count_log_level(entries, entry_count, "ERROR");
    if (error_count > 10) {
        printf("\nHigh error count detected: %d errors\n", error_count);
    }
    
    return 0;
}