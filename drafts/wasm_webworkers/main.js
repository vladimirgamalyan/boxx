
console.log("start main.js");
var myWorker = new Worker('worker.js');

// runWorker = function () {
	// console.log("runWorker");
	// myWorker.postMessage(42);
// }

myWorker.onmessage = function(e) {
  console.log('Message received from worker ' + e.data);
}



// Create a Web Worker (separate thread) that we'll pass the WebAssembly module to.
var g_WebWorker = new Worker("worker.js");
g_WebWorker.onerror = function (evt) { console.log(`Error from Web Worker: ${evt.message}`); }
g_WebWorker.onmessage = function (evt) {
	//alert(`Message from the Web Worker:\n\n ${evt.data}`);
	console.log('Message received from worker ' + evt.data);
}
// Request the wasm file from the server and compile it...(Typically we would call
// 'WebAssembly.instantiate' which compiles and instantiates the module. In this
// case, however, we just want the compiled module which will be passed to the Web
// Worker. The Web Worker will be responsible for instantiating the module.)
fetch("worker_wasm.wasm").then(response =>
	response.arrayBuffer()
).then(bytes =>
	WebAssembly.compile(bytes)
).then(WasmModule =>
	g_WebWorker.postMessage({ "MessagePurpose": "CompiledModule", "WasmModule": WasmModule })
);

function OnClickTest() {
// Ask the Web Worker to add two values
	g_WebWorker.postMessage({ "MessagePurpose": "AddValues", "Val1": 1, "Val2": 2 });
}

// fetch("worker_wasm.wasm").then(function (response) {
	// WebAssembly.instantiate(response.arrayBuffer()).then(({ instance }) => {
		// console.log(instance.exports.add(40, 2));
	// });
// });
