//  To parse this JSON data, first install
//
//      json.hpp  https://github.com/nlohmann/json
//
//  Then include this file, and then do
//
//     Function data = nlohmann::json::parse(jsonString);
//     CapabilityType data = nlohmann::json::parse(jsonString);
//     ControlUnit data = nlohmann::json::parse(jsonString);
//     DeviceInfo data = nlohmann::json::parse(jsonString);
//     TrainState data = nlohmann::json::parse(jsonString);
//     EventType data = nlohmann::json::parse(jsonString);
//     Event data = nlohmann::json::parse(jsonString);
//     RfidEvent data = nlohmann::json::parse(jsonString);
//     TrainCommand data = nlohmann::json::parse(jsonString);
//     Mp3Command data = nlohmann::json::parse(jsonString);

#pragma once

#include <optional>
#include <nlohmann/json.hpp>

#ifndef NLOHMANN_OPT_HELPER
#define NLOHMANN_OPT_HELPER
namespace nlohmann {
    template <typename T>
    struct adl_serializer<std::shared_ptr<T>> {
        static void to_json(json & j, const std::shared_ptr<T> & opt) {
            if (!opt) j = nullptr; else j = *opt;
        }

        static std::shared_ptr<T> from_json(const json & j) {
            if (j.is_null()) return std::make_shared<T>(); else return std::make_shared<T>(j.get<T>());
        }
    };
    template <typename T>
    struct adl_serializer<std::optional<T>> {
        static void to_json(json & j, const std::optional<T> & opt) {
            if (!opt) j = nullptr; else j = *opt;
        }

        static std::optional<T> from_json(const json & j) {
            if (j.is_null()) return std::make_optional<T>(); else return std::make_optional<T>(j.get<T>());
        }
    };
}
#endif

namespace railschema {
    using nlohmann::json;

    #ifndef NLOHMANN_UNTYPED_railschema_HELPER
    #define NLOHMANN_UNTYPED_railschema_HELPER
    inline json get_untyped(const json & j, const char * property) {
        if (j.find(property) != j.end()) {
            return j.at(property).get<json>();
        }
        return json();
    }

    inline json get_untyped(const json & j, std::string property) {
        return get_untyped(j, property.data());
    }
    #endif

    #ifndef NLOHMANN_OPTIONAL_railschema_HELPER
    #define NLOHMANN_OPTIONAL_railschema_HELPER
    template <typename T>
    inline std::shared_ptr<T> get_heap_optional(const json & j, const char * property) {
        auto it = j.find(property);
        if (it != j.end() && !it->is_null()) {
            return j.at(property).get<std::shared_ptr<T>>();
        }
        return std::shared_ptr<T>();
    }

    template <typename T>
    inline std::shared_ptr<T> get_heap_optional(const json & j, std::string property) {
        return get_heap_optional<T>(j, property.data());
    }
    template <typename T>
    inline std::optional<T> get_stack_optional(const json & j, const char * property) {
        auto it = j.find(property);
        if (it != j.end() && !it->is_null()) {
            return j.at(property).get<std::optional<T>>();
        }
        return std::optional<T>();
    }

    template <typename T>
    inline std::optional<T> get_stack_optional(const json & j, std::string property) {
        return get_stack_optional<T>(j, property.data());
    }
    #endif

    enum class CapabilityType : int { PLAYER, PLAY_ID, PLAY_URL, STOP_PLAY, TRAIN, TURNOUT };

    class Capability {
        public:
        std::string discriminator;
        Capability()= default;
        virtual ~Capability() = default;

        CapabilityType type;
        std::string value;
    };

    class Device {
        public:
        std::string discriminator;
        Device()= default;
        virtual ~Device() = default;

        std::vector<Capability> capabilities;
        std::string id;
        nlohmann::json type;
    };

    class ControlUnit {
        public:
        std::string discriminator;
        ControlUnit()= default;
        virtual ~ControlUnit() = default;

        std::vector<Device> devices;
        std::string id;
    };

    enum class Function : int { BREAK, MOVE_BACKWARD, MOVE_FORWARD, PLAY_ID, PLAY_URL, STOP_PLAY, TURNOUT_POS1, TURNOUT_POS2 };

    class Command {
        public:
        std::string discriminator;
        Command()= default;
        virtual ~Command() = default;

        Function function;
    };

    class State {
        public:
        std::string discriminator;
        State() { discriminator =  "State"; }
        virtual ~State() = default;

