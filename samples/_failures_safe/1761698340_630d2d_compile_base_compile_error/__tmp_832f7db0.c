//DeepSeek-V3 V2.5 Category: Safe ; Style: heap_stack_mix ; Variation: log_analyzer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_LINE_LENGTH 1024
#define MAX_ENTRIES 1000

typedef struct {
    char timestamp[32];
    char level[16];
    char message[256];
    int severity;
} LogEntry;

typedef struct {
    LogEntry* entries;
    int count;
    int capacity;
} LogCollection;

int parse_severity(const char* level) {
    if (strcmp(level, "DEBUG") == 0) return 0;
    if (strcmp(level, "INFO") == 0) return 1;
    if (strcmp(level, "WARNING") == 0) return 2;
    if (strcmp(level, "ERROR") == 0) return 3;
    if (strcmp(level, "CRITICAL") == 0) return 4;
    return -1;
}

int validate_timestamp(const char* timestamp) {
    struct tm tm = {0};
    return strptime(timestamp, "%Y-%m-%d %H:%M:%S", &tm) != NULL;
}

int read_log_entry(FILE* file, LogEntry* entry) {
    char line[MAX_LINE_LENGTH];
    if (fgets(line, sizeof(line), file) == NULL) return 0;
    
    char timestamp[32], level[16], message[256];
    if (sscanf(line, "%31s %15s %255[^\n]", timestamp, level, message) != 3) return 0;
    
    if (!validate_timestamp(timestamp)) return 0;
    
    int severity = parse_severity(level);
    if (severity == -1) return 0;
    
    strncpy(entry->timestamp, timestamp, sizeof(entry->timestamp) - 1);
    entry->timestamp[sizeof(entry->timestamp) - 1] = '\0';
    strncpy(entry->level, level, sizeof(entry->level) - 1);
    entry->level[sizeof(entry->level) - 1] = '\0';
    strncpy(entry->message, message, sizeof(entry->message) - 1);
    entry->message[sizeof(entry->message) - 1] = '\0';
    entry->severity = severity;
    
    return 1;
}

void init_log_collection(LogCollection* collection, int initial_capacity) {
    if (initial_capacity <= 0) initial_capacity = 10;
    if (initial_capacity > MAX_ENTRIES) initial_capacity = MAX_ENTRIES;
    collection->entries = malloc(initial_capacity * sizeof(LogEntry));
    if (collection->entries == NULL) exit(EXIT_FAILURE);
    collection->count = 0;
    collection->capacity = initial_capacity;
}

void add_log_entry(LogCollection* collection, const LogEntry* entry) {
    if (collection->count >= collection->capacity) {
        int new_capacity = collection->capacity * 2;
        if (new_capacity > MAX_ENTRIES) new_capacity = MAX_ENTRIES;
        if (new_capacity <= collection->capacity) return;
        LogEntry* new_entries = realloc(collection->entries, new_capacity * sizeof(LogEntry));
        if (new_entries == NULL) exit(EXIT_FAILURE);
        collection->entries = new_entries;
        collection->capacity = new_capacity;
    }
    collection->entries[collection->count] = *entry;
    collection->count++;
}

void analyze_logs(const LogCollection* collection) {
    int severity_count[5] = {0};
    char* severity_names[] = {"DEBUG", "INFO", "WARNING", "ERROR", "CRITICAL"};
    
    for (int i = 0; i < collection->count; i++) {
        if (collection->entries[i].severity >= 0 && collection->entries[i].severity < 5) {
            severity_count[collection->entries[i].severity]++;
        }
    }
    
    printf("Log Analysis Summary:\n");
    printf("Total entries: %d\n", collection->count);
    for (int i = 0; i < 5; i++) {
        printf("%s: %d\n", severity_names[i], severity_count[i]);
    }
}

void find_errors(const LogCollection* collection) {
    printf("\nRecent ERROR and CRITICAL entries:\n");
    int found = 0;
    for (int i = collection->count - 1; i >= 0 && found < 5; i--) {
        if (collection->entries[i].severity >= 3) {
            printf("%s %s: %s\n", collection->entries[i].timestamp, 
                   collection->entries[i].level, collection->entries[i].message);
            found++;
        }
    }
    if (found == 0) printf("No ERROR or CRITICAL entries found.\n");
}

void free_log_collection(LogCollection* collection) {
    free(collection->entries);
    collection->entries = NULL;
    collection->count = 0;
    collection->capacity = 0;
}

int main() {
    char filename[256];
    printf("Enter log file name: ");
    if (scanf("%255s", filename) != 1) {
        printf("Invalid input\n");
        return EXIT_FAILURE;
    }
    
    FILE* file = fopen(filename, "r");
    if (file == NULL) {