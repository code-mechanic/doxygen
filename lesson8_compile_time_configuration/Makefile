docs:
	mkdir -p docs
	doxygen Doxyfile
	@echo "Documentation generated in the 'docs' directory."

pdf: docs
	$(MAKE) -C docs/latex
	@echo "PDF generated at 'docs/latex/refman.pdf'."

clean:
	rm -rf docs
	@echo "Documentation cleaned."

.PHONY: docs pdf clean
