#include <stdlib.h>
#include <X11/Xft/Xft.h>
#include "../kernel/openbox.h"
#include "font.h"

#include "../src/gettext.h"
#define _(str) gettext(str)

void font_startup(void)
{
#ifdef DEBUG
    int version;
#endif /* DEBUG */
    if (!XftInit(0)) {
        g_warning(_("Couldn't initialize Xft.\n\n"));
        exit(3);
    }
#ifdef DEBUG
    version = XftGetVersion();
    g_message("Using Xft %d.%d.%d (Built against %d.%d.%d).\n",
              version / 10000 % 100, version / 100 % 100, version % 100,
              XFT_MAJOR, XFT_MINOR, XFT_REVISION);
#endif
}

ObFont *font_open(char *fontstring)
{
    ObFont *out;
    XftFont *xf;
    
    if ((xf = XftFontOpenName(ob_display, ob_screen, fontstring))) {
        out = malloc(sizeof(ObFont));
        out->xftfont = xf;
        return out;
    }
    g_warning(_("Unable to load font: %s\n"), fontstring);
    g_warning(_("Trying fallback font: %s\n"), "fixed");

    if ((xf = XftFontOpenName(ob_display, ob_screen, "fixed"))) {
        out = malloc(sizeof(ObFont));
        out->xftfont = xf;
        return out;
    }
    g_warning(_("Unable to load font: %s\n"), "fixed");
    g_warning(_("Aborting!.\n"));

    exit(3); // can't continue without a font
}


void font_close(ObFont *f)
{
    XftFontClose(ob_display, f->xftfont);
}


int font_measure_string(ObFont *f, const char *str, int shadow, int offset)
{
    XGlyphInfo info;

    XftTextExtentsUtf8(ob_display, f->xftfont,
                       (FcChar8*)str, strlen(str), &info);

    return (signed) info.xOff + (shadow ? offset : 0);
}


int font_height(ObFont *f, int shadow, int offset)
{
  return (signed) f->xftfont->height + (shadow ? offset : 0);
}


int font_max_char_width(ObFont *f)
{
  return (signed) f->xftfont->max_advance_width;
}
