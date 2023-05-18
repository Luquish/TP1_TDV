// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: ortools/constraint_solver/search_limit.proto

#ifndef GOOGLE_PROTOBUF_INCLUDED_ortools_2fconstraint_5fsolver_2fsearch_5flimit_2eproto
#define GOOGLE_PROTOBUF_INCLUDED_ortools_2fconstraint_5fsolver_2fsearch_5flimit_2eproto

#include <limits>
#include <string>

#include <google/protobuf/port_def.inc>
#if PROTOBUF_VERSION < 3021000
#error This file was generated by a newer version of protoc which is
#error incompatible with your Protocol Buffer headers. Please update
#error your headers.
#endif
#if 3021012 < PROTOBUF_MIN_PROTOC_VERSION
#error This file was generated by an older version of protoc which is
#error incompatible with your Protocol Buffer headers. Please
#error regenerate this file with a newer version of protoc.
#endif

#include <google/protobuf/port_undef.inc>
#include <google/protobuf/io/coded_stream.h>
#include <google/protobuf/arena.h>
#include <google/protobuf/arenastring.h>
#include <google/protobuf/generated_message_util.h>
#include <google/protobuf/metadata_lite.h>
#include <google/protobuf/generated_message_reflection.h>
#include <google/protobuf/message.h>
#include <google/protobuf/repeated_field.h>  // IWYU pragma: export
#include <google/protobuf/extension_set.h>  // IWYU pragma: export
#include <google/protobuf/unknown_field_set.h>
// @@protoc_insertion_point(includes)
#include <google/protobuf/port_def.inc>
#define PROTOBUF_INTERNAL_EXPORT_ortools_2fconstraint_5fsolver_2fsearch_5flimit_2eproto
PROTOBUF_NAMESPACE_OPEN
namespace internal {
class AnyMetadata;
}  // namespace internal
PROTOBUF_NAMESPACE_CLOSE