        std::optional<Command> command;
        std::optional<std::string> description;
        std::string id;
        bool ok;
    };

    class DeviceInfo {
        public:
        std::string discriminator;
        DeviceInfo()= default;
        virtual ~DeviceInfo() = default;

        Device device;
        std::optional<State> state;
    };

    enum class Direction : int { BACKWARD, FORWARD, STOP };

    class TrainState : public State {
        public:
        std::string discriminator;
        TrainState() { discriminator =  "TrainState"; }
        virtual ~TrainState() = default;

        std::optional<Direction> direction;
        int64_t speed;
    };

    enum class EventType : int { TRAIN };

    class Event {
        public:
        std::string discriminator;
        Event() { discriminator =  "Event"; }
        virtual ~Event() = default;

        EventType type;
    };

    class RfidEvent : public Event {
        public:
        std::string discriminator;
        RfidEvent() { discriminator =  "RfidEvent"; }
        virtual ~RfidEvent() = default;

        std::string value;
    };

    class TrainCommand : public Command {
        public:
        std::string discriminator;
        TrainCommand() { discriminator =  "TrainCommand"; }
        virtual ~TrainCommand() = default;

        int64_t speed;
    };

    class Mp3Command : public Command {
        public:
        std::string discriminator;
        Mp3Command() { discriminator =  "Mp3Command"; }
        virtual ~Mp3Command() = default;

        std::string url;
    };
}

namespace railschema {
    void from_json(const json & j, Capability * x);
    void to_json(json & j, const Capability * x);

    void from_json(const json & j, Device * x);
    void to_json(json & j, const Device * x);

    void from_json(const json & j, ControlUnit * x);
    void to_json(json & j, const ControlUnit * x);

    void from_json(const json & j, Command * x);
    void to_json(json & j, const Command * x);

    void from_json(const json & j, State * x);
    void to_json(json & j, const State * x);

    void from_json(const json & j, DeviceInfo * x);
    void to_json(json & j, const DeviceInfo * x);

    void from_json(const json & j, TrainState * x);
    void to_json(json & j, const TrainState * x);

    void from_json(const json & j, Event * x);
    void to_json(json & j, const Event * x);

    void from_json(const json & j, RfidEvent * x);
    void to_json(json & j, const RfidEvent * x);

    void from_json(const json & j, TrainCommand * x);
    void to_json(json & j, const TrainCommand * x);

    void from_json(const json & j, Mp3Command * x);
    void to_json(json & j, const Mp3Command * x);

    void from_json(const json & j, CapabilityType & x);
    void to_json(json & j, const CapabilityType & x);

    void from_json(const json & j, Function & x);
    void to_json(json & j, const Function & x);

    void from_json(const json & j, Direction & x);
    void to_json(json & j, const Direction & x);

    void from_json(const json & j, EventType & x);
    void to_json(json & j, const EventType & x);

    inline void from_json(const json & j, Capability& x) {
        x.type = j.at("type").get<CapabilityType>();
        x.value = j.at("value").get<std::string>();
    }

    inline void to_json(json & j, const Capability & x) {
        j = json::object();
        j["type"] = x.type;
        j["value"] = x.value;
    }

    inline void from_json(const json & j, Device& x) {
        x.capabilities = j.at("capabilities").get<std::vector<Capability>>();
        x.id = j.at("id").get<std::string>();
        x.type = get_untyped(j, "type");
    }

    inline void to_json(json & j, const Device & x) {
        j = json::object();
        j["capabilities"] = x.capabilities;
        j["id"] = x.id;
        j["type"] = x.type;
    }

    inline void from_json(const json & j, ControlUnit& x) {
        x.devices = j.at("devices").get<std::vector<Device>>();
        x.id = j.at("id").get<std::string>();
    }

    inline void to_json(json & j, const ControlUnit & x) {
        j = json::object();
        j["devices"] = x.devices;
        j["id"] = x.id;
    }

    inline void from_json(const json & j, Command& x) {
        x.function = j.at("function").get<Function>();
    }

    inline void to_json(json & j, const Command & x) {
        j = json::object();
        j["function"] = x.function;
    }

    inline void from_json(const json & j, State& x) {
        x.command = get_stack_optional<Command>(j, "command");
        x.description = get_stack_optional<std::string>(j, "description");
        x.id = j.at("id").get<std::string>();
        x.ok = j.at("ok").get<bool>();
    }

