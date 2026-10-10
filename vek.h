#ifndef VEK_H_
#define VEK_H_

#include <stddef.h>

#ifndef VEK_INIT_CAP
#define VEK_INIT_CAP 128
#endif /* ! VEK_INIT_CAP */

#ifndef VEK_GROWTH_CAP
#define VEK_GROWTH_CAP 2
#endif /* ! VEK_GROWTH_CAP */

typedef struct {
    void *data;
    size_t len;
    size_t cap;
    size_t esize;
} vek_t;

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

vek_t *vek_create(size_t esize);
void   vek_free(vek_t *self);

/* element access */
void  *vek_at(const vek_t *self, size_t i);

/* capacity */
int    vek_reserve(vek_t *self, size_t new_cap);

/* modifiers */
int    vek_push_back(vek_t *self, const void *value);
int    vek_pop_back(vek_t *self, void *out);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* ! VEK_H_ */

#ifdef VEK_IMPLEMENTATION

#include <assert.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

vek_t *vek_create(size_t esize)
{
    assert(esize > 0 && "esize must be > 0");

    vek_t *v = (vek_t *)malloc(sizeof *v);
    if (!v) return NULL;

    v->data = NULL;
    v->len = 0;
    v->cap = 0;
    v->esize = esize;

    if (vek_reserve(v, VEK_INIT_CAP) != 0) {
        free(v);
        return NULL;
    }
    return v;
}

void vek_free(vek_t *self)
{
    free(self->data);
    free(self);
}

static int vek__grow(vek_t *v)
{
    if (v->len < v->cap)
        return 0; /* enough capacity, no need to grow */

    size_t new_cap = (size_t)(v->cap * VEK_GROWTH_CAP);
    if (new_cap <= v->cap)
        new_cap = v->cap + 1;

    return vek_reserve(v, new_cap);
}

static char *vek__slot(const vek_t *v, size_t i)
{
    return (char *)v->data + i * v->esize;
}

void *vek_at(const vek_t *self, size_t i)
{
    if (i >= self->len)
        return NULL; /* index out of range */

    return vek__slot(self, i);
}

int vek_reserve(vek_t *self, size_t new_cap)
{
    if (new_cap <= self->cap)
        return 0;  /* reserve never shrinks */
    if (new_cap > SIZE_MAX / self->esize)
        return -1; /* new_cap * esize would overflow */

    void *tmp = realloc(self->data, new_cap * self->esize);
    if (!tmp)
        return -1;

    self->data = tmp;
    self->cap = new_cap;
    return 0;
}

int vek_push_back(vek_t *self, const void *value)
{
    if (vek__grow(self) != 0)
        return -1;

    memcpy(vek__slot(self, self->len), value, self->esize);
    self->len++;
    return 0;
}

int vek_pop_back(vek_t *self, void *out)
{
    if (self->len == 0)
        return -1; /* vector is empty */

    self->len--;
    if (out)
        memcpy(out, vek__slot(self, self->len), self->esize);
    return 0;
}

#endif /* VEK_IMPLEMENTATION */
