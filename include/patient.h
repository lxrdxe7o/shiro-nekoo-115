#ifndef HMS_PATIENT_H
#define HMS_PATIENT_H

#include <stddef.h>
#include "types.h"
#include "vector.h"
#include "hashtable.h"

typedef struct {
    int id;
    char* name;              // Dynamically allocated
    char* address;           // Dynamically allocated
    char* prescription;      // Dynamically allocated
    char* username;          // Dynamically allocated
    char* password;          // Dynamically allocated
    char* contact;           // Dynamically allocated
    char* emergency_contact; // Dynamically allocated
    char blood_type[8];      // Small enough to keep static
    int age;
    float height;
    float weight;
} Patient;

// Lifecycle
Patient* patient_create(void);
Patient* patient_clone(const Patient* src);
void     patient_destroy(void* p_ptr); // Void* for compatibility with generic containers

// Accessors (deep copies/safe access)
void     patient_set_name(Patient* p, const char* name);
void     patient_set_address(Patient* p, const char* address);
void     patient_set_prescription(Patient* p, const char* prescription);
void     patient_set_credentials(Patient* p, const char* user, const char* pass);
void     patient_set_contacts(Patient* p, const char* contact, const char* emergency);

// Manager for collection of patients
typedef struct {
    Vector* patients_list;    // For iteration/indexing
    HashTable* id_index;      // For O(1) lookup by ID (stringified)
    HashTable* username_index;// For O(1) lookup by username
    int next_id;
} PatientManager;

PatientManager* patient_manager_create(void);
Result          patient_manager_add(PatientManager* pm, Patient* p);
Patient*        patient_manager_find_by_name(PatientManager* pm, const char* name);
Patient*        patient_manager_find_by_id(PatientManager* pm, int id);
Patient*        patient_manager_find_by_username(PatientManager* pm, const char* username);
Result          patient_manager_remove(PatientManager* pm, int id);
Vector*         patient_manager_search(PatientManager* pm, const char* query);
Result          patient_manager_authenticate(PatientManager* pm, const char* user, const char* pass);
void            patient_manager_destroy(PatientManager* pm);

#endif // HMS_PATIENT_H
