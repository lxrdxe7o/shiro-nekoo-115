#include "../include/patient.h"
#include "../include/memory.h"
#include "../include/error.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

static char* safe_strdup(const char* src) {
    if (!src) return NULL;
    size_t len = strlen(src) + 1;
    char* dest = (char*)MALLOC(len);
    if (dest) {
        memcpy(dest, src, len);
    }
    return dest;
}

Patient* patient_create(void) {
    Patient* p = (Patient*)CALLOC(1, sizeof(Patient));
    return p;
}

void patient_destroy(void* p_ptr) {
    Patient* p = (Patient*)p_ptr;
    if (!p) return;
    
    FREE(p->name);
    FREE(p->address);
    FREE(p->prescription);
    FREE(p->username);
    FREE(p->password);
    FREE(p->contact);
    FREE(p->emergency_contact);
    
    FREE(p);
}

Patient* patient_clone(const Patient* src) {
    if (!src) return NULL;
    
    Patient* p = patient_create();
    if (!p) return NULL;
    
    p->id = src->id;
    p->age = src->age;
    p->height = src->height;
    p->weight = src->weight;
    
    if (src->name) p->name = safe_strdup(src->name);
    if (src->address) p->address = safe_strdup(src->address);
    if (src->prescription) p->prescription = safe_strdup(src->prescription);
    if (src->username) p->username = safe_strdup(src->username);
    if (src->password) p->password = safe_strdup(src->password);
    if (src->contact) p->contact = safe_strdup(src->contact);
    if (src->emergency_contact) p->emergency_contact = safe_strdup(src->emergency_contact);
    
    strcpy(p->blood_type, src->blood_type);
    
    return p;
}

void patient_set_name(Patient* p, const char* name) {
    if (p->name) FREE(p->name);
    p->name = safe_strdup(name);
}

void patient_set_address(Patient* p, const char* address) {
    if (p->address) FREE(p->address);
    p->address = safe_strdup(address);
}

void patient_set_prescription(Patient* p, const char* prescription) {
    if (p->prescription) FREE(p->prescription);
    p->prescription = safe_strdup(prescription);
}

void patient_set_credentials(Patient* p, const char* user, const char* pass) {
    if (p->username) FREE(p->username);
    if (p->password) FREE(p->password);
    p->username = safe_strdup(user);
    p->password = safe_strdup(pass);
}

void patient_set_contacts(Patient* p, const char* contact, const char* emergency) {
    if (p->contact) FREE(p->contact);
    if (p->emergency_contact) FREE(p->emergency_contact);
    p->contact = safe_strdup(contact);
    p->emergency_contact = safe_strdup(emergency);
}

// -----------------------------------------------------------------------------
// Manager Implementation
// -----------------------------------------------------------------------------

PatientManager* patient_manager_create(void) {
    PatientManager* pm = (PatientManager*)MALLOC(sizeof(PatientManager));
    if (!pm) return NULL;
    
    pm->patients_list = vector_create(16, patient_destroy);
    pm->id_index = hashtable_create(32, NULL); // We don't free values here, they are owned by vector
    pm->username_index = hashtable_create(32, NULL);
    pm->next_id = 1;
    
    return pm;
}

void patient_manager_destroy(PatientManager* pm) {
    if (!pm) return;
    
    vector_destroy(pm->patients_list);
    hashtable_destroy(pm->id_index);
    hashtable_destroy(pm->username_index);
    
    FREE(pm);
}

Result patient_manager_add(PatientManager* pm, Patient* p) {
    Result res = {0, HMS_OK, ""};
    
    if (p->username && hashtable_contains(pm->username_index, p->username)) {
        res.success = 0;
        res.error_code = HMS_ERR_DUPLICATE;
        snprintf(res.error_msg, sizeof(res.error_msg), "Username '%s' already exists", p->username);
        return res;
    }
    
    if (p->id == 0) {
        p->id = pm->next_id++;
    } else if (p->id >= pm->next_id) {
        pm->next_id = p->id + 1;
    }
    
    vector_push(pm->patients_list, p);
    
    // Update indexes
    char id_str[16];
    snprintf(id_str, sizeof(id_str), "%d", p->id);
    hashtable_put(pm->id_index, id_str, p);
    
    if (p->username) {
        hashtable_put(pm->username_index, p->username, p);
    }
    
    res.success = 1;
    return res;
}

Patient* patient_manager_find_by_id(PatientManager* pm, int id) {
    char id_str[16];
    snprintf(id_str, sizeof(id_str), "%d", id);
    return (Patient*)hashtable_get(pm->id_index, id_str);
}

Patient* patient_manager_find_by_username(PatientManager* pm, const char* username) {
    return (Patient*)hashtable_get(pm->username_index, username);
}

Result patient_manager_authenticate(PatientManager* pm, const char* user, const char* pass) {
    Result res = {0, HMS_ERR_AUTH_FAILED, "Invalid username or password"};
    
    Patient* p = patient_manager_find_by_username(pm, user);
    if (!p) return res;
    
    if (p->password && strcmp(p->password, pass) == 0) {
        res.success = 1;
        res.error_code = HMS_OK;
        res.error_msg[0] = '\0';
    }
    
    return res;
}

Patient* patient_manager_find_by_name(PatientManager* pm, const char* name) {
    // Linear search as we don't index by real name (duplicates allowed)
    for (size_t i = 0; i < vector_size(pm->patients_list); i++) {
        Patient* p = (Patient*)vector_get(pm->patients_list, i);
        if (p->name && strcasecmp(p->name, name) == 0) {
            return p;
        }
    }
    return NULL;
}

Result patient_manager_remove(PatientManager* pm, int id) {
    Result res = {0, HMS_ERR_NOT_FOUND, "Patient not found"};
    
    Patient* p = patient_manager_find_by_id(pm, id);
    if (!p) return res;
    
    // Remove from indexes
    char id_str[16];
    snprintf(id_str, sizeof(id_str), "%d", id);
    hashtable_remove(pm->id_index, id_str);
    
    if (p->username) {
        hashtable_remove(pm->username_index, p->username);
    }
    
    // Remove from vector - THIS IS SLOW (O(N)), but necessary to keep vector clean
    // For a real DB we wouldn't scan, but for in-memory vector it's fine for small N
    int index = -1;
    for (size_t i = 0; i < vector_size(pm->patients_list); i++) {
        if (vector_get(pm->patients_list, i) == p) {
            index = i;
            break;
        }
    }
    
    if (index != -1) {
        // vector_remove will free the patient!
        vector_remove(pm->patients_list, index);
        res.success = 1;
        res.error_code = HMS_OK;
        res.error_msg[0] = '\0';
    }
    
    return res;
}

Vector* patient_manager_search(PatientManager* pm, const char* query) {
    Vector* results = vector_create(8, NULL); // Doesn't own the patients
    
    for (size_t i = 0; i < vector_size(pm->patients_list); i++) {
        Patient* p = (Patient*)vector_get(pm->patients_list, i);
        if ((p->name && strcasestr(p->name, query)) || 
            (p->username && strcasestr(p->username, query))) {
            vector_push(results, p);
        }
    }
    
    return results;
}
