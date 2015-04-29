// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: attrval_msg.proto

#define INTERNAL_SUPPRESS_PROTOBUF_FIELD_DEPRECATION
#include "attrval_msg.pb.h"

#include <algorithm>

#include <google/protobuf/stubs/common.h>
#include <google/protobuf/stubs/once.h>
#include <google/protobuf/io/coded_stream.h>
#include <google/protobuf/wire_format_lite_inl.h>
#include <google/protobuf/descriptor.h>
#include <google/protobuf/generated_message_reflection.h>
#include <google/protobuf/reflection_ops.h>
#include <google/protobuf/wire_format.h>
// @@protoc_insertion_point(includes)

namespace ac {
namespace serialization {

namespace {

const ::google::protobuf::Descriptor* AttrValMsg_descriptor_ = NULL;
const ::google::protobuf::internal::GeneratedMessageReflection*
  AttrValMsg_reflection_ = NULL;

}  // namespace


void protobuf_AssignDesc_attrval_5fmsg_2eproto() {
  protobuf_AddDesc_attrval_5fmsg_2eproto();
  const ::google::protobuf::FileDescriptor* file =
    ::google::protobuf::DescriptorPool::generated_pool()->FindFileByName(
      "attrval_msg.proto");
  GOOGLE_CHECK(file != NULL);
  AttrValMsg_descriptor_ = file->message_type(0);
  static const int AttrValMsg_offsets_[2] = {
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(AttrValMsg, type_),
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(AttrValMsg, value_),
  };
  AttrValMsg_reflection_ =
    new ::google::protobuf::internal::GeneratedMessageReflection(
      AttrValMsg_descriptor_,
      AttrValMsg::default_instance_,
      AttrValMsg_offsets_,
      GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(AttrValMsg, _has_bits_[0]),
      GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(AttrValMsg, _unknown_fields_),
      -1,
      ::google::protobuf::DescriptorPool::generated_pool(),
      ::google::protobuf::MessageFactory::generated_factory(),
      sizeof(AttrValMsg));
}

namespace {

GOOGLE_PROTOBUF_DECLARE_ONCE(protobuf_AssignDescriptors_once_);
inline void protobuf_AssignDescriptorsOnce() {
  ::google::protobuf::GoogleOnceInit(&protobuf_AssignDescriptors_once_,
                 &protobuf_AssignDesc_attrval_5fmsg_2eproto);
}

void protobuf_RegisterTypes(const ::std::string&) {
  protobuf_AssignDescriptorsOnce();
  ::google::protobuf::MessageFactory::InternalRegisterGeneratedMessage(
    AttrValMsg_descriptor_, &AttrValMsg::default_instance());
}

}  // namespace

void protobuf_ShutdownFile_attrval_5fmsg_2eproto() {
  delete AttrValMsg::default_instance_;
  delete AttrValMsg_reflection_;
}

void protobuf_AddDesc_attrval_5fmsg_2eproto() {
  static bool already_here = false;
  if (already_here) return;
  already_here = true;
  GOOGLE_PROTOBUF_VERIFY_VERSION;

  ::google::protobuf::DescriptorPool::InternalAddGeneratedFile(
    "\n\021attrval_msg.proto\022\020ac.serialization\")\n"
    "\nAttrValMsg\022\014\n\004type\030\001 \002(\005\022\r\n\005value\030\002 \001(\014", 80);
  ::google::protobuf::MessageFactory::InternalRegisterGeneratedFile(
    "attrval_msg.proto", &protobuf_RegisterTypes);
  AttrValMsg::default_instance_ = new AttrValMsg();
  AttrValMsg::default_instance_->InitAsDefaultInstance();
  ::google::protobuf::internal::OnShutdown(&protobuf_ShutdownFile_attrval_5fmsg_2eproto);
}

// Force AddDescriptors() to be called at static initialization time.
struct StaticDescriptorInitializer_attrval_5fmsg_2eproto {
  StaticDescriptorInitializer_attrval_5fmsg_2eproto() {
    protobuf_AddDesc_attrval_5fmsg_2eproto();
  }
} static_descriptor_initializer_attrval_5fmsg_2eproto_;

// ===================================================================

#ifndef _MSC_VER
const int AttrValMsg::kTypeFieldNumber;
const int AttrValMsg::kValueFieldNumber;
#endif  // !_MSC_VER

AttrValMsg::AttrValMsg()
  : ::google::protobuf::Message() {
  SharedCtor();
  // @@protoc_insertion_point(constructor:ac.serialization.AttrValMsg)
}

void AttrValMsg::InitAsDefaultInstance() {
}

AttrValMsg::AttrValMsg(const AttrValMsg& from)
  : ::google::protobuf::Message() {
  SharedCtor();
  MergeFrom(from);
  // @@protoc_insertion_point(copy_constructor:ac.serialization.AttrValMsg)
}

void AttrValMsg::SharedCtor() {
  ::google::protobuf::internal::GetEmptyString();
  _cached_size_ = 0;
  type_ = 0;
  value_ = const_cast< ::std::string*>(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
  ::memset(_has_bits_, 0, sizeof(_has_bits_));
}

AttrValMsg::~AttrValMsg() {
  // @@protoc_insertion_point(destructor:ac.serialization.AttrValMsg)
  SharedDtor();
}

void AttrValMsg::SharedDtor() {
  if (value_ != &::google::protobuf::internal::GetEmptyStringAlreadyInited()) {
    delete value_;
  }
  if (this != default_instance_) {
  }
}

void AttrValMsg::SetCachedSize(int size) const {
  GOOGLE_SAFE_CONCURRENT_WRITES_BEGIN();
  _cached_size_ = size;
  GOOGLE_SAFE_CONCURRENT_WRITES_END();
}
const ::google::protobuf::Descriptor* AttrValMsg::descriptor() {
  protobuf_AssignDescriptorsOnce();
  return AttrValMsg_descriptor_;
}

const AttrValMsg& AttrValMsg::default_instance() {
  if (default_instance_ == NULL) protobuf_AddDesc_attrval_5fmsg_2eproto();
  return *default_instance_;
}

AttrValMsg* AttrValMsg::default_instance_ = NULL;

AttrValMsg* AttrValMsg::New() const {
  return new AttrValMsg;
}

void AttrValMsg::Clear() {
  if (_has_bits_[0 / 32] & 3) {
    type_ = 0;
    if (has_value()) {
      if (value_ != &::google::protobuf::internal::GetEmptyStringAlreadyInited()) {
        value_->clear();
      }
    }
  }
  ::memset(_has_bits_, 0, sizeof(_has_bits_));
  mutable_unknown_fields()->Clear();
}

bool AttrValMsg::MergePartialFromCodedStream(
    ::google::protobuf::io::CodedInputStream* input) {
#define DO_(EXPRESSION) if (!(EXPRESSION)) goto failure
  ::google::protobuf::uint32 tag;
  // @@protoc_insertion_point(parse_start:ac.serialization.AttrValMsg)
  for (;;) {
    ::std::pair< ::google::protobuf::uint32, bool> p = input->ReadTagWithCutoff(127);
    tag = p.first;
    if (!p.second) goto handle_unusual;
    switch (::google::protobuf::internal::WireFormatLite::GetTagFieldNumber(tag)) {
      // required int32 type = 1;
      case 1: {
        if (tag == 8) {
          DO_((::google::protobuf::internal::WireFormatLite::ReadPrimitive<
                   ::google::protobuf::int32, ::google::protobuf::internal::WireFormatLite::TYPE_INT32>(
                 input, &type_)));
          set_has_type();
        } else {
          goto handle_unusual;
        }
        if (input->ExpectTag(18)) goto parse_value;
        break;
      }

      // optional bytes value = 2;
      case 2: {
        if (tag == 18) {
         parse_value:
          DO_(::google::protobuf::internal::WireFormatLite::ReadBytes(
                input, this->mutable_value()));
        } else {
          goto handle_unusual;
        }
        if (input->ExpectAtEnd()) goto success;
        break;
      }

      default: {
      handle_unusual:
        if (tag == 0 ||
            ::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_END_GROUP) {
          goto success;
        }
        DO_(::google::protobuf::internal::WireFormat::SkipField(
              input, tag, mutable_unknown_fields()));
        break;
      }
    }
  }
success:
  // @@protoc_insertion_point(parse_success:ac.serialization.AttrValMsg)
  return true;
failure:
  // @@protoc_insertion_point(parse_failure:ac.serialization.AttrValMsg)
  return false;
#undef DO_
}

void AttrValMsg::SerializeWithCachedSizes(
    ::google::protobuf::io::CodedOutputStream* output) const {
  // @@protoc_insertion_point(serialize_start:ac.serialization.AttrValMsg)
  // required int32 type = 1;
  if (has_type()) {
    ::google::protobuf::internal::WireFormatLite::WriteInt32(1, this->type(), output);
  }

  // optional bytes value = 2;
  if (has_value()) {
    ::google::protobuf::internal::WireFormatLite::WriteBytesMaybeAliased(
      2, this->value(), output);
  }

  if (!unknown_fields().empty()) {
    ::google::protobuf::internal::WireFormat::SerializeUnknownFields(
        unknown_fields(), output);
  }
  // @@protoc_insertion_point(serialize_end:ac.serialization.AttrValMsg)
}

::google::protobuf::uint8* AttrValMsg::SerializeWithCachedSizesToArray(
    ::google::protobuf::uint8* target) const {
  // @@protoc_insertion_point(serialize_to_array_start:ac.serialization.AttrValMsg)
  // required int32 type = 1;
  if (has_type()) {
    target = ::google::protobuf::internal::WireFormatLite::WriteInt32ToArray(1, this->type(), target);
  }

  // optional bytes value = 2;
  if (has_value()) {
    target =
      ::google::protobuf::internal::WireFormatLite::WriteBytesToArray(
        2, this->value(), target);
  }

  if (!unknown_fields().empty()) {
    target = ::google::protobuf::internal::WireFormat::SerializeUnknownFieldsToArray(
        unknown_fields(), target);
  }
  // @@protoc_insertion_point(serialize_to_array_end:ac.serialization.AttrValMsg)
  return target;
}

int AttrValMsg::ByteSize() const {
  int total_size = 0;

  if (_has_bits_[0 / 32] & (0xffu << (0 % 32))) {
    // required int32 type = 1;
    if (has_type()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::Int32Size(
          this->type());
    }

    // optional bytes value = 2;
    if (has_value()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::BytesSize(
          this->value());
    }

  }
  if (!unknown_fields().empty()) {
    total_size +=
      ::google::protobuf::internal::WireFormat::ComputeUnknownFieldsSize(
        unknown_fields());
  }
  GOOGLE_SAFE_CONCURRENT_WRITES_BEGIN();
  _cached_size_ = total_size;
  GOOGLE_SAFE_CONCURRENT_WRITES_END();
  return total_size;
}

void AttrValMsg::MergeFrom(const ::google::protobuf::Message& from) {
  GOOGLE_CHECK_NE(&from, this);
  const AttrValMsg* source =
    ::google::protobuf::internal::dynamic_cast_if_available<const AttrValMsg*>(
      &from);
  if (source == NULL) {
    ::google::protobuf::internal::ReflectionOps::Merge(from, this);
  } else {
    MergeFrom(*source);
  }
}

void AttrValMsg::MergeFrom(const AttrValMsg& from) {
  GOOGLE_CHECK_NE(&from, this);
  if (from._has_bits_[0 / 32] & (0xffu << (0 % 32))) {
    if (from.has_type()) {
      set_type(from.type());
    }
    if (from.has_value()) {
      set_value(from.value());
    }
  }
  mutable_unknown_fields()->MergeFrom(from.unknown_fields());
}

void AttrValMsg::CopyFrom(const ::google::protobuf::Message& from) {
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

void AttrValMsg::CopyFrom(const AttrValMsg& from) {
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

bool AttrValMsg::IsInitialized() const {
  if ((_has_bits_[0] & 0x00000001) != 0x00000001) return false;

  return true;
}

void AttrValMsg::Swap(AttrValMsg* other) {
  if (other != this) {
    std::swap(type_, other->type_);
    std::swap(value_, other->value_);
    std::swap(_has_bits_[0], other->_has_bits_[0]);
    _unknown_fields_.Swap(&other->_unknown_fields_);
    std::swap(_cached_size_, other->_cached_size_);
  }
}

::google::protobuf::Metadata AttrValMsg::GetMetadata() const {
  protobuf_AssignDescriptorsOnce();
  ::google::protobuf::Metadata metadata;
  metadata.descriptor = AttrValMsg_descriptor_;
  metadata.reflection = AttrValMsg_reflection_;
  return metadata;
}


// @@protoc_insertion_point(namespace_scope)

}  // namespace serialization
}  // namespace ac

// @@protoc_insertion_point(global_scope)
