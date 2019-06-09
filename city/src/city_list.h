#ifndef _CITY_LIST_H_
#define _CITY_LIST_H_

/**********************************************
 * INPUT:
 *    list
 *       City linked list
 * OUTPUT:
 *    NONE
 * RETURN:
 *    The next item in the list. NULL if it's empty
 * DESCRIPTION:
 *    See RETURN for description.
 **********************************************/
#define city_list_next(list)  ((NULL != list) ? (((city_list_s *)(list))->next) : NULL)

typedef struct city_list {
    void *data;
    struct city_list *next;
} city_list_s;

/**********************************************
 * INPUT:
 *    list
 *       City linked list
 * OUTPUT:
 *    NONE
 * RETURN:
 *     0: List is empty
 *    1+: Length of the list
 * DESCRIPTION:
 *    Gets the length of the list.
 **********************************************/
unsigned int city_list_length(const city_list_s *list);

/**********************************************
 * INPUT:
 *    list
 *       City linked list
 *    data
 *       Data being added to the list
 * OUTPUT:
 *    NONE
 * RETURN:
 *    Updated city list on success, else NULL
 * DESCRIPTION:
 *    Data is being prepended into the list.
 **********************************************/
city_list_s *city_list_add(city_list_s *list, void *data);

/**********************************************
 * INPUT:
 *    NONE
 * OUTPUT:
 *    list
 *       First item in the city list removed
 * RETURN:
 *    The item that was deleted
 * DESCRIPTION:
 *    Removes the first item in the city list.
 **********************************************/
city_list_s *city_list_pop(city_list_s **list);

#endif /* _CITY_LIST_H_*/
