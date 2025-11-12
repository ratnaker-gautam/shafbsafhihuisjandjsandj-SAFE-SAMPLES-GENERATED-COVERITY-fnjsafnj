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
    if (!line || !entry) return 0;
    
    char temp_line[MAX_LINE_LENGTH];
    strncpy(temp_line, line, MAX_LINE_LENGTH - 1);
    temp_line[MAX_LINE_LENGTH - 1] = '\0';
    
    char* timestamp = strtok(temp_line, " ");
    if (!timestamp) return 0;
    
    char* level = strtok(NULL, " ");
    if (!level) return 0;
    
    char* message = strtok(NULL, "\n");
    if (!message) return 0;
    
    if (strlen(timestamp) >= sizeof(entry->timestamp) ||
        strlen(level) >= sizeof(entry->level) ||
        strlen(message) >= sizeof(entry->message)) {
        return 0;
    }
    
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
    const char* levels[] = {"INFO", "WARN", "ERROR", "DEBUG"};
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
        "Service started"
    };
    
    time_t base_time = time(NULL);
    *count = 0;
    
    for (int i = 0; i < 500 && *count < MAX_ENTRIES; i++) {
        LogEntry entry;
        time_t log_time = base_time - (rand() % 86400);
        struct tm* tm_info = localtime(&log_time);
        
        strftime(entry.timestamp, sizeof(entry.timestamp), 
                "%Y-%m-%d %H:%M:%S", tm_info);
        
        strcpy(entry.level, levels[rand() % 4]);
        strcpy(entry.message, messages[rand() % 10]);
        
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
    
    const char* levels[] = {"INFO", "WARN", "ERROR", "DEBUG"};
    for (int i = 0; i < 4; i++) {
        int count = count_log_level(entries, entry_count, levels[i]);
        printf("%s: %d (%.1f%%)\n", levels[i], count, 
               entry_count > 0 ? (count * 100.0) / entry_count : 0.0);
    }
    
    int error_count = count_log_level(entries, entry_count, "ERROR");
    if (error_count > 0) {
        printf("\nRecent ERROR entries:\n");
        int printed = 0;
        for (int i = entry_count - 1; i >= 0 && printed < 3; i--) {
            if (strcmp(entries[i].level, "ERROR") == 0) {
                printf("%s: %s\n", entries[i].timestamp, entries[i].message);
                printed++;
            }
        }
    }
    
    char search_term[64];
    printf("\nEnter search term (or 'quit' to exit): ");
    if (fgets(search_term, sizeof(search_term), stdin)) {
        search_term[strcspn(search_term, "\n")] = '\0';
        
        if (strcmp(search_term, "quit") != 0) {
            int matches = 0;
            printf("Search results for '%s':\n", search_term);
            
            for (int i = 0; i < entry_count; i++) {
                if (strstr(entries[i].message, search_term) != NULL) {
                    printf("%s [%s] %s\n", entries[i].timestamp, 
                           entries[i].level, entries[i].message);
                    matches++;
                }
            }
            
            printf("Found %d matches\n", matches);
        }
    }
    
    return 0;
}