    inline void to_json(json & j, const State & x) {
        j = json::object();
        j["command"] = x.command;
        j["description"] = x.description;
        j["id"] = x.id;
        j["ok"] = x.ok;
    }

    inline void from_json(const json & j, DeviceInfo& x) {
        x.device = j.at("device").get<Device>();
        x.state = get_stack_optional<State>(j, "state");
    }

    inline void to_json(json & j, const DeviceInfo & x) {
        j = json::object();
        j["device"] = x.device;
        j["state"] = x.state;
    }

    inline void from_json(const json & j, TrainState& x) {
        
                  from_json(j, (State&)x);
                  
        x.direction = get_stack_optional<Direction>(j, "direction");
        x.speed = j.at("speed").get<int64_t>();
    }

    inline void to_json(json & j, const TrainState & x) {
        
                  to_json(j, (const State&)x);
                  
        j["direction"] = x.direction;
        j["speed"] = x.speed;
    }

    inline void from_json(const json & j, Event& x) {
        x.type = j.at("type").get<EventType>();
    }

    inline void to_json(json & j, const Event & x) {
        j = json::object();
        j["type"] = x.type;
    }

    inline void from_json(const json & j, RfidEvent& x) {
        
                  from_json(j, (Event&)x);
                  
        x.value = j.at("value").get<std::string>();
    }

    inline void to_json(json & j, const RfidEvent & x) {
        
                  to_json(j, (const Event&)x);
                  
        j["value"] = x.value;
    }

    inline void from_json(const json & j, TrainCommand& x) {
        
                  from_json(j, (Command&)x);
                  
        x.speed = j.at("speed").get<int64_t>();
    }

    inline void to_json(json & j, const TrainCommand & x) {
        
                  to_json(j, (const Command&)x);
                  
        j["speed"] = x.speed;
    }

    inline void from_json(const json & j, Mp3Command& x) {
        
                  from_json(j, (Command&)x);
                  
        x.url = j.at("url").get<std::string>();
    }

    inline void to_json(json & j, const Mp3Command & x) {
        
                  to_json(j, (const Command&)x);
                  
        j["url"] = x.url;
    }

    inline void from_json(const json & j, CapabilityType & x) {
        if (j == "player") x = CapabilityType::PLAYER;
        else if (j == "play_id") x = CapabilityType::PLAY_ID;
        else if (j == "play_url") x = CapabilityType::PLAY_URL;
        else if (j == "stop_play") x = CapabilityType::STOP_PLAY;
        else if (j == "train") x = CapabilityType::TRAIN;
        else if (j == "turnout") x = CapabilityType::TURNOUT;
        else { throw std::runtime_error("Input JSON does not conform to schema!"); }
    }

    inline void to_json(json & j, const CapabilityType & x) {
        switch (x) {
            case CapabilityType::PLAYER: j = "player"; break;
            case CapabilityType::PLAY_ID: j = "play_id"; break;
            case CapabilityType::PLAY_URL: j = "play_url"; break;
            case CapabilityType::STOP_PLAY: j = "stop_play"; break;
            case CapabilityType::TRAIN: j = "train"; break;
            case CapabilityType::TURNOUT: j = "turnout"; break;
            default: throw std::runtime_error("This should not happen");
        }
    }

    inline void from_json(const json & j, Function & x) {
        if (j == "break") x = Function::BREAK;
        else if (j == "move_backward") x = Function::MOVE_BACKWARD;
        else if (j == "move_forward") x = Function::MOVE_FORWARD;
        else if (j == "play_id") x = Function::PLAY_ID;
        else if (j == "play_url") x = Function::PLAY_URL;
        else if (j == "stop_play") x = Function::STOP_PLAY;
        else if (j == "turnout_pos1") x = Function::TURNOUT_POS1;
        else if (j == "turnout_pos2") x = Function::TURNOUT_POS2;
        else { throw std::runtime_error("Input JSON does not conform to schema!"); }
    }

    inline void to_json(json & j, const Function & x) {
        switch (x) {
            case Function::BREAK: j = "break"; break;
            case Function::MOVE_BACKWARD: j = "move_backward"; break;
            case Function::MOVE_FORWARD: j = "move_forward"; break;
            case Function::PLAY_ID: j = "play_id"; break;
            case Function::PLAY_URL: j = "play_url"; break;
            case Function::STOP_PLAY: j = "stop_play"; break;
            case Function::TURNOUT_POS1: j = "turnout_pos1"; break;
            case Function::TURNOUT_POS2: j = "turnout_pos2"; break;
            default: throw std::runtime_error("This should not happen");
        }
    }

