/* llist.h: a CPP-based template implementation of singly-linked list */

#ifndef LLIST_H_INCLUDED
#define LLIST_H_INCLUDED

#include <stdlib.h>

#define LLIST_PROTO(T, N) \
	typedef struct N##_pair { T car; struct N##_pair *cdr; } N##_pair; \
	typedef struct N { int len; struct N##_pair *first; struct N##_pair *last; } N; \
	typedef struct N##_pair *N##_iterator; \
	N *N##_new(void); \
	void N##_free(N *s); \
	N##_pair *N##_pair_new(T item); \
	int N##_insert(N *s, T item, int pos); \
	T N##_pop(N *s, int pos); \
	T N##_get(const N *s, int pos); \
	void N##_set(N *s, T item, int pos); \
	N##_iterator N##_iterate(const N *s); \
	int N##_next(const N *s, N##_iterator *iter); \
	T N##_get_at(const N *s, N##_iterator iter); \
	void N##_set_at(N *s, T item, N##_iterator iter); \
	int N##_insert_after(N *s, T item, N##_iterator iter); \
	T N##_pop_after(N *s, N##_iterator iter)

#define LLIST(T, N) \
	N *N##_new(void) \
	{ \
		N *s; \
		s = malloc(sizeof(struct N)); \
		if (!s) return NULL; \
		s->len = 0; \
		s->first = NULL; \
		s->last = NULL; \
		return s; \
	} \
	void N##_free(N *s) \
	{ \
		N##_pair *p, *temp; \
		temp = NULL; \
		for (p=s->first; p; ) { \
			temp = p; \
			p = p->cdr; \
			free(temp); \
		} \
		free(s); \
	} \
	N##_pair *N##_pair_new(T item) \
	{ \
		N##_pair *p; \
		p = malloc(sizeof(struct N##_pair)); \
		if (!p) return NULL; \
		p->car = item; \
		p->cdr = NULL; \
		return p; \
	} \
	int N##_insert(N *s, T item, int pos) \
	{ \
		int i; \
		N##_pair *newp, *p; \
		newp = N##_pair_new(item); \
		if (!newp) return 0; \
		if (!s->first) { \
			s->first = s->last = newp; \
			++s->len; \
			return 1; \
		} \
		if (pos < 0 || pos == s->len) { \
			s->last->cdr = newp; \
			s->last = newp; \
			++s->len; \
			return 1; \
		} \
		if (pos == 0) { \
			newp->cdr = s->first; \
			s->first = newp; \
			++s->len; \
			return 1; \
		} \
		for (p=s->first, i=0; i<pos-1 && p; ++i) p=p->cdr; \
		if (!p) { \
			free(newp); \
			return 0; \
		} \
		newp->cdr = p->cdr; \
		p->cdr = newp; \
		++s->len; \
		if (!newp->cdr || p == s->last) { \
			s->last = newp; \
		} \
		return 1; \
	} \
	T N##_pop(N *s, int pos) \
	{ \
		N##_pair *p, *p2; \
		T temp; \
		int i; \
		if (pos == 0) { \
			p = s->first; \
			s->first = p->cdr; \
			temp = p->car; \
			if (!p->cdr) s->last = NULL; \
			free(p); \
			--s->len; \
			return temp; \
		} \
		if (pos < 0) { \
			pos = s->len - 1; \
		} \
		for (p=s->first, i=0; i<pos-1 && p; ++i) p=p->cdr; \
		temp = p->cdr->car; \
		p2 = p->cdr; \
		p->cdr = p2->cdr; \
		if (!p->cdr) { \
			s->last = p; \
		} \
		free(p2); \
		--s->len; \
		return temp; \
	} \
	T N##_get(const N *s, int pos) \
	{ \
		int i; \
		N##_pair *p; \
		if (pos == 0) { \
			return s->first->car; \
		} \
		if (pos < 0) { \
			return s->last->car; \
		} \
		for (p=s->first, i=0; i<pos && p; ++i) p=p->cdr; \
		return p->car; \
	} \
	void N##_set(N *s, T item, int pos) \
	{ \
		int i; \
		N##_pair *p; \
		if (pos == 0) { \
			s->first->car = item; \
		} else if (pos < 0) { \
			s->last->car = item; \
		} else { \
			for (p=s->first, i=0; i<pos && p; ++i) p=p->cdr; \
			p->car = item; \
		} \
	} \
	N##_iterator N##_iterate(const N *s) \
	{ \
		return NULL; \
	} \
	int N##_next(const N *s, N##_iterator *iter) \
	{ \
		if (!*iter) { \
			*iter = s->first; \
			return 1; \
		} \
		if (!(*iter)->cdr) { \
			return 0; \
		} \
		*iter = (*iter)->cdr; \
		return 1; \
	} \
	T N##_get_at(const N *s, N##_iterator iter) \
	{ \
		return iter->car; \
	} \
	void N##_set_at(N *s, T item, N##_iterator iter) \
	{ \
		iter->car = item; \
	} \
	int N##_insert_after(N *s, T item, N##_iterator iter) \
	{ \
		N##_pair *newp; \
		newp = N##_pair_new(item); \
		if (!newp) return 0; \
		++s->len; \
		if (!iter) { \
			newp->cdr = s->first; \
			if (!s->last) s->last = iter; \
			s->first = iter; \
			return 1; \
		} \
		newp->cdr = iter->cdr; \
		iter->cdr = newp; \
		if (iter == s->last) { \
			s->last = newp; \
		} \
		return 1; \
	} \
	T N##_pop_after(N *s, N##_iterator iter) \
	{ \
		T val; \
		N##_pair *temp; \
		--s->len; \
		if (!iter) { \
			temp = s->first; \
			val = temp->car; \
			s->first = temp->cdr; \
			if (!s->first) s->last = NULL; \
		} else { \
			temp = iter->cdr; \
			val = temp->car; \
			iter->cdr = temp->cdr; \
			if (temp == s->last) s->last = iter; \
		} \
		free(temp); \
		return val; \
	} \
	struct N /* to avoid extra semicolon outside of a function */

#endif /* ifndef LLIST_H_INCLUDED */
