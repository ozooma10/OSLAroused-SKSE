#pragma once

#define _INC_WINDOWS
#include <SKSE/Impl/PCH.h>
#include <RE/Skyrim.h>
#include <SKSE/SKSE.h>
#include <REL/Relocation.h>
#undef _INC_WINDOWS

#include <ShlObj_core.h>
#include <Windows.h>
#include <Psapi.h>
#include <stdio.h>
#include <stdexcept>

#include <format>
#include <gtest/gtest.h>

class ExampleTestBase : public ::testing::Test {
protected:

   void SetUp() override {
       // ...
   }

   void TearDown() override {
       // ...
   }
};
