# zoomloop #

Create zooming video loops into recursively self-tiled images.

Example: [The Land-o-Lakes Butter Box](https://vimeo.com/56182160/)

## Requirements ##

* C compiler
* GNU make
* ffmpeg
* Imlib2
* ImageMagick

## Using It ##

1. Create a symlink to your image file. Call it `input.img`.
2. Find the fixed point `(ox,oy)` of the self-tiling in rectangular coordinates, where `(0,0)` is the top left and `(width,height)` is the lower right.
3. Find the scaling factor of the self-tiling, which is a floating point number in the interval `[0,1]`.
4. Run `make video OX=$ox OY=$oy SCALE=$scale`.
5. Optionally, loop the output video `out.mp4` with [this ffmpeg 1.1 video concatenation trick](http://ffmpeg.org/trac/ffmpeg/wiki/How%20to%20concatenate%20%28join,%20merge%29%20media%20files).

## Known Issues ##

* The self-tiled images jitter slightly due to rounding error. Switching away from [Imlib2](http://docs.enlightenment.org/api/imlib2/html/) to something that supports floating point precision bitblts is necessary.
* The zoom accelerates. Linear zoom seems like a better default.
* The code isn't commented.

## Notes ##

* The code was based on my [simple demo of programmatically generated video](https://github.com/acg/generating-video-demo).

