// To parse this data:
//
//   import { Convert, Function, CapabilityType, ControlUnit, DeviceInfo, TrainState, EventType, Event, RFIDEvent, TrainCommand, Mp3Command, LightCommand, OtaCommand } from "./file";
//
//   const function = Convert.toFunction(json);
//   const capabilityType = Convert.toCapabilityType(json);
//   const controlUnit = Convert.toControlUnit(json);
//   const deviceInfo = Convert.toDeviceInfo(json);
//   const trainState = Convert.toTrainState(json);
//   const eventType = Convert.toEventType(json);
//   const event = Convert.toEvent(json);
//   const rFIDEvent = Convert.toRFIDEvent(json);
//   const trainCommand = Convert.toTrainCommand(json);
//   const mp3Command = Convert.toMp3Command(json);
//   const lightCommand = Convert.toLightCommand(json);
//   const otaCommand = Convert.toOtaCommand(json);
//
// These functions will throw an error if the JSON doesn't
// match the expected interface, even if the JSON is valid.

export interface ControlUnit{
    devices: Device[];
    id:      string;
    [property: string]: any;
}

export interface Device{
    capabilities: Capability[];
    id:           string;
    type:         any;
    [property: string]: any;
}

export interface Capability{
    type:  CapabilityType;
    value: string;
    [property: string]: any;
}

export enum CapabilityType {
    Light = "light",
    Ota = "ota",
    PlayID = "play_id",
    PlayURL = "play_url",
    Player = "player",
    StopPlay = "stop_play",
    Train = "train",
    Turnout = "turnout",
}

export interface DeviceInfo{
    device: Device;
    state?: State;
    [property: string]: any;
}

export interface State{
    command?:     Command;
    description?: string;
    id:           string;
    ok:           boolean;
    [property: string]: any;
}

export interface Command{
    function: Function;
    [property: string]: any;
}

export enum Function {
    Break = "break",
    MoveBackward = "move_backward",
    MoveForward = "move_forward",
    Off = "off",
    On = "on",
    PlayID = "play_id",
    PlayURL = "play_url",
    StopPlay = "stop_play",
    TurnoutPos1 = "turnout_pos1",
    TurnoutPos2 = "turnout_pos2",
    Update = "update",
}

export interface TrainState extends State {
    direction?: Direction;
    speed:      number;
    [property: string]: any;
}

export enum Direction {
    Backward = "backward",
    Forward = "forward",
    Stop = "stop",
}

export interface Event{
    type: EventType;
    [property: string]: any;
}

export enum EventType {
    Train = "train",
}

export interface RFIDEvent extends Event {
    value: string;
    [property: string]: any;
}

export interface TrainCommand extends Command {
    speed: number;
    [property: string]: any;
}

export interface Mp3Command extends Command {
    url: string;
    [property: string]: any;
}

export interface LightCommand extends Command {
    name: string;
    [property: string]: any;
}

export interface OtaCommand extends Command {
    url: string;
    [property: string]: any;
}

// Converts JSON strings to/from your types
// and asserts the results of JSON.parse at runtime
export class Convert {
    public static toFunction(json: string): Function {
        return cast(JSON.parse(json), r("Function"));
    }

    public static functionToJson(value: Function): string {
        return JSON.stringify(uncast(value, r("Function")), null, 2);
    }

    public static toCapabilityType(json: string): CapabilityType {
        return cast(JSON.parse(json), r("CapabilityType"));
    }

    public static capabilityTypeToJson(value: CapabilityType): string {
        return JSON.stringify(uncast(value, r("CapabilityType")), null, 2);
    }

    public static toControlUnit(json: string): ControlUnit {
        return cast(JSON.parse(json), r("ControlUnit"));
    }

    public static controlUnitToJson(value: ControlUnit): string {
        return JSON.stringify(uncast(value, r("ControlUnit")), null, 2);
    }

    public static toDeviceInfo(json: string): DeviceInfo {
        return cast(JSON.parse(json), r("DeviceInfo"));
    }

    public static deviceInfoToJson(value: DeviceInfo): string {
        return JSON.stringify(uncast(value, r("DeviceInfo")), null, 2);
    }

    public static toTrainState(json: string): TrainState {
        return cast(JSON.parse(json), r("TrainState"));
    }

    public static trainStateToJson(value: TrainState): string {
        return JSON.stringify(uncast(value, r("TrainState")), null, 2);
    }

    public static toEventType(json: string): EventType {
        return cast(JSON.parse(json), r("EventType"));
    }

