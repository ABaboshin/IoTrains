import { RendererOptions } from "quicktype-core/dist/language/options.types";
import { LanguageName } from "quicktype-core/dist/language/types";
import { RenderContext } from "quicktype-core/dist/Renderer";
import { OptionValues, getOptionValues } from "quicktype-core/dist/RendererOptions";
import { TargetLanguage } from "quicktype-core/dist/TargetLanguage";
import { cPlusPlusOptions, CPlusPlusTargetLanguage } from "./CPlusPlus/language";
import { CPlusPlusRenderer } from "./CPlusPlus/CPlusPlusRenderer";
import { ClassType, EnumType, Type, UnionType } from "quicktype-core/dist/Type/Type";
import { Name } from "quicktype-core/dist/Naming";
import { extendsTypeAttributeKind } from "./extendattribute";
import { nameTypeAttributeKind } from "./nameattribute";
import { Sourcelike } from "quicktype-core/dist/Source";
import { defined } from "quicktype-core/dist/support/Support";
import { WrappingCode } from "./CPlusPlus/utils";
import { stringEscape } from "quicktype-core/dist/support/Strings";
import { removeNullFromUnion } from "quicktype-core/dist/Type/TypeUtils";

export class CustomCPPTargetLanguage extends CPlusPlusTargetLanguage {
  protected makeRenderer<Lang extends LanguageName = "c++">(
      renderContext: RenderContext,
      untypedOptionValues: RendererOptions<Lang>,
    ): CPlusPlusRenderer {
    let options = getOptionValues(cPlusPlusOptions, untypedOptionValues);
    options.codeFormat = false;
    options.includeLocation = false;
    return new CustomCPPRenderer(this, renderContext, options);
  }
}

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

    const name = nameTypeAttributeKind.tryGetInAttributes(attributes);
    if (baseclass !== undefined) {
      if (name !== undefined) {
        this.baseClasses.set(name, baseclass);
      }
    }

    this.emitBlock(["class ", className, baseclass === undefined ? "" : " : public " + baseclass], true, () => {
      const constraints = this.generateClassConstraints(c);
      this.emitLine("public:");
      this.emitLine("std::string discriminator;");
      if (constraints === undefined) {
        if (name === undefined) {
          this.emitLine(className, "()= default;");
        } else {
          this.emitLine(className, `() { discriminator =  "${name}"; }`);
        }

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

      this.emitClassMembers(c, constraints);
    });
  }

  protected emitClassFunctions(c: ClassType, className: Name): void {
    const ourQualifier = this.ourQualifier(false);
    let cppType: Sourcelike;
    let toType: Sourcelike;

    const attributes = c.getAttributes();
    const baseclass = extendsTypeAttributeKind.tryGetInAttributes(attributes);

    this.emitBlock(
      ["inline void from_json(", this.withConst("json"), " & j, ", ourQualifier, className, "& x)"],
      false,
      () => {
        if (baseclass !== undefined) {
          this.emitLine(`
          from_json(j, (${baseclass}&)x);
          `);
        }
        this.forEachClassProperty(c, "none", (name, json, p) => {
          const [, , setterName] = defined(this._gettersAndSettersForPropertyName.get(name));
          const propType = p.type;



          let assignment: WrappingCode;
          if (this._options.codeFormat) {
            assignment = new WrappingCode(["x.", setterName, "("], [")"]);
          } else {
            assignment = new WrappingCode(["x.", name, " = "], []);
          }

          if (propType.kind === "null" || propType.kind === "any") {
            this.emitLine(
              assignment.wrap(
                [],
                [
                  ourQualifier,
                  "get_untyped(j, ",
                  this._stringType.wrapEncodingChange(
                    [ourQualifier],
                    this._stringType.getType(),
                    this.NarrowString.getType(),
                    [this._stringType.createStringLiteral([stringEscape(json)])]
                  ),
                  ")"
                ]
              ),
              ";"
            );
            return;
          }
          if (p.isOptional || propType instanceof UnionType) {
            const [nullOrOptional, typeSet] = (function (): [boolean, ReadonlySet<Type>] {
              if (propType instanceof UnionType) {
                const [maybeNull, nonNulls] = removeNullFromUnion(propType, true);
                return [maybeNull !== null || p.isOptional, nonNulls];
              } else {
                const set = new Set<Type>();
                set.add(propType);
                return [true, set];
              }
            })();
            if (nullOrOptional) {
              cppType = this.cppTypeInOptional(
                typeSet,
                {
                  needsForwardIndirection: false,
                  needsOptionalIndirection: false,
                  inJsonNamespace: false
                },
                false,
                true
              );
              toType = this.cppTypeInOptional(
                typeSet,
                {
                  needsForwardIndirection: false,
                  needsOptionalIndirection: false,
                  inJsonNamespace: false
                },
                false,
                false
              );
              this.emitLine(
                assignment.wrap(
                  [],
                  [
                    this._stringType.wrapEncodingChange(
                      [ourQualifier],
                      [this.optionalType(propType), "<", cppType, ">"],
                      [this.optionalType(propType), "<", toType, ">"],
                      [
                        ourQualifier,
                        `get_${this.optionalTypeLabel(propType)}_optional<`,
                        cppType,
                        ">(j, ",
                        this._stringType.wrapEncodingChange(
                          [ourQualifier],
                          this._stringType.getType(),
                          this.NarrowString.getType(),
                          [this._stringType.createStringLiteral([stringEscape(json)])]
                        ),
                        ")"
                      ]
                    )
                  ]
                ),
                ";"
              );
              return;
            }
          }
          cppType = this.cppType(
            propType,
            {
              needsForwardIndirection: true,
              needsOptionalIndirection: true,
              inJsonNamespace: false
            },
            false,
            true,
            p.isOptional
          );
          toType = this.cppType(
            propType,
            {
              needsForwardIndirection: true,
              needsOptionalIndirection: true,
              inJsonNamespace: false
            },
            false,
            false,
            p.isOptional
          );
          this.emitLine(
            assignment.wrap(
              [],
              this._stringType.wrapEncodingChange([ourQualifier], cppType, toType, [
                "j.at(",
                this._stringType.wrapEncodingChange(
                  [ourQualifier],
                  this._stringType.getType(),
                  this.NarrowString.getType(),
                  this._stringType.createStringLiteral([stringEscape(json)])
                ),
                ").get<",
                cppType,
                ">()"
              ])
            ),
            ";"
          );
        });
      }
    );
    this.ensureBlankLine();

    this.emitBlock(
      ["inline void to_json(json & j, ", this.withConst([ourQualifier, className]), " & x)"],
      false,
      () => {
        if (baseclass !== undefined) {
          this.emitLine(`
          to_json(j, (const ${baseclass}&)x);
          `);
        } else {
          this.emitLine("j = json::object();");
        }

        this.forEachClassProperty(c, "none", (name, json, p) => {
          const propType = p.type;
          cppType = this.cppType(
            propType,
            {
              needsForwardIndirection: true,
              needsOptionalIndirection: true,
              inJsonNamespace: false
            },
            false,
            false,
            p.isOptional
          );
          toType = this.cppType(
            propType,
            {
              needsForwardIndirection: true,
              needsOptionalIndirection: true,
              inJsonNamespace: false
            },
            false,
            true,
            p.isOptional
          );
          const [getterName, ,] = defined(this._gettersAndSettersForPropertyName.get(name));
          let getter: Sourcelike[];
          if (this._options.codeFormat) {
            getter = [getterName, "()"];
          } else {
            getter = [name];
          }
          const assignment: Sourcelike[] = [
            "j[",
            this._stringType.wrapEncodingChange(
              [ourQualifier],
              this._stringType.getType(),
              this.NarrowString.getType(),
              this._stringType.createStringLiteral([stringEscape(json)])
            ),
            "] = ",
            this._stringType.wrapEncodingChange([ourQualifier], cppType, toType, ["x.", getter]),
            ";"
          ];
          if (p.isOptional && this._options.hideNullOptional) {
            this.emitBlock(
              [
                "if (",
                this._stringType.wrapEncodingChange([ourQualifier], cppType, toType, ["x.", getter]),
                ")"
              ],
              false,
              () => {
                this.emitLine(assignment);
              }
            );
          } else {
            this.emitLine(assignment);
          }
        });
      }
    );
  }

  protected emitUserNamespaceImpls() {
    this.forEachObject("leading-and-interposing", (c: ClassType, className: Name) =>
      this.emitClassFunctions(c, className)
    );

    this.forEachEnum("leading-and-interposing", (e: EnumType, enumName: Name) =>
      this.emitEnumFunctions(e, enumName)
    );

    this.emitLine([
      `
    template<typename T>
    inline std::shared_ptr<T> from_json(const json & j) {
        return nullptr;
    }

    template<typename T>
    inline void to_json(json & j, std::shared_ptr<T> data) {

    }
      `
    ]);

    const baseTypes = [...new Set(Array.from(this.baseClasses.values()))];
    baseTypes.forEach(baseType => {
      this.emitLine([
        `template<> inline std::shared_ptr<${baseType}> from_json<${baseType}>(const json& j) {
          const auto discriminator = j.at("discriminator").get<std::string>();
          if (discriminator == "${baseType}") {
            std::shared_ptr<${baseType}> ptr;
            from_json(j, *ptr);
            ptr->discriminator = "${baseType}";
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
            ptr->discriminator = "${derivedType}";
            return ptr;
          }
          `
        ])
      );
      this.emitLine([
        `return nullptr; }`
      ]);

      this.emitLine([
        `template<> inline void to_json(json& j, std::shared_ptr<${baseType}> data) {
          if (data->discriminator == "${baseType}") {
            to_json(j, *data.get());
          }
          `
      ]);

      derivedTypes.forEach(derivedType =>
        this.emitLine([
          `
          if (data->discriminator == "${derivedType}") {
            to_json(j, *(${derivedType}*)data.get());
          }
          `
        ])
      );
      this.emitLine([
        `}`
      ]);
    });
  }
}
