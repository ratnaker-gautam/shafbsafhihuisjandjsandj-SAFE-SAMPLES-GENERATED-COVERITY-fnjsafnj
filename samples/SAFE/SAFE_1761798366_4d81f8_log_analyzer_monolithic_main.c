//DeepSeek-V3 V2.5 Category: Safe ; Style: monolithic_main ; Variation: log_analyzer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>

#define MAX_LINE_LENGTH 1024
#define MAX_ENTRIES 1000

struct LogEntry {
    char timestamp[64];
    char level[16];
    char message[256];
};

int parse_log_line(const char* line, struct LogEntry* entry) {
    if (line == NULL || entry == NULL) return 0;
    
    char buffer[MAX_LINE_LENGTH];
    if (strlen(line) >= MAX_LINE_LENGTH - 1) return 0;
    strncpy(buffer, line, MAX_LINE_LENGTH - 1);
    buffer[MAX_LINE_LENGTH - 1] = '\0';
    
    char* parts[3];
    int part_count = 0;
    char* token = strtok(buffer, "|");
    
    while (token != NULL && part_count < 3) {
        while (*token && isspace(*token)) token++;
        char* end = token + strlen(token) - 1;
        while (end > token && isspace(*end)) *end-- = '\0';
        parts[part_count++] = token;
        token = strtok(NULL, "|");
    }
    
    if (part_count != 3) return 0;
    
    if (strlen(parts[0]) >= sizeof(entry->timestamp) - 1) return 0;
    if (strlen(parts[1]) >= sizeof(entry->level) - 1) return 0;
    if (strlen(parts[2]) >= sizeof(entry->message) - 1) return 0;
    
    strncpy(entry->timestamp, parts[0], sizeof(entry->timestamp) - 1);
    entry->timestamp[sizeof(entry->timestamp) - 1] = '\0';
    strncpy(entry->level, parts[1], sizeof(entry->level) - 1);
    entry->level[sizeof(entry->level) - 1] = '\0';
    strncpy(entry->message, parts[2], sizeof(entry->message) - 1);
    entry->message[sizeof(entry->message) - 1] = '\0';
    
    return 1;
}

int main(void) {
    struct LogEntry entries[MAX_ENTRIES];
    int entry_count = 0;
    int error_count = 0;
    int warning_count = 0;
    int info_count = 0;
    
    char line[MAX_LINE_LENGTH];
    
    printf("Log Analyzer - Enter log lines (format: timestamp|level|message)\\n");
    printf("Enter 'END' on a separate line to finish input\\n");
    
    while (entry_count < MAX_ENTRIES) {
        if (fgets(line, sizeof(line), stdin) == NULL) break;
        
        if (strlen(line) > 0 && line[strlen(line) - 1] == '\\n') {
            line[strlen(line) - 1] = '\\0';
        }
        
        if (strcmp(line, "END") == 0) break;
        
        if (strlen(line) == 0) continue;
        
        struct LogEntry entry;
        if (parse_log_line(line, &entry)) {
            entries[entry_count] = entry;
            entry_count++;
            
            if (strcmp(entry.level, "ERROR") == 0) error_count++;
            else if (strcmp(entry.level, "WARNING") == 0) warning_count++;
            else if (strcmp(entry.level, "INFO") == 0) info_count++;
        }
    }
    
    printf("\\n=== Log Analysis Results ===\\n");
    printf("Total entries processed: %d\\n", entry_count);
    printf("ERROR entries: %d\\n", error_count);
    printf("WARNING entries: %d\\n", warning_count);
    printf("INFO entries: %d\\n", info_count);
    
    if (error_count > 0) {
        printf("\\n=== ERROR Entries ===\\n");
        for (int i = 0; i < entry_count; i++) {
            if (strcmp(entries[i].level, "ERROR") == 0) {
                printf("%s | %s | %s\\n", entries[i].timestamp, entries[i].level, entries[i].message);
            }
        }
    }
    
    if (warning_count > 0) {
        printf("\\n=== WARNING Entries ===\\n");
        for (int i = 0; i < entry_count; i++) {
            if (strcmp(entries[i].level, "WARNING") == 0) {
                printf("%s | %s | %s\\n", entries[i].timestamp, entries[i].level, entries[i].message);
            }
        }
    }
    
    return 0;
}