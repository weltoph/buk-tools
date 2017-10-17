all : while

WHILESRC=while/src/

.PHONY: all while

while :
	@echo "making while emulator whemu"
	@$(MAKE) -C $(WHILESRC)
