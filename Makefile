SOURCE_DIR = plugin

format:
	find $(SOURCE_DIR) -iname '*.h' -o -iname '*.cpp' | xargs clang-format -i