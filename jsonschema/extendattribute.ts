import { JSONSchema, JSONSchemaAttributes, JSONSchemaType, Ref, TypeAttributeKind } from "quicktype-core";

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

export const extendsTypeAttributeKind = new ExtendsTypeAttributeKind();

export function extendsAttributeProducer(
  schema: JSONSchema,
  _canonicalRef: Ref,
  types: Set<JSONSchemaType>
): JSONSchemaAttributes | undefined {
  if (typeof schema !== "object") return undefined;

  if (!types.has("object")) return undefined;

  return { forType: extendsTypeAttributeKind.makeAttributes(schema.extends) };
}