    inline void from_json(const json & j, Direction & x) {
        if (j == "backward") x = Direction::BACKWARD;
        else if (j == "forward") x = Direction::FORWARD;
        else if (j == "stop") x = Direction::STOP;
        else { throw std::runtime_error("Input JSON does not conform to schema!"); }
    }

    inline void to_json(json & j, const Direction & x) {
        switch (x) {
            case Direction::BACKWARD: j = "backward"; break;
            case Direction::FORWARD: j = "forward"; break;
            case Direction::STOP: j = "stop"; break;
            default: throw std::runtime_error("This should not happen");
        }
    }

    inline void from_json(const json & j, EventType & x) {
        if (j == "train") x = EventType::TRAIN;
        else { throw std::runtime_error("Input JSON does not conform to schema!"); }
    }

    inline void to_json(json & j, const EventType & x) {
        switch (x) {
            case EventType::TRAIN: j = "train"; break;
            default: throw std::runtime_error("This should not happen");
        }
    }
    
        template<typename T>
        inline std::shared_ptr<T> from_json(const json & j) {
            return nullptr;
        }
    
        template<typename T>
        inline void to_json(json & j, std::shared_ptr<T> data) {
    
        }
          
    template<> inline std::shared_ptr<State> from_json<State>(const json& j) {
              const auto discriminator = j.at("discriminator").get<std::string>();
              if (discriminator == "State") {
                std::shared_ptr<State> ptr;
                from_json(j, *ptr);
                ptr->discriminator = "State";
                return ptr;
              }
              
    
              if (discriminator == "TrainState") {
                std::shared_ptr<State> ptr = std::make_shared<TrainState>();
                from_json(j, *(TrainState*)ptr.get());
                ptr->discriminator = "TrainState";
                return ptr;
              }
              
    return nullptr; }
    template<> inline void to_json(json& j, std::shared_ptr<State> data) {
              if (data->discriminator == "State") {
                to_json(j, *data.get());
              }
              
    
              if (data->discriminator == "TrainState") {
                to_json(j, *(TrainState*)data.get());
              }
              
    }
    template<> inline std::shared_ptr<Event> from_json<Event>(const json& j) {
              const auto discriminator = j.at("discriminator").get<std::string>();
              if (discriminator == "Event") {
                std::shared_ptr<Event> ptr;
                from_json(j, *ptr);
                ptr->discriminator = "Event";
                return ptr;
              }
              
    
              if (discriminator == "RfidEvent") {
                std::shared_ptr<Event> ptr = std::make_shared<RfidEvent>();
                from_json(j, *(RfidEvent*)ptr.get());
                ptr->discriminator = "RfidEvent";
                return ptr;
              }
              
    return nullptr; }
    template<> inline void to_json(json& j, std::shared_ptr<Event> data) {
              if (data->discriminator == "Event") {
                to_json(j, *data.get());
              }
              
    
              if (data->discriminator == "RfidEvent") {
                to_json(j, *(RfidEvent*)data.get());
              }
              
    }
    template<> inline std::shared_ptr<Command> from_json<Command>(const json& j) {
              const auto discriminator = j.at("discriminator").get<std::string>();
              if (discriminator == "Command") {
                std::shared_ptr<Command> ptr;
                from_json(j, *ptr);
                ptr->discriminator = "Command";
                return ptr;
              }
              
    
              if (discriminator == "TrainCommand") {
                std::shared_ptr<Command> ptr = std::make_shared<TrainCommand>();
                from_json(j, *(TrainCommand*)ptr.get());
                ptr->discriminator = "TrainCommand";
                return ptr;
              }
              
    
              if (discriminator == "Mp3Command") {
                std::shared_ptr<Command> ptr = std::make_shared<Mp3Command>();
                from_json(j, *(Mp3Command*)ptr.get());
                ptr->discriminator = "Mp3Command";
                return ptr;
              }
              
    return nullptr; }
    template<> inline void to_json(json& j, std::shared_ptr<Command> data) {
              if (data->discriminator == "Command") {
                to_json(j, *data.get());
              }
              
    
              if (data->discriminator == "TrainCommand") {
                to_json(j, *(TrainCommand*)data.get());
              }
              
    
              if (data->discriminator == "Mp3Command") {
                to_json(j, *(Mp3Command*)data.get());
              }
              
    }
}
