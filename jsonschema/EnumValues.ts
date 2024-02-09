// https://github.com/glideapps/quicktype/blob/master/packages/quicktype-core/src/attributes/EnumValues.ts
import { mapMap } from "collection-utils";
import { EnumType, JSONSchema, JSONSchemaAttributes, JSONSchemaType, Ref, TypeAttributeKind } from "quicktype-core";
import { AccessorNames, lookupKey, makeAccessorNames } from "./AccessorNames";


class EnumValuesTypeAttributeKind extends TypeAttributeKind<AccessorNames> {
  constructor() {
    super("enumValues");
  }
  makeInferred(_: AccessorNames) {
    return undefined;
  }
}

export const enumValuesTypeAttributeKind: TypeAttributeKind<AccessorNames> = new EnumValuesTypeAttributeKind();

export function enumCaseValues(e: EnumType, language: string): Map<string, [string, boolean] | undefined> {
  const enumValues = enumValuesTypeAttributeKind.tryGetInAttributes(e.getAttributes());
  if (enumValues === undefined) return mapMap(e.cases.entries(), _ => undefined);
  return mapMap(e.cases.entries(), c => lookupKey(enumValues, c, language));
}

export function enumValuesAttributeProducer(
  schema: JSONSchema,
  _canonicalRef: Ref | undefined,
  _types: Set<JSONSchemaType>
): JSONSchemaAttributes | undefined {
  if (typeof schema !== "object") return undefined;

  const maybeEnumValues = schema["qt-enum-values"];

  if (maybeEnumValues === undefined) return undefined;

  return { forType: enumValuesTypeAttributeKind.makeAttributes(makeAccessorNames(maybeEnumValues)) };
}
