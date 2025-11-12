//DeepSeek-V3 V2.5 Category: Safe ; Style: function_pointers ; Variation: log_analyzer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>

#define MAX_LINE_LEN 1024
#define MAX_ENTRIES 1000

typedef struct {
    char timestamp[64];
    char level[16];
    char message[256];
} LogEntry;

typedef void (*LogProcessor)(LogEntry*, int);

void count_by_level(LogEntry* entries, int count) {
    int error_count = 0, warn_count = 0, info_count = 0;
    
    for (int i = 0; i < count; i++) {
        if (strcmp(entries[i].level, "ERROR") == 0) {
            error_count++;
        } else if (strcmp(entries[i].level, "WARN") == 0) {
            warn_count++;
        } else if (strcmp(entries[i].level, "INFO") == 0) {
            info_count++;
        }
    }
    
    printf("Log level summary:\n");
    printf("ERROR: %d\n", error_count);
    printf("WARN:  %d\n", warn_count);
    printf("INFO:  %d\n", info_count);
}

void find_recent_logs(LogEntry* entries, int count) {
    time_t now = time(NULL);
    time_t threshold = now - (24 * 60 * 60);
    int recent_count = 0;
    
    for (int i = 0; i < count; i++) {
        struct tm tm_time = {0};
        if (strptime(entries[i].timestamp, "%Y-%m-%d %H:%M:%S", &tm_time) != NULL) {
            time_t log_time = mktime(&tm_time);
            if (log_time >= threshold && log_time <= now) {
                recent_count++;
            }
        }
    }
    
    printf("Logs from last 24 hours: %d\n", recent_count);
}

void search_messages(LogEntry* entries, int count) {
    char search_term[64];
    printf("Enter search term: ");
    
    if (fgets(search_term, sizeof(search_term), stdin) == NULL) {
        return;
    }
    
    size_t len = strlen(search_term);
    if (len > 0 && search_term[len - 1] == '\n') {
        search_term[len - 1] = '\0';
    }
    
    if (strlen(search_term) == 0) {
        return;
    }
    
    int found_count = 0;
    for (int i = 0; i < count; i++) {
        if (strstr(entries[i].message, search_term) != NULL) {
            found_count++;
        }
    }
    
    printf("Found %d logs containing '%s'\n", found_count, search_term);
}

int parse_log_line(const char* line, LogEntry* entry) {
    if (line == NULL || entry == NULL) {
        return 0;
    }
    
    char timestamp[64], level[16], message[256];
    
    int result = sscanf(line, "%63s %15s %255[^\n]", timestamp, level, message);
    if (result != 3) {
        return 0;
    }
    
    if (strlen(timestamp) > 0 && strlen(level) > 0 && strlen(message) > 0) {
        strncpy(entry->timestamp, timestamp, sizeof(entry->timestamp) - 1);
        strncpy(entry->level, level, sizeof(entry->level) - 1);
        strncpy(entry->message, message, sizeof(entry->message) - 1);
        entry->timestamp[sizeof(entry->timestamp) - 1] = '\0';
        entry->level[sizeof(entry->level) - 1] = '\0';
        entry->message[sizeof(entry->message) - 1] = '\0';
        return 1;
    }
    
    return 0;
}

int main(void) {
    LogEntry entries[MAX_ENTRIES];
    char line[MAX_LINE_LEN];
    int entry_count = 0;
    
    printf("Enter log entries (format: TIMESTAMP LEVEL MESSAGE):\n");
    printf("Example: 2024-01-15 10:30:00 INFO System started\n");
    printf("Enter empty line to finish input\n");
    
    while (entry_count < MAX_ENTRIES) {
        if (fgets(line, sizeof(line), stdin) == NULL) {
            break;
        }
        
        size_t len = strlen(line);
        if (len > 0 && line[len - 1] == '\n') {
            line[len - 1] = '\0';
        }
        
        if (strlen(line) == 0) {
            break;
        }
        
        if (parse_log_line(line, &entries[entry_count])) {
            entry_count++;
        } else {
            printf("Invalid log format. Skipping.\n");
        }
    }
    
    if (entry_count == 0) {
        printf("No valid log entries provided.\n");
        return 1;
    }
    
    LogProcessor processors[] = {
        count_by_level,
        find_recent_logs,
        search_messages
    };
    
    const char* options[] = {
        "Count logs by level",
        "Find recent logs (24h)",
        "Search in messages"
    };
    
    int num_processors = sizeof(processors) / sizeof(processors[0]);
    
    while (1