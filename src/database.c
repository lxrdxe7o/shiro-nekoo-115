#include "../include/database.h"
#include "../include/memory.h"
#include "../include/error.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>


#define DB_MAGIC 0x484D5331 // "HMS1"

static char* safe_strdup(const char* src) {
    if (!src) return NULL;
    size_t len = strlen(src) + 1;
    char* dest = (char*)MALLOC(len);
    if (dest) memcpy(dest, src, len);
    return dest;
}

Database* database_create(const char* data_dir) {
    Database* db = (Database*)CALLOC(1, sizeof(Database));
    if (!db) return NULL;
    
    db->data_dir = safe_strdup(data_dir);
    db->patients = patient_manager_create();
    db->doctors = doctor_manager_create();
    db->wards = ward_manager_create();
    db->dirty = false;
    
    return db;
}

void database_destroy(Database* db) {
    if (!db) return;
    
    patient_manager_destroy(db->patients);
    doctor_manager_destroy(db->doctors);
    ward_manager_destroy(db->wards);
    FREE(db->data_dir);
    FREE(db);
}

// -----------------------------------------------------------------------------
// Serialization Helpers
// -----------------------------------------------------------------------------

static void write_string(FILE* f, const char* str) {
    uint32_t len = str ? (uint32_t)strlen(str) : 0;
    fwrite(&len, sizeof(len), 1, f);
    if (len > 0) {
        fwrite(str, 1, len, f);
    }
}

static char* read_string(FILE* f) {
    uint32_t len;
    if (fread(&len, sizeof(len), 1, f) != 1) return NULL;
    
    if (len == 0) return NULL;
    
    char* str = (char*)MALLOC(len + 1);
    if (fread(str, 1, len, f) != (size_t)len) {
        FREE(str);
        return NULL;
    }
    str[len] = '\0';
    return str;
}

static void write_int(FILE* f, int v) {
    fwrite(&v, sizeof(v), 1, f);
}

static int read_int(FILE* f) {
    int v;
    fread(&v, sizeof(v), 1, f);
    return v;
}

static void write_float(FILE* f, float v) {
    fwrite(&v, sizeof(v), 1, f);
}

static float read_float(FILE* f) {
    float v;
    fread(&v, sizeof(v), 1, f);
    return v;
}

// -----------------------------------------------------------------------------
// Save Implementations
// -----------------------------------------------------------------------------

static void save_patients(Database* db, const char* filename) {
    FILE* f = fopen(filename, "wb");
    if (!f) return;
    
    uint32_t magic = DB_MAGIC;
    fwrite(&magic, sizeof(magic), 1, f);
    
    size_t count = vector_size(db->patients->patients_list);
    fwrite(&count, sizeof(size_t), 1, f);
    
    write_int(f, db->patients->next_id);
    
    for (size_t i = 0; i < count; i++) {
        Patient* p = (Patient*)vector_get(db->patients->patients_list, i);
        write_int(f, p->id);
        write_string(f, p->name);
        write_string(f, p->address);
        write_string(f, p->prescription);
        write_string(f, p->username);
        write_string(f, p->password);
        write_string(f, p->contact);
        write_string(f, p->emergency_contact);
        write_string(f, p->blood_type); // Technically fixed size but string safe
        write_int(f, p->age);
        write_float(f, p->height);
        write_float(f, p->weight);
    }
    
    fclose(f);
}

static void save_doctors(Database* db, const char* filename) {
    FILE* f = fopen(filename, "wb");
    if (!f) return;
    
    uint32_t magic = DB_MAGIC;
    fwrite(&magic, sizeof(magic), 1, f);
    
    size_t count = vector_size(db->doctors->doctors_list);
    fwrite(&count, sizeof(size_t), 1, f);
    
    write_int(f, db->doctors->next_id);
    
    for (size_t i = 0; i < count; i++) {
        Doctor* d = (Doctor*)vector_get(db->doctors->doctors_list, i);
        write_int(f, d->id);
        write_string(f, d->name);
        write_string(f, d->address);
        write_string(f, d->specialty);
        write_string(f, d->username);
        write_string(f, d->password);
        write_string(f, d->contact);
        write_int(f, d->experience);
        
        // Save Calendar
        for (int day = 0; day < DAYS_IN_WEEK; day++) {
            for (int slot = 0; slot < SLOTS_PER_DAY; slot++) {
                TimeSlot* ts = &d->calendar[day][slot];
                write_int(f, ts->available);
                write_int(f, ts->max_appointments);
                
                size_t appt_count = vector_size(ts->patient_ids);
                fwrite(&appt_count, sizeof(size_t), 1, f);
                
                for (size_t k = 0; k < appt_count; k++) {
                    int* pid = (int*)vector_get(ts->patient_ids, k);
                    write_int(f, *pid);
                }
            }
        }
    }
    
    fclose(f);
}

static void save_wards(Database* db, const char* filename) {
    FILE* f = fopen(filename, "wb");
    if (!f) return;
    
    uint32_t magic = DB_MAGIC;
    fwrite(&magic, sizeof(magic), 1, f);
    
    size_t count = vector_size(db->wards->wards_list);
    fwrite(&count, sizeof(size_t), 1, f);
    
    write_int(f, db->wards->next_id);
    
    for (size_t i = 0; i < count; i++) {
        Ward* w = (Ward*)vector_get(db->wards->wards_list, i);
        write_int(f, w->id);
        write_string(f, w->name);
        write_int(f, w->capacity);
        write_int(f, w->assigned_doctor_id);
        
        size_t pat_count = vector_size(w->patients);
        fwrite(&pat_count, sizeof(size_t), 1, f);
        
        for (size_t k = 0; k < pat_count; k++) {
            Patient* p = (Patient*)vector_get(w->patients, k);
            if(p) write_int(f, p->id);
        }
    }
    
    fclose(f);
}

