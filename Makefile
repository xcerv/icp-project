# ICP 2024/25 Project - FSM interpreter and editor

SRC=src
BUILD=build
DOC=doc
DOC_FOLDER=html

EXAMPLES=examples
DEBUG_DIR=debug_bld

ARCHIVE_NAME=xcervia00-xkadlet00-xzejdoj00-40-40-20

# Project output
PROJECT=ICP
TARGET=icp_fsm_interpreter

# Qmake
QMAKE:=qmake
QT_PRO=$(SRC)/*.pro

# Merlin specific 
MERLIN_HOSTNAME:=merlin.fit.vutbr.cz
MERLIN_QT_PATH=/usr/local/share/Qt-5.9/5.9.2/

ifeq ("${HOSTNAME}","${MERLIN_HOSTNAME}")
export QT_PLUGN_PATH:=$(MERLIN_QT_PATH)/gcc_64/plugins
export QT_QPA_PLATFORM_PLUGIN_PATH:=$(MERLIN_QT_PATH)/gcc_64/plugins
QMAKE:=$(MERLIN_QT_PATH)/gcc_64/bin/qmake
endif


all: $(BUILD)
	$(MAKE) -j8 -C $(BUILD)

debug: $(DEBUG_DIR)
	$(MAKE) -j8 -C $(DEBUG_DIR)

run: all
	./$(BUILD)/$(TARGET)

run_debug: debug
	./$(DEBUG_DIR)/$(TARGET)

doxygen: 
	@doxygen Doxyfile

pack:
	@mkdir -p $(ARCHIVE_NAME)
	@cp -r $(SRC) $(ARCHIVE_NAME)/
	@cp -r $(EXAMPLES) $(ARCHIVE_NAME)/
	@mkdir -p $(ARCHIVE_NAME)/$(DOC)
	@cp README.txt $(ARCHIVE_NAME)/
	@cp README.md $(ARCHIVE_NAME)/
	@cp Makefile $(ARCHIVE_NAME)/
	@cp Doxyfile $(ARCHIVE_NAME)/
	@find $(ARCHIVE_NAME)/$(SRC) -name "*.pro.user" -delete
	@cd $(ARCHIVE_NAME); zip -r ../$(ARCHIVE_NAME).zip *
	@rm -rf $(ARCHIVE_NAME)

clean:
	@rm -rf ./$(BUILD)
	@rm -rf ./$(DEBUG_DIR)
	@rm -rf ./$(DOC)/$(DOC_FOLDER)
	@rm -f ./$(DOC)/doxygen_warnings.txt

$(BUILD): $(QT_PRO)
	@mkdir -p $(BUILD)
	@cd $(BUILD) && $(QMAKE) ../$(QT_PRO) "CONFIG+=release" "CONFIG+=warn_on"

$(DEBUG_DIR): $(QT_PRO)
	@mkdir -p $(DEBUG_DIR)
	@cd $(DEBUG_DIR) && $(QMAKE) ../$(QT_PRO) "CONFIG+=debug" "CONFIG+=warn_on"

.PHONY: all run pack clean doxygen
