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

// import * as util from 'util';

class CustomCPPRenderer extends CPlusPlusRenderer {

  // protected baseClasses: Map<string, string>;

  protected emitClass(c: ClassType, className: Name): void {
    this.emitDescription(this.descriptionForType(c));

    const attributes = c.getAttributes();
    const baseclass = extendsTypeAttributeKind.tryGetInAttributes(attributes);
    const responseclass = responseTypeAttributeKind.tryGetInAttributes(attributes);

    // if (baseclass !== undefined)
    // {
    //   // this.baseClasses.set(className, baseclass);
    //   console.log(util.inspect(c));
    //   console.log(util.inspect(className));
    //   console.log(typeof className)
    // }

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

  // protected emitClassFunctions(c: ClassType, className: Name): void {
  //   super.emitClassFunctions(c, className);
  // }

  // protected emitUserNamespaceImpls() {
  //   super.emitUserNamespaceImpls();

  //   this.emitLine([
  //     `
  //   template<typename T>
  //   inline std::shared_ptr<T> from_json(const json & j) {
  //       return nullptr;
  //   }
  //     `
  //   ]);

  //   this.forEachObject("leading-and-interposing", (_c: ClassType, className: Name) =>
  //     {
  //     this.forEachClassProperty(c2, "none", (name, json, p) => {

  //     });

  //     this.emitLine([
  //       "template<> inline std::shared_ptr<", className,
  //       "> from_json(const json& j) {",
  //       className,
  //       " obj; from_json(j, obj);",
  //       "if (obj.discripminator == \"", className, "\");",
  //       "return nullptr",
  //       "}"
  //     ]);
  //     }
  //   );
  // }
}
