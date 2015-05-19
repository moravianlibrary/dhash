#include "dhash.h"

#include <string.h>

#include <wand/MagickWand.h>

const char* err_type_strings[] = {
    "OK",
    "ImageMagick error"
};

void dhash_init() {
    MagickWandGenesis();
}

void dhash_final() {
    MagickWandTerminus();
}

const char* dhash_err_type_str(const dhash_err_type err_type) {
    return err_type_strings[err_type];
}

dhash_err* dhash_new_err() {
    dhash_err* err = (dhash_err*) malloc(sizeof(dhash_err));
    err->err_type = OK;
    err->description = NULL;
    return err;
}

void dhash_free_err(dhash_err* err) {
    if (err == NULL) {
        return;
    }
    if (err->description) {
        free(err->description);
    }
    free(err);
}

void set_mw_err(MagickWand* magick_wand, dhash_err* error) {
    fprintf(stderr, "called set_mw_err");
    ExceptionType severity;
    char* description;
    char* err_description;
    description = MagickGetException(magick_wand, &severity);
    err_description = (char*) malloc(sizeof(char) * strlen(description) + 1);
    strcpy(err_description, description);
    description = (char *) MagickRelinquishMemory(description);
    error->err_type = IMAGE_MAGICK_ERROR;
    if (error->description) {
        free(error->description);
    }
    error->description = err_description;
}

static inline void addOneBit(uint64_t* value, const size_t pos) {
    *value |= (uint64_t) 1 << (63 - pos);
}

uint64_t dhash_compute_internal(MagickWand* magick_wand, dhash_err* error) {
    MagickBooleanType status;
    PixelWand** pixels;
    size_t width;
    size_t pos = 0;
    uint64_t hash = 0;
    // Resize
    status = MagickResizeImage(magick_wand, 9, 8, LanczosFilter, 1.0);
    if (status == MagickFalse) {
        set_mw_err(magick_wand, error);
        return 0;
    }
    // Transform to grayscale
    status = MagickTransformImageColorspace(magick_wand, GRAYColorspace);
    if (status == MagickFalse) {
        set_mw_err(magick_wand, error);
        return 0;
    }
    PixelIterator* iterator = NewPixelIterator(magick_wand);
    if (iterator == NULL) {
        set_mw_err(magick_wand, error);
        return 0;
    }
    for (size_t y = 0; y < MagickGetImageHeight(magick_wand); y++) {
        pixels = PixelGetNextIteratorRow(iterator, &width);
        if (pixels == NULL) {
            break;
        }
        for (size_t x = 0; x < width - 1; x++) {
            double lbright = PixelGetRed(pixels[x]);
            double rbright = PixelGetRed(pixels[x + 1]);

            if (lbright > rbright) {
                addOneBit(&hash, pos);
            }
            pos++;
        }
    }
    iterator = DestroyPixelIterator(iterator);

    error->err_type = OK;
    return hash;
}

uint64_t dhash_compute_filename(const char* filename, dhash_err* error) {
    MagickBooleanType status;
    MagickWand *magick_wand;

    magick_wand = NewMagickWand();
    status = MagickReadImage(magick_wand, filename);
    if (status == MagickFalse) {
        set_mw_err(magick_wand, error);
        magick_wand = DestroyMagickWand(magick_wand);
        return 0;
    }
    uint64_t hash = dhash_compute_internal(magick_wand, error);
    magick_wand = DestroyMagickWand(magick_wand);

    return hash;
}

uint64_t dhash_compute_filedesc(FILE* file, dhash_err* error) {
    MagickBooleanType status;
    MagickWand *magick_wand;

    magick_wand = NewMagickWand();
    status = MagickReadImageFile(magick_wand, file);
    if (status == MagickFalse) {
        set_mw_err(magick_wand, error);
        magick_wand = DestroyMagickWand(magick_wand);
        return 0;
    }
    uint64_t hash = dhash_compute_internal(magick_wand, error);
    magick_wand = DestroyMagickWand(magick_wand);

    return hash;
}

uint64_t dhash_compute_blob(const void* blob, const size_t length, dhash_err* error) {
    MagickBooleanType status;
    MagickWand *magick_wand;

    magick_wand = NewMagickWand();
    status = MagickReadImageBlob(magick_wand, blob, length);
    if (status == MagickFalse) {
        set_mw_err(magick_wand, error);
        magick_wand = DestroyMagickWand(magick_wand);
        return 0;
    }
    uint64_t hash = dhash_compute_internal(magick_wand, error);
    magick_wand = DestroyMagickWand(magick_wand);

    return hash;
}
