MM_OBJS = libmm/src/page/region.o \
	  libmm/src/page/watermark.o

libmm.a: CFLAGS+=-Ilibmm/include

libmm.a: $(MM_OBJS)
	$(AR) rcs $@ $(MM_OBJS)

ALL_OBJS += $(MM_OBJS)
ALL_ARS  += libmm.a
