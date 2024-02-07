import { CPlusPlusRenderer, CPlusPlusTargetLanguage, ClassType, Name, OptionValues, RenderContext, Sourcelike, TargetLanguage, cPlusPlusOptions, getOptionValues } from "quicktype-core";
import { extendsTypeAttributeKind } from "./extendattribute";
import { responseTypeAttributeKind } from "./responseattribute";
import { nameTypeAttributeKind } from "./nameattribute";

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

import * as util from 'util';

class CustomCPPRenderer extends CPlusPlusRenderer {

  protected baseClasses: Map<string, string>;

  constructor(targetLanguage: TargetLanguage, renderContext: RenderContext, _options: OptionValues<typeof cPlusPlusOptions>) {
    super(targetLanguage, renderContext, _options);
    this.baseClasses = new Map<string, string>();
  }

  protected emitClass(c: ClassType, className: Name): void {
    this.emitDescription(this.descriptionForType(c));

    const attributes = c.getAttributes();
    const baseclass = extendsTypeAttributeKind.tryGetInAttributes(attributes);
    const responseclass = responseTypeAttributeKind.tryGetInAttributes(attributes);

    if (baseclass !== undefined) {
      const name = nameTypeAttributeKind.tryGetInAttributes(attributes);
      if (name !== undefined) {
        this.baseClasses.set(name, baseclass);
      }
    }

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
  //   this.emitLine([
  //     "// test"
  //   ]);
  //   super.emitClassFunctions(c, className);
  // }

  protected emitUserNamespaceImpls() {
    super.emitUserNamespaceImpls();

    this.emitLine([
      `
    template<typename T>
    inline std::shared_ptr<T> from_json(const json & j) {
        return nullptr;
    }
      `
    ]);

    console.log(util.inspect(this.baseClasses));

    const baseTypes = [...new Set(Array.from(this.baseClasses.values()))];
    console.log(util.inspect(baseTypes));
    baseTypes.forEach(baseType => {
      console.log(util.inspect(baseType));
      this.emitLine([
        `template<> inline std::shared_ptr<${baseType}> from_json<${baseType}>(const json& j) {
          const auto discriminator = j.at("discriminator").get<std::string>();
          if (discriminator == "${baseType}") {
            std::shared_ptr<${baseType}> ptr;
            from_json(j, *ptr);
            return ptr;
          }
          `
      ]);
      const derivedTypes = Array.from(this.baseClasses.entries()).filter(x => x[1] == baseType).map(x => x[0]);
      derivedTypes.forEach(derivedType =>
        this.emitLine([
          `
          if (discriminator == "${derivedType}") {
            std::shared_ptr<${baseType}> ptr = std::make_shared<${derivedType}>();
            from_json(j, *(${derivedType}*)ptr.get());
            return ptr;
          }
          `
        ])
      );
      this.emitLine([
        `return nullptr; }`
      ]);
    });
  }
}
