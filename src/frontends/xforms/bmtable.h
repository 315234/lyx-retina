/**
 * \file bmtable.h
 * Copyright 1995-1996 Alejandro Aguilar Sierra
 * This file is part of LyX, the document processor.
 * Licence details can be found in the file COPYING.
 *
 * \author Alejandro Aguilar Sierra
 *
 * Full author contact details are available in file CREDITS
 */

/* A bitmap table uses a single bitmap to simulate a 2d array
 * of bitmap buttons. It can be used to build bitmap menus.
 */

#ifndef BMTABLE_H
#define BMTABLE_H


#include FORMS_H_LOCATION

#if defined(__cplusplus)
extern "C"
{
#endif

/**/
#define FL_BMTABLE 1500
/* A flat bitmap table */
#define FL_BMTABLE_FLAT 0
/* A grided bitmap table */
#define FL_BMTABLE_GRID 1


/*  Same as fl_bitmapbutton */


/**
 *  normal bmtable default
 */
#define FL_BMTABLE_BOXTYPE	FL_UP_BOX


/**/
FL_OBJECT *fl_create_bmtable(int, FL_Coord, FL_Coord,
			     FL_Coord, FL_Coord, char const *);
/**/
FL_OBJECT *fl_add_bmtable(int, FL_Coord, FL_Coord,
			  FL_Coord, FL_Coord, char const *);

/** Same as fl_get_button_numb() */
int fl_get_bmtable_numb(FL_OBJECT *ob);
/**/
void fl_set_bmtable(FL_OBJECT *, int pushed, int pos);

/** Number of columns and rows, and the background bitmap */
void fl_set_bmtable_data(FL_OBJECT *, int, int, int, int, unsigned char const *);
/**/
void fl_set_bmtable_pixmap_data(FL_OBJECT *, int, int, char **);
/**/
void fl_set_bmtable_file(FL_OBJECT *, int, int, char const *);
/**/
void fl_set_bmtable_pixmap_file(FL_OBJECT *, int, int, char const *);

/** Adjust bitmap origin (ox, oy) and cell dimensions (dx, dy) incrementally */
void fl_set_bmtable_adjust(FL_OBJECT *, int ox, int oy, int dx, int dy);

/** The number of items is by default nc x nr, but you can change it */
void fl_set_bmtable_maxitems(FL_OBJECT *, int);
/**/
int fl_get_bmtable_maxitems(FL_OBJECT *);

/** Returns the index of the selected item or -1 if none was selected */
int fl_get_bmtable(FL_OBJECT *);

/** Replace an item's sub_bitmap at id position */
void fl_replace_bmtable_item(FL_OBJECT *ob, int id, int  cw, int ch, char *data);

/** Get the sub_bitmap from id position. You must alloc memory for the data. */
void fl_get_bmtable_item(FL_OBJECT *ob, int id, int *cw, int *ch, char *data);

/** Returns a pixmap from the table, if available. */
Pixmap fl_get_bmtable_pixmap(FL_OBJECT *);

/** Draws a single item on a drawable */
void fl_draw_bmtable_item(FL_OBJECT *ob, int i, Drawable d, int xx, int yy);

/** Free the current bitmap in preparation for installing a new one */
void fl_free_bmtable_bitmap(FL_OBJECT *ob);

/** Free the current pixmap in preparation for installing a new one */
void fl_free_bmtable_pixmap(FL_OBJECT *ob);

#if defined(__cplusplus)
}
#endif

#endif  /* BMTABLE_H */
