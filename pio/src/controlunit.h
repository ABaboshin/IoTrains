// /*
//  *  This file is generated with Embedded Proto, PLEASE DO NOT EDIT!
//  *  source: controlunit.proto
//  */

// // This file is generated. Please do not edit!
// #ifndef CONTROLUNIT_H
// #define CONTROLUNIT_H

// #include <cstdint>
// #include <MessageInterface.h>
// #include <WireFormatter.h>
// #include <Fields.h>
// #include <MessageSizeCalculator.h>
// #include <ReadBufferSection.h>
// #include <RepeatedFieldFixedSize.h>
// #include <FieldStringBytes.h>
// #include <Errors.h>
// #include <Defines.h>
// #include <limits>

// // Include external proto definitions

// namespace railway {

// enum class DeviceType : uint32_t
// {
//   Train = 0,
//   Turnout = 1
// };

// enum class Function : uint32_t
// {
//   Default = 0,
//   Music = 1
// };

// template<
//     uint32_t Device_id_LENGTH, 
//     uint32_t Device_Functions_REP_LENGTH
// >
// class Device final: public ::EmbeddedProto::MessageInterface
// {
//   public:
//     Device() = default;
//     Device(const Device& rhs )
//     {
//       set_id(rhs.get_id());
//       set_type(rhs.get_type());
//       set_Functions(rhs.get_Functions());
//     }

//     Device(const Device&& rhs ) noexcept
//     {
//       set_id(rhs.get_id());
//       set_type(rhs.get_type());
//       set_Functions(rhs.get_Functions());
//     }

//     ~Device() override = default;

//     enum class FieldNumber : uint32_t
//     {
//       NOT_SET = 0,
//       ID = 1,
//       TYPE = 2,
//       FUNCTIONS = 3
//     };

//     Device& operator=(const Device& rhs)
//     {
//       set_id(rhs.get_id());
//       set_type(rhs.get_type());
//       set_Functions(rhs.get_Functions());
//       return *this;
//     }

//     Device& operator=(const Device&& rhs) noexcept
//     {
//       set_id(rhs.get_id());
//       set_type(rhs.get_type());
//       set_Functions(rhs.get_Functions());
//       return *this;
//     }

//     static constexpr char const* ID_NAME = "id";
//     inline void clear_id() { id_.clear(); }
//     inline ::EmbeddedProto::FieldString<Device_id_LENGTH>& mutable_id() { return id_; }
//     inline void set_id(const ::EmbeddedProto::FieldString<Device_id_LENGTH>& rhs) { id_.set(rhs); }
//     inline const ::EmbeddedProto::FieldString<Device_id_LENGTH>& get_id() const { return id_; }
//     inline const char* id() const { return id_.get_const(); }

//     static constexpr char const* TYPE_NAME = "type";
//     inline void clear_type() { type_.clear(); }
//     inline void set_type(const DeviceType& value) { type_ = value; }
//     inline void set_type(const DeviceType&& value) { type_ = value; }
//     inline const DeviceType& get_type() const { return type_.get(); }
//     inline DeviceType type() const { return type_.get(); }

//     static constexpr char const* FUNCTIONS_NAME = "Functions";
//     inline const EmbeddedProto::enumeration<Function>& Functions(uint32_t index) const { return Functions_[index]; }
//     inline void clear_Functions() { Functions_.clear(); }
//     inline void set_Functions(uint32_t index, const EmbeddedProto::enumeration<Function>& value) { Functions_.set(index, value); }
//     inline void set_Functions(uint32_t index, const EmbeddedProto::enumeration<Function>&& value) { Functions_.set(index, value); }
//     inline void set_Functions(const ::EmbeddedProto::RepeatedFieldFixedSize<EmbeddedProto::enumeration<Function>, Device_Functions_REP_LENGTH>& values) { Functions_ = values; }
//     inline void add_Functions(const EmbeddedProto::enumeration<Function>& value) { Functions_.add(value); }
//     inline ::EmbeddedProto::RepeatedFieldFixedSize<EmbeddedProto::enumeration<Function>, Device_Functions_REP_LENGTH>& mutable_Functions() { return Functions_; }
//     inline EmbeddedProto::enumeration<Function>& mutable_Functions(uint32_t index) { return Functions_[index]; }
//     inline const ::EmbeddedProto::RepeatedFieldFixedSize<EmbeddedProto::enumeration<Function>, Device_Functions_REP_LENGTH>& get_Functions() const { return Functions_; }
//     inline const ::EmbeddedProto::RepeatedFieldFixedSize<EmbeddedProto::enumeration<Function>, Device_Functions_REP_LENGTH>& Functions() const { return Functions_; }


