#include <stdio.h>

#define VEK_INIT_CAP 1
#define VEK_IMPLEMENTATION
#include "vek.h"

int main(void)
{
    vek_t *v = vek_create(sizeof(int));
    if (!v) {
        fprintf(stderr, "Failed to create vector\n");
        return 1;
    }

    vek_pop_back(v, NULL); /* should do nothing */

    size_t len = 0;
    scanf("%zu", &len);

    for (size_t i = 0; i < len; ++i) {
        int x = 0;

        scanf("%d", &x);
        vek_push_back(v, &x);
    }

    int value = 0;
    vek_pop_back(v, &value);
    printf("Popped value: %d\n", value);

    puts("Vector contents:");
    for (size_t i = 0; i < v->len; ++i) {
        int *elem = (int *)vek_at(v, i);
        printf("v[%zu] = %d\n", i, *elem);
    }

    printf("v->len = %zu\n", v->len);
    printf("v->cap = %zu\n", v->cap);

    vek_free(v);
    return 0;
}