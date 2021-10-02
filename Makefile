# This Makefile contain rules for typical command line task you may want to perform

# You can do "make help" to get a list of rules

PLATFORMIO=~/.platformio/penv/bin/platformio

default: build_teensy

build_teensy:  ## (default) Compiles the code mfor the teensy
	${PLATFORMIO} run --environment teensy31

flash_teensy:  ## Flashes the code into the Teensy using openOCD
	${PLATFORMIO} run --target upload --environment teensy31

clean_teensy:  ## Cleans Teensy builds
	${PLATFORMIO} run --target clean --environment teensy31



clean_native:  ## Cleans Teensy builds
	${PLATFORMIO} run --target clean --environment native

test_native:  ## Lauinches tests for the native version
	${PLATFORMIO} test --environment native

test_native-verbose:  ## Lauinches tests for the native version (verbose output)
	${PLATFORMIO} test -v --environment native

debug_native_test:  ## Launches GDB for the native tests
	${PLATFORMIO}  debug -e native --interface=gdb

.PHONY: help
help: ## show this help.
	@fgrep -h "##" $(MAKEFILE_LIST) | fgrep -v fgrep | sed -e 's/\\$$//' | sed -e 's/##//'