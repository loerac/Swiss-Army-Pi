#include <stdio.h>
#include <stdlib.h>

#include "city_list.h"

/**********************************************
 * See city_list.h for description.
 **********************************************/
unsigned int city_length(const city_list_s *list) {
   unsigned int length = 0;

   while (NULL != list) {
      length++;
      list = list->next;
   }

   return length;
}

/**********************************************
 * See city_list.h for description.
 **********************************************/
city_list_s *city_list_add(city_list_s *list, void *data) {
   city_list_s *new_item = (city_list_s*)malloc(sizeof(city_list_s));

   if (NULL == new_item) {
      printf("EMERG: Allocating memory failed - error(%m)\n");
   } else {
      new_item->data = data;
      new_item->next = list;
      //list = new_item;
   }

   return new_item;
}

/**********************************************
 * See city_list.h for description.
 **********************************************/
city_list_s *city_list_pop(city_list_s **list) {
   city_list_s *pop_item = *list;

   if (NULL != *list) {
      *list = (*list)->next;
   }

   return pop_item;
}
