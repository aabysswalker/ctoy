ctoy.o: main.cpp $(DEPS)
	clang++ -g -O3 main.cpp `llvm-config --cxxflags --ldflags --system-libs --libs core` -o ctoy

.PHONY: run
run: ctoy.o
	./ctoy
