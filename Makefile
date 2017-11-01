all : while tm register

WHILESRC=while/src/
TMSRC=tm/src/
REGISTERSRC=register/src/

.PHONY: all while tm register

while :
	@echo "making while interpreter whint"
	@$(MAKE) -C $(WHILESRC)

tm :
	@echo "making turingmachine interpreter tmint"
	@$(MAKE) -C $(TMSRC)

register :
	@echo "making register machine interpreter reint"
	@$(MAKE) -C $(REGISTERSRC)
