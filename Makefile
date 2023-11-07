BUILD_DIR := build/
BUILD_TYPE := Debug
TEST_LOG_FILE := test.log

.PHONY: build test enforce_code_style clean

build:
	cmake . -B $(BUILD_DIR) -DSLOPP_INTERFACE_BUILD_TESTS=ON -DSLOPP_IMPLEMENTATION_BUILD_TESTS=ON
	cmake --build $(BUILD_DIR) --config $(BUILD_TYPE) -- -j `nproc --all`

test: build
	ctest --test-dir $(BUILD_DIR) -j `nproc --all` --output-log $(TEST_LOG_FILE) --output-on-failure

enforce_code_style:
	find src/ -iname "*.hpp" -o -iname "*.cpp" | xargs clang-format -i

clean:
	rm -rf $(BUILD_DIR) .cache/ *.log
