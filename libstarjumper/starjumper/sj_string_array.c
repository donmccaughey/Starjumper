#include "sj_string_array.h"

#include <stddef.h>

#include "alloc_or_die.h"


struct sj_string_array *
sj_string_array_alloc_collect_strings(void const *array,
                                      int count,
                                      size_t element_size,
                                      sj_string_array_alloc_string_for_element alloc_string_for_element)
{
  struct sj_string_array *string_array = malloc_or_die(sizeof(struct sj_string_array));
  string_array->elements = calloc_or_die(count + 1, sizeof(char *));
  string_array->count = (int) count;
  
  for (size_t i = 0; i < count; ++i) {
    size_t element_offset = element_size * i;
    void const *element = array + element_offset;
    string_array->elements[i] = alloc_string_for_element(element);
  }
  
  size_t null_terminator_index = count;
  string_array->elements[null_terminator_index] = NULL;
  
  return string_array;
}


void
sj_string_array_free(struct sj_string_array *string_array)
{
  for (int i = 0; i < string_array->count; ++i) {
    free_or_die(string_array->elements[i]);
  }
  free_or_die(string_array->elements);
  free_or_die(string_array);
}
