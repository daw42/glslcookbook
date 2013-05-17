ifndef verbose
#  SILENT = @
endif

OBJDIR     = obj
TARGETDIR  = .
TARGET     = $(TARGETDIR)/chapter04
DEFINES   += -D_CRT_SECURE_NO_WARNINGS -D_SCL_SECURE_NO_WARNINGS -D_DEBUG
INCLUDES  += -I../ingredients -I$(GLFW_INCLUDE) -I$(GLM_DIR)
CPPFLAGS  += -MMD -MP $(DEFINES) $(INCLUDES)
CFLAGS    += $(CPPFLAGS) $(ARCH) -g
CXXFLAGS  += $(CFLAGS) 
LDFLAGS   += -L../ingredients -L$(GLFW_DIR)/lib
LIBS      += -pthread -lglfw -lrt -lingredients -lGL -lpng12
RESFLAGS  += $(DEFINES) $(INCLUDES) 
LDDEPS    += 
LINKCMD    = $(CXX) -o $(TARGET) $(OBJS) $(LDFLAGS) $(ARCH) $(LIBS)

OBJECTS := \
	scenetexture.o \
	main.o \
#	scenemultitex.o \
#	scenealphatest.o \
#	scenereflectcube.o \
#	scenerendertotex.o \
#	skybox.o \
#	scenenormalmap.o \
#	sceneprojtex.o \
#	scenerefractcube.o \
	
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
