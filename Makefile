CXX_FLAGS += -O0

a-cpp: a.cc
	$(CXX) $(CXX_FLAGS) -E -o $@ $<;
	sed -n '/mark from here on/,/mark until here/{p}' a-cpp

a: a.cc
	$(CXX) $(CXX_FLAGS) -o $@ $<;
	./a
