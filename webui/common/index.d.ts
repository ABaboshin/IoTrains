export interface ControlUnit {
    devices: Device[];
    id: string;
    [property: string]: any;
}
export interface Device {
    capabilities: Capability[];
    id: string;
    [property: string]: any;
}
export interface Capability {
    type: CapabilityType;
    value: string;
    [property: string]: any;
}
export declare enum CapabilityType {
    Detector = "detector",
    Light = "light",
    Ota = "ota",
    PlayID = "play_id",
    PlayURL = "play_url",
    Player = "player",
    StopPlay = "stop_play",
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
    description?: string;
    id: string;
    ok: boolean;
    [property: string]: any;
}
export interface Command {
    function: Function;
    [property: string]: any;
}
export declare enum Function {
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
    Update = "update"
}
export interface TrainState extends State {
    direction?: Direction;
    speed: number;
    [property: string]: any;
}
export declare enum Direction {
    Backward = "backward",
    Forward = "forward",
    Stop = "stop"
}
export interface Event {
    source: string;
    type: EventType;
    value: string;
    [property: string]: any;
}
export declare enum EventType {
    Train = "train"
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
export declare class Convert {
    static toFunction(json: string): Function;
    static functionToJson(value: Function): string;
    static toCapabilityType(json: string): CapabilityType;
    static capabilityTypeToJson(value: CapabilityType): string;
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
    static toTrainCommand(json: string): TrainCommand;
    static trainCommandToJson(value: TrainCommand): string;
    static toMp3Command(json: string): Mp3Command;
    static mp3CommandToJson(value: Mp3Command): string;
    static toLightCommand(json: string): LightCommand;
    static lightCommandToJson(value: LightCommand): string;
    static toOtaCommand(json: string): OtaCommand;
    static otaCommandToJson(value: OtaCommand): string;
}
