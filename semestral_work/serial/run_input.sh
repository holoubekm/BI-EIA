#!/bin/bash

dir="./results/"
mkdir -p "$dir" 2> /dev/null

run_exec() {
	./"$1" naive "$2" >> "${dir}${3}_naive"
	./"$1" kmp "$2"  >> "${dir}${3}_kmp"
	# valgrind --tool=cachegrind --cachegrind-out-file=/dev/null ./"$1" naive "$2" >> "${dir}/${3}_naive_cache" 2>&1
	# valgrind --tool=cachegrind --cachegrind-out-file=/dev/null ./"$1" kmp "$2"   >> "${dir}/${3}_kmp_cache" 2>&1
}

if [ "$1" == "" ]; then
	echo "No argument!"
else
	file=$(basename "$1")
	# run_exec new_basic_o0 "$1" "$file"
	run_exec new_basic_o3 "$1" "$file"
	run_exec new_chunked "$1" "$file"
	run_exec new_uint64_t "$1" "$file"
	run_exec new_uint64_t_chunked "$1" "$file"
	run_exec new_uint64_t_unrolled "$1" "$file"
	run_exec new_uint64_t_vectorized "$1" "$file"
fi

