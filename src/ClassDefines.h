#pragma once

#define THIS (*this)
#define super_type __super

#define VIRTUAL_DESTRUCTOR(CLASS) \
virtual ~CLASS() {}

#define CLASS_MAKE_STATIC(CLASS, ...) \
CLASS() = delete; \
~CLASS() = delete; \
CLASS ( CLASS __VA_ARGS__ & ) = delete; \
CLASS & operator=( const CLASS __VA_ARGS__ & ) = delete; \
CLASS ( CLASS __VA_ARGS__ && ) = delete; \
CLASS & operator=( CLASS __VA_ARGS__ && ) = delete