#pragma once

#include <QtGlobal>
#include <cstdint>

namespace CONST{
static const u_int32_t MAGIC_NUMBER = 0x33220F0E;

static const u_int32_t FILE_TYPE_STUDENT = 0x1;
static const u_int32_t FILE_TYPE_TEACHER = 0x2;
static const u_int32_t FILE_TYPE_TEACHING_ASSISTANT = 0x3;
static const u_int32_t FILE_TYPE_TUTOR = 0x4;
static const u_int32_t FILE_TYPE_POSTGRADUATE = 0x5;

static const u_int32_t VERSION_1_20131109 = 0x1;
}
