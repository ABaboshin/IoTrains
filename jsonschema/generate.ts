import {
  quicktype,
  InputData,
  JSONSchemaInput,
  FetchingJSONSchemaStore,
  readFromFileOrURL,
  TargetLanguage
} from "quicktype-core";
import * as fs from "fs";

async function quicktypeJSONSchema(targetLanguage : string | TargetLanguage) {
  const schemaInput = new JSONSchemaInput(new FetchingJSONSchemaStore());

  await schemaInput.addSource({ name: "Function", schema: await readFromFileOrURL("./function.json") });
  await schemaInput.addSource({ name: "DeviceType", schema: await readFromFileOrURL("./devicetype.json") });
  // await schemaInput.addSource({ name: "Device", schema: await readFromFileOrURL("./device.json") });
  await schemaInput.addSource({ name: "Command", schema: await readFromFileOrURL("./command.json") });
  await schemaInput.addSource({ name: "ControlUnit", schema: await readFromFileOrURL("./controlunit.json") });

  const inputData = new InputData();
  inputData.addInput(schemaInput);

  return await quicktype({
    inputData,
    lang: targetLanguage,
    rendererOptions: {
      boost: false,
      namespace: "railschema"
    }
  });
}

async function main() {
  const { lines: linescpp } = await quicktypeJSONSchema("c++");
  fs.writeFileSync("../pio/src/schema.hpp", linescpp.join("\n"));

  const { lines: linests } = await quicktypeJSONSchema("typescript");
  fs.writeFileSync("../webui/common/index.ts", linests.join("\n"));
}

main();
