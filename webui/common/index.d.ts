export interface Command {
    function: Function;
    value?: string;
    [property: string]: any;
}
export declare enum Function {
    MoveBackward = "move_backward",
    MoveForward = "move_forward",
    Stop = "stop",
    TurnoutPos1 = "turnout_pos1",
    TurnoutPos2 = "turnout_pos2"
}
export interface ControlUnit {
    devices: Device[];
    id: string;
    [property: string]: any;
}
export interface Device {
    functions?: Function[];
    id: string;
    type: DeviceType;
    [property: string]: any;
}
export declare enum DeviceType {
    Train = "train",
    Turnout = "turnout"
}
export declare class Convert {
    static toFunction(json: string): Function;
    static functionToJson(value: Function): string;
    static toDeviceType(json: string): DeviceType;
    static deviceTypeToJson(value: DeviceType): string;
    static toCommand(json: string): Command;
    static commandToJson(value: Command): string;
    static toControlUnit(json: string): ControlUnit;
    static controlUnitToJson(value: ControlUnit): string;
}
