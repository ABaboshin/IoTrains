import { CPlusPlusRenderer, CPlusPlusTargetLanguage, ClassType, Name, RenderContext, Sourcelike, cPlusPlusOptions, getOptionValues } from "quicktype-core";
import { extendsTypeAttributeKind } from "./extendattribute";
import { responseTypeAttributeKind } from "./responseattribute";

export class CustomCPPTargetLanguage extends CPlusPlusTargetLanguage {
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
    const responseclass = responseTypeAttributeKind.tryGetInAttributes(attributes);

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

      if (baseclass !== undefined && responseclass) {
        this.emitLine([
          "void to_json(json & j) override;"
        ]);
      } else if (responseclass) {
        this.emitLine([
          "virtual void to_json(json & j);"
        ]);
      }

      this.emitClassMembers(c, constraints);
    });
  }
}
