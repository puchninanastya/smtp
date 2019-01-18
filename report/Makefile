# Report name
REPORT_NAME = report.pdf

# Compiler
CC = gcc
STD = -std=gnu99

# Directories
UTILS_DIR = ./utils
DOT_DIR = ./dot
TEX_DIR = ./tex
IMG_DIR = ./images
PYTHON2_ENV_DIR = ./report_env

# Utilities
MAKE2DOT = $(UTILS_DIR)/makefile2dot
FSM2DOT = $(UTILS_DIR)/fsm2dot

# TARGES:

all: clean report

report:
	echo "Report with name $(REPORT_NAME) was not created. Kek."

clean:
	rm -rf report.pdf $(TEX_DIR)/serverfsm_dot.tex $(DOT_DIR)/serverfsm_def.dot


# GENERATE FSM IMAGES

# .def -> _def.dot
./dot/serverfsm_def.dot: ./../source/server/autogen/server.def
	$(FSM2DOT) $< > $@

./dot/serverfsm_report_def.dot: ./../source/server/autogen/server_report.def
	$(FSM2DOT) $< > $@

.PHONY: generate_fsm_images
generate_fsm_images:
	 dot -Tpng $(DOT_DIR)/serverfsm_def.dot > $(IMG_DIR)/serverfsm.png; \
	 dot -Tpng $(DOT_DIR)/serverfsm_report_def.dot > $(IMG_DIR)/serverfsm_report.png

# TEX FILES GENERATION:

# .dot -> _dot.tex
./../report/tex/serverfsm_dot.tex: ./../report/dot/serverfsm_def.dot
	source report_env/bin/activate ; \
	dot2tex -ftikz --autosize --crop  $< > $@
