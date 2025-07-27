import {
  JSONSchema,
  JSONSchemaAttributes,
  JSONSchemaType,
  Ref,
  TypeAttributeKind,
} from "quicktype-core";

class NameTypeAttributeKind extends TypeAttributeKind<string> {
  constructor() {
    super("name");
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

export const nameTypeAttributeKind = new NameTypeAttributeKind();

export function nameAttributeProducer(
  schema: JSONSchema,
  _canonicalRef: Ref,
  types: Set<JSONSchemaType>
): JSONSchemaAttributes | undefined {
  if (typeof schema !== "object") return undefined;

  if (!types.has("object")) return undefined;

  return { forType: nameTypeAttributeKind.makeAttributes(schema.name) };
}
