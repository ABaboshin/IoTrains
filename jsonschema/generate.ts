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
import { nameAttributeProducer } from "./nameattribute";

async function quicktypeJSONSchema(targetLanguage: TargetLanguage) {
  const schemaInput = new JSONSchemaInput(new FetchingJSONSchemaStore(), [extendsAttributeProducer, responseAttributeProducer, nameAttributeProducer]);

  await schemaInput.addSource({ name: "Function", schema: await readFromFileOrURL("./function.json") });
  await schemaInput.addSource({ name: "CapabilityType", schema: await readFromFileOrURL("./capabilitytype.json") });
  await schemaInput.addSource({ name: "ControlUnit", schema: await readFromFileOrURL("./controlunit.json") });
  await schemaInput.addSource({ name: "DeviceInfo", schema: await readFromFileOrURL("./deviceinfo.json") });
  await schemaInput.addSource({ name: "TrainState", schema: await readFromFileOrURL("./trainstate.json") });
  await schemaInput.addSource({ name: "EventType", schema: await readFromFileOrURL("./eventtype.json") });
  await schemaInput.addSource({ name: "Event", schema: await readFromFileOrURL("./event.json") });
  await schemaInput.addSource({ name: "TrainCommand", schema: await readFromFileOrURL("./traincommand.json") });
  await schemaInput.addSource({ name: "Mp3Command", schema: await readFromFileOrURL("./mp3command.json") });
  await schemaInput.addSource({ name: "LightCommand", schema: await readFromFileOrURL("./lightcommand.json") });
  await schemaInput.addSource({ name: "OtaCommand", schema: await readFromFileOrURL("./otacommand.json") });

  const inputData = new InputData();
  inputData.addInput(schemaInput);

  return await quicktype({
    inputData,
    lang: targetLanguage,
    rendererOptions: {
      boost: false,
      namespace: "railschema",
      preferTypes: true
    }
  });
}

async function main() {
  const { lines: linescpp } = await quicktypeJSONSchema(new CustomCPPTargetLanguage());
  fs.writeFileSync("../firmware/lib/iotrains/src/schema.hpp", linescpp.join("\n"));

  const { lines: linests } = await quicktypeJSONSchema(new CustomTypeScriptTargetLanguage());
  fs.writeFileSync("../webui/common/index.ts", linests.join("\n"));
}

main();

