//  To parse this JSON data, first install
//
//      json.hpp  https://github.com/nlohmann/json
//
//  Then include this file, and then do
//
//     Schema data = nlohmann::json::parse(jsonString);

#pragma once

#include "json.hpp"

#include <optional>
#include <stdexcept>
#include <regex>

namespace quicktype {
    using nlohmann::json;

    #ifndef NLOHMANN_UNTYPED_quicktype_HELPER
    #define NLOHMANN_UNTYPED_quicktype_HELPER
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

    class Type {
        public:
        Type() = default;
        virtual ~Type() = default;

        private:
        std::string type;
        std::vector<std::string> type_enum;

        public:
        const std::string & get_type() const { return type; }
        std::string & get_mutable_type() { return type; }
        void set_type(const std::string & value) { this->type = value; }

        const std::vector<std::string> & get_type_enum() const { return type_enum; }
        std::vector<std::string> & get_mutable_type_enum() { return type_enum; }
        void set_type_enum(const std::vector<std::string> & value) { this->type_enum = value; }
    };

    class Functions {
        public:
        Functions() = default;
        virtual ~Functions() = default;

        private:
        std::string type;
        std::vector<Type> items;

        public:
        const std::string & get_type() const { return type; }
        std::string & get_mutable_type() { return type; }
        void set_type(const std::string & value) { this->type = value; }

        const std::vector<Type> & get_items() const { return items; }
        std::vector<Type> & get_mutable_items() { return items; }
        void set_items(const std::vector<Type> & value) { this->items = value; }
    };

    class Id {
        public:
        Id() = default;
        virtual ~Id() = default;

        private:
        std::string type;

        public:
        const std::string & get_type() const { return type; }
        std::string & get_mutable_type() { return type; }
        void set_type(const std::string & value) { this->type = value; }
    };

    class ItemProperties {
        public:
        ItemProperties() = default;
        virtual ~ItemProperties() = default;

        private:
        Id id;
        Type type;
        Functions functions;

        public:
        const Id & get_id() const { return id; }
        Id & get_mutable_id() { return id; }
        void set_id(const Id & value) { this->id = value; }

        const Type & get_type() const { return type; }
        Type & get_mutable_type() { return type; }
        void set_type(const Type & value) { this->type = value; }

        const Functions & get_functions() const { return functions; }
        Functions & get_mutable_functions() { return functions; }
        void set_functions(const Functions & value) { this->functions = value; }
    };

    class Item {
        public:
        Item() = default;
        virtual ~Item() = default;

        private:
        std::string type;
        ItemProperties properties;
        std::vector<std::string> required;

        public:
        const std::string & get_type() const { return type; }
        std::string & get_mutable_type() { return type; }
        void set_type(const std::string & value) { this->type = value; }

        const ItemProperties & get_properties() const { return properties; }
        ItemProperties & get_mutable_properties() { return properties; }
        void set_properties(const ItemProperties & value) { this->properties = value; }

        const std::vector<std::string> & get_required() const { return required; }
        std::vector<std::string> & get_mutable_required() { return required; }
        void set_required(const std::vector<std::string> & value) { this->required = value; }
    };

    class Devices {
        public:
        Devices() = default;
        virtual ~Devices() = default;

        private:
        std::string type;
        std::vector<Item> items;

        public:
        const std::string & get_type() const { return type; }
        std::string & get_mutable_type() { return type; }
        void set_type(const std::string & value) { this->type = value; }

        const std::vector<Item> & get_items() const { return items; }
        std::vector<Item> & get_mutable_items() { return items; }
        void set_items(const std::vector<Item> & value) { this->items = value; }
    };

    class SchemaProperties {
        public:
        SchemaProperties() = default;
        virtual ~SchemaProperties() = default;

        private:
        Id id;
        Devices devices;

        public:
        const Id & get_id() const { return id; }
        Id & get_mutable_id() { return id; }
        void set_id(const Id & value) { this->id = value; }

        const Devices & get_devices() const { return devices; }
        Devices & get_mutable_devices() { return devices; }
        void set_devices(const Devices & value) { this->devices = value; }
    };

    class Schema {
        public:
        Schema() = default;
        virtual ~Schema() = default;

        private:
        std::string schema;
        std::string type;
        SchemaProperties properties;
        std::vector<std::string> required;

