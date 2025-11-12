//DeepSeek-V3 V2.5 Category: Safe ; Style: heap_stack_mix ; Variation: log_analyzer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdint.h>

#define MAX_LINE_LENGTH 1024
#define MAX_ENTRIES 1000

typedef struct {
    char timestamp[32];
    char level[16];
    char message[256];
    uint32_t count;
} LogEntry;

typedef struct {
    LogEntry entries[MAX_ENTRIES];
    size_t count;
} LogAnalyzer;

void parse_log_line(const char* line, LogEntry* entry) {
    if (!line || !entry) return;
    
    char temp_line[MAX_LINE_LENGTH];
    strncpy(temp_line, line, sizeof(temp_line) - 1);
    temp_line[sizeof(temp_line) - 1] = '\0';
    
    char* timestamp = strtok(temp_line, " ");
    char* level = strtok(NULL, " ");
    char* message = strtok(NULL, "\n");
    
    if (timestamp) {
        strncpy(entry->timestamp, timestamp, sizeof(entry->timestamp) - 1);
        entry->timestamp[sizeof(entry->timestamp) - 1] = '\0';
    }
    
    if (level) {
        strncpy(entry->level, level, sizeof(entry->level) - 1);
        entry->level[sizeof(entry->level) - 1] = '\0';
    }
    
    if (message) {
        strncpy(entry->message, message, sizeof(entry->message) - 1);
        entry->message[sizeof(entry->message) - 1] = '\0';
    }
    
    entry->count = 1;
}

void add_log_entry(LogAnalyzer* analyzer, const LogEntry* entry) {
    if (!analyzer || !entry || analyzer->count >= MAX_ENTRIES) return;
    
    for (size_t i = 0; i < analyzer->count; i++) {
        if (strcmp(analyzer->entries[i].timestamp, entry->timestamp) == 0 &&
            strcmp(analyzer->entries[i].level, entry->level) == 0 &&
            strcmp(analyzer->entries[i].message, entry->message) == 0) {
            analyzer->entries[i].count++;
            return;
        }
    }
    
    analyzer->entries[analyzer->count] = *entry;
    analyzer->count++;
}

void analyze_logs(LogAnalyzer* analyzer) {
    if (!analyzer) return;
    
    uint32_t error_count = 0;
    uint32_t warning_count = 0;
    uint32_t info_count = 0;
    
    for (size_t i = 0; i < analyzer->count; i++) {
        if (strcmp(analyzer->entries[i].level, "ERROR") == 0) {
            error_count += analyzer->entries[i].count;
        } else if (strcmp(analyzer->entries[i].level, "WARNING") == 0) {
            warning_count += analyzer->entries[i].count;
        } else if (strcmp(analyzer->entries[i].level, "INFO") == 0) {
            info_count += analyzer->entries[i].count;
        }
    }
    
    printf("Log Analysis Summary:\n");
    printf("ERROR entries: %u\n", error_count);
    printf("WARNING entries: %u\n", warning_count);
    printf("INFO entries: %u\n", info_count);
    printf("Total unique entries: %zu\n", analyzer->count);
    
    printf("\nMost frequent entries:\n");
    for (size_t i = 0; i < analyzer->count && i < 5; i++) {
        printf("%s [%s]: %s (count: %u)\n",
               analyzer->entries[i].timestamp,
               analyzer->entries[i].level,
               analyzer->entries[i].message,
               analyzer->entries[i].count);
    }
}

int main(void) {
    LogAnalyzer analyzer_stack;
    analyzer_stack.count = 0;
    
    LogAnalyzer* analyzer_heap = malloc(sizeof(LogAnalyzer));
    if (!analyzer_heap) {
        fprintf(stderr, "Memory allocation failed\n");
        return 1;
    }
    analyzer_heap->count = 0;
    
    char line[MAX_LINE_LENGTH];
    printf("Enter log lines (format: TIMESTAMP LEVEL MESSAGE):\n");
    printf("Type 'END' on a separate line to finish input.\n");
    
    while (fgets(line, sizeof(line), stdin)) {
        if (strncmp(line, "END", 3) == 0) {
            break;
        }
        
        line[strcspn(line, "\n")] = '\0';
        
        if (strlen(line) == 0) {
            continue;
        }
        
        LogEntry entry_stack;
        parse_log_line(line, &entry_stack);
        add_log_entry(&analyzer_stack, &entry_stack);
        
        LogEntry* entry_heap = malloc(sizeof(LogEntry));
        if (entry_heap) {
            parse_log_line(line, entry_heap);
            add_log_entry(analyzer_heap, entry_heap);
            free(entry_heap);
        }
    }
    
    printf("\n=== Stack-based Analysis ===\n");
    analyze_logs(&analyzer_stack);
    
    printf("\n=== Heap-based Analysis ===\n");
    analyze_logs(analyzer_heap);
    
    free(analyzer_heap);
    return 0;
}