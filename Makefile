# **************************************************
# * CMAKE BUILD                                    *
# **************************************************
BUILD_PATH = build
BUILD_OPT = -DCMAKE_EXPORT_COMPILE_COMMANDS=1

# **************************************************
# * RULE                                           *
# **************************************************
all: build

build:
	cmake -B ${BUILD_PATH} . ${BUILD_OPT}\
		&& cmake --build ${BUILD_PATH}\
		&& ln -sf build/compile_commands.json .

run:
	@./build/bin/scop

clean:
	rm -rf ${BUILD_PATH}
	rm compile_commands.json

re:
	make clean
	make all

# **************************************************
# * PHONY                                          *
# **************************************************
.PHONY: all build clean re
