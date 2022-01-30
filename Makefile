CXX_FLAGS += -O0

a: a.cc
	$(CXX) $(CXX_FLAGS) -o $@ $<;
	./a
