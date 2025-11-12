//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: cpu_bound ; Variation: log_analyzer
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
    
    strncpy(entry->timestamp, parts[0], 31);
    entry->timestamp[31] = '\0';
    strncpy(entry->level, parts[1], 15);
    entry->level[15] = '\0';
    strncpy(entry->message, parts[2], 255);
    entry->message[255] = '\0';
    
    return 1;
}

int count_log_level(const LogEntry* logs, int count, const char* level) {
    int total = 0;
    for (int i = 0; i < count; i++) {
        if (strcmp(logs[i].level, level) == 0) {
            total++;
        }
    }
    return total;
}

void generate_sample_logs(LogEntry* logs, int* count) {
    const char* levels[] = {"INFO", "WARNING", "ERROR", "DEBUG"};
    const char* messages[] = {
        "User login successful",
        "Database connection established",
        "File upload completed",
        "Memory usage high",
        "Network timeout detected",
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
        
        strftime(entry.timestamp, 31, "%Y-%m-%d %H:%M:%S", tm_info);
        strncpy(entry.level, levels[rand() % 4], 15);
        entry.level[15] = '\0';
        strncpy(entry.message, messages[rand() % 8], 255);
        entry.message[255] = '\0';
        
        logs[(*count)++] = entry;
    }
}

int main(void) {
    LogEntry logs[MAX_ENTRIES];
    int log_count = 0;
    
    srand((unsigned int)time(NULL));
    generate_sample_logs(logs, &log_count);
    
    if (log_count == 0) {
        printf("No logs to analyze\n");
        return 1;
    }
    
    printf("Log Analysis Results:\n");
    printf("Total log entries: %d\n", log_count);
    
    const char* levels[] = {"INFO", "WARNING", "ERROR", "DEBUG"};
    for (int i = 0; i < 4; i++) {
        int count = count_log_level(logs, log_count, levels[i]);
        printf("%s: %d (%.1f%%)\n", levels[i], count, 
               (count * 100.0) / log_count);
    }
    
    int error_count = count_log_level(logs, log_count, "ERROR");
    if (error_count > 0) {
        printf("\nRecent ERROR entries:\n");
        int printed = 0;
        for (int i = log_count - 1; i >= 0 && printed < 5; i--) {
            if (strcmp(logs[i].level, "ERROR") == 0) {
                printf("%s | %s | %s\n", logs[i].timestamp, 
                       logs[i].level, logs[i].message);
                printed++;
            }
        }
    }
    
    char search_term[64];
    printf("\nEnter search term (or 'quit' to exit): ");
    if (fgets(search_term, sizeof(search_term), stdin)) {
        search_term[strcspn(search_term, "\n")] = '\0';
        
        if (strcmp(search_term, "quit") != 0) {
            printf("Search results for '%s':\n", search_term);
            int found = 0;
            for (int i = 0; i < log_count; i++) {
                if (strstr(logs[i].message, search_term) != NULL) {
                    printf("%s | %s | %s\n", logs[i].timestamp, 
                           logs[i].level, logs[i].message);
                    found++;
                }
            }
            printf("Found %d matches\n", found);
        }
    }
    
    return 0;
}