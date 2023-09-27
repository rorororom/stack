all:
	g++-13 -DWITH_CANARY_AND_HASHE main.cpp stack_funcs.cpp stack_funcs_hash.cpp error_func.cpp log_funcs.cpp -o main.out
