#pragma once

#include <boost/typeof/decltype.hpp>

#define THIS (*this)
#define super_type __super

#define CLASS_VIRTUAL_DESTRUCTOR(CLASS) \
virtual ~CLASS() {}

#define CLASS_MAKE_STATIC(CLASS, ...) \
CLASS() = delete; \
~CLASS() = delete; \
CLASS ( CLASS __VA_ARGS__ & ) = delete; \
CLASS & operator=( const CLASS __VA_ARGS__ & ) = delete; \
CLASS ( CLASS __VA_ARGS__ && ) = delete; \
CLASS & operator=( CLASS __VA_ARGS__ && ) = delete

#define CLASS_DEFAULT_COPY(CLASS, ...) \
CLASS ( CLASS __VA_ARGS__ & ) = default; \
CLASS & operator=( const CLASS __VA_ARGS__ & ) = default

#define CLASS_DEFAULT_MOVE(CLASS, ...) \
CLASS ( CLASS __VA_ARGS__ && ) = default; \
CLASS & operator=( CLASS __VA_ARGS__ && ) = default

#define CLASS_DEFAULT_COPY_MOVE(CLASS, ...) \
CLASS_DEFAULT_COPY(CLASS, __VA_ARGS__); \
CLASS_DEFAULT_MOVE(CLASS, __VA_ARGS__)