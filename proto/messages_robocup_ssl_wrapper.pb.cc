// Generated by the protocol buffer compiler.  DO NOT EDIT!

#define INTERNAL_SUPPRESS_PROTOBUF_FIELD_DEPRECATION
#include "messages_robocup_ssl_wrapper.pb.h"
#include <google/protobuf/stubs/once.h>
#include <google/protobuf/io/coded_stream.h>
#include <google/protobuf/wire_format_lite_inl.h>
#include <google/protobuf/descriptor.h>
#include <google/protobuf/reflection_ops.h>
#include <google/protobuf/wire_format.h>

namespace {

const ::google::protobuf::Descriptor* SSL_WrapperPacket_descriptor_ = NULL;
const ::google::protobuf::internal::GeneratedMessageReflection*
  SSL_WrapperPacket_reflection_ = NULL;

}  // namespace


void protobuf_AssignDesc_messages_5frobocup_5fssl_5fwrapper_2eproto() {
  protobuf_AddDesc_messages_5frobocup_5fssl_5fwrapper_2eproto();
  const ::google::protobuf::FileDescriptor* file =
    ::google::protobuf::DescriptorPool::generated_pool()->FindFileByName(
      "messages_robocup_ssl_wrapper.proto");
  GOOGLE_CHECK(file != NULL);
  SSL_WrapperPacket_descriptor_ = file->message_type(0);
  static const int SSL_WrapperPacket_offsets_[2] = {
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(SSL_WrapperPacket, detection_),
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(SSL_WrapperPacket, geometry_),
  };
  SSL_WrapperPacket_reflection_ =
    new ::google::protobuf::internal::GeneratedMessageReflection(
      SSL_WrapperPacket_descriptor_,
      SSL_WrapperPacket::default_instance_,
      SSL_WrapperPacket_offsets_,
      GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(SSL_WrapperPacket, _has_bits_[0]),
      GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(SSL_WrapperPacket, _unknown_fields_),
      -1,
      ::google::protobuf::DescriptorPool::generated_pool(),
      ::google::protobuf::MessageFactory::generated_factory(),
      sizeof(SSL_WrapperPacket));
}

namespace {

GOOGLE_PROTOBUF_DECLARE_ONCE(protobuf_AssignDescriptors_once_);
inline void protobuf_AssignDescriptorsOnce() {
  ::google::protobuf::GoogleOnceInit(&protobuf_AssignDescriptors_once_,
                 &protobuf_AssignDesc_messages_5frobocup_5fssl_5fwrapper_2eproto);
}

void protobuf_RegisterTypes(const ::std::string&) {
  protobuf_AssignDescriptorsOnce();
  ::google::protobuf::MessageFactory::InternalRegisterGeneratedMessage(
    SSL_WrapperPacket_descriptor_, &SSL_WrapperPacket::default_instance());
}

}  // namespace

void protobuf_ShutdownFile_messages_5frobocup_5fssl_5fwrapper_2eproto() {
  delete SSL_WrapperPacket::default_instance_;
  delete SSL_WrapperPacket_reflection_;
}

void protobuf_AddDesc_messages_5frobocup_5fssl_5fwrapper_2eproto() {
  static bool already_here = false;
  if (already_here) return;
  already_here = true;
  GOOGLE_PROTOBUF_VERIFY_VERSION;

  ::protobuf_AddDesc_messages_5frobocup_5fssl_5fdetection_2eproto();
  ::protobuf_AddDesc_messages_5frobocup_5fssl_5fgeometry_2eproto();
  ::google::protobuf::DescriptorPool::InternalAddGeneratedFile(
    "\n\"messages_robocup_ssl_wrapper.proto\032$me"
    "ssages_robocup_ssl_detection.proto\032#mess"
    "ages_robocup_ssl_geometry.proto\"`\n\021SSL_W"
    "rapperPacket\022&\n\tdetection\030\001 \001(\0132\023.SSL_De"
    "tectionFrame\022#\n\010geometry\030\002 \001(\0132\021.SSL_Geo"
    "metryData", 209);
  ::google::protobuf::MessageFactory::InternalRegisterGeneratedFile(
    "messages_robocup_ssl_wrapper.proto", &protobuf_RegisterTypes);
  SSL_WrapperPacket::default_instance_ = new SSL_WrapperPacket();
  SSL_WrapperPacket::default_instance_->InitAsDefaultInstance();
  ::google::protobuf::internal::OnShutdown(&protobuf_ShutdownFile_messages_5frobocup_5fssl_5fwrapper_2eproto);
}

