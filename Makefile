PREFIX         ?= /usr
DESTDIR        ?=

RM             ?= rm -f
RMDIR          ?= rm -rf
MKDIR          ?= mkdir -m 0755 -p

SOURCE         := $(wildcard src/flit/*.cpp)
HEADERS        := src/flit.h
FLIT_HEADERS   := $(wildcard src/flit/*.h)

# Install variables

SCRIPT_DIR        := scripts/flitcli
BASH_COMPLETE_DIR := scripts/bash-completion
DATA_DIR          := data
CONFIG_DIR        := $(SCRIPT_DIR)/config
DOC_DIR           := documentation
LITMUS_TESTS      += $(wildcard litmus-tests/tests/*.cpp)
LITMUS_TESTS      += $(wildcard litmus-tests/tests/*.h)

EFFECTIVE_PREFIX       := $(DESTDIR)$(PREFIX)
INST_BINDIR            := $(EFFECTIVE_PREFIX)/bin
INST_LIBDIR            := $(EFFECTIVE_PREFIX)/lib
INST_INCLUDEDIR        := $(EFFECTIVE_PREFIX)/include/flit
INST_SHAREDIR          := $(EFFECTIVE_PREFIX)/share/flit
INST_SRCDIR            := $(EFFECTIVE_PREFIX)/share/flit/src
INST_LICENSEDIR        := $(EFFECTIVE_PREFIX)/share/licenses/flit
INST_FLIT_CONFIG       := $(EFFECTIVE_PREFIX)/share/flit/scripts/flitconfig.py
INST_BASH_COMPLETE_DIR := $(EFFECTIVE_PREFIX)/share/bash-completion/completions

-include tests/color_out.mk

# Be silent by default
ifndef VERBOSE
.SILENT:
endif

.PHONY: help
help:
	@echo
	@echo "FLiT is an automation and analysis tool for reproducibility of"
	@echo "floating-point algorithms with respect to compilers, architectures,"
	@echo "and compiler flags."
	@echo
	@echo "The following targets are available:"
	@echo
	@echo "  help        Shows this help message and exits (default)"
	@echo "  install     Installs FLiT.  You may override the PREFIX variable"
	@echo "              to install to a different directory.  The default"
	@echo "              PREFIX value is /usr."
	@echo '                exe: "make install PREFIX=$$HOME/installs/usr"'
	@echo "  uninstall   Delete a FLiT installation.  You must use the same"
	@echo "              PREFIX value used during installation."
	@echo "  check       Run tests for FLiT framework"
	@echo "  clean       Clean up after the tests"
	@echo

.PHONY: check
check:
	$(MAKE) --no-print-directory check --directory tests

.PHONY: clean
clean:
	$(MAKE) --no-print-directory clean --directory tests

.PHONY: install
install:
	@$(call color_out,BLUE,Installing: DESTDIR=$(DESTDIR) PREFIX=$(PREFIX))
	$(MKDIR) $(INST_BINDIR)
	$(MKDIR) $(INST_INCLUDEDIR)
	$(MKDIR) $(INST_BASH_COMPLETE_DIR)
	$(MKDIR) $(INST_SHAREDIR)/scripts/experimental
	$(MKDIR) $(INST_SHAREDIR)/doc
	$(MKDIR) $(INST_SHAREDIR)/data/tests
	$(MKDIR) $(INST_SHAREDIR)/data/db
	$(MKDIR) $(INST_SHAREDIR)/config
	$(MKDIR) $(INST_SHAREDIR)/litmus-tests
	$(MKDIR) $(INST_SHAREDIR)/benchmarks
	$(MKDIR) $(INST_SRCDIR)
	$(MKDIR) $(INST_LICENSEDIR)
	@$(call color_out,CYAN,  Creating command-line symbolic link $(INST_BINDIR)/flit)
	ln -sf ../share/flit/scripts/flit.py $(INST_BINDIR)/flit
	@$(call color_out,BROWN,  Installing includes into $(INST_INCLUDEDIR))
	install -m 0644 $(HEADERS) $(EFFECTIVE_PREFIX)/include
	install -m 0644 $(FLIT_HEADERS) $(INST_INCLUDEDIR)
	@$(call color_out,BROWN,  Installing source into $(INST_SRCDIR))
	install -m 0644 $(SOURCE) $(INST_SRCDIR)
	@$(call color_out,BROWN,  Installing scripts into $(INST_SHAREDIR)/scripts)
	install -m 0755 $(SCRIPT_DIR)/flit.py $(INST_SHAREDIR)/scripts/
	install -m 0755 $(SCRIPT_DIR)/flit_*.py $(INST_SHAREDIR)/scripts/
	install -m 0755 $(SCRIPT_DIR)/experimental/flit_*.py $(INST_SHAREDIR)/scripts/experimental/
	install -m 0644 $(SCRIPT_DIR)/experimental/ninja_syntax.py $(INST_SHAREDIR)/scripts/experimental/
	install -m 0644 $(SCRIPT_DIR)/flitutil.py $(INST_SHAREDIR)/scripts/
	install -m 0644 $(SCRIPT_DIR)/flitelf.py $(INST_SHAREDIR)/scripts/
	install -m 0644 $(SCRIPT_DIR)/README.md $(INST_SHAREDIR)/scripts/
	@$(call color_out,BROWN,  Intalling bash-completion script into $(INST_BASH_COMPLETE_DIR))
	@$(call color_out,GREEN,    You can source it in your ~/.bashrc or copy it to /etc/bash_completion.d/)
	install -m 0644 $(BASH_COMPLETE_DIR)/* $(INST_BASH_COMPLETE_DIR)/
	@$(call color_out,BROWN,  Installing documentation into $(INST_SHAREDIR)/doc)
	install -m 0644 $(DOC_DIR)/*.md $(INST_SHAREDIR)/doc/
	install -m 0644 $(DATA_DIR)/Makefile.in $(INST_SHAREDIR)/data/
	install -m 0644 $(DATA_DIR)/Makefile_bisect_binary.in $(INST_SHAREDIR)/data/
	install -m 0644 $(DATA_DIR)/custom.mk $(INST_SHAREDIR)/data/
	install -m 0644 $(DATA_DIR)/main.cpp $(INST_SHAREDIR)/data/
	install -m 0644 $(DATA_DIR)/tests/Empty.cpp $(INST_SHAREDIR)/data/tests/
	install -m 0644 $(DATA_DIR)/db/tables-sqlite.sql $(INST_SHAREDIR)/data/db/
	install -m 0644 $(CONFIG_DIR)/version.txt $(INST_SHAREDIR)/config/
	install -m 0644 $(CONFIG_DIR)/flit-default.toml.in $(INST_SHAREDIR)/config/
	@$(call color_out,BROWN,  Installing litmus tests into $(INST_SHAREDIR)/litmus-tests)
	install -m 0644 $(LITMUS_TESTS) $(INST_SHAREDIR)/litmus-tests/
	install -m 0644 LICENSE $(INST_LICENSEDIR)
	@$(call color_out,BROWN,  Intsalling benchmarks into $(INST_SHAREDIR)/benchmarks)
	cp -r benchmarks/* $(INST_SHAREDIR)/benchmarks/
	@$(call color_out,CYAN,  Generating $(INST_FLIT_CONFIG))
	# Make the flitconfig.py script specifying this installation information
	echo "'''"                                                                  > $(INST_FLIT_CONFIG)
	echo "Contains paths and other configurations for the flit installation."  >> $(INST_FLIT_CONFIG)
	echo "This particular file was autogenerated at the time of installation." >> $(INST_FLIT_CONFIG)
	echo "This is the file that allows installations to work from any prefix." >> $(INST_FLIT_CONFIG)
	echo "'''"                                                                 >> $(INST_FLIT_CONFIG)
	echo                                                                       >> $(INST_FLIT_CONFIG)
	echo "import os"                                                           >> $(INST_FLIT_CONFIG)
	echo                                                                       >> $(INST_FLIT_CONFIG)
	echo "all = ["                                                             >> $(INST_FLIT_CONFIG)
	echo "    'version',"                                                      >> $(INST_FLIT_CONFIG)
	echo "    'script_dir',"                                                   >> $(INST_FLIT_CONFIG)
	echo "    'doc_dir',"                                                      >> $(INST_FLIT_CONFIG)
	echo "    'src_dir',"                                                      >> $(INST_FLIT_CONFIG)
	echo "    'include_dir',"                                                  >> $(INST_FLIT_CONFIG)
	echo "    'config_dir',"                                                   >> $(INST_FLIT_CONFIG)
	echo "    'bash_completion_dir',"                                          >> $(INST_FLIT_CONFIG)
	echo "    'data_dir',"                                                     >> $(INST_FLIT_CONFIG)
	echo "    'litmus_test_dir',"                                              >> $(INST_FLIT_CONFIG)
	echo "    ]"                                                               >> $(INST_FLIT_CONFIG)
	echo                                                                       >> $(INST_FLIT_CONFIG)
	echo "_scriptpath = os.path.dirname(os.path.abspath(__file__))"            >> $(INST_FLIT_CONFIG)
	echo "_prefix = os.path.realpath("                                         >> $(INST_FLIT_CONFIG)
	echo "    os.path.join(_scriptpath, '..', '..', '..'))"                    >> $(INST_FLIT_CONFIG)
	echo                                                                       >> $(INST_FLIT_CONFIG)
	echo "# flit scripts"                                                      >> $(INST_FLIT_CONFIG)
	echo "script_dir = os.path.join(_prefix, 'share', 'flit', 'scripts')"      >> $(INST_FLIT_CONFIG)
	echo                                                                       >> $(INST_FLIT_CONFIG)
	echo "# flit documentation"                                                >> $(INST_FLIT_CONFIG)
	echo "doc_dir = os.path.join(_prefix, 'share', 'flit', 'doc')"             >> $(INST_FLIT_CONFIG)
	echo                                                                       >> $(INST_FLIT_CONFIG)
	echo "# flit C++ include files, primarily flit/flit.h"                     >> $(INST_FLIT_CONFIG)
	echo "include_dir = os.path.join(_prefix, 'include')"                      >> $(INST_FLIT_CONFIG)
	echo                                                                       >> $(INST_FLIT_CONFIG)
	echo "# flit C++ source files"                                             >> $(INST_FLIT_CONFIG)
	echo "src_dir = os.path.join(_prefix, 'share', 'flit', 'src')"             >> $(INST_FLIT_CONFIG)
	echo                                                                       >> $(INST_FLIT_CONFIG)
	echo "# default configuration for flit init"                               >> $(INST_FLIT_CONFIG)
	echo "config_dir = os.path.join(_prefix, 'share', 'flit', 'config')"       >> $(INST_FLIT_CONFIG)
	echo                                                                       >> $(INST_FLIT_CONFIG)
	echo "# where bash completion scripts for flit reside"                     >> $(INST_FLIT_CONFIG)
	echo "bash_completion_dir = os.path.join("                                 >> $(INST_FLIT_CONFIG)
	echo "    _prefix, 'share', 'bash-completion', 'completions')"             >> $(INST_FLIT_CONFIG)
	echo                                                                       >> $(INST_FLIT_CONFIG)
	echo "# current version"                                                   >> $(INST_FLIT_CONFIG)
	echo "version_file = os.path.join(config_dir, 'version.txt')"              >> $(INST_FLIT_CONFIG)
	echo "with open(version_file, 'r') as _version_file_opened:"               >> $(INST_FLIT_CONFIG)
	echo "    version = _version_file_opened.read().strip()"                   >> $(INST_FLIT_CONFIG)
	echo                                                                       >> $(INST_FLIT_CONFIG)
	echo "# default data files such as Makefile.in and main.cpp"               >> $(INST_FLIT_CONFIG)
	echo "data_dir = os.path.join(_prefix, 'share', 'flit', 'data')"           >> $(INST_FLIT_CONFIG)
	echo                                                                       >> $(INST_FLIT_CONFIG)
	echo "# directory containing litmus tests"                                 >> $(INST_FLIT_CONFIG)
	echo "litmus_test_dir = os.path.join("                                     >> $(INST_FLIT_CONFIG)
	echo "    _prefix, 'share', 'flit', 'litmus-tests')"                       >> $(INST_FLIT_CONFIG)

.PHONY: uninstall
uninstall:
	@$(call color_out_noline,BLUE,Uninstalling... )
	$(RMDIR) $(INST_INCLUDEDIR)
	$(RMDIR) $(INST_SRCDIR)
	$(RMDIR) $(INST_SHAREDIR)
	$(RMDIR) $(INST_LICENSEDIR)
	$(RM) $(INST_BINDIR)/flit
	$(RM) $(EFFECTIVE_PREFIX)/include/flit.h
	$(RM) $(INST_BASH_COMPLETE_DIR)/flit
	-rmdir --ignore-fail-on-non-empty $(EFFECTIVE_PREFIX)/include 2>/dev/null
	-rmdir --ignore-fail-on-non-empty $(EFFECTIVE_PREFIX)/share/bash-completion/completions 2>/dev/null
	-rmdir --ignore-fail-on-non-empty $(EFFECTIVE_PREFIX)/share/bash-completion 2>/dev/null
	-rmdir --ignore-fail-on-non-empty $(EFFECTIVE_PREFIX)/share/licenses 2>/dev/null
	-rmdir --ignore-fail-on-non-empty $(EFFECTIVE_PREFIX)/share 2>/dev/null
	-rmdir --ignore-fail-on-non-empty $(EFFECTIVE_PREFIX)/bin 2>/dev/null
	-rmdir --ignore-fail-on-non-empty $(EFFECTIVE_PREFIX) 2>/dev/null
	@$(call color_out,GREEN,Done)
