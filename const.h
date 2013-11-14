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
static const QString FILE_EXTENSION_STUDENT = "psls";
static const QString FILE_EXTENSION_TEACHER = "pslt";
static const QString FILE_EXTENSION_POSTGRADUATE = "pslp";
static const QString FILE_EXTENSION_TA = "pslta";

// table headings map, must be consecutive from zero
enum class HDG{
    ID = 0,
    NAME = 1,
    SEX = 2,
    BIRTHDAY = 3,
    CLASSNO =4,
    IDNUMBER = 5,
    MAJOR = 6,
    TUTORID = 7,
    DEPT = 8,
    POSITION = 9,
    COUNT = 10  // COUNT should be the number of elements in the enum, except COUNT
};
}
