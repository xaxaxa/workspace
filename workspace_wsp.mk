.PHONY: clean All

All:
	@echo ----------Building project:[ generic_ui - Debug ]----------
	@cd "../../../dd/workspace/generic_ui" && "$(MAKE)" -f "generic_ui.mk"
clean:
	@echo ----------Cleaning project:[ generic_ui - Debug ]----------
	@cd "../../../dd/workspace/generic_ui" && "$(MAKE)" -f "generic_ui.mk" clean
