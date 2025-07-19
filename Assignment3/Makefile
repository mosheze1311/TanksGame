# Top-level Makefile
SO_DIRS =  \
	Algorithm \
	GameManager 

MAIN_DIRS = Simulator

all:
	
	$(MAKE) -C Algorithm -f Makefile2;
	$(MAKE) -C Algorithm -f Makefile3;
	@for dir in $(SO_DIRS); do \
		echo "=== Building in $$dir ==="; \
		$(MAKE) -C $$dir; \
	done
	
		@for dir in $(MAIN_DIRS); do \
		echo "=== Building in $$dir ==="; \
		$(MAKE) -C $$dir; \
	done
	
clean:
	$(MAKE) -C Algorithm -f Makefile2 clean;
	$(MAKE) -C Algorithm -f Makefile3 clean;
	@for dir in $(SO_DIRS); do \
		echo "=== Cleaning in $$dir ==="; \
		$(MAKE) -C $$dir clean; \
	done

		@for dir in $(MAIN_DIRS); do \
		echo "=== Cleaning in $$dir ==="; \
		$(MAKE) -C $$dir clean; \
	done