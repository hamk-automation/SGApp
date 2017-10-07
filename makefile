CC := g++

OBJDIR := obj
SRCDIR := src

TARGET := SGApp

SRCS := main.cc \
        utils.cc \
				scan_data_frame.cc \
				scan_data_pack.cc \
				road_record.cc \
				matrix.cc \
				gl_canvas.cc \
				frame.cc \
				app.cc \
				sg_app.cc \
				\
				\
				data_filter/data_filter.cc \
				data_filter/data_filter_emxAPI.cc \
				data_filter/data_filter_emxutil.cc \
				data_filter/data_filter_initialize.cc \
				data_filter/data_filter_terminate.cc \
				data_filter/rtGetInf.cc \
				data_filter/rtGetNaN.cc \
				data_filter/rt_nonfinite.cc

HDRS := timer.h \
        libs.h \
			  utils.h \
			  global_var.h \
				scan_data_frame.h \
				scan_data_pack.h \
				road_record.h \
				matrix.h \
				gl_canvas.h \
				frame.h \
				app.h \
				sg_app.h \
				\
				\
				data_filter/data_filter_emxAPI.h \
				data_filter/data_filter_emxutil.h \
				data_filter/data_filter.h \
				data_filter/data_filter_initialize.h \
				data_filter/data_filter_terminate.h \
				data_filter/data_filter_types.h \
				data_filter/rtGetInf.h \
				data_filter/rtGetNaN.h \
				data_filter/rt_nonfinite.h \
				data_filter/rtwtypes.h

OBJS := $(patsubst %.cc, %.o, $(SRCS))
SRCS := $(patsubst %, $(SRCDIR)/%, $(SRCS))
HDRS := $(patsubst %, $(SRCDIR)/%, $(HDRS))
OBJS := $(patsubst %, $(OBJDIR)/%, $(OBJS))

CFLAGS := -Wall -g -std=gnu++11 `wx-config --cxxflags`
LFLAGS := -L'/usr/lib/x86_64-linux-gnu' \
					-lusb-1.0 \
					-lpthread \
					-lGL \
					-lGLU \
					-lwx_gtk3u_gl-3.0 \
					`wx-config --libs`

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(OBJS) $(LFLAGS) -o $@

$(OBJDIR)/%.o: $(SRCDIR)/%.cc $(HDRS)
	$(CC) $(CFLAGS) -c $< -o $@

.PHONY: run
run: $(TARGET)
	./$(TARGET)

.PHONY: clean
clean:
	rm $(TARGET) $(OBJS)
