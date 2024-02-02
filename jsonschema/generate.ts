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
  Sourcelike,
  Name,
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

class ExtendsTypeAttributeKind extends TypeAttributeKind<string> {
  constructor() {
    super("extends");
  }

  combine(attrs: string[]): string {
    return attrs[0];
  }

  makeInferred(_: string): undefined {
    return undefined;
  }

  stringify(isProcess: string): string {
    return isProcess.toString();
  }
}

const extendsTypeAttributeKind = new ExtendsTypeAttributeKind();

function extendsAttributeProducer(
  schema: JSONSchema,
  _canonicalRef: Ref,
  types: Set<JSONSchemaType>
): JSONSchemaAttributes | undefined {
  if (typeof schema !== "object") return undefined;

  if (!types.has("object")) return undefined;

  return { forType: extendsTypeAttributeKind.makeAttributes(schema.extends) };
}

class CustomCPPTargetLanguage extends CPlusPlusTargetLanguage {
  constructor() {
    super("C++", ["cpp"], "cpp");
  }

  protected makeRenderer(renderContext: RenderContext, untypedOptionValues: { [name: string]: any }): CPlusPlusRenderer {
    let options = getOptionValues(cPlusPlusOptions, untypedOptionValues);
    options.codeFormat = false;
    return new CustomCPPRenderer(this, renderContext, options);
  }
}

class CustomCPPRenderer extends CPlusPlusRenderer {

  protected emitClass(c: ClassType, className: Name): void {
    this.emitDescription(this.descriptionForType(c));

    const attributes = c.getAttributes();
    const baseclass = extendsTypeAttributeKind.tryGetInAttributes(attributes);

    this.emitBlock(["class ", className, baseclass === undefined ? "" : " : public " + baseclass], true, () => {
      const constraints = this.generateClassConstraints(c);
      this.emitLine("public:");
      if (constraints === undefined) {
        this.emitLine(className, "() = default;");
      } else {
        this.emitLine(className, "() :");
        let numEmits = 0;
        constraints.forEach((initializer: Sourcelike, _propName: string) => {
          numEmits++;
          this.indent(() => {
            if (numEmits === constraints.size) {
              this.emitLine(initializer);
            } else {
              this.emitLine(initializer, ",");
            }
          });
        });
        this.emitLine("{}");
      }

      this.emitLine("virtual ~", className, "() = default;");
      this.ensureBlankLine();

      if (baseclass !== undefined) {
        this.emitLine([
          "void to_json(json & j);"
        ]);
      } else {
        this.emitLine([
          "virtual void to_json(json & j) {}"
        ]);
      }

      this.emitClassMembers(c, constraints);
    });
  }
}

async function quicktypeJSONSchema(targetLanguage: string | TargetLanguage) {
  const schemaInput = new JSONSchemaInput(new FetchingJSONSchemaStore(), [processAttributeProducer, extendsAttributeProducer]);

  await schemaInput.addSource({ name: "Function", schema: await readFromFileOrURL("./function.json") });
  await schemaInput.addSource({ name: "DeviceType", schema: await readFromFileOrURL("./devicetype.json") });
  await schemaInput.addSource({ name: "Command", schema: await readFromFileOrURL("./command.json") });
  await schemaInput.addSource({ name: "ControlUnit", schema: await readFromFileOrURL("./controlunit.json") });
  await schemaInput.addSource({ name: "TrainState", schema: await readFromFileOrURL("./trainstate.json") });

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

