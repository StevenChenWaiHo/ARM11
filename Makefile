
all:
	$(MAKE) -C src

server: all
	ruby arm11_testsuite/testserver.rb src/build/bin/	 