//     ::EmbeddedProto::Error serialize(::EmbeddedProto::WriteBufferInterface& buffer) const override
//     {
//       ::EmbeddedProto::Error return_value = ::EmbeddedProto::Error::NO_ERRORS;

//       if(::EmbeddedProto::Error::NO_ERRORS == return_value)
//       {
//         return_value = id_.serialize_with_id(static_cast<uint32_t>(FieldNumber::ID), buffer, false);
//       }

//       if((static_cast<DeviceType>(0) != type_.get()) && (::EmbeddedProto::Error::NO_ERRORS == return_value))
//       {
//         return_value = type_.serialize_with_id(static_cast<uint32_t>(FieldNumber::TYPE), buffer, false);
//       }

//       if(::EmbeddedProto::Error::NO_ERRORS == return_value)
//       {
//         return_value = Functions_.serialize_with_id(static_cast<uint32_t>(FieldNumber::FUNCTIONS), buffer, false);
//       }

//       return return_value;
//     };

//     ::EmbeddedProto::Error deserialize(::EmbeddedProto::ReadBufferInterface& buffer) override
//     {
//       ::EmbeddedProto::Error return_value = ::EmbeddedProto::Error::NO_ERRORS;
//       ::EmbeddedProto::WireFormatter::WireType wire_type = ::EmbeddedProto::WireFormatter::WireType::VARINT;
//       uint32_t id_number = 0;
//       FieldNumber id_tag = FieldNumber::NOT_SET;

//       ::EmbeddedProto::Error tag_value = ::EmbeddedProto::WireFormatter::DeserializeTag(buffer, wire_type, id_number);
//       while((::EmbeddedProto::Error::NO_ERRORS == return_value) && (::EmbeddedProto::Error::NO_ERRORS == tag_value))
//       {
//         id_tag = static_cast<FieldNumber>(id_number);
//         switch(id_tag)
//         {
//           case FieldNumber::ID:
//             return_value = id_.deserialize_check_type(buffer, wire_type);
//             break;

//           case FieldNumber::TYPE:
//             return_value = type_.deserialize_check_type(buffer, wire_type);
//             break;

//           case FieldNumber::FUNCTIONS:
//             return_value = Functions_.deserialize_check_type(buffer, wire_type);
//             break;

//           case FieldNumber::NOT_SET:
//             return_value = ::EmbeddedProto::Error::INVALID_FIELD_ID;
//             break;

//           default:
//             return_value = skip_unknown_field(buffer, wire_type);
//             break;
//         }

//         if(::EmbeddedProto::Error::NO_ERRORS == return_value)
//         {
//           // Read the next tag.
//           tag_value = ::EmbeddedProto::WireFormatter::DeserializeTag(buffer, wire_type, id_number);
//         }
//       }

//       // When an error was detect while reading the tag but no other errors where found, set it in the return value.
//       if((::EmbeddedProto::Error::NO_ERRORS == return_value)
//          && (::EmbeddedProto::Error::NO_ERRORS != tag_value)
//          && (::EmbeddedProto::Error::END_OF_BUFFER != tag_value)) // The end of the buffer is not an array in this case.
//       {
//         return_value = tag_value;
//       }

//       return return_value;
//     };

//     void clear() override
//     {
//       clear_id();
//       clear_type();
//       clear_Functions();

//     }

//     static char const* field_number_to_name(const FieldNumber fieldNumber)
//     {
//       char const* name = nullptr;
//       switch(fieldNumber)
//       {
//         case FieldNumber::ID:
//           name = ID_NAME;
//           break;
//         case FieldNumber::TYPE:
//           name = TYPE_NAME;
//           break;
//         case FieldNumber::FUNCTIONS:
//           name = FUNCTIONS_NAME;
//           break;
//         default:
//           name = "Invalid FieldNumber";
//           break;
//       }
//       return name;
//     }

// #ifdef MSG_TO_STRING

