emcc expand_image.cpp -O1 -s WASM=1 -s EXPORTED_FUNCTIONS="[_set_arg, _get_result, _get_result_size, _exec]" -o expand_image.html
wasm2wat expand_image.wasm -o expand_image.wast