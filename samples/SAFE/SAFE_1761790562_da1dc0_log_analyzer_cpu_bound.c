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
        "Backup completed"
    };
    
    time_t now = time(NULL);
    *count = 5000;
    
    for (int i = 0; i < *count; i++) {
        time_t offset = now - (rand() % 86400);
        struct tm* tm_info = localtime(&offset);
        strftime(entries[i].timestamp, 32, "%Y-%m-%d %H:%M:%S", tm_info);
        
        int level_idx = rand() % 4;
        strcpy(entries[i].level, levels[level_idx]);
        
        int msg_idx = rand() % 8;
        strcpy(entries[i].message, messages[msg_idx]);
    }
}

int main() {
    LogEntry entries[MAX_ENTRIES];
    int entry_count = 0;
    
    printf("Log Analyzer - Processing synthetic log data...\n");
    
    generate_sample_logs(entries, &entry_count);
    
    if (entry_count == 0) {
        printf("No log entries to process.\n");
        return 1;
    }
    
    printf("Processed %d log entries.\n", entry_count);
    
    int info_count = count_log_level(entries, entry_count, "INFO");
    int warning_count = count_log_level(entries, entry_count, "WARNING");
    int error_count = count_log_level(entries, entry_count, "ERROR");
    int debug_count = count_log_level(entries, entry_count, "DEBUG");
    
    printf("\nLog Level Statistics:\n");
    printf("INFO: %d entries (%.1f%%)\n", info_count, (info_count * 100.0) / entry_count);
    printf("WARNING: %d entries (%.1f%%)\n", warning_count, (warning_count * 100.0) / entry_count);
    printf("ERROR: %d entries (%.1f%%)\n", error_count, (error_count * 100.0) / entry_count);
    printf("DEBUG: %d entries (%.1f%%)\n", debug_count, (debug_count * 100.0) / entry_count);
    
    int error_entries = 0;
    printf("\nRecent ERROR entries:\n");
    for (int i = entry_count - 1; i >= 0 && error_entries < 5; i--) {
        if (strcmp(entries[i].level, "ERROR") == 0) {
            printf("%s | %s | %s\n", entries[i].timestamp, entries[i].level, entries[i].message);
            error_entries++;
        }
    }
    
    if (error_entries == 0) {
        printf("No ERROR entries found.\n");
    }
    
    return 0;
}