// Internal implementation detail -- do not use these members.
struct TableStruct_ortools_2fconstraint_5fsolver_2fsearch_5flimit_2eproto {
  static const uint32_t offsets[];
};
extern const ::PROTOBUF_NAMESPACE_ID::internal::DescriptorTable descriptor_table_ortools_2fconstraint_5fsolver_2fsearch_5flimit_2eproto;
namespace operations_research {
class RegularLimitParameters;
struct RegularLimitParametersDefaultTypeInternal;
extern RegularLimitParametersDefaultTypeInternal _RegularLimitParameters_default_instance_;
}  // namespace operations_research
PROTOBUF_NAMESPACE_OPEN
template<> ::operations_research::RegularLimitParameters* Arena::CreateMaybeMessage<::operations_research::RegularLimitParameters>(Arena*);
PROTOBUF_NAMESPACE_CLOSE
namespace operations_research {

// ===================================================================

class RegularLimitParameters final :
    public ::PROTOBUF_NAMESPACE_ID::Message /* @@protoc_insertion_point(class_definition:operations_research.RegularLimitParameters) */ {
 public:
  inline RegularLimitParameters() : RegularLimitParameters(nullptr) {}
  ~RegularLimitParameters() override;
  explicit PROTOBUF_CONSTEXPR RegularLimitParameters(::PROTOBUF_NAMESPACE_ID::internal::ConstantInitialized);

  RegularLimitParameters(const RegularLimitParameters& from);
  RegularLimitParameters(RegularLimitParameters&& from) noexcept
    : RegularLimitParameters() {
    *this = ::std::move(from);
  }

  inline RegularLimitParameters& operator=(const RegularLimitParameters& from) {
    CopyFrom(from);
    return *this;
  }
  inline RegularLimitParameters& operator=(RegularLimitParameters&& from) noexcept {
    if (this == &from) return *this;
    if (GetOwningArena() == from.GetOwningArena()
  #ifdef PROTOBUF_FORCE_COPY_IN_MOVE
        && GetOwningArena() != nullptr
  #endif  // !PROTOBUF_FORCE_COPY_IN_MOVE
    ) {
      InternalSwap(&from);
    } else {
      CopyFrom(from);
    }
    return *this;
  }

  static const ::PROTOBUF_NAMESPACE_ID::Descriptor* descriptor() {
    return GetDescriptor();
  }
  static const ::PROTOBUF_NAMESPACE_ID::Descriptor* GetDescriptor() {
    return default_instance().GetMetadata().descriptor;
  }
  static const ::PROTOBUF_NAMESPACE_ID::Reflection* GetReflection() {
    return default_instance().GetMetadata().reflection;
  }
  static const RegularLimitParameters& default_instance() {
    return *internal_default_instance();
  }
  static inline const RegularLimitParameters* internal_default_instance() {
    return reinterpret_cast<const RegularLimitParameters*>(
               &_RegularLimitParameters_default_instance_);
  }
  static constexpr int kIndexInFileMessages =
    0;

  friend void swap(RegularLimitParameters& a, RegularLimitParameters& b) {
    a.Swap(&b);
  }
  inline void Swap(RegularLimitParameters* other) {
    if (other == this) return;
  #ifdef PROTOBUF_FORCE_COPY_IN_SWAP
    if (GetOwningArena() != nullptr &&
        GetOwningArena() == other->GetOwningArena()) {
   #else  // PROTOBUF_FORCE_COPY_IN_SWAP
    if (GetOwningArena() == other->GetOwningArena()) {
  #endif  // !PROTOBUF_FORCE_COPY_IN_SWAP
      InternalSwap(other);
    } else {
      ::PROTOBUF_NAMESPACE_ID::internal::GenericSwap(this, other);
    }
  }
  void UnsafeArenaSwap(RegularLimitParameters* other) {
    if (other == this) return;
    GOOGLE_DCHECK(GetOwningArena() == other->GetOwningArena());
    InternalSwap(other);
  }

  // implements Message ----------------------------------------------

  RegularLimitParameters* New(::PROTOBUF_NAMESPACE_ID::Arena* arena = nullptr) const final {
    return CreateMaybeMessage<RegularLimitParameters>(arena);
  }
  using ::PROTOBUF_NAMESPACE_ID::Message::CopyFrom;
  void CopyFrom(const RegularLimitParameters& from);
  using ::PROTOBUF_NAMESPACE_ID::Message::MergeFrom;
  void MergeFrom( const RegularLimitParameters& from) {
    RegularLimitParameters::MergeImpl(*this, from);
  }
  private:
  static void MergeImpl(::PROTOBUF_NAMESPACE_ID::Message& to_msg, const ::PROTOBUF_NAMESPACE_ID::Message& from_msg);
  public:
  PROTOBUF_ATTRIBUTE_REINITIALIZES void Clear() final;
  bool IsInitialized() const final;

  size_t ByteSizeLong() const final;
  const char* _InternalParse(const char* ptr, ::PROTOBUF_NAMESPACE_ID::internal::ParseContext* ctx) final;
  uint8_t* _InternalSerialize(
      uint8_t* target, ::PROTOBUF_NAMESPACE_ID::io::EpsCopyOutputStream* stream) const final;
  int GetCachedSize() const final { return _impl_._cached_size_.Get(); }

  private:
  void SharedCtor(::PROTOBUF_NAMESPACE_ID::Arena* arena, bool is_message_owned);
  void SharedDtor();
  void SetCachedSize(int size) const final;
  void InternalSwap(RegularLimitParameters* other);

  private:
  friend class ::PROTOBUF_NAMESPACE_ID::internal::AnyMetadata;
  static ::PROTOBUF_NAMESPACE_ID::StringPiece FullMessageName() {
    return "operations_research.RegularLimitParameters";
  }
  protected:
  explicit RegularLimitParameters(::PROTOBUF_NAMESPACE_ID::Arena* arena,
                       bool is_message_owned = false);
  public:

  static const ClassData _class_data_;
  const ::PROTOBUF_NAMESPACE_ID::Message::ClassData*GetClassData() const final;

  ::PROTOBUF_NAMESPACE_ID::Metadata GetMetadata() const final;

  // nested types ----------------------------------------------------

  // accessors -------------------------------------------------------

  enum : int {
    kTimeFieldNumber = 1,
    kBranchesFieldNumber = 2,
    kFailuresFieldNumber = 3,
    kSolutionsFieldNumber = 4,
    kSmartTimeCheckFieldNumber = 5,
    kCumulativeFieldNumber = 6,
  };
  // int64 time = 1;
  void clear_time();
  int64_t time() const;
  void set_time(int64_t value);
  private:
  int64_t _internal_time() const;
  void _internal_set_time(int64_t value);
  public:

  // int64 branches = 2;
  void clear_branches();
  int64_t branches() const;
  void set_branches(int64_t value);
  private:
  int64_t _internal_branches() const;
  void _internal_set_branches(int64_t value);
  public:

  // int64 failures = 3;
  void clear_failures();
  int64_t failures() const;
  void set_failures(int64_t value);
  private:
  int64_t _internal_failures() const;
  void _internal_set_failures(int64_t value);
  public:

  // int64 solutions = 4;
  void clear_solutions();
  int64_t solutions() const;
  void set_solutions(int64_t value);
  private:
  int64_t _internal_solutions() const;
  void _internal_set_solutions(int64_t value);
  public:

  // bool smart_time_check = 5;
  void clear_smart_time_check();
  bool smart_time_check() const;
  void set_smart_time_check(bool value);
  private:
  bool _internal_smart_time_check() const;
  void _internal_set_smart_time_check(bool value);
  public:

  // bool cumulative = 6;
  void clear_cumulative();
  bool cumulative() const;
  void set_cumulative(bool value);
  private:
  bool _internal_cumulative() const;
  void _internal_set_cumulative(bool value);
  public:

  // @@protoc_insertion_point(class_scope:operations_research.RegularLimitParameters)
 private:
  class _Internal;

  template <typename T> friend class ::PROTOBUF_NAMESPACE_ID::Arena::InternalHelper;
  typedef void InternalArenaConstructable_;
  typedef void DestructorSkippable_;
  struct Impl_ {
    int64_t time_;
    int64_t branches_;
    int64_t failures_;
    int64_t solutions_;
    bool smart_time_check_;
    bool cumulative_;
    mutable ::PROTOBUF_NAMESPACE_ID::internal::CachedSize _cached_size_;
  };
  union { Impl_ _impl_; };
  friend struct ::TableStruct_ortools_2fconstraint_5fsolver_2fsearch_5flimit_2eproto;
};
// ===================================================================


// ===================================================================

#ifdef __GNUC__
  #pragma GCC diagnostic push
  #pragma GCC diagnostic ignored "-Wstrict-aliasing"
#endif  // __GNUC__
// RegularLimitParameters

// int64 time = 1;
inline void RegularLimitParameters::clear_time() {
  _impl_.time_ = int64_t{0};
}
inline int64_t RegularLimitParameters::_internal_time() const {
  return _impl_.time_;
}
inline int64_t RegularLimitParameters::time() const {
  // @@protoc_insertion_point(field_get:operations_research.RegularLimitParameters.time)
  return _internal_time();
}
inline void RegularLimitParameters::_internal_set_time(int64_t value) {
  
  _impl_.time_ = value;
}
inline void RegularLimitParameters::set_time(int64_t value) {
  _internal_set_time(value);
  // @@protoc_insertion_point(field_set:operations_research.RegularLimitParameters.time)
}

// int64 branches = 2;
inline void RegularLimitParameters::clear_branches() {
  _impl_.branches_ = int64_t{0};
}
inline int64_t RegularLimitParameters::_internal_branches() const {
  return _impl_.branches_;
}
inline int64_t RegularLimitParameters::branches() const {
  // @@protoc_insertion_point(field_get:operations_research.RegularLimitParameters.branches)
  return _internal_branches();
}
inline void RegularLimitParameters::_internal_set_branches(int64_t value) {
  
  _impl_.branches_ = value;
}
inline void RegularLimitParameters::set_branches(int64_t value) {
  _internal_set_branches(value);
  // @@protoc_insertion_point(field_set:operations_research.RegularLimitParameters.branches)
}

// int64 failures = 3;
inline void RegularLimitParameters::clear_failures() {
  _impl_.failures_ = int64_t{0};
}
inline int64_t RegularLimitParameters::_internal_failures() const {
  return _impl_.failures_;
}
inline int64_t RegularLimitParameters::failures() const {
  // @@protoc_insertion_point(field_get:operations_research.RegularLimitParameters.failures)
  return _internal_failures();
}
inline void RegularLimitParameters::_internal_set_failures(int64_t value) {
  
  _impl_.failures_ = value;
}
inline void RegularLimitParameters::set_failures(int64_t value) {
  _internal_set_failures(value);
  // @@protoc_insertion_point(field_set:operations_research.RegularLimitParameters.failures)
}

// int64 solutions = 4;
inline void RegularLimitParameters::clear_solutions() {
  _impl_.solutions_ = int64_t{0};
}
inline int64_t RegularLimitParameters::_internal_solutions() const {
  return _impl_.solutions_;
}
inline int64_t RegularLimitParameters::solutions() const {
  // @@protoc_insertion_point(field_get:operations_research.RegularLimitParameters.solutions)
  return _internal_solutions();
}
inline void RegularLimitParameters::_internal_set_solutions(int64_t value) {
  
  _impl_.solutions_ = value;
}
inline void RegularLimitParameters::set_solutions(int64_t value) {
  _internal_set_solutions(value);
  // @@protoc_insertion_point(field_set:operations_research.RegularLimitParameters.solutions)
}

// bool smart_time_check = 5;
inline void RegularLimitParameters::clear_smart_time_check() {
  _impl_.smart_time_check_ = false;
}
inline bool RegularLimitParameters::_internal_smart_time_check() const {
  return _impl_.smart_time_check_;
}
inline bool RegularLimitParameters::smart_time_check() const {
  // @@protoc_insertion_point(field_get:operations_research.RegularLimitParameters.smart_time_check)
  return _internal_smart_time_check();
}
inline void RegularLimitParameters::_internal_set_smart_time_check(bool value) {
  
  _impl_.smart_time_check_ = value;
}
inline void RegularLimitParameters::set_smart_time_check(bool value) {
  _internal_set_smart_time_check(value);
  // @@protoc_insertion_point(field_set:operations_research.RegularLimitParameters.smart_time_check)
}

// bool cumulative = 6;
inline void RegularLimitParameters::clear_cumulative() {
  _impl_.cumulative_ = false;
}
inline bool RegularLimitParameters::_internal_cumulative() const {
  return _impl_.cumulative_;
}
inline bool RegularLimitParameters::cumulative() const {
  // @@protoc_insertion_point(field_get:operations_research.RegularLimitParameters.cumulative)
  return _internal_cumulative();
}
inline void RegularLimitParameters::_internal_set_cumulative(bool value) {
  
  _impl_.cumulative_ = value;
}
inline void RegularLimitParameters::set_cumulative(bool value) {
  _internal_set_cumulative(value);
  // @@protoc_insertion_point(field_set:operations_research.RegularLimitParameters.cumulative)
}

#ifdef __GNUC__
  #pragma GCC diagnostic pop
#endif  // __GNUC__

// @@protoc_insertion_point(namespace_scope)

}  // namespace operations_research

// @@protoc_insertion_point(global_scope)

#include <google/protobuf/port_undef.inc>
#endif  // GOOGLE_PROTOBUF_INCLUDED_GOOGLE_PROTOBUF_INCLUDED_ortools_2fconstraint_5fsolver_2fsearch_5flimit_2eproto
