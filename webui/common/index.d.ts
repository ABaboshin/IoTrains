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
export declare enum Function {
    Break = "break",
    MoveBackward = "move_backward",
    MoveForward = "move_forward",
    Play = "play",
    StopPlay = "stop_play",
    TurnoutPos1 = "turnout_pos1",
    TurnoutPos2 = "turnout_pos2"
}
export declare enum DeviceType {
    Player = "player",
    Train = "train",
    Turnout = "turnout"
}
export interface DeviceInfo {
    device: Device;
    state?: State;
    [property: string]: any;
}
export interface State {
    command?: Command;
    id: string;
    ok: boolean;
    [property: string]: any;
}
export interface Command {
    function: Function;
    value?: string;
    [property: string]: any;
}
export interface TrainState extends State {
    direction?: Direction;
    speed?: number;
    [property: string]: any;
}
export declare enum Direction {
    Backward = "backward",
    Forward = "forward",
    Stop = "stop"
}
export interface Event {
    type: EventType;
    vakue?: string;
    [property: string]: any;
}
export declare enum EventType {
    Train = "train"
}
export declare class Convert {
    static toFunction(json: string): Function;
    static functionToJson(value: Function): string;
    static toDeviceType(json: string): DeviceType;
    static deviceTypeToJson(value: DeviceType): string;
    static toControlUnit(json: string): ControlUnit;
    static controlUnitToJson(value: ControlUnit): string;
    static toDeviceInfo(json: string): DeviceInfo;
    static deviceInfoToJson(value: DeviceInfo): string;
    static toTrainState(json: string): TrainState;
    static trainStateToJson(value: TrainState): string;
    static toEventType(json: string): EventType;
    static eventTypeToJson(value: EventType): string;
    static toEvent(json: string): Event;
    static eventToJson(value: Event): string;
}