        public:
        const std::string & get_schema() const { return schema; }
        std::string & get_mutable_schema() { return schema; }
        void set_schema(const std::string & value) { this->schema = value; }

        const std::string & get_type() const { return type; }
        std::string & get_mutable_type() { return type; }
        void set_type(const std::string & value) { this->type = value; }

        const SchemaProperties & get_properties() const { return properties; }
        SchemaProperties & get_mutable_properties() { return properties; }
        void set_properties(const SchemaProperties & value) { this->properties = value; }

        const std::vector<std::string> & get_required() const { return required; }
        std::vector<std::string> & get_mutable_required() { return required; }
        void set_required(const std::vector<std::string> & value) { this->required = value; }
    };
}

namespace quicktype {
    void from_json(const json & j, Type & x);
    void to_json(json & j, const Type & x);

    void from_json(const json & j, Functions & x);
    void to_json(json & j, const Functions & x);

    void from_json(const json & j, Id & x);
    void to_json(json & j, const Id & x);

    void from_json(const json & j, ItemProperties & x);
    void to_json(json & j, const ItemProperties & x);

    void from_json(const json & j, Item & x);
    void to_json(json & j, const Item & x);

    void from_json(const json & j, Devices & x);
    void to_json(json & j, const Devices & x);

    void from_json(const json & j, SchemaProperties & x);
    void to_json(json & j, const SchemaProperties & x);

    void from_json(const json & j, Schema & x);
    void to_json(json & j, const Schema & x);

    inline void from_json(const json & j, Type& x) {
        x.set_type(j.at("type").get<std::string>());
        x.set_type_enum(j.at("enum").get<std::vector<std::string>>());
    }

    inline void to_json(json & j, const Type & x) {
        j = json::object();
        j["type"] = x.get_type();
        j["enum"] = x.get_type_enum();
    }

    inline void from_json(const json & j, Functions& x) {
        x.set_type(j.at("type").get<std::string>());
        x.set_items(j.at("items").get<std::vector<Type>>());
    }

    inline void to_json(json & j, const Functions & x) {
        j = json::object();
        j["type"] = x.get_type();
        j["items"] = x.get_items();
    }

    inline void from_json(const json & j, Id& x) {
        x.set_type(j.at("type").get<std::string>());
    }

    inline void to_json(json & j, const Id & x) {
        j = json::object();
        j["type"] = x.get_type();
    }

    inline void from_json(const json & j, ItemProperties& x) {
        x.set_id(j.at("id").get<Id>());
        x.set_type(j.at("type").get<Type>());
        x.set_functions(j.at("functions").get<Functions>());
    }

    inline void to_json(json & j, const ItemProperties & x) {
        j = json::object();
        j["id"] = x.get_id();
        j["type"] = x.get_type();
        j["functions"] = x.get_functions();
    }

    inline void from_json(const json & j, Item& x) {
        x.set_type(j.at("type").get<std::string>());
        x.set_properties(j.at("properties").get<ItemProperties>());
        x.set_required(j.at("required").get<std::vector<std::string>>());
    }

    inline void to_json(json & j, const Item & x) {
        j = json::object();
        j["type"] = x.get_type();
        j["properties"] = x.get_properties();
        j["required"] = x.get_required();
    }

    inline void from_json(const json & j, Devices& x) {
        x.set_type(j.at("type").get<std::string>());
        x.set_items(j.at("items").get<std::vector<Item>>());
    }

    inline void to_json(json & j, const Devices & x) {
        j = json::object();
        j["type"] = x.get_type();
        j["items"] = x.get_items();
    }

    inline void from_json(const json & j, SchemaProperties& x) {
        x.set_id(j.at("id").get<Id>());
        x.set_devices(j.at("devices").get<Devices>());
    }

    inline void to_json(json & j, const SchemaProperties & x) {
        j = json::object();
        j["id"] = x.get_id();
        j["devices"] = x.get_devices();
    }

    inline void from_json(const json & j, Schema& x) {
        x.set_schema(j.at("$schema").get<std::string>());
        x.set_type(j.at("type").get<std::string>());
        x.set_properties(j.at("properties").get<SchemaProperties>());
        x.set_required(j.at("required").get<std::vector<std::string>>());
    }

    inline void to_json(json & j, const Schema & x) {
        j = json::object();
        j["$schema"] = x.get_schema();
        j["type"] = x.get_type();
        j["properties"] = x.get_properties();
        j["required"] = x.get_required();
    }
}
