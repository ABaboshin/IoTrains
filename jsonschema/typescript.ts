import { ClassType, Name, RenderContext, TypeScriptRenderer, TypeScriptTargetLanguage, getOptionValues, tsFlowOptions } from "quicktype-core";
import { extendsTypeAttributeKind } from "./extendattribute";

export class CustomTypeScriptTargetLanguage extends TypeScriptTargetLanguage {
  constructor() {
    super();
  }

  protected makeRenderer(
    renderContext: RenderContext,
    untypedOptionValues: { [name: string]: any }
  ): TypeScriptRenderer {
    return new CustomTypeScriptRenderer(this, renderContext, getOptionValues(tsFlowOptions, untypedOptionValues));
  }
}

class CustomTypeScriptRenderer extends TypeScriptRenderer {

  protected emitClassBlock(c: ClassType, className: Name): void {

    const attributes = c.getAttributes();
    const baseclass = extendsTypeAttributeKind.tryGetInAttributes(attributes);
    const suffix = baseclass !== undefined ? ` extends ${baseclass} ` : "";

    this.emitBlock(
      this._tsFlowOptions.preferTypes
        ? ["export type ", className, " = "]
        : ["export interface ", className, suffix],
      "",
      () => {
        this.emitClassBlockBody(c);
      }
    );
  }
}
