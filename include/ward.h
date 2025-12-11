#ifndef HMS_WARD_H
#define HMS_WARD_H

#include "types.h"
#include "patient.h"
#include "doctor.h"
#include "vector.h"

typedef struct {
    int id;
    char* name;
    int capacity;
    Vector* patients;       // Vector of Patient* (non-owning)
    int assigned_doctor_id; // Optional: Head doctor of the ward
} Ward;

// Lifecycle
Ward* ward_create(const char* name, int capacity);
void  ward_destroy(void* w_ptr);

// Operations
Result ward_add_patient(Ward* w, Patient* p);
Result ward_remove_patient(Ward* w, int patient_id);
bool   ward_is_full(Ward* w);
bool   ward_has_patient(Ward* w, int patient_id);

// Manager
typedef struct {
    Vector* wards_list;
    HashTable* id_index;
    int next_id;
} WardManager;

WardManager* ward_manager_create(void);
void         ward_manager_destroy(WardManager* wm);
Result       ward_manager_add(WardManager* wm, Ward* w);
Ward*        ward_manager_find_by_id(WardManager* wm, int id);
Ward*        ward_manager_find_by_name(WardManager* wm, const char* name);

// Cleanup helper: Remove a patient from ALL wards (called when patient is deleted)
void         ward_manager_remove_patient_global(WardManager* wm, int patient_id);

#endif // HMS_WARD_H
