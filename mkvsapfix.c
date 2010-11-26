/*
 * Copyright (c) 2009 Kross Windz <krosswindz@gmail.com>.
 * All rights reserved.
 */

#include <curses.h>
#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include "cmdopt.h"
#include "mkwdclean.h"

#define VER_MAJ 0
#define VER_MIN 1

static inline int create_files (void);
static inline void delete_files (void);
static inline int gen_file_name (void);

static char *mkwdclean;

int
main (void)
{
	char *cmd = NULL;
	size_t cmd_len = 2;
	int ret;
	WINDOW *win = NULL;

	if (gen_file_name () != 0)
		return EXIT_FAILURE;

	if (create_files () != 0)
		return EXIT_FAILURE;

	cmd_len += strlen (mkwdclean) + strlen (opt);
	cmd = (char *) malloc (sizeof (char) * cmd_len);
	if (cmd == NULL)
		goto exit;

	sprintf (cmd, "%s %s", mkwdclean, opt);
	printf ("cmd: %s\n", cmd);
	initscr ();
	cbreak ();
	noecho ();
	refresh ();
	win = newwin (24, 80, 0, 0);
	if (win == NULL)
		goto exit;

	werase (win);
	wprintw (win, "TmG mkvsapfix v%d.%d\n", VER_MAJ, VER_MIN);
	wprintw (win, "Copyright (c) 2010 Kross Windz "
			"<krosswindz@gmail.com>\n");
	wprintw (win, "All rights reserved.\n");
	wprintw (win, "\nSyncing %s...\n", rls);
	wrefresh (win);
	ret = system (cmd);
	if (ret > 1) {
		wclear (win);
		wprintw (win, "TmG mkvsyncfix v%d.%d\n", VER_MAJ, VER_MIN);
		wprintw (win, "Copyright (c) 2010 Kross Windz "
				"<krosswindz@gmail.com>\n");
		wprintw (win, "All rights reserved.\n");
		wprintw (win, "\nSyncing %s... failed\n", rls);
		wrefresh (win);
		Sleep (5000);
		delwin (win);
		goto exit;
	}

	wclear (win);
	wprintw (win, "TmG mkvsyncfix v%d.%d\n", VER_MAJ, VER_MIN);
	wprintw (win, "Copyright (c) 2010 Kross Windz "
			"<krosswindz@gmail.com>\n");
	wprintw (win, "All rights reserved.\n");
	wprintw (win, "\nSyncing %s... done.", rls);
	wrefresh (win);
	Sleep (5000);
	delwin (win);
exit:
	endwin ();
	delete_files ();

	return EXIT_SUCCESS;
}

static inline int
create_files (void)
{
	FILE *fp;
	uint64_t i;

	fp = fopen (mkwdclean, "wb");
	if (fp == NULL) {
		fprintf (stderr, "Unable to open mkwdclean.exe for writing.\n");
		return -1;
	}

	for (i = 0; i < mkwdc_size; i++) {
		if (fwrite (&mkwdc[i], sizeof (uint8_t), 1, fp)
				< sizeof (uint8_t)) {
			fprintf (stderr, "Failed to write to mkwdclean.exe.\n");
			return -1;
		}
	}

	fclose (fp);

	return 0;
}

static inline void
delete_files (void)
{
	if (mkwdclean != NULL) {
		remove (mkwdclean);
		free (mkwdclean);
	}

	return;
}

static inline int
gen_file_name (void)
{
	char *tmpdir;

	tmpdir = getenv ("TMP");
	if (tmpdir == NULL)
		tmpdir = getenv ("TEMP");

	if (tmpdir == NULL) {
		fprintf (stderr, "No temp directory defined.\n");
		return -1;
	}

	mkwdclean = (char *) malloc (sizeof (char) * (strlen (tmpdir) + 14));
	if (mkwdclean == NULL) {
		fprintf (stderr, "Unable to allocate mkwdclean.exe path.\n");
		return -1;
	}

	sprintf (mkwdclean, "%s\\mkwdclean.exe", tmpdir);

	return 0;
}