// Force AddDescriptors() to be called at static initialization time.
struct StaticDescriptorInitializer_messages_5frobocup_5fssl_5fwrapper_2eproto {
  StaticDescriptorInitializer_messages_5frobocup_5fssl_5fwrapper_2eproto() {
    protobuf_AddDesc_messages_5frobocup_5fssl_5fwrapper_2eproto();
  }
} static_descriptor_initializer_messages_5frobocup_5fssl_5fwrapper_2eproto_;


// ===================================================================

#ifndef _MSC_VER
const int SSL_WrapperPacket::kDetectionFieldNumber;
const int SSL_WrapperPacket::kGeometryFieldNumber;
#endif  // !_MSC_VER

SSL_WrapperPacket::SSL_WrapperPacket() {
  SharedCtor();
}

void SSL_WrapperPacket::InitAsDefaultInstance() {
  detection_ = const_cast< ::SSL_DetectionFrame*>(&::SSL_DetectionFrame::default_instance());
  geometry_ = const_cast< ::SSL_GeometryData*>(&::SSL_GeometryData::default_instance());
}

SSL_WrapperPacket::SSL_WrapperPacket(const SSL_WrapperPacket& from) {
  SharedCtor();
  MergeFrom(from);
}

void SSL_WrapperPacket::SharedCtor() {
  _cached_size_ = 0;
  detection_ = NULL;
  geometry_ = NULL;
  ::memset(_has_bits_, 0, sizeof(_has_bits_));
}

SSL_WrapperPacket::~SSL_WrapperPacket() {
  SharedDtor();
}

void SSL_WrapperPacket::SharedDtor() {
  if (this != default_instance_) {
    delete detection_;
    delete geometry_;
  }
}

const ::google::protobuf::Descriptor* SSL_WrapperPacket::descriptor() {
  protobuf_AssignDescriptorsOnce();
  return SSL_WrapperPacket_descriptor_;
}

const SSL_WrapperPacket& SSL_WrapperPacket::default_instance() {
  if (default_instance_ == NULL) protobuf_AddDesc_messages_5frobocup_5fssl_5fwrapper_2eproto();  return *default_instance_;
}

SSL_WrapperPacket* SSL_WrapperPacket::default_instance_ = NULL;

SSL_WrapperPacket* SSL_WrapperPacket::New() const {
  return new SSL_WrapperPacket;
}

void SSL_WrapperPacket::Clear() {
  if (_has_bits_[0 / 32] & (0xffu << (0 % 32))) {
    if (_has_bit(0)) {
      if (detection_ != NULL) detection_->::SSL_DetectionFrame::Clear();
    }
    if (_has_bit(1)) {
      if (geometry_ != NULL) geometry_->::SSL_GeometryData::Clear();
    }
  }
  ::memset(_has_bits_, 0, sizeof(_has_bits_));
  mutable_unknown_fields()->Clear();
}

bool SSL_WrapperPacket::MergePartialFromCodedStream(
    ::google::protobuf::io::CodedInputStream* input) {
#define DO_(EXPRESSION) if (!(EXPRESSION)) return false
  ::google::protobuf::uint32 tag;
  while ((tag = input->ReadTag()) != 0) {
    switch (::google::protobuf::internal::WireFormatLite::GetTagFieldNumber(tag)) {
      // optional .SSL_DetectionFrame detection = 1;
      case 1: {
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) !=
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_LENGTH_DELIMITED) {
          goto handle_uninterpreted;
        }
        DO_(::google::protobuf::internal::WireFormatLite::ReadMessageNoVirtual(
             input, mutable_detection()));
        if (input->ExpectTag(18)) goto parse_geometry;
        break;
      }
      
      // optional .SSL_GeometryData geometry = 2;
      case 2: {
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) !=
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_LENGTH_DELIMITED) {
          goto handle_uninterpreted;
        }
       parse_geometry:
        DO_(::google::protobuf::internal::WireFormatLite::ReadMessageNoVirtual(
             input, mutable_geometry()));
        if (input->ExpectAtEnd()) return true;
        break;
      }
      
      default: {
      handle_uninterpreted:
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_END_GROUP) {
          return true;
        }
        DO_(::google::protobuf::internal::WireFormat::SkipField(
              input, tag, mutable_unknown_fields()));
        break;
      }
    }
  }
  return true;
#undef DO_
}