    public static eventTypeToJson(value: EventType): string {
        return JSON.stringify(uncast(value, r("EventType")), null, 2);
    }

    public static toEvent(json: string): Event {
        return cast(JSON.parse(json), r("Event"));
    }

    public static eventToJson(value: Event): string {
        return JSON.stringify(uncast(value, r("Event")), null, 2);
    }

    public static toRFIDEvent(json: string): RFIDEvent {
        return cast(JSON.parse(json), r("RFIDEvent"));
    }

    public static rFIDEventToJson(value: RFIDEvent): string {
        return JSON.stringify(uncast(value, r("RFIDEvent")), null, 2);
    }

    public static toTrainCommand(json: string): TrainCommand {
        return cast(JSON.parse(json), r("TrainCommand"));
    }

    public static trainCommandToJson(value: TrainCommand): string {
        return JSON.stringify(uncast(value, r("TrainCommand")), null, 2);
    }

    public static toMp3Command(json: string): Mp3Command {
        return cast(JSON.parse(json), r("Mp3Command"));
    }

    public static mp3CommandToJson(value: Mp3Command): string {
        return JSON.stringify(uncast(value, r("Mp3Command")), null, 2);
    }

    public static toLightCommand(json: string): LightCommand {
        return cast(JSON.parse(json), r("LightCommand"));
    }

    public static lightCommandToJson(value: LightCommand): string {
        return JSON.stringify(uncast(value, r("LightCommand")), null, 2);
    }

    public static toOtaCommand(json: string): OtaCommand {
        return cast(JSON.parse(json), r("OtaCommand"));
    }

    public static otaCommandToJson(value: OtaCommand): string {
        return JSON.stringify(uncast(value, r("OtaCommand")), null, 2);
    }
}

function invalidValue(typ: any, val: any, key: any, parent: any = ''): never {
    const prettyTyp = prettyTypeName(typ);
    const parentText = parent ? ` on ${parent}` : '';
    const keyText = key ? ` for key "${key}"` : '';
    throw Error(`Invalid value${keyText}${parentText}. Expected ${prettyTyp} but got ${JSON.stringify(val)}`);
}

function prettyTypeName(typ: any): string {
    if (Array.isArray(typ)) {
        if (typ.length === 2 && typ[0] === undefined) {
            return `an optional ${prettyTypeName(typ[1])}`;
        } else {
            return `one of [${typ.map(a => { return prettyTypeName(a); }).join(", ")}]`;
        }
    } else if (typeof typ === "object" && typ.literal !== undefined) {
        return typ.literal;
    } else {
        return typeof typ;
    }
}

function jsonToJSProps(typ: any): any {
    if (typ.jsonToJS === undefined) {
        const map: any = {};
        typ.props.forEach((p: any) => map[p.json] = { key: p.js, typ: p.typ });
        typ.jsonToJS = map;
    }
    return typ.jsonToJS;
}

function jsToJSONProps(typ: any): any {
    if (typ.jsToJSON === undefined) {
        const map: any = {};
        typ.props.forEach((p: any) => map[p.js] = { key: p.json, typ: p.typ });
        typ.jsToJSON = map;
    }
    return typ.jsToJSON;
}

