.PHONY: all clean

# Specify extensions of files to be deleted when cleaning
CLEANEXTS   = o so out

CUR_DIR := $(shell pwd)

CC := gcc

APP_NAME := app
LIB_NAME := foo.so
AUDIT_LIB_NAME := audit.so

DIST_DIR := $(CUR_DIR)
DIST_APP := $(DIST_DIR)/$(APP_NAME)
DIST_LIB := $(DIST_DIR)/$(LIB_NAME)
DIST_AUDIT := $(DIST_DIR)/$(AUDIT_LIB_NAME)

all: $(DIST_APP) $(DIST_LIB) $(DIST_AUDIT)

clean:
	for file in $(CLEANEXTS); do rm -fv *.$$file; done
	rm -rvf $(DIST_APP)

COMMON_CFLAGS := \
	-Wall -Werror -Wextra \
	-std=gnu99

LIBS_COMMON_CFLAGS := \
	-fPIC

LIBS_COMMON_LDFLAGS := \
	-shared

####################
# Libfoo
LIB_SRC_DIR := $(CUR_DIR)
LIB_INCLUDE_DIR := $(LIB_SRC_DIR)
LIB_SRC := \
	library.c

LIB_CFLAGS := \
	$(COMMON_CFLAGS) \
	$(LIBS_COMMON_CFLAGS)

LIB_LDFLAGS := \
	$(LIBS_COMMON_LDFLAGS)

$(DIST_LIB): $(subst .c,.o,$(LIB_SRC))
	$(CC) $(LIB_CFLAGS) $(LIB_LDFLAGS) $^ -o $@

###########################
# Libaudit
LIBAUDIT_SRC_DIR := $(CUR_DIR)
LIBAUDIT_INCLUDE_DIR := $(LIBAUDIT_SRC_DIR)
LIBAUDIT_SRC := \
	rtld_audit.c

LIBAUDIT_CFLAGS := \
	$(COMMON_CFLAGS) \
	$(LIBS_COMMON_CFLAGS)

LIBAUDIT_LDFLAGS := \
	$(LIBS_COMMON_LDFLAGS)

$(DIST_AUDIT): $(subst .c,.o,$(LIBAUDIT_SRC))
	$(CC) $(LIBAUDIT_CFLAGS) $(LIBAUDIT_LDFLAGS) $^ -o $@

#########################
# APP
APP_SRC_DIR := $(CUR_DIR)
APP_SRC := \
	hello.c

APP_CFLAGS := \
	$(COMMON_CFLAGS) \
	-I$(LIB_INCLUDE_DIR)

APP_LDFLAGS := -Wl,--audit $(DIST_AUDIT)
# don't need, because libraries are specified in prereq, so
# -lname will be added to recipe automatically
#  see: https://www.gnu.org/software/make/manual/make.html#Libraries_002fSearch
APP_LIBS :=

APP_OBJS := $(APP_SRC:.c=.o)

$(DIST_APP): \
  $(APP_OBJS) \
  $(DIST_LIB) \
  $(DIST_AUDIT)
	$(CC) $(APP_CFLAGS) -o $@ $^ $(APP_LDFLAGS) $(APP_LIBS)

$(APP_OBJS): \
  $(APP_SRC)
	$(CC) $(APP_CFLAGS) -c $< -o $@
