######################################
######### BMSTU COURSE WORK ##########
########## SMTP MTA SERVER ###########
#### CREATED BY PUCHNINA ANASTASIA ###
######################################

# Main program name
PROG_NAME = smtp_server
# Report name
REPORT_NAME = report.pdf

# Compiler
CC = gcc
STD = -std=gnu99

# Directories
COMMON_INCLUDE_DIR = ./../common/include
COMMON_SOURCE_DIR = ./../common/source
BUILD_DIR = build
AUTOGEN_DIR = autogen

# FLAGS:

# Compilation Flags
RFLAGS = -Wall -Werror -I$(COMMON_INCLUDE_DIR) -I$(AUTOGEN_DIR) -O
DFLAGS = -Wall -I$(COMMON_INCLUDE_DIR) -g

# Linking and Builing Flags
LDFLAGS += $(shell autoopts-config ldflags)


# TARGETS:

all: clean build_dir compile-all smtp_server test_client report

smtp_server:
	$(CC) $(STD) $(LDFLAGS) -o ./$(BUILD_DIR)/$(PROG_NAME) \
    ./build/*.o

test_client:
	$(CC) $(STD) $(RFLAGS) -o ./$(BUILD_DIR)/test_client \
	test_client.c $(COMMON_SOURCE_DIR)/*.c

report:
	echo "Report with name $(REPORT_NAME) was not created. Kek."


# ADDITIONAL:

clean:
	rm -rf *.o ./build

build_dir:
	mkdir -p $(BUILD_DIR)

move_objs:
	mv *.o $(BUILD_DIR)


# COMPILATION:

compile-all: server-objs server-fsm.o serveropts.o move_objs

# Main server object files
server-objs:
	$(CC) $(STD) -c $(RFLAGS) -I$prefix/include \
    server.c my_socket.c main.c $(COMMON_SOURCE_DIR)/*.c \
    server_fsm_handlers.c

# Compiling Options with Autogen
serveropts.o: $(AUTOGEN_DIR)/serveropts.c
	cc -o serveropts.o -I$prefix/include -c $(AUTOGEN_DIR)/serveropts.c

# Compiling FSM with Autogen
server-fsm.o: $(AUTOGEN_DIR)/server-fsm.c
	$(CC) -Wall -I$(AUTOGEN_DIR) -o $@ -c $<


# GENERAION OF AUTOGEN HEADERS AND SOURCE FILES:

autogen_all: autogen_serveropts autogen_server-fsm

# Generate check options files with autogen
autogen_serveropts: $(AUTOGEN_DIR)/serveropts.def
	SHELL=/bin/sh cd $(AUTOGEN_DIR)/ && autogen -L $prefix/share/autogen ./serveropts.def

# Generate Finite State Machine files with autogen
autogen_server-fsm: $(AUTOGEN_DIR)/server.def
	SHELL=/bin/sh cd $(AUTOGEN_DIR)/ && autogen ./server.def