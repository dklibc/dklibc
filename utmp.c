/* Functions to read/search/write /var/run/utmp */

#include <utmp.h>
#include <stdio.h>
#include <string.h>

static struct utmp utent;
static FILE *fp;

/* Open utmp file or rewind it to start */
void setutent (void)
{
	if (fp) {
		if (fseek (fp, 0, SEEK_SET) < 0) {
			fclose (fp);
			fp = NULL;
		}
		return;
	}
	
	fp = fopen ("/root/denix/utmp", "r+");
}

/* Close utmp file */
void endutent (void)
{
	fclose (fp);
	fp = NULL;
}

/*
Get next entry. Return NULL in case of EOF or error
*/
struct utmp *getutent (void)
{
	if (fread ((char *)&utent, sizeof(utent), 1, fp) != 1)
		return NULL;
	
	return &utent;
}

/*
Find the fisrt utmp entry from the current position that ut_id is equal 
to ut->ut_id. Return NULL in case of not found or error
*/
struct utmp *getutid (struct utmp *ut)
{	
	struct utmp *p;
	
	setpwent();

	while (p = getutent()) {
		if (p->ut_id == ut->ut_id)
			break;
	}	

	endpwent();
	return p;
}

/*
Find the first utmp entry from the current position that ut_line is 
equal to ut->ut_line. Return NULL in case of error or not found
*/
struct utmp *getutline (struct utmp *ut)
{	
	struct utmp *p;
	
	setpwent();

	while (p = getutent()) {
		if (!strcmp(p->ut_line, ut->ut_line))
			break;
	}	

	endpwent();
	return p;
}

/*
Write utmp entry to file. Find file position by getutline(). If 
getutline() return NULL append the entry to the end of file
*/
struct utmp *pututline (struct utmp *ut)
{
	struct utmp *p;
	int pos;
	
	pos = ftell(fp);

	p = getutline(ut);
	if (p) { // Found
		if (fseek(fp, -sizeof(*ut), SEEK_CUR) ||
fwrite ((char *)&ut, sizeof(*ut), 1, fp) != 1 || fflush (fp))
			return NULL;
	} else { // Not found - append it
		if (fseek (fp, 0, SEEK_END) ||
fwrite ((char *)&ut, sizeof(*ut), 1, fp) != 1 || fflush (fp))
		return NULL;
	}
	fseek (fp, pos, SEEK_SET); 
	return ut;
}

