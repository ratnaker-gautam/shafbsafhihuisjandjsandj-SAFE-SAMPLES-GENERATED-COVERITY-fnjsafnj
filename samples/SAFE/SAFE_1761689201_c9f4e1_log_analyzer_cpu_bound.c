//DeepSeek-V3 V2.5 Category: Safe ; Style: cpu_bound ; Variation: log_analyzer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>

#define MAX_LINE_LEN 1024
#define MAX_ENTRIES 10000

typedef struct {
    char timestamp[32];
    char level[16];
    char message[256];
} LogEntry;

int parse_log_line(const char* line, LogEntry* entry) {
    if (!line || !entry) return 0;
    
    char temp[MAX_LINE_LEN];
    strncpy(temp, line, MAX_LINE_LEN - 1);
    temp[MAX_LINE_LEN - 1] = '\0';
    
    char* parts[3];
    int part_count = 0;
    char* token = strtok(temp, "|");
    
    while (token && part_count < 3) {
        parts[part_count++] = token;
        token = strtok(NULL, "|");
    }
    
    if (part_count != 3) return 0;
    
    if (strlen(parts[0]) >= sizeof(entry->timestamp) ||
        strlen(parts[1]) >= sizeof(entry->level) ||
        strlen(parts[2]) >= sizeof(entry->message)) {
        return 0;
    }
    
    strncpy(entry->timestamp, parts[0], sizeof(entry->timestamp) - 1);
    entry->timestamp[sizeof(entry->timestamp) - 1] = '\0';
    
    strncpy(entry->level, parts[1], sizeof(entry->level) - 1);
    entry->level[sizeof(entry->level) - 1] = '\0';
    
    strncpy(entry->message, parts[2], sizeof(entry->message) - 1);
    entry->message[sizeof(entry->message) - 1] = '\0';
    
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
        "Security alert triggered"
    };
    
    time_t now = time(NULL);
    *count = 0;
    
    for (int i = 0; i < 500 && *count < MAX_ENTRIES; i++) {
        LogEntry entry;
        time_t offset = now - (rand() % 86400);
        struct tm* tm_info = localtime(&offset);
        
        if (tm_info) {
            strftime(entry.timestamp, sizeof(entry.timestamp), 
                    "%Y-%m-%d %H:%M:%S", tm_info);
        } else {
            strcpy(entry.timestamp, "2024-01-01 00:00:00");
        }
        
        strncpy(entry.level, levels[rand() % 4], sizeof(entry.level) - 1);
        entry.level[sizeof(entry.level) - 1] = '\0';
        
        strncpy(entry.message, messages[rand() % 8], sizeof(entry.message) - 1);
        entry.message[sizeof(entry.message) - 1] = '\0';
        
        entries[(*count)++] = entry;
    }
}

int main(void) {
    LogEntry entries[MAX_ENTRIES];
    int entry_count = 0;
    
    srand(42);
    generate_sample_logs(entries, &entry_count);
    
    printf("Log Analysis Results:\n");
    printf("Total entries processed: %d\n", entry_count);
    printf("\n");
    
    const char* levels[] = {"INFO", "WARNING", "ERROR", "DEBUG"};
    for (int i = 0; i < 4; i++) {
        int count = count_log_level(entries, entry_count, levels[i]);
        printf("%s: %d (%.1f%%)\n", levels[i], count, 
               entry_count > 0 ? (count * 100.0 / entry_count) : 0.0);
    }
    
    printf("\nSample entries:\n");
    int samples = entry_count < 5 ? entry_count : 5;
    for (int i = 0; i < samples; i++) {
        printf("%s | %s | %s\n", 
               entries[i].timestamp, entries[i].level, entries[i].message);
    }
    
    return 0;
}