//     ::EmbeddedProto::string_view to_string(::EmbeddedProto::string_view& str) const
//     {
//       return this->to_string(str, 0, nullptr, true);
//     }

//     ::EmbeddedProto::string_view to_string(::EmbeddedProto::string_view& str, const uint32_t indent_level, char const* name, const bool first_field) const override
//     {
//       ::EmbeddedProto::string_view left_chars = str;
//       int32_t n_chars_used = 0;

//       if(!first_field)
//       {
//         // Add a comma behind the previous field.
//         n_chars_used = snprintf(left_chars.data, left_chars.size, ",\n");
//         if(0 < n_chars_used)
//         {
//           // Update the character pointer and characters left in the array.
//           left_chars.data += n_chars_used;
//           left_chars.size -= n_chars_used;
//         }
//       }

//       if(nullptr != name)
//       {
//         if( 0 == indent_level)
//         {
//           n_chars_used = snprintf(left_chars.data, left_chars.size, "\"%s\": {\n", name);
//         }
//         else
//         {
//           n_chars_used = snprintf(left_chars.data, left_chars.size, "%*s\"%s\": {\n", indent_level, " ", name);
//         }
//       }
//       else
//       {
//         if( 0 == indent_level)
//         {
//           n_chars_used = snprintf(left_chars.data, left_chars.size, "{\n");
//         }
//         else
//         {
//           n_chars_used = snprintf(left_chars.data, left_chars.size, "%*s{\n", indent_level, " ");
//         }
//       }
      
//       if(0 < n_chars_used)
//       {
//         left_chars.data += n_chars_used;
//         left_chars.size -= n_chars_used;
//       }

//       left_chars = id_.to_string(left_chars, indent_level + 2, ID_NAME, true);
//       left_chars = type_.to_string(left_chars, indent_level + 2, TYPE_NAME, false);
//       left_chars = Functions_.to_string(left_chars, indent_level + 2, FUNCTIONS_NAME, false);
  
//       if( 0 == indent_level) 
//       {
//         n_chars_used = snprintf(left_chars.data, left_chars.size, "\n}");
//       }
//       else 
//       {
//         n_chars_used = snprintf(left_chars.data, left_chars.size, "\n%*s}", indent_level, " ");
//       }

//       if(0 < n_chars_used)
//       {
//         left_chars.data += n_chars_used;
//         left_chars.size -= n_chars_used;
//       }

//       return left_chars;
//     }

// #endif // End of MSG_TO_STRING

//   private:


//       ::EmbeddedProto::FieldString<Device_id_LENGTH> id_;
//       EmbeddedProto::enumeration<DeviceType> type_ = static_cast<DeviceType>(0);
//       ::EmbeddedProto::RepeatedFieldFixedSize<EmbeddedProto::enumeration<Function>, Device_Functions_REP_LENGTH> Functions_;

// };

// template<
//     uint32_t ControlUnit_id_LENGTH, 
//     uint32_t ControlUnit_device_REP_LENGTH, 
//     uint32_t ControlUnit_device_Device_id_LENGTH, 
//     uint32_t ControlUnit_device_Device_Functions_REP_LENGTH
// >
// class ControlUnit final: public ::EmbeddedProto::MessageInterface
// {
//   public:
//     ControlUnit() = default;
//     ControlUnit(const ControlUnit& rhs )
//     {
//       set_id(rhs.get_id());
//       set_device(rhs.get_device());
//     }

//     ControlUnit(const ControlUnit&& rhs ) noexcept
//     {
//       set_id(rhs.get_id());
//       set_device(rhs.get_device());
//     }

//     ~ControlUnit() override = default;

//     enum class FieldNumber : uint32_t
//     {
//       NOT_SET = 0,
//       ID = 1,
//       DEVICE = 2
//     };

//     ControlUnit& operator=(const ControlUnit& rhs)
//     {
//       set_id(rhs.get_id());
//       set_device(rhs.get_device());
//       return *this;
//     }

//     ControlUnit& operator=(const ControlUnit&& rhs) noexcept
//     {
//       set_id(rhs.get_id());
//       set_device(rhs.get_device());
//       return *this;
//     }

