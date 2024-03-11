"use strict";
// To parse this data:
//
//   import { Convert, Function, CapabilityType, ControlUnit, DeviceInfo, TrainState, EventType, Event, TrainCommand, Mp3Command, LightCommand, OtaCommand } from "./file";
//
//   const function = Convert.toFunction(json);
//   const capabilityType = Convert.toCapabilityType(json);
//   const controlUnit = Convert.toControlUnit(json);
//   const deviceInfo = Convert.toDeviceInfo(json);
//   const trainState = Convert.toTrainState(json);
//   const eventType = Convert.toEventType(json);
//   const event = Convert.toEvent(json);
//   const trainCommand = Convert.toTrainCommand(json);
//   const mp3Command = Convert.toMp3Command(json);
//   const lightCommand = Convert.toLightCommand(json);
//   const otaCommand = Convert.toOtaCommand(json);
//
// These functions will throw an error if the JSON doesn't
// match the expected interface, even if the JSON is valid.
exports.__esModule = true;
exports.Convert = exports.EventType = exports.Direction = exports.Function = exports.CapabilityType = void 0;
var CapabilityType;
(function (CapabilityType) {
    CapabilityType["Detector"] = "detector";
    CapabilityType["Light"] = "light";
    CapabilityType["Ota"] = "ota";
    CapabilityType["PlayID"] = "play_id";
    CapabilityType["PlayURL"] = "play_url";
    CapabilityType["Player"] = "player";
    CapabilityType["StopPlay"] = "stop_play";
    CapabilityType["Train"] = "train";
    CapabilityType["Turnout"] = "turnout";
})(CapabilityType = exports.CapabilityType || (exports.CapabilityType = {}));
var Function;
(function (Function) {
    Function["Break"] = "break";
    Function["MoveBackward"] = "move_backward";
    Function["MoveForward"] = "move_forward";
    Function["Off"] = "off";
    Function["On"] = "on";
    Function["PlayID"] = "play_id";
    Function["PlayURL"] = "play_url";
    Function["StopPlay"] = "stop_play";
    Function["TurnoutPos1"] = "turnout_pos1";
    Function["TurnoutPos2"] = "turnout_pos2";
    Function["Update"] = "update";
})(Function = exports.Function || (exports.Function = {}));
var Direction;
(function (Direction) {
    Direction["Backward"] = "backward";
    Direction["Forward"] = "forward";
    Direction["Stop"] = "stop";
})(Direction = exports.Direction || (exports.Direction = {}));
var EventType;
(function (EventType) {
    EventType["Train"] = "train";
})(EventType = exports.EventType || (exports.EventType = {}));
// Converts JSON strings to/from your types
// and asserts the results of JSON.parse at runtime
var Convert = /** @class */ (function () {
    function Convert() {
    }
    Convert.toFunction = function (json) {
        return cast(JSON.parse(json), r("Function"));
    };
    Convert.functionToJson = function (value) {
        return JSON.stringify(uncast(value, r("Function")), null, 2);
    };
    Convert.toCapabilityType = function (json) {
        return cast(JSON.parse(json), r("CapabilityType"));
    };
    Convert.capabilityTypeToJson = function (value) {
        return JSON.stringify(uncast(value, r("CapabilityType")), null, 2);
    };
    Convert.toControlUnit = function (json) {
        return cast(JSON.parse(json), r("ControlUnit"));
    };
    Convert.controlUnitToJson = function (value) {
        return JSON.stringify(uncast(value, r("ControlUnit")), null, 2);
    };
    Convert.toDeviceInfo = function (json) {
        return cast(JSON.parse(json), r("DeviceInfo"));
    };
    Convert.deviceInfoToJson = function (value) {
        return JSON.stringify(uncast(value, r("DeviceInfo")), null, 2);
    };
    Convert.toTrainState = function (json) {
        return cast(JSON.parse(json), r("TrainState"));
    };
    Convert.trainStateToJson = function (value) {
        return JSON.stringify(uncast(value, r("TrainState")), null, 2);
    };
    Convert.toEventType = function (json) {
        return cast(JSON.parse(json), r("EventType"));
    };
    Convert.eventTypeToJson = function (value) {
        return JSON.stringify(uncast(value, r("EventType")), null, 2);
    };
    Convert.toEvent = function (json) {
        return cast(JSON.parse(json), r("Event"));
    };
    Convert.eventToJson = function (value) {
        return JSON.stringify(uncast(value, r("Event")), null, 2);
    };
    Convert.toTrainCommand = function (json) {
        return cast(JSON.parse(json), r("TrainCommand"));
    };
    Convert.trainCommandToJson = function (value) {
        return JSON.stringify(uncast(value, r("TrainCommand")), null, 2);
    };
    Convert.toMp3Command = function (json) {
        return cast(JSON.parse(json), r("Mp3Command"));
    };
    Convert.mp3CommandToJson = function (value) {
        return JSON.stringify(uncast(value, r("Mp3Command")), null, 2);
    };
    Convert.toLightCommand = function (json) {
        return cast(JSON.parse(json), r("LightCommand"));
    };
    Convert.lightCommandToJson = function (value) {
        return JSON.stringify(uncast(value, r("LightCommand")), null, 2);
    };
    Convert.toOtaCommand = function (json) {
        return cast(JSON.parse(json), r("OtaCommand"));
    };
    Convert.otaCommandToJson = function (value) {
        return JSON.stringify(uncast(value, r("OtaCommand")), null, 2);
    };
    return Convert;
}());
exports.Convert = Convert;
function invalidValue(typ, val, key, parent) {
    if (parent === void 0) { parent = ''; }
    var prettyTyp = prettyTypeName(typ);
    var parentText = parent ? " on ".concat(parent) : '';
    var keyText = key ? " for key \"".concat(key, "\"") : '';
    throw Error("Invalid value".concat(keyText).concat(parentText, ". Expected ").concat(prettyTyp, " but got ").concat(JSON.stringify(val)));
}
function prettyTypeName(typ) {
    if (Array.isArray(typ)) {
        if (typ.length === 2 && typ[0] === undefined) {
            return "an optional ".concat(prettyTypeName(typ[1]));
        }
        else {
            return "one of [".concat(typ.map(function (a) { return prettyTypeName(a); }).join(", "), "]");
        }
    }
    else if (typeof typ === "object" && typ.literal !== undefined) {
        return typ.literal;
    }
    else {
        return typeof typ;
    }
}
function jsonToJSProps(typ) {
    if (typ.jsonToJS === undefined) {
        var map_1 = {};
        typ.props.forEach(function (p) { return map_1[p.json] = { key: p.js, typ: p.typ }; });
        typ.jsonToJS = map_1;
    }
    return typ.jsonToJS;
}
function jsToJSONProps(typ) {
    if (typ.jsToJSON === undefined) {
        var map_2 = {};
        typ.props.forEach(function (p) { return map_2[p.js] = { key: p.json, typ: p.typ }; });
        typ.jsToJSON = map_2;
    }
    return typ.jsToJSON;
}
function transform(val, typ, getProps, key, parent) {
    if (key === void 0) { key = ''; }
    if (parent === void 0) { parent = ''; }
    function transformPrimitive(typ, val) {
        if (typeof typ === typeof val)
            return val;
        return invalidValue(typ, val, key, parent);
    }
    function transformUnion(typs, val) {
        // val must validate against one typ in typs
        var l = typs.length;
        for (var i = 0; i < l; i++) {
            var typ_1 = typs[i];
            try {
                return transform(val, typ_1, getProps);
            }
            catch (_) { }
        }
        return invalidValue(typs, val, key, parent);
    }
    function transformEnum(cases, val) {
        if (cases.indexOf(val) !== -1)
            return val;
        return invalidValue(cases.map(function (a) { return l(a); }), val, key, parent);
    }
    function transformArray(typ, val) {
        // val must be an array with no invalid elements
        if (!Array.isArray(val))
            return invalidValue(l("array"), val, key, parent);
        return val.map(function (el) { return transform(el, typ, getProps); });
    }
    function transformDate(val) {
        if (val === null) {
            return null;
        }
        var d = new Date(val);
        if (isNaN(d.valueOf())) {
            return invalidValue(l("Date"), val, key, parent);
        }
        return d;
    }
    function transformObject(props, additional, val) {
        if (val === null || typeof val !== "object" || Array.isArray(val)) {
            return invalidValue(l(ref || "object"), val, key, parent);
        }
        var result = {};
        Object.getOwnPropertyNames(props).forEach(function (key) {
            var prop = props[key];
            var v = Object.prototype.hasOwnProperty.call(val, key) ? val[key] : undefined;
            result[prop.key] = transform(v, prop.typ, getProps, key, ref);
        });
        Object.getOwnPropertyNames(val).forEach(function (key) {
            if (!Object.prototype.hasOwnProperty.call(props, key)) {
                result[key] = transform(val[key], additional, getProps, key, ref);
            }
        });
        return result;
    }
    if (typ === "any")
        return val;
    if (typ === null) {
        if (val === null)
            return val;
        return invalidValue(typ, val, key, parent);
    }
    if (typ === false)
        return invalidValue(typ, val, key, parent);
    var ref = undefined;
    while (typeof typ === "object" && typ.ref !== undefined) {
        ref = typ.ref;
        typ = typeMap[typ.ref];
    }
    if (Array.isArray(typ))
        return transformEnum(typ, val);
    if (typeof typ === "object") {
        return typ.hasOwnProperty("unionMembers") ? transformUnion(typ.unionMembers, val)
            : typ.hasOwnProperty("arrayItems") ? transformArray(typ.arrayItems, val)
                : typ.hasOwnProperty("props") ? transformObject(getProps(typ), typ.additional, val)
                    : invalidValue(typ, val, key, parent);
    }
    // Numbers can be parsed by Date but shouldn't be.
    if (typ === Date && typeof val !== "number")
        return transformDate(val);
    return transformPrimitive(typ, val);
}
function cast(val, typ) {
    return transform(val, typ, jsonToJSProps);
}
function uncast(val, typ) {
    return transform(val, typ, jsToJSONProps);
}
function l(typ) {
    return { literal: typ };
}
function a(typ) {
    return { arrayItems: typ };
}
function u() {
    var typs = [];
    for (var _i = 0; _i < arguments.length; _i++) {
        typs[_i] = arguments[_i];
    }
    return { unionMembers: typs };
}
function o(props, additional) {
    return { props: props, additional: additional };
}
function m(additional) {
    return { props: [], additional: additional };
}
function r(name) {
    return { ref: name };
}
var typeMap = {
    "ControlUnit": o([
        { json: "devices", js: "devices", typ: a(r("Device")) },
        { json: "id", js: "id", typ: "" },
    ], "any"),
    "Device": o([
        { json: "capabilities", js: "capabilities", typ: a(r("Capability")) },
        { json: "id", js: "id", typ: "" },
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
        { json: "source", js: "source", typ: "" },
        { json: "type", js: "type", typ: r("EventType") },
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
        "detector",
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
    ]
};
