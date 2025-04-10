SRC=src
BUILD=build
DOC=doc
EXAMPLES=examples
DEBUG_DIR=debug_bld

ARCHIVE_NAME=xcervia00-xkadlet00-xzejdoj00

# Project output
PROJECT=ICP
TARGET=icp_fsm_interpreter

# Qmake
QMAKE=qmake
QT_PRO=$(SRC)/*.pro

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
	@cp Makefile $(ARCHIVE_NAME)/
	@find $(ARCHIVE_NAME)/$(SRC) -name "*.pro.user" -delete
	@cd $(ARCHIVE_NAME); zip -r ../$(ARCHIVE_NAME).zip *
	@rm -rf $(ARCHIVE_NAME)

clean:
	@rm -rf ./$(BUILD)
	@rm -rf ./$(DEBUG_DIR)
	@rm -rf ./$(DOC)/*

$(BUILD): $(QT_PRO)
	@mkdir -p $(BUILD)
	@cd $(BUILD) && $(QMAKE) ../$(QT_PRO) "CONFIG+=release" "CONFIG+=warn_on"

$(DEBUG_DIR): $(QT_PRO)
	@mkdir -p $(DEBUG_DIR)
	@cd $(DEBUG_DIR) && $(QMAKE) ../$(QT_PRO) "CONFIG+=debug" "CONFIG+=warn_on"

.PHONY: all run pack clean doxygen