function transform(val: any, typ: any, getProps: any, key: any = '', parent: any = ''): any {
    function transformPrimitive(typ: string, val: any): any {
        if (typeof typ === typeof val) return val;
        return invalidValue(typ, val, key, parent);
    }

    function transformUnion(typs: any[], val: any): any {
        // val must validate against one typ in typs
        const l = typs.length;
        for (let i = 0; i < l; i++) {
            const typ = typs[i];
            try {
                return transform(val, typ, getProps);
            } catch (_) {}
        }
        return invalidValue(typs, val, key, parent);
    }

    function transformEnum(cases: string[], val: any): any {
        if (cases.indexOf(val) !== -1) return val;
        return invalidValue(cases.map(a => { return l(a); }), val, key, parent);
    }

    function transformArray(typ: any, val: any): any {
        // val must be an array with no invalid elements
        if (!Array.isArray(val)) return invalidValue(l("array"), val, key, parent);
        return val.map(el => transform(el, typ, getProps));
    }

    function transformDate(val: any): any {
        if (val === null) {
            return null;
        }
        const d = new Date(val);
        if (isNaN(d.valueOf())) {
            return invalidValue(l("Date"), val, key, parent);
        }
        return d;
    }

    function transformObject(props: { [k: string]: any }, additional: any, val: any): any {
        if (val === null || typeof val !== "object" || Array.isArray(val)) {
            return invalidValue(l(ref || "object"), val, key, parent);
        }
        const result: any = {};
        Object.getOwnPropertyNames(props).forEach(key => {
            const prop = props[key];
            const v = Object.prototype.hasOwnProperty.call(val, key) ? val[key] : undefined;
            result[prop.key] = transform(v, prop.typ, getProps, key, ref);
        });
        Object.getOwnPropertyNames(val).forEach(key => {
            if (!Object.prototype.hasOwnProperty.call(props, key)) {
                result[key] = transform(val[key], additional, getProps, key, ref);
            }
        });
        return result;
    }

    if (typ === "any") return val;
    if (typ === null) {
        if (val === null) return val;
        return invalidValue(typ, val, key, parent);
    }
    if (typ === false) return invalidValue(typ, val, key, parent);
    let ref: any = undefined;
    while (typeof typ === "object" && typ.ref !== undefined) {
        ref = typ.ref;
        typ = typeMap[typ.ref];
    }
    if (Array.isArray(typ)) return transformEnum(typ, val);
    if (typeof typ === "object") {
        return typ.hasOwnProperty("unionMembers") ? transformUnion(typ.unionMembers, val)
            : typ.hasOwnProperty("arrayItems")    ? transformArray(typ.arrayItems, val)
            : typ.hasOwnProperty("props")         ? transformObject(getProps(typ), typ.additional, val)
            : invalidValue(typ, val, key, parent);
    }
    // Numbers can be parsed by Date but shouldn't be.
    if (typ === Date && typeof val !== "number") return transformDate(val);
    return transformPrimitive(typ, val);
}

function cast<T>(val: any, typ: any): T {
    return transform(val, typ, jsonToJSProps);
}

function uncast<T>(val: T, typ: any): any {
    return transform(val, typ, jsToJSONProps);
}

function l(typ: any) {
    return { literal: typ };
}

function a(typ: any) {
    return { arrayItems: typ };
}

function u(...typs: any[]) {
    return { unionMembers: typs };
}

function o(props: any[], additional: any) {
    return { props, additional };
}

function m(additional: any) {
    return { props: [], additional };
}

function r(name: string) {
    return { ref: name };
}

const typeMap: any = {
    "ControlUnit": o([
        { json: "devices", js: "devices", typ: a(r("Device")) },
        { json: "id", js: "id", typ: "" },
    ], "any"),
    "Device": o([
        { json: "capabilities", js: "capabilities", typ: a(r("Capability")) },
        { json: "id", js: "id", typ: "" },
        { json: "type", js: "type", typ: "any" },
    ], "any"),
    "Capability": o([
        { json: "type", js: "type", typ: r("CapabilityType") },
        { json: "value", js: "value", typ: "" },
    ], "any"),
    "DeviceInfo": o([
        { json: "device", js: "device", typ: r("Device") },
        { json: "state", js: "state", typ: u(undefined, r("State")) },
    ], "any"),
    "State": o([
        { json: "command", js: "command", typ: u(undefined, r("Command")) },
        { json: "description", js: "description", typ: u(undefined, "") },
        { json: "id", js: "id", typ: "" },
        { json: "ok", js: "ok", typ: true },
    ], "any"),
    "Command": o([
        { json: "function", js: "function", typ: r("Function") },
    ], "any"),
    "TrainState": o([
        { json: "direction", js: "direction", typ: u(undefined, r("Direction")) },
        { json: "speed", js: "speed", typ: 0 },
    ], "any"),
    "Event": o([
        { json: "type", js: "type", typ: r("EventType") },
    ], "any"),
    "RFIDEvent": o([
        { json: "value", js: "value", typ: "" },
    ], "any"),
    "TrainCommand": o([
        { json: "speed", js: "speed", typ: 0 },
    ], "any"),
    "Mp3Command": o([
        { json: "url", js: "url", typ: "" },
    ], "any"),
    "LightCommand": o([
        { json: "name", js: "name", typ: "" },
    ], "any"),
    "OtaCommand": o([
        { json: "url", js: "url", typ: "" },
    ], "any"),
    "CapabilityType": [
        "light",
        "ota",
        "play_id",
        "play_url",
        "player",
        "stop_play",
        "train",
        "turnout",
    ],
    "Function": [
        "break",
        "move_backward",
        "move_forward",
        "off",
        "on",
        "play_id",
        "play_url",
        "stop_play",
        "turnout_pos1",
        "turnout_pos2",
        "update",
    ],
    "Direction": [
        "backward",
        "forward",
        "stop",
    ],
    "EventType": [
        "train",
    ],
};
