#include <stdio.h>
#include <stdlib.h>    // exit(3)
#include <Imlib2.h>


double fps = 30.0;
double zoom_rate = 5;
double ox = -1;
double oy = -1;
double scale = -1;


typedef struct
{
  double x;
  double y;
} point_t;

typedef struct
{
  point_t p0;
  point_t p1;
} rect_t;


inline double rect_cx( rect_t *r ) { return r->p1.x - r->p0.x; }
inline double rect_cy( rect_t *r ) { return r->p1.y - r->p0.y; }

void point_scale( point_t *p, double a )
{
  p->x *= a;
  p->y *= a;
}

void point_add( point_t *p, point_t *q )
{
  p->x += q->x;
  p->y += q->y;
}

void rect_transform( rect_t *r, double a, point_t *u )
{
  point_scale( &r->p0, a );
  point_add( &r->p0, u );
  point_scale( &r->p1, a );
  point_add( &r->p1, u );
}


void die(const char *msg)
{
  fprintf(stderr, "%s\n", msg);
  exit(111);
}

int main(int argc, char **argv)
{
  const char *prog;
  const char *path;

  prog = *argv++; argc--;

  if (argc != 4)
  {
    fprintf(stderr, "usage: %s [OPTIONS] image ox oy scale\n", prog);
    return 1;
  }

  path = *argv++; argc--;
  ox = atof( *argv++ ); argc--;
  oy = atof( *argv++ ); argc--;
  scale = atof( *argv++ ); argc--;

  Imlib_Image buffer;
  Imlib_Image image;
  double cx, cy;

  if (!(image = imlib_load_image( path )))
    die("error reading input file");

  imlib_context_set_image( image );
  cx = imlib_image_get_width();
  cy = imlib_image_get_height();

  if (!(buffer = imlib_create_image( (int)cx, (int)cy )))
    die("error creating buffer image");

  imlib_context_set_image( buffer );
  imlib_context_set_color( 0, 0, 0, 0 );

  int nframes = fps * zoom_rate;
  int frame;
  double a;
  double a0 = 1.0;
  double a1 = scale;

  for (frame=0, a=a0; frame<nframes; frame++, a=a0+(a1-a0)*frame/nframes)
  {
    rect_t r = { { 0, 0 }, { cx, cy } };
    point_t b = { (1.0 - scale) * ox, (1.0 - scale) * oy };
    point_t binv = { ((a - 1.0) / a) * ox, ((a - 1.0) / a) * oy };

    rect_transform( &r, 1.0/a, &binv );

    double s;

    for (s=1.0/a; s*cx>1.0 && s*cy>1.0; s*=scale)
    {
      // TODO round these?
      int x = (int)r.p0.x;
      int y = (int)r.p0.y;
      int w = (int)rect_cx(&r);
      int h = (int)rect_cy(&r);

      imlib_image_fill_rectangle( x, y, w, h );
      imlib_blend_image_onto_image( image, 1, 0, 0, cx, cy, x, y, w, h );

      fprintf( stderr, "%d (%d,%d) - (%d,%d) [%dx%d]\n", frame, x, y, x+w, y+h, w, h );

      rect_transform( &r, scale, &b );
    }

    /* ARGB pixel bytes */
    char *pixels = (char*)imlib_image_get_data_for_reading_only();
    size_t len = (int)( cx * cy * 4 );

    fwrite( pixels, len, 1, stdout );
  }

  return 0;
}