Result database_save(Database* db) {
    char path[512];
    
    snprintf(path, sizeof(path), "%s/patients.hms", db->data_dir);
    save_patients(db, path);
    
    snprintf(path, sizeof(path), "%s/doctors.hms", db->data_dir);
    save_doctors(db, path);
    
    snprintf(path, sizeof(path), "%s/wards.hms", db->data_dir);
    save_wards(db, path);
    
    db->dirty = false;
    return (Result){1, HMS_OK, ""};
}

// -----------------------------------------------------------------------------
// Load Implementations
// -----------------------------------------------------------------------------

static void load_patients(Database* db, const char* filename) {
    FILE* f = fopen(filename, "rb");
    if (!f) return;
    
    uint32_t magic;
    fread(&magic, sizeof(magic), 1, f);
    if (magic != DB_MAGIC) {
        fclose(f);
        return;
    }
    
    size_t count;
    fread(&count, sizeof(size_t), 1, f);
    
    int next_id = read_int(f);
    db->patients->next_id = next_id;
    
    for (size_t i = 0; i < count; i++) {
        Patient* p = patient_create();
        p->id = read_int(f);
        p->name = read_string(f);
        p->address = read_string(f);
        p->prescription = read_string(f);
        p->username = read_string(f);
        p->password = read_string(f);
        p->contact = read_string(f);
        p->emergency_contact = read_string(f);
        
        char* bt = read_string(f);
        if (bt) {
            strncpy(p->blood_type, bt, sizeof(p->blood_type) - 1);
            FREE(bt);
        }
        
        p->age = read_int(f);
        p->height = read_float(f);
        p->weight = read_float(f);
        
        patient_manager_add(db->patients, p);
    }
    
    fclose(f);
}

static void load_doctors(Database* db, const char* filename) {
    FILE* f = fopen(filename, "rb");
    if (!f) return;
    
    uint32_t magic;
    fread(&magic, sizeof(magic), 1, f);
    if (magic != DB_MAGIC) {
        fclose(f);
        return;
    }
    
    size_t count;
    fread(&count, sizeof(size_t), 1, f);
    
    int next_id = read_int(f);
    db->doctors->next_id = next_id;
    
    for (size_t i = 0; i < count; i++) {
        Doctor* d = doctor_create();
        d->id = read_int(f);
        d->name = read_string(f);
        d->address = read_string(f);
        d->specialty = read_string(f);
        d->username = read_string(f);
        d->password = read_string(f);
        d->contact = read_string(f);
        d->experience = read_int(f);
        
        // Load Calendar
        for (int day = 0; day < DAYS_IN_WEEK; day++) {
            for (int slot = 0; slot < SLOTS_PER_DAY; slot++) {
                TimeSlot* ts = &d->calendar[day][slot];
                ts->available = read_int(f);
                ts->max_appointments = read_int(f);
                
                size_t appt_count;
                fread(&appt_count, sizeof(size_t), 1, f);
                
                for (size_t k = 0; k < appt_count; k++) {
                    int pid = read_int(f);
                    // Just storing ID, valid/invalid check requires patient existing
                    int* pid_ptr = (int*)MALLOC(sizeof(int));
                    *pid_ptr = pid;
                    vector_push(ts->patient_ids, pid_ptr);
                }
            }
        }
        
        doctor_manager_add(db->doctors, d);
    }
    
    fclose(f);
}

static void load_wards(Database* db, const char* filename) {
    FILE* f = fopen(filename, "rb");
    if (!f) return;
    
    uint32_t magic;
    fread(&magic, sizeof(magic), 1, f);
    if (magic != DB_MAGIC) {
        fclose(f);
        return;
    }
    
    size_t count;
    fread(&count, sizeof(size_t), 1, f);
    
    int next_id = read_int(f);
    db->wards->next_id = next_id;
    
    for (size_t i = 0; i < count; i++) {
        int id = read_int(f);
        char* name = read_string(f);
        int capacity = read_int(f);
        
        Ward* w = ward_create(name, capacity);
        FREE(name); // ward_create duplicates it
        
        w->id = id;
        w->assigned_doctor_id = read_int(f);
        
        size_t pat_count;
        fread(&pat_count, sizeof(size_t), 1, f);
        
        for (size_t k = 0; k < pat_count; k++) {
            int pid = read_int(f);
            Patient* p = patient_manager_find_by_id(db->patients, pid);
            if (p) {
                ward_add_patient(w, p);
            }
        }
        
        ward_manager_add(db->wards, w);
    }
    
    fclose(f);
}

Result database_load(Database* db) {
    char path[512];
    
    snprintf(path, sizeof(path), "%s/patients.hms", db->data_dir);
    load_patients(db, path);
    
    snprintf(path, sizeof(path), "%s/doctors.hms", db->data_dir);
    load_doctors(db, path);
    
    snprintf(path, sizeof(path), "%s/wards.hms", db->data_dir);
    load_wards(db, path);
    
    return (Result){1, HMS_OK, ""};
}

bool database_migration_needed(const char* legacy_dir) {
    // Stub
    (void)legacy_dir;
    return false;
}
