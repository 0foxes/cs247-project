# final: test.o #final target
# 	g++ test.o -o flood -lX11
CXXFLAGS=-MMD -MP -g -Iincludes -std=c++14

SOURCES = $(shell find src -name "*.cc")
OBJECTS := $(patsubst src/%.cc, build/%.o, $(SOURCES))
DEPENDS := $(patsubst src/%.cc, build/%.d, $(SOURCES))

RAIInet: raiinet

raiinet: $(OBJECTS)
	g++ $(CXXFLAGS) $^ -o $@ -lX11

build/%.o: src/%.cc Makefile
	@mkdir -p $(dir $@)
	g++ $(CXXFLAGS) -c $< -o $@ -lX11

-include $(DEPENDS)

.PHONY: clean
clean:
	rm -r build raiinet