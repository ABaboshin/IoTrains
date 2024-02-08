import { ClassType, Name, RenderContext, TypeScriptRenderer, TypeScriptTargetLanguage, getOptionValues, tsFlowOptions } from "quicktype-core";
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

    // this.emitBlock(
    //   ["export class ", className, "Impl implements ", className],
    //   "",
    //   () => {
    //     this.emitLine([`
    //     constructor(){
    //     `]);
    //     this.forEachClassProperty(c, "none", (name, jsonName, p) => {
    //       console.log(jsonName);
    //       console.log(util.inspect(name));
    //       console.log(util.inspect(p));
    //     });
    //     // this.emitPropertyTable(c, (name, _jsonName, p) => {
    //     //   const t = p.type;
    //     //   this.emitLine([
    //     //     `this.${modifySource(x => x, name)} = new ${this.sourceFor(t).source}Impl();`
    //     //   ]);

    //     //   return [
    //     //   ];
    //     // });
    //     this.emitLine([`
    //     }
    //     `]);
    //   }
    // );
  }
}
