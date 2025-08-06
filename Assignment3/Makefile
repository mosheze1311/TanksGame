# Top-level Makefile
SO_DIRS =  \
	Algorithm \
	GameManager 

MAIN_DIRS = Simulator

all:
	@for dir in $(SO_DIRS); do \
		echo "=== Building in $$dir ==="; \
		$(MAKE) -C $$dir; \
	done
	
	@for dir in $(MAIN_DIRS); do \
		echo "=== Building in $$dir ==="; \
		$(MAKE) -C $$dir; \
	done
	
clean:
	@for dir in $(SO_DIRS); do \
		echo "=== Cleaning in $$dir ==="; \
		$(MAKE) -C $$dir clean; \
	done

	@for dir in $(MAIN_DIRS); do \
		echo "=== Cleaning in $$dir ==="; \
		$(MAKE) -C $$dir clean; \
	done