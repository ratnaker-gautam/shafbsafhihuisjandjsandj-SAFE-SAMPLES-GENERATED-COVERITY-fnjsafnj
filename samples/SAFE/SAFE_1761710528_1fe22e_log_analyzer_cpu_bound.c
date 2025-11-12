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
    int part_count = 0;
    char* token = strtok(temp, "|");
    
    while (token != NULL && part_count < 3) {
        parts[part_count++] = token;
        token = strtok(NULL, "|");
    }
    
    if (part_count != 3) return 0;
    
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
        "File upload completed",
        "Memory usage high",
        "Network timeout occurred",
        "Configuration loaded",
        "Backup process started",
        "Security check passed"
    };
    
    *count = 5000;
    for (int i = 0; i < *count; i++) {
        snprintf(entries[i].timestamp, 31, "2024-01-%02d %02d:%02d:%02d", 
                 (i % 30) + 1, (i % 24), (i % 60), (i % 60));
        strncpy(entries[i].level, levels[i % 4], 15);
        entries[i].level[15] = '\0';
        strncpy(entries[i].message, messages[i % 8], 255);
        entries[i].message[255] = '\0';
    }
}

int main(void) {
    LogEntry entries[MAX_ENTRIES];
    int entry_count = 0;
    
    printf("Generating sample log data...\n");
    generate_sample_logs(entries, &entry_count);
    
    if (entry_count == 0) {
        printf("No log entries to analyze.\n");
        return 1;
    }
    
    printf("Analyzing %d log entries...\n", entry_count);
    
    int info_count = count_log_level(entries, entry_count, "INFO");
    int warning_count = count_log_level(entries, entry_count, "WARNING");
    int error_count = count_log_level(entries, entry_count, "ERROR");
    int debug_count = count_log_level(entries, entry_count, "DEBUG");
    
    printf("\nLog Level Distribution:\n");
    printf("INFO: %d entries (%.1f%%)\n", info_count, (info_count * 100.0) / entry_count);
    printf("WARNING: %d entries (%.1f%%)\n", warning_count, (warning_count * 100.0) / entry_count);
    printf("ERROR: %d entries (%.1f%%)\n", error_count, (error_count * 100.0) / entry_count);
    printf("DEBUG: %d entries (%.1f%%)\n", debug_count, (debug_count * 100.0) / entry_count);
    
    int error_entries = 0;
    for (int i = 0; i < entry_count; i++) {
        if (strcmp(entries[i].level, "ERROR") == 0) {
            error_entries++;
            if (error_entries <= 5) {
                printf("Error %d: %s - %s\n", error_entries, entries[i].timestamp, entries[i].message);
            }
        }
    }
    
    if (error_entries > 5) {
        printf("... and %d more error entries\n", error_entries - 5);
    }
    
    printf("\nAnalysis complete.\n");
    return 0;
}