void SSL_WrapperPacket::SerializeWithCachedSizes(
    ::google::protobuf::io::CodedOutputStream* output) const {
  ::google::protobuf::uint8* raw_buffer = output->GetDirectBufferForNBytesAndAdvance(_cached_size_);
  if (raw_buffer != NULL) {
    SSL_WrapperPacket::SerializeWithCachedSizesToArray(raw_buffer);
    return;
  }
  
  // optional .SSL_DetectionFrame detection = 1;
  if (_has_bit(0)) {
    ::google::protobuf::internal::WireFormatLite::WriteMessageNoVirtual(
      1, this->detection(), output);
  }
  
  // optional .SSL_GeometryData geometry = 2;
  if (_has_bit(1)) {
    ::google::protobuf::internal::WireFormatLite::WriteMessageNoVirtual(
      2, this->geometry(), output);
  }
  
  if (!unknown_fields().empty()) {
    ::google::protobuf::internal::WireFormat::SerializeUnknownFields(
        unknown_fields(), output);
  }
}

::google::protobuf::uint8* SSL_WrapperPacket::SerializeWithCachedSizesToArray(
    ::google::protobuf::uint8* target) const {
  // optional .SSL_DetectionFrame detection = 1;
  if (_has_bit(0)) {
    target = ::google::protobuf::internal::WireFormatLite::
      WriteMessageNoVirtualToArray(
        1, this->detection(), target);
  }
  
  // optional .SSL_GeometryData geometry = 2;
  if (_has_bit(1)) {
    target = ::google::protobuf::internal::WireFormatLite::
      WriteMessageNoVirtualToArray(
        2, this->geometry(), target);
  }
  
  if (!unknown_fields().empty()) {
    target = ::google::protobuf::internal::WireFormat::SerializeUnknownFieldsToArray(
        unknown_fields(), target);
  }
  return target;
}

int SSL_WrapperPacket::ByteSize() const {
  int total_size = 0;
  
  if (_has_bits_[0 / 32] & (0xffu << (0 % 32))) {
    // optional .SSL_DetectionFrame detection = 1;
    if (has_detection()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::MessageSizeNoVirtual(
          this->detection());
    }
    
    // optional .SSL_GeometryData geometry = 2;
    if (has_geometry()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::MessageSizeNoVirtual(
          this->geometry());
    }
    
  }
  if (!unknown_fields().empty()) {
    total_size +=
      ::google::protobuf::internal::WireFormat::ComputeUnknownFieldsSize(
        unknown_fields());
  }
  _cached_size_ = total_size;
  return total_size;
}

void SSL_WrapperPacket::MergeFrom(const ::google::protobuf::Message& from) {
  GOOGLE_CHECK_NE(&from, this);
  const SSL_WrapperPacket* source =
    ::google::protobuf::internal::dynamic_cast_if_available<const SSL_WrapperPacket*>(
      &from);
  if (source == NULL) {
    ::google::protobuf::internal::ReflectionOps::Merge(from, this);
  } else {
    MergeFrom(*source);
  }
}

void SSL_WrapperPacket::MergeFrom(const SSL_WrapperPacket& from) {
  GOOGLE_CHECK_NE(&from, this);
  if (from._has_bits_[0 / 32] & (0xffu << (0 % 32))) {
    if (from._has_bit(0)) {
      mutable_detection()->::SSL_DetectionFrame::MergeFrom(from.detection());
    }
    if (from._has_bit(1)) {
      mutable_geometry()->::SSL_GeometryData::MergeFrom(from.geometry());
    }
  }
  mutable_unknown_fields()->MergeFrom(from.unknown_fields());
}

void SSL_WrapperPacket::CopyFrom(const ::google::protobuf::Message& from) {
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

void SSL_WrapperPacket::CopyFrom(const SSL_WrapperPacket& from) {
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

bool SSL_WrapperPacket::IsInitialized() const {
  
  if (has_detection()) {
    if (!this->detection().IsInitialized()) return false;
  }
  if (has_geometry()) {
    if (!this->geometry().IsInitialized()) return false;
  }
  return true;
}

void SSL_WrapperPacket::Swap(SSL_WrapperPacket* other) {
  if (other != this) {
    std::swap(detection_, other->detection_);
    std::swap(geometry_, other->geometry_);
    std::swap(_has_bits_[0], other->_has_bits_[0]);
    _unknown_fields_.Swap(&other->_unknown_fields_);
    std::swap(_cached_size_, other->_cached_size_);
  }
}

::google::protobuf::Metadata SSL_WrapperPacket::GetMetadata() const {
  protobuf_AssignDescriptorsOnce();
  ::google::protobuf::Metadata metadata;
  metadata.descriptor = SSL_WrapperPacket_descriptor_;
  metadata.reflection = SSL_WrapperPacket_reflection_;
  return metadata;
}

