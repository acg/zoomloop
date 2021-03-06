TARGET = zoomloop
SOURCES = $(TARGET).c
OBJECTS = ${SOURCES:%.c=%.o}
LIBS = -lImlib2

IMAGE ?= input.img
IMAGE_CX ?= $(shell identify -format "%w" ${IMAGE})
IMAGE_CY ?= $(shell identify -format "%h" ${IMAGE})
OX ?= $(shell echo ${IMAGE_CX}/2 | bc )
OY ?= $(shell echo ${IMAGE_CY}/2 | bc )
SCALE ?= 0.25

CFLAGS += -O3
CFLAGS += -std=c99
CFLAGS += -Wall

# video input options
FFMPEG_OPTS += -f rawvideo
FFMPEG_OPTS += -pix_fmt rgb32
FFMPEG_OPTS += -s "$(IMAGE_CX)x$(IMAGE_CY)"
FFMPEG_OPTS += -i /dev/stdin

# video output options
FFMPEG_OPTS += -y
FFMPEG_OPTS += -vcodec mpeg4
FFMPEG_OPTS += -sameq -r 30000/1001 -b 200k -bt 240k
#FFMPEG_OPTS += -vcodec libx264
#FFMPEG_OPTS += -vpre hq -vpre ipod640


all : $(TARGET)

$(TARGET) : $(OBJECTS)
	$(CC) -o $@ $(OBJECTS) $(LIBS)


video : out.mp4

out.mp4 : $(TARGET)
	./$(TARGET) $(IMAGE) $(OX) $(OY) $(SCALE) | ffmpeg $(FFMPEG_OPTS) $@

clean :
	rm -f $(TARGET) $(OBJECTS)

