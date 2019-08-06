#define _GNU_SOURCE

#include <stdio.h>
#include <dlfcn.h>

void *malloc(size_t size)
{
  fprintf(stderr,"Message: mymalloc!\n");
  void *(*original_malloc)(size_t);
  original_malloc = dlsym(RTLD_NEXT, "malloc");
  
  void *p = NULL;
  //fprintf(stderr, "malloc(%d) = ", size);
  p = original_malloc(size);
  //fprintf(stderr, "%p\n", p);
  return p;
}

void free(void *ptr)
{
  fprintf(stderr,"Message: myfree!\n");
  void (*original_free)(void *ptr);
  original_free = dlsym(RTLD_NEXT, "free");
  original_free(ptr);
  //fprintf(stderr, "freed = %p\n", ptr);
}
