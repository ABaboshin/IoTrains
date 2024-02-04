//  To parse this JSON data, first install
//
//      json.hpp  https://github.com/nlohmann/json
//
//  Then include this file, and then do
//
//     Function data = nlohmann::json::parse(jsonString);
//     DeviceType data = nlohmann::json::parse(jsonString);
//     ControlUnit data = nlohmann::json::parse(jsonString);
//     DeviceInfo data = nlohmann::json::parse(jsonString);
//     TrainState data = nlohmann::json::parse(jsonString);

#pragma once

#include <optional>
#include "json.hpp"

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

    enum class Function : int { MOVE_BACKWARD, MOVE_FORWARD, PLAY, STOP, TURNOUT_POS1, TURNOUT_POS2 };

    enum class DeviceType : int { PLAYER, TRAIN, TURNOUT };

    class Device {
        public:
        Device() = default;
        virtual ~Device() = default;

        std::optional<std::vector<Function>> functions;
        std::string id;
        DeviceType type;
    };

    class ControlUnit {
        public:
        ControlUnit() = default;
        virtual ~ControlUnit() = default;

        std::vector<Device> devices;
        std::string id;
    };

    class Command {
        public:
        Command() = default;
        virtual ~Command() = default;

        Function function;
        std::optional<std::string> value;
    };

    class State {
        public:
        State() = default;
        virtual ~State() = default;

        virtual void to_json(json & j);
        std::optional<Command> command;
        std::string id;
        bool ok;
    };

    class DeviceInfo {
        public:
        DeviceInfo() = default;
        virtual ~DeviceInfo() = default;

        Device device;
        std::optional<State> state;
    };

    enum class Direction : int { BACKWARD, FORWARD, STOP };

    class TrainState : public State {
        public:
        TrainState() = default;
        virtual ~TrainState() = default;

        void to_json(json & j) override;
        std::optional<Direction> direction;
        std::optional<double> speed;
    };
}

namespace railschema {
    void from_json(const json & j, Device & x);
    void to_json(json & j, const Device & x);

    void from_json(const json & j, ControlUnit & x);
    void to_json(json & j, const ControlUnit & x);

    void from_json(const json & j, Command & x);
    void to_json(json & j, const Command & x);

    void from_json(const json & j, State & x);
    void to_json(json & j, const State & x);

    void from_json(const json & j, DeviceInfo & x);
    void to_json(json & j, const DeviceInfo & x);

    void from_json(const json & j, TrainState & x);
    void to_json(json & j, const TrainState & x);

    void from_json(const json & j, Function & x);
    void to_json(json & j, const Function & x);

    void from_json(const json & j, DeviceType & x);
    void to_json(json & j, const DeviceType & x);

    void from_json(const json & j, Direction & x);
    void to_json(json & j, const Direction & x);

    inline void from_json(const json & j, Device& x) {
        x.functions = get_stack_optional<std::vector<Function>>(j, "functions");
        x.id = j.at("id").get<std::string>();
        x.type = j.at("type").get<DeviceType>();
    }

    inline void to_json(json & j, const Device & x) {
        j = json::object();
        j["functions"] = x.functions;
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
        x.value = get_stack_optional<std::string>(j, "value");
    }

    inline void to_json(json & j, const Command & x) {
        j = json::object();
        j["function"] = x.function;
        j["value"] = x.value;
    }

    inline void from_json(const json & j, State& x) {
        x.command = get_stack_optional<Command>(j, "command");
        x.id = j.at("id").get<std::string>();
        x.ok = j.at("ok").get<bool>();
    }

    inline void to_json(json & j, const State & x) {
        j = json::object();
        j["command"] = x.command;
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
        x.direction = get_stack_optional<Direction>(j, "direction");
        x.speed = get_stack_optional<double>(j, "speed");
    }

    inline void to_json(json & j, const TrainState & x) {
        j = json::object();
        j["direction"] = x.direction;
        j["speed"] = x.speed;
    }

    inline void from_json(const json & j, Function & x) {
        if (j == "move_backward") x = Function::MOVE_BACKWARD;
        else if (j == "move_forward") x = Function::MOVE_FORWARD;
        else if (j == "play") x = Function::PLAY;
        else if (j == "stop") x = Function::STOP;
        else if (j == "turnout_pos1") x = Function::TURNOUT_POS1;
        else if (j == "turnout_pos2") x = Function::TURNOUT_POS2;
        else { throw std::runtime_error("Input JSON does not conform to schema!"); }
    }

    inline void to_json(json & j, const Function & x) {
        switch (x) {
            case Function::MOVE_BACKWARD: j = "move_backward"; break;
            case Function::MOVE_FORWARD: j = "move_forward"; break;
            case Function::PLAY: j = "play"; break;
            case Function::STOP: j = "stop"; break;
            case Function::TURNOUT_POS1: j = "turnout_pos1"; break;
            case Function::TURNOUT_POS2: j = "turnout_pos2"; break;
            default: throw std::runtime_error("This should not happen");
        }
    }

    inline void from_json(const json & j, DeviceType & x) {
        if (j == "player") x = DeviceType::PLAYER;
        else if (j == "train") x = DeviceType::TRAIN;
        else if (j == "turnout") x = DeviceType::TURNOUT;
        else { throw std::runtime_error("Input JSON does not conform to schema!"); }
    }

    inline void to_json(json & j, const DeviceType & x) {
        switch (x) {
            case DeviceType::PLAYER: j = "player"; break;
            case DeviceType::TRAIN: j = "train"; break;
            case DeviceType::TURNOUT: j = "turnout"; break;
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
}
