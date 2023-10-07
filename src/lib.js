const bindings = require("bindings")("my_addon.node");

console.log("-----------------------------------------------");
console.log("------------test throw from C++----------------");
try {
  bindings.crash();
} catch (e) {
  console.log(e);
}
console.log("-----------------------------------------------\n\n");

console.log("-----------------------------------------------");
console.log("-----------test add number from C++------------");
const addResult = bindings.add(1, 2, 3, 4);
console.log(`add 1,2,3,4 from C++, the result: ${addResult}`);
console.log("-----------------------------------------------\n\n");
