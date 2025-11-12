//DeepSeek-V3 V2.5 Category: Safe ; Style: function_pointers ; Variation: log_analyzer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>

#define MAX_LINE_LENGTH 1024
#define MAX_ENTRIES 1000

typedef struct {
    char timestamp[64];
    char level[16];
    char message[256];
} LogEntry;

typedef void (*LogProcessor)(LogEntry*, int);

void count_by_level(LogEntry* entries, int count) {
    int info_count = 0, warn_count = 0, error_count = 0;
    
    for (int i = 0; i < count; i++) {
        if (strcmp(entries[i].level, "INFO") == 0) {
            info_count++;
        } else if (strcmp(entries[i].level, "WARN") == 0) {
            warn_count++;
        } else if (strcmp(entries[i].level, "ERROR") == 0) {
            error_count++;
        }
    }
    
    printf("Log level summary:\n");
    printf("INFO:  %d\n", info_count);
    printf("WARN:  %d\n", warn_count);
    printf("ERROR: %d\n", error_count);
}

void find_recent_logs(LogEntry* entries, int count) {
    time_t now = time(NULL);
    int recent_count = 0;
    
    for (int i = 0; i < count; i++) {
        struct tm tm_time = {0};
        if (strptime(entries[i].timestamp, "%Y-%m-%d %H:%M:%S", &tm_time) != NULL) {
            time_t log_time = mktime(&tm_time);
            if (log_time != -1 && difftime(now, log_time) < 86400) {
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
    int parsed = sscanf(line, "%63s %15s %255[^\n]", timestamp, level, message);
    
    if (parsed != 3) {
        return 0;
    }
    
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

int read_log_file(const char* filename, LogEntry* entries, int max_entries) {
    if (filename == NULL || entries == NULL || max_entries <= 0) {
        return 0;
    }
    
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        return 0;
    }
    
    char line[MAX_LINE_LENGTH];
    int count = 0;
    
    while (fgets(line, sizeof(line), file) != NULL && count < max_entries) {
        size_t len = strlen(line);
        if (len > 0 && line[len - 1] == '\n') {
            line[len - 1] = '\0';
        }
        
        if (parse_log_line(line, &entries[count])) {
            count++;
        }
    }
    
    fclose(file);
    return count;
}

void show_menu(void) {
    printf("\nLog Analyzer Menu:\n");
    printf("1. Count logs by level\n");
    printf("2. Find recent logs (24h)\n");
    printf("3. Search messages\n");
    printf("4. Exit\n");
    printf("Choose option: ");
}

int main(void) {
    LogEntry entries[MAX_ENTRIES];
    LogProcessor processors[] = {
        count_by_level,
        find_recent_logs,
        search_messages
    };
    
    printf("Enter log filename: ");
    char filename[256];
    if (fgets(filename, sizeof(filename), stdin) == NULL) {
        return 1;
    }
    
    size_t len = strlen(filename);
    if (len > 0 && filename[len - 1] == '\n') {
        filename[len - 1] = '\0';
    }
    
    if (strlen(filename) == 0) {
        printf("Invalid filename\n");
        return 1;
    }
    
    int entry_count = read_log_file(filename, entries, MAX_ENT