set shell := ["bash", "-c"]

include_dir := 'include'
src_dir := 'src'
out_dir := 'out'
bin_dir := 'bin'
lib_dir := 'lib'

@default:
    just --list

@compile day="01":
    [[ -d {{ out_dir }} ]] || mkdir -p {{ out_dir }}
    [[ -d {{ out_dir }}/{{ day }} ]] || mkdir -p {{ out_dir }}/{{ day }}

    find {{ src_dir }}/{{ day }} -name "*.c" -exec sh -c 'gcc -c "$1" -I {{ include_dir }} -std=c99 -fsanitize=address -g -o "{{ out_dir }}/{{ day }}/$(basename "${1%.c}").o"' sh {} \;

@link day="01":
    [[ -d {{ bin_dir }} ]] || mkdir -p {{ bin_dir }}
    [[ -f {{ lib_dir }}/lib.a ]] || just build_lib

    gcc {{ out_dir }}/{{ day }}/* -std=c99 -fsanitize=address -static-libasan -L{{ lib_dir }} -l:lib.a -o {{ bin_dir }}/aoc25_{{ day }}

@compile_lib:
    [[ -d {{ out_dir }}/lib ]] || mkdir -p {{ out_dir }}/lib
    gcc -c {{ src_dir }}/lib/*.c -I {{ include_dir }} -std=c99 -fsanitize=address -g -o {{ out_dir }}/lib/lib.o

@archive_lib:
    [[ -d {{ lib_dir }} ]] || mkdir -p {{ lib_dir }}
    ar rcs {{ lib_dir }}/lib.a {{ out_dir }}/lib/*.o

@build_lib: compile_lib archive_lib

@build day="01":
    just compile {{ day }}
    just link {{ day }}

@bear:
    bear -- just compile
    sed -i 's|"/nix/store/[^"]*gcc[^"]*|\"gcc|g' compile_commands.json
