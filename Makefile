all:


clean:
	-find ./ -name "*.o" -exec rm -rf {} \;
	-find ./ -name "*.out" -exec rm -rf {} \;
