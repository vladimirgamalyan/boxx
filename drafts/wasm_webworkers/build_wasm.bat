set emcc=C:\emsdk\upstream\emscripten\emcc
set wasm2wat=K:\projects\boxx\drafts\wasm_webworkers\wabt\wasm2wat.exe

call %emcc% worker_wasm.cpp -std=c++11 -O3 --js-library library.js -s EXPORTED_FUNCTIONS="['_run']" -s TOTAL_MEMORY=33554432 -s ALLOW_MEMORY_GROWTH=1 -o worker_wasm.wasm
:: %wasm2wat% worker_wasm.wasm
