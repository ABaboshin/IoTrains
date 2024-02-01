//  To parse this JSON data, first install
//
//      json.hpp  https://github.com/nlohmann/json
//
//  Then include this file, and then do
//
//     Function data = nlohmann::json::parse(jsonString);
//     DeviceType data = nlohmann::json::parse(jsonString);
//     Command data = nlohmann::json::parse(jsonString);
//     ControlUnit data = nlohmann::json::parse(jsonString);

#pragma once

#include <optional>
#include "json.hpp"

#include <optional>
#include <stdexcept>
#include <regex>

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

    enum class Function : int { MOVE_BACKWARD, MOVE_FORWARD, STOP, TURNOUT_POS1, TURNOUT_POS2 };

    class Command {
        public:
        Command() = default;
        virtual ~Command() = default;

        private:
        Function function;
        std::optional<std::string> value;

        public:
        const Function & get_function() const { return function; }
        Function & get_mutable_function() { return function; }
        void set_function(const Function & value) { this->function = value; }

        std::optional<std::string> get_value() const { return value; }
        void set_value(std::optional<std::string> value) { this->value = value; }
    };

    enum class DeviceType : int { TRAIN, TURNOUT };

    class Device {
        public:
        Device() = default;
        virtual ~Device() = default;

        private:
        std::optional<std::vector<Function>> functions;
        std::string id;
        DeviceType type;

        public:
        std::optional<std::vector<Function>> get_functions() const { return functions; }
        void set_functions(std::optional<std::vector<Function>> value) { this->functions = value; }

        const std::string & get_id() const { return id; }
        std::string & get_mutable_id() { return id; }
        void set_id(const std::string & value) { this->id = value; }

        const DeviceType & get_type() const { return type; }
        DeviceType & get_mutable_type() { return type; }
        void set_type(const DeviceType & value) { this->type = value; }
    };

    class ControlUnit {
        public:
        ControlUnit() = default;
        virtual ~ControlUnit() = default;

        private:
        std::vector<Device> devices;
        std::string id;

        public:
        const std::vector<Device> & get_devices() const { return devices; }
        std::vector<Device> & get_mutable_devices() { return devices; }
        void set_devices(const std::vector<Device> & value) { this->devices = value; }

        const std::string & get_id() const { return id; }
        std::string & get_mutable_id() { return id; }
        void set_id(const std::string & value) { this->id = value; }
    };
}

namespace railschema {
    void from_json(const json & j, Command & x);
    void to_json(json & j, const Command & x);

    void from_json(const json & j, Device & x);
    void to_json(json & j, const Device & x);

    void from_json(const json & j, ControlUnit & x);
    void to_json(json & j, const ControlUnit & x);

    void from_json(const json & j, Function & x);
    void to_json(json & j, const Function & x);

    void from_json(const json & j, DeviceType & x);
    void to_json(json & j, const DeviceType & x);

    inline void from_json(const json & j, Command& x) {
        x.set_function(j.at("function").get<Function>());
        x.set_value(get_stack_optional<std::string>(j, "value"));
    }

    inline void to_json(json & j, const Command & x) {
        j = json::object();
        j["function"] = x.get_function();
        j["value"] = x.get_value();
    }

    inline void from_json(const json & j, Device& x) {
        x.set_functions(get_stack_optional<std::vector<Function>>(j, "functions"));
        x.set_id(j.at("id").get<std::string>());
        x.set_type(j.at("type").get<DeviceType>());
    }

    inline void to_json(json & j, const Device & x) {
        j = json::object();
        j["functions"] = x.get_functions();
        j["id"] = x.get_id();
        j["type"] = x.get_type();
    }

    inline void from_json(const json & j, ControlUnit& x) {
        x.set_devices(j.at("devices").get<std::vector<Device>>());
        x.set_id(j.at("id").get<std::string>());
    }

    inline void to_json(json & j, const ControlUnit & x) {
        j = json::object();
        j["devices"] = x.get_devices();
        j["id"] = x.get_id();
    }

    inline void from_json(const json & j, Function & x) {
        if (j == "move_backward") x = Function::MOVE_BACKWARD;
        else if (j == "move_forward") x = Function::MOVE_FORWARD;
        else if (j == "stop") x = Function::STOP;
        else if (j == "turnout_pos1") x = Function::TURNOUT_POS1;
        else if (j == "turnout_pos2") x = Function::TURNOUT_POS2;
        else { throw std::runtime_error("Input JSON does not conform to schema!"); }
    }

    inline void to_json(json & j, const Function & x) {
        switch (x) {
            case Function::MOVE_BACKWARD: j = "move_backward"; break;
            case Function::MOVE_FORWARD: j = "move_forward"; break;
            case Function::STOP: j = "stop"; break;
            case Function::TURNOUT_POS1: j = "turnout_pos1"; break;
            case Function::TURNOUT_POS2: j = "turnout_pos2"; break;
            default: throw std::runtime_error("This should not happen");
        }
    }

    inline void from_json(const json & j, DeviceType & x) {
        if (j == "train") x = DeviceType::TRAIN;
        else if (j == "turnout") x = DeviceType::TURNOUT;
        else { throw std::runtime_error("Input JSON does not conform to schema!"); }
    }

    inline void to_json(json & j, const DeviceType & x) {
        switch (x) {
            case DeviceType::TRAIN: j = "train"; break;
            case DeviceType::TURNOUT: j = "turnout"; break;
            default: throw std::runtime_error("This should not happen");
        }
    }
}
