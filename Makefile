SRC=src
BUILD=build
DOC=doc
EXAMPLES=examples

ARCHIVE_NAME=xcervia00-xkadlet00-xzejdoj00

# Project output
PROJECT=ICP
TARGET=icp_fsm_interpreter

# Qmake
QMAKE=qmake
QT_PRO=$(SRC)/*.pro

all: $(BUILD)
	$(MAKE) -j8 -C $(BUILD)

run: all
	./$(BUILD)/$(TARGET)

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
	@rm -rf ./$(DOC)/*

$(BUILD): $(QT_PRO)
	@mkdir -p $(BUILD)
	@cd $(BUILD) && $(QMAKE) ../$(QT_PRO)

.PHONY: all run pack clean doxygen
