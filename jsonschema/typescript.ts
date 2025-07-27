import {
  ClassType,
  Name,
  RenderContext,
  TypeScriptRenderer,
  TypeScriptTargetLanguage,
  getOptionValues,
  tsFlowOptions,
} from "quicktype-core";
import { extendsTypeAttributeKind } from "./extendattribute";

// https://github.com/glideapps/quicktype/blob/master/packages/quicktype-core/src/language/TypeScriptFlow.ts
export class CustomTypeScriptTargetLanguage extends TypeScriptTargetLanguage {
  constructor() {
    super();
  }

  protected makeRenderer(
    renderContext: RenderContext,
    untypedOptionValues: { [name: string]: any }
  ): TypeScriptRenderer {
    let options = getOptionValues(tsFlowOptions, untypedOptionValues);
    // options.preferTypes = true;
    return new CustomTypeScriptRenderer(this, renderContext, options);
  }
}

// import * as util from 'util'

class CustomTypeScriptRenderer extends TypeScriptRenderer {
  protected emitClassBlock(c: ClassType, className: Name): void {
    const attributes = c.getAttributes();
    const baseclass = extendsTypeAttributeKind.tryGetInAttributes(attributes);
    const suffix = baseclass !== undefined ? ` extends ${baseclass} ` : "";

    this.emitBlock(
      this._tsFlowOptions.preferTypes
        ? ["export class ", className, suffix]
        : ["export interface ", className, suffix],
      "",
      () => {
        this.emitClassBlockBody(c);
      }
    );
  }
}
