all: runtests
CXX=g++
CXX_FLAGS=-g -std=c++11
CXX_DEPFLAGS= -MT $@ -MMD -MP -MF obj/$*.td

INCLUDE_TEST=-ICatch2/single_include -Iinclude

SRC_TEST= 	test/runtests.cpp \
		test/test_parser.cpp \
		test/test_flag.cpp \
		test/test_multiple_flag.cpp \
		test/test_value.cpp

OBJ_TEST= $(patsubst test/%.cpp, obj/%.o, $(SRC_TEST) )

runtests: ${OBJ_TEST}
	${CXX} ${CXX_FLAGS} ${OBJ_TEST} -o runtests

obj/%.o: tests/%.cpp
	mkdir -p $(@D)
	${CXX} ${CXX_FLAGS} ${CXX_DEPFLAGS} ${INCLUDE_TEST} -c $< -o $@
	mv obj/$*.td obj/$*.d 

.d: ;
.PRECIOUS: %.d
-include $(patsubst test/%.cpp, obj/%.d, ${SRC_TEST})

clean:
	rm -f obj/*.o
	rm -f obj/*.d

