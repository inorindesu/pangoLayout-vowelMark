/* 
 * pangoLayout
 *
 * Copyright (C) 2013  Inori Sakura <inorindesu@gmail.com>
 * 
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or (at
 * your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 * 
 */

#include <stdio.h>
#include <pango/pango.h>
#include <pango/pangocairo.h>
#include <cairo/cairo-pdf.h>

/*
  The string.
  From first sentence of
  http://www.lotsawahouse.org/bo/tibetan-masters/mipham/prayer-to-gesar
 */
char string[] = "གེ་སར་གསོལ་འདེབས་བཞུགས།";
/* 
   We'd like to mark the 'o' vowel mark
   to red.
   
   The character index is recorded here.
*/
int markCharIndex = 8; 
char string_markup[] = "གེ་སར་ག<span foreground='#FF0000'>ོ</span>སལ་འདེབས་བཞུགས།";

int main(int argc, char** argv)
{
  char* outputFile = "/tmp/out.pdf";
  if (argc == 2)
    {
      outputFile = argv[1];
    }
  
  cairo_t* cr;
  cairo_surface_t* surface;
  surface = cairo_pdf_surface_create(outputFile, 10, 10);
  cr = cairo_create(surface);
  
  PangoLayout* layout;
  layout = pango_cairo_create_layout(cr);
  
  /* Both of the ways cannot mark the vowel only...*/
  /*
  pango_layout_set_text(layout, string, -1);
  PangoAttrList* attrs = pango_attr_list_new();
  PangoAttribute* colorAttr =  pango_attr_foreground_new(65535, 0.0, 0.0);
  colorAttr->start_index = g_utf8_offset_to_pointer(string, markCharIndex - 1) - string;
  colorAttr->end_index = g_utf8_offset_to_pointer(string, markCharIndex) - string;
  pango_attr_list_insert(attrs, colorAttr);
  pango_layout_set_attributes(layout, attrs);
  pango_attr_list_unref(attrs);
  */
  pango_layout_set_markup(layout, string_markup, -1);
  
  PangoRectangle rect;
  pango_layout_get_extents(layout, &rect, NULL);
  fprintf(stderr, "Width: %d (%f), Height: %d (%f)\n", rect.width, (float)rect.width / PANGO_SCALE,
          rect.height, (float)rect.height / PANGO_SCALE);
  cairo_pdf_surface_set_size(surface, (float)rect.width / PANGO_SCALE, (float)rect.height / PANGO_SCALE);
  fprintf(stderr, "X, Y = (%d, %d) [%f, %f]\n", rect.x, (float)rect.x / PANGO_SCALE, 
          rect.y, (float)rect.y / PANGO_SCALE);
  cairo_translate(cr, (float)-rect.x / PANGO_SCALE, (float)-rect.y / PANGO_SCALE);
  pango_cairo_show_layout(cr, layout);
  cairo_surface_show_page(surface);
  
  cairo_destroy(cr);
  cairo_surface_destroy(surface);
}
