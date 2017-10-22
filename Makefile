all : while tm

WHILESRC=while/src/
TMSRC=tm/src/

.PHONY: all while tm

while :
	@echo "making while interpreter whint"
	@$(MAKE) -C $(WHILESRC)

tm :
	@echo "making turingmachine interpreter tmint"
	@$(MAKE) -C $(TMSRC)


