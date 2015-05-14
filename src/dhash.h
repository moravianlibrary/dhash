#ifndef DHASH_H
#define DHASH_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stdio.h>

typedef enum {
    OK,
    IMAGE_MAGICK_ERROR
} dhash_err_type;

typedef struct {
    dhash_err_type err_type;
    char* description;
} dhash_err;

void dhash_init();

void dhash_final();

const char* dhash_err_type_str(const dhash_err_type err_type);

dhash_err* dhash_new_err();

void dhash_free_err(dhash_err* err);

uint64_t dhash_compute_filename(const char* filename, dhash_err* error);

uint64_t dhash_compute_filedesc(FILE* file, dhash_err* error);

uint64_t dhash_compute_blob(const void* blob, const size_t length, dhash_err* error);

#ifdef __cplusplus
}
#endif

#endif
