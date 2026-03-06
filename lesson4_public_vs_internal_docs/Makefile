docs:
	mkdir -p docs/public
	doxygen Doxyfile.public
	@echo "Public documentation generated in 'docs/public'."

internal-docs:
	mkdir -p docs/internal
	doxygen Doxyfile.internal
	@echo "Internal documentation generated in 'docs/internal'."

pdf: docs
	$(MAKE) -C docs/public/latex
	@echo "Public PDF generated at 'docs/public/latex/refman.pdf'."

internal-pdf: internal-docs
	$(MAKE) -C docs/internal/latex
	@echo "Internal PDF generated at 'docs/internal/latex/refman.pdf'."

clean:
	rm -rf docs
	@echo "Documentation cleaned."

.PHONY: docs internal-docs pdf internal-pdf clean
