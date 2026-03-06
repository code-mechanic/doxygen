docs:
	mkdir -p docs
	doxygen Doxyfile
	@echo "Documentation generated in the 'docs' directory."

pdf: docs
	$(MAKE) -C docs/latex
	@echo "PDF generated at 'docs/latex/refman.pdf'."

check:
	cc -Iinclude -Wall -Wextra -Werror -c src/watchdog_recovery.c
	@echo "Compile check completed successfully."

clean:
	rm -rf docs
	@echo "Documentation cleaned."

.PHONY: docs pdf check clean
