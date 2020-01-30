CXX = g++
CXXFLAGS = -std=c++17 -O3 -pthread
BINDIR = bin
OBJDIR = obj
SRCDIR = src
TESTDIR = test
GTESTBUILDDIR = gtest-build
GTESTLIBDIR = $(GTESTBUILDDIR)/lib
TESTPREFIX = unittest-
SOURCES = $(wildcard $(SRCDIR)/*.cc)
TESTS = $(wildcard $(TESTDIR)/*.cc)
SRCOBJS = $(addprefix $(OBJDIR)/, $(notdir $(SOURCES:.cc=.o))) dependencies/hand-isomorphism/src/hand_index.o
TESTOBJS = $(addprefix $(OBJDIR)/, $(notdir $(TESTS:.cc=.o)))

$(BINDIR)/poker-hand-clustering: $(SRCOBJS)
	-mkdir -p $(BINDIR)
	$(CXX) $(CXXFLAGS) -o $@ $^

$(BINDIR)/unittest: $(TESTOBJS) $(filter-out $(OBJDIR)/main.o, $(SRCOBJS)) $(GTESTLIBDIR)/libgtest_main.a
	-mkdir -p $(BINDIR)
	$(CXX) $(CXXFLAGS) -o $@ $^ -pthread -L$(GTESTLIBDIR) -lgtest -lgtest_main

$(OBJDIR)/$(TESTPREFIX)%.o: $(TESTDIR)/$(TESTPREFIX)%.cc
	-mkdir -p $(OBJDIR)
	$(CXX) $(CXXFLAGS) -o $@ -Idependencies/googletest/googletest/include -I$(SRCDIR) -c $^

$(OBJDIR)/%.o: $(SRCDIR)/%.cc
	-mkdir -p $(OBJDIR)
	$(CXX) $(CXXFLAGS) -o $@ -c $^

$(GTESTLIBDIR)/libgtest_main.a: .gitmodules
	-mkdir -p $(GTESTBUILDDIR) && cd $(GTESTBUILDDIR) && cmake ../dependencies/googletest && make

dependencies/hand-isomorphism/src/hand_index.o:
	make -C dependencies/hand-isomorphism

.PHONY: run check clean

run: $(BINDIR)/poker-hand-clustering
	$^

check: $(BINDIR)/unittest
	$^

clean:
	rm -rf $(BINDIR) $(OBJDIR) $(GTESTBUILDDIR)