//     static constexpr char const* ID_NAME = "id";
//     inline void clear_id() { id_.clear(); }
//     inline ::EmbeddedProto::FieldString<ControlUnit_id_LENGTH>& mutable_id() { return id_; }
//     inline void set_id(const ::EmbeddedProto::FieldString<ControlUnit_id_LENGTH>& rhs) { id_.set(rhs); }
//     inline const ::EmbeddedProto::FieldString<ControlUnit_id_LENGTH>& get_id() const { return id_; }
//     inline const char* id() const { return id_.get_const(); }

//     static constexpr char const* DEVICE_NAME = "device";
//     inline const Device<ControlUnit_device_Device_id_LENGTH, ControlUnit_device_Device_Functions_REP_LENGTH>& device(uint32_t index) const { return device_[index]; }
//     inline void clear_device() { device_.clear(); }
//     inline void set_device(uint32_t index, const Device<ControlUnit_device_Device_id_LENGTH, ControlUnit_device_Device_Functions_REP_LENGTH>& value) { device_.set(index, value); }
//     inline void set_device(uint32_t index, const Device<ControlUnit_device_Device_id_LENGTH, ControlUnit_device_Device_Functions_REP_LENGTH>&& value) { device_.set(index, value); }
//     inline void set_device(const ::EmbeddedProto::RepeatedFieldFixedSize<Device<ControlUnit_device_Device_id_LENGTH, ControlUnit_device_Device_Functions_REP_LENGTH>, ControlUnit_device_REP_LENGTH>& values) { device_ = values; }
//     inline void add_device(const Device<ControlUnit_device_Device_id_LENGTH, ControlUnit_device_Device_Functions_REP_LENGTH>& value) { device_.add(value); }
//     inline ::EmbeddedProto::RepeatedFieldFixedSize<Device<ControlUnit_device_Device_id_LENGTH, ControlUnit_device_Device_Functions_REP_LENGTH>, ControlUnit_device_REP_LENGTH>& mutable_device() { return device_; }
//     inline Device<ControlUnit_device_Device_id_LENGTH, ControlUnit_device_Device_Functions_REP_LENGTH>& mutable_device(uint32_t index) { return device_[index]; }
//     inline const ::EmbeddedProto::RepeatedFieldFixedSize<Device<ControlUnit_device_Device_id_LENGTH, ControlUnit_device_Device_Functions_REP_LENGTH>, ControlUnit_device_REP_LENGTH>& get_device() const { return device_; }
//     inline const ::EmbeddedProto::RepeatedFieldFixedSize<Device<ControlUnit_device_Device_id_LENGTH, ControlUnit_device_Device_Functions_REP_LENGTH>, ControlUnit_device_REP_LENGTH>& device() const { return device_; }


//     ::EmbeddedProto::Error serialize(::EmbeddedProto::WriteBufferInterface& buffer) const override
//     {
//       ::EmbeddedProto::Error return_value = ::EmbeddedProto::Error::NO_ERRORS;

//       if(::EmbeddedProto::Error::NO_ERRORS == return_value)
//       {
//         return_value = id_.serialize_with_id(static_cast<uint32_t>(FieldNumber::ID), buffer, false);
//       }

//       if(::EmbeddedProto::Error::NO_ERRORS == return_value)
//       {
//         return_value = device_.serialize_with_id(static_cast<uint32_t>(FieldNumber::DEVICE), buffer, false);
//       }

//       return return_value;
//     };

//     ::EmbeddedProto::Error deserialize(::EmbeddedProto::ReadBufferInterface& buffer) override
//     {
//       ::EmbeddedProto::Error return_value = ::EmbeddedProto::Error::NO_ERRORS;
//       ::EmbeddedProto::WireFormatter::WireType wire_type = ::EmbeddedProto::WireFormatter::WireType::VARINT;
//       uint32_t id_number = 0;
//       FieldNumber id_tag = FieldNumber::NOT_SET;

//       ::EmbeddedProto::Error tag_value = ::EmbeddedProto::WireFormatter::DeserializeTag(buffer, wire_type, id_number);
//       while((::EmbeddedProto::Error::NO_ERRORS == return_value) && (::EmbeddedProto::Error::NO_ERRORS == tag_value))
//       {
//         id_tag = static_cast<FieldNumber>(id_number);
//         switch(id_tag)
//         {
//           case FieldNumber::ID:
//             return_value = id_.deserialize_check_type(buffer, wire_type);
//             break;

//           case FieldNumber::DEVICE:
//             return_value = device_.deserialize_check_type(buffer, wire_type);
//             break;

