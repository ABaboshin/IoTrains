import {
  quicktype,
  InputData,
  JSONSchemaInput,
  FetchingJSONSchemaStore,
  readFromFileOrURL,
  TargetLanguage,
} from "quicktype-core";
import * as fs from "fs";
import { CustomCPPTargetLanguage } from "./cpp";
import { CustomTypeScriptTargetLanguage } from "./typescript";
import { extendsAttributeProducer } from "./extendattribute";
import { responseAttributeProducer } from "./responseattribute";

async function quicktypeJSONSchema(targetLanguage: string | TargetLanguage) {
  const schemaInput = new JSONSchemaInput(new FetchingJSONSchemaStore(), [extendsAttributeProducer, responseAttributeProducer]);

  await schemaInput.addSource({ name: "Function", schema: await readFromFileOrURL("./function.json") });
  await schemaInput.addSource({ name: "DeviceType", schema: await readFromFileOrURL("./devicetype.json") });
  // await schemaInput.addSource({ name: "Command", schema: await readFromFileOrURL("./command.json") });
  await schemaInput.addSource({ name: "ControlUnit", schema: await readFromFileOrURL("./controlunit.json") });
  // await schemaInput.addSource({ name: "State", schema: await readFromFileOrURL("./state.json") });
  await schemaInput.addSource({ name: "DeviceInfo", schema: await readFromFileOrURL("./deviceinfo.json") });
  await schemaInput.addSource({ name: "TrainState", schema: await readFromFileOrURL("./trainstate.json") });
  await schemaInput.addSource({ name: "EventType", schema: await readFromFileOrURL("./eventtype.json") });
  await schemaInput.addSource({ name: "Event", schema: await readFromFileOrURL("./event.json") });

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
  const { lines: linescpp } = await quicktypeJSONSchema(new CustomCPPTargetLanguage());
  fs.writeFileSync("../firmware/src/schema.hpp", linescpp.join("\n"));

  const { lines: linests } = await quicktypeJSONSchema(new CustomTypeScriptTargetLanguage());
  fs.writeFileSync("../webui/common/index.ts", linests.join("\n"));
}

main();

