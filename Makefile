all:
	g++ -DWITH_CANARY -DWITH_HASH main.cpp stack.cpp error_func.cpp log_funcs.cpp -o main.out