//           case FieldNumber::NOT_SET:
//             return_value = ::EmbeddedProto::Error::INVALID_FIELD_ID;
//             break;

//           default:
//             return_value = skip_unknown_field(buffer, wire_type);
//             break;
//         }

//         if(::EmbeddedProto::Error::NO_ERRORS == return_value)
//         {
//           // Read the next tag.
//           tag_value = ::EmbeddedProto::WireFormatter::DeserializeTag(buffer, wire_type, id_number);
//         }
//       }

//       // When an error was detect while reading the tag but no other errors where found, set it in the return value.
//       if((::EmbeddedProto::Error::NO_ERRORS == return_value)
//          && (::EmbeddedProto::Error::NO_ERRORS != tag_value)
//          && (::EmbeddedProto::Error::END_OF_BUFFER != tag_value)) // The end of the buffer is not an array in this case.
//       {
//         return_value = tag_value;
//       }

//       return return_value;
//     };

//     void clear() override
//     {
//       clear_id();
//       clear_device();

//     }

//     static char const* field_number_to_name(const FieldNumber fieldNumber)
//     {
//       char const* name = nullptr;
//       switch(fieldNumber)
//       {
//         case FieldNumber::ID:
//           name = ID_NAME;
//           break;
//         case FieldNumber::DEVICE:
//           name = DEVICE_NAME;
//           break;
//         default:
//           name = "Invalid FieldNumber";
//           break;
//       }
//       return name;
//     }

// #ifdef MSG_TO_STRING

//     ::EmbeddedProto::string_view to_string(::EmbeddedProto::string_view& str) const
//     {
//       return this->to_string(str, 0, nullptr, true);
//     }

//     ::EmbeddedProto::string_view to_string(::EmbeddedProto::string_view& str, const uint32_t indent_level, char const* name, const bool first_field) const override
//     {
//       ::EmbeddedProto::string_view left_chars = str;
//       int32_t n_chars_used = 0;

//       if(!first_field)
//       {
//         // Add a comma behind the previous field.
//         n_chars_used = snprintf(left_chars.data, left_chars.size, ",\n");
//         if(0 < n_chars_used)
//         {
//           // Update the character pointer and characters left in the array.
//           left_chars.data += n_chars_used;
//           left_chars.size -= n_chars_used;
//         }
//       }

//       if(nullptr != name)
//       {
//         if( 0 == indent_level)
//         {
//           n_chars_used = snprintf(left_chars.data, left_chars.size, "\"%s\": {\n", name);
//         }
//         else
//         {
//           n_chars_used = snprintf(left_chars.data, left_chars.size, "%*s\"%s\": {\n", indent_level, " ", name);
//         }
//       }
//       else
//       {
//         if( 0 == indent_level)
//         {
//           n_chars_used = snprintf(left_chars.data, left_chars.size, "{\n");
//         }
//         else
//         {
//           n_chars_used = snprintf(left_chars.data, left_chars.size, "%*s{\n", indent_level, " ");
//         }
//       }
      
//       if(0 < n_chars_used)
//       {
//         left_chars.data += n_chars_used;
//         left_chars.size -= n_chars_used;
//       }

//       left_chars = id_.to_string(left_chars, indent_level + 2, ID_NAME, true);
//       left_chars = device_.to_string(left_chars, indent_level + 2, DEVICE_NAME, false);
  
//       if( 0 == indent_level) 
//       {
//         n_chars_used = snprintf(left_chars.data, left_chars.size, "\n}");
//       }
//       else 
//       {
//         n_chars_used = snprintf(left_chars.data, left_chars.size, "\n%*s}", indent_level, " ");
//       }

//       if(0 < n_chars_used)
//       {
//         left_chars.data += n_chars_used;
//         left_chars.size -= n_chars_used;
//       }

//       return left_chars;
//     }

// #endif // End of MSG_TO_STRING

//   private:


//       ::EmbeddedProto::FieldString<ControlUnit_id_LENGTH> id_;
//       ::EmbeddedProto::RepeatedFieldFixedSize<Device<ControlUnit_device_Device_id_LENGTH, ControlUnit_device_Device_Functions_REP_LENGTH>, ControlUnit_device_REP_LENGTH> device_;

// };

// } // End of namespace railway
// #endif // CONTROLUNIT_H