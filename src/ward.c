#include "../include/ward.h"
#include "../include/memory.h"
#include "../include/error.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

static char* safe_strdup(const char* src) {
    if (!src) return NULL;
    size_t len = strlen(src) + 1;
    char* dest = (char*)MALLOC(len);
    if (dest) memcpy(dest, src, len);
    return dest;
}

Ward* ward_create(const char* name, int capacity) {
    Ward* w = (Ward*)CALLOC(1, sizeof(Ward));
    if (!w) return NULL;
    
    w->name = safe_strdup(name);
    w->capacity = capacity;
    w->patients = vector_create(capacity, NULL); // Non-owning vector
    
    return w;
}

void ward_destroy(void* w_ptr) {
    Ward* w = (Ward*)w_ptr;
    if (!w) return;
    
    FREE(w->name);
    vector_destroy(w->patients); // Doesn't free patients, just the vector
    FREE(w);
}

Result ward_add_patient(Ward* w, Patient* p) {
    Result res = {0, HMS_OK, ""};
    
    if (ward_is_full(w)) {
        res.error_code = HMS_ERR_CAPACITY_FULL;
        snprintf(res.error_msg, sizeof(res.error_msg), "Ward '%s' is full", w->name);
        return res;
    }
    
    if (ward_has_patient(w, p->id)) {
        res.error_code = HMS_ERR_DUPLICATE;
        snprintf(res.error_msg, sizeof(res.error_msg), "Patient already in ward");
        return res;
    }
    
    vector_push(w->patients, p);
    res.success = 1;
    return res;
}

Result ward_remove_patient(Ward* w, int patient_id) {
    Result res = {0, HMS_ERR_NOT_FOUND, "Patient not found in ward"};
    
    for (size_t i = 0; i < vector_size(w->patients); i++) {
        Patient* p = (Patient*)vector_get(w->patients, i);
        if (p->id == patient_id) {
            vector_remove(w->patients, i); // Doesn't free p because valid_free_fn is NULL
            res.success = 1;
            res.error_code = HMS_OK;
            return res;
        }
    }
    
    return res;
}

bool ward_is_full(Ward* w) {
    return vector_size(w->patients) >= (size_t)w->capacity;
}

bool ward_has_patient(Ward* w, int patient_id) {
    for (size_t i = 0; i < vector_size(w->patients); i++) {
        Patient* p = (Patient*)vector_get(w->patients, i);
        if (p->id == patient_id) return true;
    }
    return false;
}

// -----------------------------------------------------------------------------
// Manager
// -----------------------------------------------------------------------------

WardManager* ward_manager_create(void) {
    WardManager* wm = (WardManager*)MALLOC(sizeof(WardManager));
    if (!wm) return NULL;
    
    wm->wards_list = vector_create(4, ward_destroy);
    wm->id_index = hashtable_create(8, NULL);
    wm->next_id = 1;
    
    return wm;
}

void ward_manager_destroy(WardManager* wm) {
    if (!wm) return;
    
    vector_destroy(wm->wards_list); // Destroys all wards
    hashtable_destroy(wm->id_index);
    FREE(wm);
}

Result ward_manager_add(WardManager* wm, Ward* w) {
    Result res = {0, HMS_OK, ""};
    
    if (w->id == 0) {
        w->id = wm->next_id++;
    } else if (w->id >= wm->next_id) {
        wm->next_id = w->id + 1;
    }
    
    vector_push(wm->wards_list, w);
    
    char id_str[16];
    snprintf(id_str, sizeof(id_str), "%d", w->id);
    hashtable_put(wm->id_index, id_str, w);
    
    res.success = 1;
    return res;
}

Ward* ward_manager_find_by_id(WardManager* wm, int id) {
    char id_str[16];
    snprintf(id_str, sizeof(id_str), "%d", id);
    return (Ward*)hashtable_get(wm->id_index, id_str);
}

Ward* ward_manager_find_by_name(WardManager* wm, const char* name) {
    for (size_t i = 0; i < vector_size(wm->wards_list); i++) {
        Ward* w = (Ward*)vector_get(wm->wards_list, i);
        if (strcasecmp(w->name, name) == 0) return w;
    }
    return NULL;
}

void ward_manager_remove_patient_global(WardManager* wm, int patient_id) {
    for (size_t i = 0; i < vector_size(wm->wards_list); i++) {
        Ward* w = (Ward*)vector_get(wm->wards_list, i);
        ward_remove_patient(w, patient_id);
    }
}
