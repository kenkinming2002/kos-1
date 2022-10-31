PAGE_OBJS = libmm/src/page/region.o \
	    libmm/src/page/watermark.o

libmm.a: CFLAGS+=-Ilibmm/include

libmm.a: $(PAGE_OBJS)
	$(AR) rcs $@ $(PAGE_OBJS)

