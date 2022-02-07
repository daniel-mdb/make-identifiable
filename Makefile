CXX_FLAGS += -O0 -std=c++20

a: a.cc
	$(CXX) $(CXX_FLAGS) -o $@ $<;
	./a
