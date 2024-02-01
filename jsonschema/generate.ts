import {
  quicktype,
  InputData,
  JSONSchemaInput,
  FetchingJSONSchemaStore,
  readFromFileOrURL,
  TargetLanguage,
  CPlusPlusRenderer,
  CPlusPlusTargetLanguage,
  RenderContext,
  cPlusPlusOptions,
  getOptionValues,
  TypeAttributeKind,
  JSONSchema,
  Ref,
  JSONSchemaType,
  JSONSchemaAttributes,
  ClassType,
  Sourcelike
} from "quicktype-core";
import * as fs from "fs";

class ProcessTypeAttributeKind extends TypeAttributeKind<boolean> {
  constructor() {
    super("process");
  }

  combine(attrs: boolean[]): boolean {
    return attrs.some(x => x);
  }

  makeInferred(_: boolean): undefined {
    return undefined;
  }

  stringify(isProcess: boolean): string {
    return isProcess.toString();
  }
}

const processTypeAttributeKind = new ProcessTypeAttributeKind();

function processAttributeProducer(
  schema: JSONSchema,
  canonicalRef: Ref,
  types: Set<JSONSchemaType>
): JSONSchemaAttributes | undefined {
  if (typeof schema !== "object") return undefined;

  if (!types.has("object")) return undefined;

  let processing: boolean;
  if (schema.processing === undefined) {
    processing = false;
  } else if (typeof schema.processing === "boolean") {
    processing = schema.processing;
  } else {
    throw new Error(`processing is not a boolean in ${canonicalRef}`);
  }

  return { forType: processTypeAttributeKind.makeAttributes(processing) };
}

class CustomCPPTargetLanguage extends CPlusPlusTargetLanguage {
  constructor() {
    super("C++", ["cpp"], "cpp");
  }

  protected makeRenderer(renderContext: RenderContext, untypedOptionValues: { [name: string]: any }): CPlusPlusRenderer {
    return new CustomCPPRenderer(this, renderContext, getOptionValues(cPlusPlusOptions, untypedOptionValues));
  }
}

class CustomCPPRenderer extends CPlusPlusRenderer {
  protected emitClassMembers(c: ClassType, constraints: Map<string, Sourcelike> | undefined): void
  {
    super.emitClassMembers(c, constraints);

    const attributes = c.getAttributes();
    const processing = processTypeAttributeKind.tryGetInAttributes(attributes);
    if (processing)
    {
      this.emitLine([
        "virtual void ProcessCommand(const Command& command) const {}"
      ]);
    }

  }
}

async function quicktypeJSONSchema(targetLanguage : string | TargetLanguage) {
  const schemaInput = new JSONSchemaInput(new FetchingJSONSchemaStore(), [processAttributeProducer]);

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
  const { lines: linescpp } = await quicktypeJSONSchema(new CustomCPPTargetLanguage());
  fs.writeFileSync("../pio/src/schema.hpp", linescpp.join("\n"));

  const { lines: linests } = await quicktypeJSONSchema("typescript");
  fs.writeFileSync("../webui/common/index.ts", linests.join("\n"));
}

main();
