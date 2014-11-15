include config.mk

CFLAGS += -std=c11 -fPIC
LDFLAGS = -shared

SRC = src/array.c src/object.c src/parser.c src/value.c
OBJ = $(SRC:.c=.o)

TARGET = libjson11.so
HEADER = json11.h

all: $(TARGET)

.c.o:
	@echo CC $<
	@${CC} $< -o $@ -c $(CFLAGS)

$(TARGET): $(OBJ)
	@echo CCLD $@
	@$(CC) $(OBJ) -o $@ $(LDFLAGS) $(CFLAGS)

clean:
	@echo Cleaning
	@$(RM) $(TARGET) $(OBJ)

install: all
	@echo Installing $(TARGET) to $(DESTDIR)$(LIBDIR)
	@mkdir -p $(DESTDIR)$(LIBDIR)
	@cp -f $(TARGET) $(DESTDIR)$(LIBDIR)/$(TARGET).0.$(VERSION)
	-@ln -sf $(DESTDIR)$(LIBDIR)/$(TARGET).0.$(VERSION) $(DESTDIR)$(LIBDIR)/$(TARGET).0
	-@ln -sf $(DESTDIR)$(LIBDIR)/$(TARGET).0.$(VERSION) $(DESTDIR)$(LIBDIR)/$(TARGET)
	@chmod 755 $(DESTDIR)$(LIBDIR)/$(TARGET).0.$(VERSION)
	-@chmod 755 $(DESTDIR)$(LIBDIR)/$(TARGET).0
	-@chmod 755 $(DESTDIR)$(LIBDIR)/$(TARGET)
	@echo Installing $(HEADER) to $(DESTDIR)$(INCLUDEDIR)
	@mkdir -p $(DESTDIR)$(INCLUDEDIR)
	@cp -f $(HEADER) $(DESTDIR)$(INCLUDEDIR)/$(HEADER)
	@chmod 644 $(DESTDIR)$(INCLUDEDIR)/$(HEADER)

uninstall:
	@echo Uninstalling
	@$(RM) $(DESTDIR)$(LIBDIR)/$(TARGET).0.$(VERSION)
	@$(RM) $(DESTDIR)$(LIBDIR)/$(TARGET).0
	@$(RM) $(DESTDIR)$(LIBDIR)/$(TARGET)
	@$(RM) $(DESTDIR)$(INCLUDEDIR)/$(HEADER)
