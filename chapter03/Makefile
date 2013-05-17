OBJDIR     = obj
TARGETDIR  = .
BASEDIR    = ..
TARGET     = $(TARGETDIR)/chapter03
DEFINES   += -D_CRT_SECURE_NO_WARNINGS -D_SCL_SECURE_NO_WARNINGS -D_DEBUG
INCLUDES  += -I$(BASEDIR)/ingredients -I$(GLFW_INCLUDE) -I$(GLM_DIR)
CPPFLAGS  += -MMD -MP $(DEFINES) $(INCLUDES)
CFLAGS    += $(CPPFLAGS) $(ARCH) -g
CXXFLAGS  += $(CFLAGS) 
LDFLAGS   += -L$(BASEDIR)/ingredients -L$(GLFW_DIR)/lib
LIBS      += -pthread -lglfw -lrt -lingredients -lGL
RESFLAGS  += $(DEFINES) $(INCLUDES) 
LDDEPS    += 
LINKCMD    = $(CXX) -o $(TARGET) $(OBJS) $(LDFLAGS) $(ARCH) $(LIBS)

OBJECTS := \
	scenemultilight.o \
	sceneperfragment.o \
	main.o \
	scenefog.o \
	scenedirectional.o \
	scenetoon.o \
	scenespot.o \
	
OBJS := $(addprefix $(OBJDIR)/, $(OBJECTS))

.PHONY: clean all

all: $(TARGET)
	@:

$(TARGET): $(OBJS) $(LDDEPS) | $(TARGETDIR)
	@echo Linking chapter03
	$(SILENT) $(LINKCMD)
	$(POSTBUILDCMDS)

$(TARGETDIR):
	@echo Creating $(TARGETDIR)
	$(SILENT) mkdir -p $(TARGETDIR)

$(OBJDIR):
	@echo Creating $(OBJDIR)
	$(SILENT) mkdir -p $(OBJDIR)

clean:
	@echo Cleaning chapter03
	$(SILENT) rm -f  $(TARGET)
	$(SILENT) rm -rf $(OBJDIR)

$(OBJS) : $(OBJDIR)/%.o : %.cpp | $(OBJDIR)
	@echo Compiling $(notdir $<)
	$(SILENT) $(CXX) $(CXXFLAGS) -o "$@" -c "$<"


