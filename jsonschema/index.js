const $RefParser = require("@apidevtools/json-schema-ref-parser");

(async () => {
    let $refs = await $RefParser.resolve("schema.json");
    let values = $refs.values();
    console.log(values);
})();


