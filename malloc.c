#include <unistd.h>

void *_heap_beg, *_heap_end;

struct blk_hdr {
	int sz;
	struct blk_hdr *pnext;
}; 

static struct blk_hdr *pfree; // Free list

#define SBRK_CHUNK 4096

void *malloc (int n)
{
	struct blk_hdr *p, *q, *prev;
	int m;
	
	// Adjust size to be multiple of 4
	n = ((n + 3) / 4) * 4 + sizeof(struct blk_hdr);

	// Look for large enough free block
	p = pfree;
	prev = 0;
	while (p) {
		if (p->sz >= n)
			break;
		prev = p;
		p = p->pnext;
	}

	if (!p) { // Not found - enlarge heap boundary
		p = (struct blk_hdr *)_heap_end;
		m = (SBRK_CHUNK > n) ? SBRK_CHUNK : n;
		if (sbrk (m) < 0)
			return (void *)0;
		p->sz = m;
		p->pnext = 0;
		if (!pfree)
			pfree = p;
	}

	p->sz -= n;
	if (!(p->sz)) { // We need entire free block
		if (!prev)
			pfree = p->pnext;
		else
			prev->pnext = p->pnext;
	}
	
	q = (struct blk_hdr *)((char *)p + p->sz);
	q->pnext = 0;
	q->sz = n;
	return ++q;
}

void free (void *ptr)
{
	struct blk_hdr *p, *q, *prev;

	p = (struct blk_hdr *)((char *)ptr - sizeof(struct blk_hdr));

	q = pfree;
	prev = 0;
	while (q) {
		if (p < q)
			break;
		prev = q;
		q = q->pnext;
	}

	p->pnext = q;
	if ((char *)p + p->sz == (char *)q) { // Absorb right neighbour
		p->pnext = q->pnext;
		p->sz += q->sz;
	}

	if (prev) {
		if ((char *)prev + prev->sz == (char*)p) {
			// Left neighbour absorb us
			prev->pnext = p->pnext;
			prev->sz += p->sz;
		} else
			prev->pnext = p;
	} else
		pfree = p;
}
