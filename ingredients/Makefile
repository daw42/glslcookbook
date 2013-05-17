OBJDIR     = obj
TARGETDIR  = .
TARGET     = $(TARGETDIR)/libingredients.a
DEFINES   += -D_CRT_SECURE_NO_WARNINGS -D_SCL_SECURE_NO_WARNINGS -D_DEBUG
INCLUDES  += -I$(GLM_DIR)
CPPFLAGS  += -MMD -MP $(DEFINES) $(INCLUDES)
CFLAGS    += $(CPPFLAGS) $(ARCH) -g
CXXFLAGS  += $(CFLAGS) 
LDFLAGS   += 
LIBS      += 
RESFLAGS  += $(DEFINES) $(INCLUDES) 
LDDEPS    += 
LINKCMD    = $(AR) -rcs $(TARGET) $(OBJS) $(GL_LOADER_OBJ)

OBJECTS := \
	vboteapotpatch.o \
	vboteapot.o \
	drawable.o \
	vbomeshadj.o \
	vbotorus.o \
	glutils.o \
	vbomesh.o \
	glslprogram.o \
	vbocube.o \
	vboplane.o \
	bmpreader.o \
	
GL_LOADER_OBJ := $(OBJDIR)/gl_core_4_3.o

OBJS := $(addprefix $(OBJDIR)/, $(OBJECTS))

.PHONY: clean all

all: $(TARGET)
	@:

$(TARGET): $(OBJS) $(GL_LOADER_OBJ) | $(TARGETDIR)
	@echo Linking ingredients
	$(SILENT) $(LINKCMD)

$(TARGETDIR):
	@echo Creating $(TARGETDIR)
	$(SILENT) mkdir -p $(TARGETDIR)
	
$(OBJDIR):
	@echo Creating $(OBJDIR)
	$(SILENT) mkdir -p $(OBJDIR)

clean:
	@echo Cleaning ingredients
	$(SILENT) rm -f  $(TARGET)
	$(SILENT) rm -rf $(OBJDIR)
	
$(GL_LOADER_OBJ) : gl_core_4_3.c
	@echo $(notdir $<)
	$(SILENT) $(CC) $(CFLAGS) -o "$@" -c "$<"

$(OBJS) : $(OBJDIR)/%.o : %.cpp | $(OBJDIR)
	@echo $(notdir $<)
	$(SILENT) $(CXX) $(CXXFLAGS) -o "$@" -c "$<"

