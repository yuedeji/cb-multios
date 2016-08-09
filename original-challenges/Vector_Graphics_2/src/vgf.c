/*

Author: Debbie Nuttall <debbie@cromulence.co>

Copyright (c) 2015 Cromulence LLC

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.

*/
#include <libcgc.h>
#include "stdlib.h"
#include "stdint.h"
#include "vgf.h"
#include "paint.h"

// Verify VGF Header
// Returns 0 for success, -1 for failure
int cgc_VGFVerify(cgc_VGF_Header *header) {
  if (header->magic != VGF_MAGIC) {
    return -1;
  }
  if (header->version != VGF_VERSION) {
    return -1;
  }
  if (header->y_size > VGF_MAX_HEIGHT) {
    return -1;
  }
  if (header->x_size > VGF_MAX_WIDTH) {
    return -1;
  }
  return 0;
}

// Process VGF file contents and render the image objects to the canvas
// Returns 0 for success, -1 for failure
int cgc_VGFProcess(cgc_uint8_t **file_data_in, cgc_uint8_t *file_end, cgc_Canvas *c) {
  cgc_uint8_t *file_data = *file_data_in;
  while (file_data < file_end) {
    cgc_VGF_Object *o = (cgc_VGF_Object *)file_data;
    file_data += sizeof(cgc_VGF_Object);
    
    if (o->type == VGF_OBJECT_LAST) {
      *file_data_in = file_data;
      return 0;
    }

    if (o->layer >= CANVAS_MAX_LAYERS) {
      continue;
    }

    if (o->color >= CANVAS_MAX_COLORS) {
      continue;
    }

    switch (o->type) {
      case VGF_OBJECT_TRIANGLE: {
        cgc_PaintTriangle(c, o->layer, o->color, o->fill, (cgc_VGF_Triangle *)file_data);
        file_data += sizeof(cgc_VGF_Triangle);
        break;
      }
      case VGF_OBJECT_LINE: {
        cgc_PaintLine(c, o->layer, o->color, o->fill, (cgc_VGF_Line *)file_data);
        file_data += sizeof(cgc_VGF_Line);
        break;
      }
      case VGF_OBJECT_CIRCLE: {
        cgc_PaintCircle(c, o->layer, o->color, o->fill, (cgc_VGF_Circle *)file_data);
        file_data += sizeof(cgc_VGF_Circle);
        break;
      }
      case VGF_OBJECT_SQUARE: {
        cgc_PaintSquare(c, o->layer, o->color, o->fill, (cgc_VGF_Square *)file_data);
        file_data += sizeof(cgc_VGF_Square);
        break;
      }
      case VGF_OBJECT_RECTANGLE: {
        cgc_PaintRectangle(c, o->layer, o->color, o->fill, (cgc_VGF_Rectangle *)file_data);
        file_data += sizeof(cgc_VGF_Rectangle);
        break;
      }
      case VGF_OBJECT_SPRAY: {
        cgc_PaintSpray(c, o->layer, o->color, o->fill, (cgc_VGF_Spray *)file_data);
        file_data += sizeof(cgc_VGF_Spray);
        break;
      }
      default:
        break;
    }
  } 
  return -1;
}
