import {
  JSONSchema,
  JSONSchemaAttributes,
  JSONSchemaType,
  Ref,
  TypeAttributeKind,
} from "quicktype-core";

class ResponseTypeAttributeKind extends TypeAttributeKind<boolean> {
  constructor() {
    super("response");
  }

  combine(attrs: boolean[]): boolean {
    return attrs.some((x) => x);
  }

  makeInferred(_: boolean): undefined {
    return undefined;
  }

  stringify(isResponse: boolean): string {
    return isResponse.toString();
  }
}

export const responseTypeAttributeKind = new ResponseTypeAttributeKind();

export function responseAttributeProducer(
  schema: JSONSchema,
  canonicalRef: Ref,
  types: Set<JSONSchemaType>
): JSONSchemaAttributes | undefined {
  if (typeof schema !== "object") return undefined;

  if (!types.has("object")) return undefined;

  let response: boolean;
  if (schema.response === undefined) {
    response = false;
  } else if (typeof schema.response === "boolean") {
    response = schema.response;
  } else {
    throw new Error(`response is not a boolean in ${canonicalRef}`);
  }

  return { forType: responseTypeAttributeKind.makeAttributes(response) };
}
