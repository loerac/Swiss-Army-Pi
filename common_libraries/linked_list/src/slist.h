#ifndef _SLIST_H_
#define _SLIST_H_

typedef struct slist {
   void *data;
   struct slist *next;
} slist_s;

slist_s *slistAppend(slist_s *list, void *data);

slist_s *slistPrepend(slist_s *list, void *data);

slist_s *slistSearch(slist_s *list, void *data);

slist_s *slistNext(slist_s *list);

slist_s *slistGetLast(slist_s *list);

unsigned int slistLength(slist_s *list);

slist_s *slistRemove(slist_s **list, void *data);

void slistDelete(slist_s **list);

#endif /* _SLIST_H_ */

