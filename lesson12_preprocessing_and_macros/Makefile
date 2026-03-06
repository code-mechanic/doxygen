docs:
	mkdir -p docs
	doxygen Doxyfile
	@echo "Documentation generated in the 'docs' directory."

pdf: docs
	$(MAKE) -C docs/latex
	@echo "PDF generated at 'docs/latex/refman.pdf'."

check:
	cc -Iinclude -DDIAG_LOG_TARGET_EXT_FLASH=1 -DDIAG_ENABLE_ISR_STAMP=1 -Wall -Wextra -Werror -c src/diag_log.c
	@echo "Compile check completed successfully."

clean:
	rm -rf docs
	@echo "Documentation cleaned."

.PHONY: docs pdf check clean
