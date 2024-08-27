#ifndef HITTABLE_LIST_H
#define HITTABLE_LIST_H 

#include "hittable.h" 
#include <stdbool.h>
#include <stdlib.h>

typedef struct {
    Hittable base;
    Hittable** objects;
    size_t size;
    size_t capacity;
} Hittable_List_t;

static inline void hittable_list_destroy(Hittable_List_t* list) {
    if (list != NULL) {
        free(list->objects);
        free(list);
    }
}

static inline void hittable_list_clear(Hittable_List_t* list) {
    for (size_t i = 0; i < list->size; i++) {
        free(list->objects[i]);
    }
    free(list->objects);
    list->objects = NULL;
    list->size = 0;
    list->capacity = 0;
}

static inline void hittable_list_add(Hittable_List_t* list, Hittable* object) {
    if (list->size == list->capacity) {
        size_t new_capacity = list->capacity == 0 ? 1 : list->capacity * 2;
        Hittable** new_objects = realloc(list->objects, new_capacity * sizeof(Hittable*));
        if (new_objects == NULL) {
            return;
        }
        list->objects = new_objects;
        list->capacity = new_capacity;
    }
    list->objects[list->size++] = object;
}

static inline bool hittable_list_hit(Hittable* hittable, Ray_t ray, Interval_t ray_t, Hit_Record* rec) {
    Hittable_List_t* list = (Hittable_List_t*)hittable;
    Hit_Record temp_rec;
    bool hit_anything = false;
    double closest_so_far = ray_t.max;

    for (size_t i = 0; i < list->size; i++) {
        if (list->objects[i]->hit(list->objects[i], ray, Interval(ray_t.min, closest_so_far), &temp_rec)) {
            hit_anything = true;
            closest_so_far = temp_rec.t;
            *rec = temp_rec;
        }
    }
    return hit_anything;
}

static inline void hittable_list_init(Hittable_List_t* list) {
    list->base.hit = hittable_list_hit;
    list->objects = NULL;
    list->size = 0;
    list->capacity = 0;
}

static inline Hittable_List_t* Hittable_List() {
    Hittable_List_t* list = (Hittable_List_t*)malloc(sizeof(Hittable_List_t));
    if (list != NULL) {
        hittable_list_init(list);
    }
    return list;
} 

#endif // HITTABLE_LIST_H
