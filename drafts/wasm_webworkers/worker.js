// const memory = new WebAssembly.Memory({ initial: 256 });
// const arrayBuffer = memory.buffer;
// const buffer = new Uint8Array(arrayBuffer);

var g_objInstance = null;

var wasmModule = {
	'wasi_snapshot_preview1': {
		'proc_exit': function () {}
	},
   'env': {
	   'emscripten_notify_memory_growth': function (v) {
		   console.log('emscripten_notify_memory_growth ' + v);
	   },
	 'cb': function(v) { console.log('foo ' + v); },
	 'logString': (data, len) => {
			const b = g_objInstance.exports.memory.buffer;
			const buffer = new Uint8Array(b);
			console.log('buffer len:' + buffer.length);
            let s = "";
            for (let i = 0; i < len; ++i)
                s += String.fromCharCode(buffer[data + i]);
				//s += buffer[data + i].toString();
				//console.log(typeof buffer[data + i]);
            console.log(data, len);
			console.log(s.length);
			console.log(s);
        }
   }
 };
 


function call_c_code(data) {
	return g_objInstance.exports.run(data.Val1, data.Val2);
}

self.onmessage = function (evt) {
  // If we've been asked to call the module's Add method then...
  var objData = evt.data;
  var sMessagePurpose = objData.MessagePurpose;
  if (sMessagePurpose === "AddValues") {
    // Call the add method in the WebAssembly module and pass the result back to the main thread
    var iResult = call_c_code(objData);
    self.postMessage(`This is the Web Worker...The result of ${objData.Val1.toString()} + ${objData.Val2.toString()} is ${iResult.toString()}.`);
  } // If we've been passed a compiled WebAssembly module then...
  else if (sMessagePurpose === "CompiledModule") {
    // NOTE: Unlike when we pass in the bytes to instantiate, we don't have a separate 'instance'
    // and 'modules' object returned in this case since we started out with the module object.
    // We're only passed back the instance in this case.
	
    // WebAssembly.instantiate(objData.WasmModule, wasmModule).then(instance => 
      // g_objInstance = instance // Hold onto the module's instance so that we can reuse it
    // );
	g_objInstance = new WebAssembly.Instance(objData.WasmModule, wasmModule);

  }
}
