//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: cpu_bound ; Variation: log_analyzer
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
    if (!line || !entry) return 0;
    
    char temp[MAX_LINE_LENGTH];
    if (strlen(line) >= MAX_LINE_LENGTH - 1) return 0;
    strcpy(temp, line);
    
    char* timestamp = strtok(temp, " ");
    char* level = strtok(NULL, " ");
    char* message = strtok(NULL, "\n");
    
    if (!timestamp || !level || !message) return 0;
    
    if (strlen(timestamp) >= 32) return 0;
    if (strlen(level) >= 16) return 0;
    if (strlen(message) >= 256) return 0;
    
    strcpy(entry->timestamp, timestamp);
    strcpy(entry->level, level);
    strcpy(entry->message, message);
    
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
        "System started successfully",
        "User login attempted",
        "Database connection established",
        "Memory usage high",
        "Network timeout occurred",
        "File not found",
        "Configuration loaded",
        "Backup completed"
    };
    
    time_t now = time(NULL);
    struct tm* tm_info = localtime(&now);
    
    for (int i = 0; i < 500; i++) {
        if (*count >= MAX_ENTRIES) break;
        
        LogEntry* entry = &entries[*count];
        
        tm_info->tm_sec = (tm_info->tm_sec + 1) % 60;
        strftime(entry->timestamp, 32, "%Y-%m-%d %H:%M:%S", tm_info);
        
        int level_idx = rand() % 4;
        strcpy(entry->level, levels[level_idx]);
        
        int msg_idx = rand() % 8;
        strcpy(entry->message, messages[msg_idx]);
        
        (*count)++;
    }
}

int main() {
    LogEntry entries[MAX_ENTRIES];
    int entry_count = 0;
    
    srand((unsigned int)time(NULL));
    
    printf("Generating sample log data...\n");
    generate_sample_logs(entries, &entry_count);
    
    printf("Analyzing %d log entries...\n\n", entry_count);
    
    int info_count = count_log_level(entries, entry_count, "INFO");
    int warning_count = count_log_level(entries, entry_count, "WARNING");
    int error_count = count_log_level(entries, entry_count, "ERROR");
    int debug_count = count_log_level(entries, entry_count, "DEBUG");
    
    printf("Log Level Statistics:\n");
    printf("INFO:    %d entries\n", info_count);
    printf("WARNING: %d entries\n", warning_count);
    printf("ERROR:   %d entries\n", error_count);
    printf("DEBUG:   %d entries\n", debug_count);
    
    printf("\nPercentage Distribution:\n");
    printf("INFO:    %.1f%%\n", (info_count * 100.0) / entry_count);
    printf("WARNING: %.1f%%\n", (warning_count * 100.0) / entry_count);
    printf("ERROR:   %.1f%%\n", (error_count * 100.0) / entry_count);
    printf("DEBUG:   %.1f%%\n", (debug_count * 100.0) / entry_count);
    
    int error_entries = 0;
    printf("\nRecent ERROR entries:\n");
    for (int i = entry_count - 1; i >= 0 && error_entries < 5; i--) {
        if (strcmp(entries[i].level, "ERROR") == 0) {
            printf("%s %s %s\n", entries[i].timestamp, entries[i].level, entries[i].message);
            error_entries++;
        }
    }
    
    if (error_entries == 0) {
        printf("No ERROR entries found.\n");
    }
    
    return 0;
}