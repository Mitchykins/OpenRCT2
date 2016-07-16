#pragma region Copyright (c) 2014-2016 OpenRCT2 Developers
/*****************************************************************************
 * OpenRCT2, an open source clone of Roller Coaster Tycoon 2.
 *
 * OpenRCT2 is the work of many authors, a full list can be found in contributors.md
 * For more information, visit https://github.com/OpenRCT2/OpenRCT2
 *
 * OpenRCT2 is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * A full copy of the GNU General Public License can be found in licence.txt
 *****************************************************************************/
#pragma endregion

#include <cassert>
#include <stdarg.h>
#include <stdio.h>
#include <windows.h>

#include "Console.hpp"
#include "Diagnostics.hpp"
#include "Guard.hpp"

extern "C"
{
    #include "../openrct2.h"
}

namespace Guard
{
    void Assert(bool expression, const char * message, ...)
    {
        va_list args;
        va_start(args, message);
        Assert_VA(expression, message, args);
        va_end(args);
    }

    void Assert_VA(bool expression, const char * message, va_list args)
    {
        if (expression) return;

        if (message != nullptr)
        {
            Console::Error::WriteLine("Assertion failed:");
            Console::Error::WriteLine_VA(message, args);
        }

#if DEBUG
        Debug::Break();
#endif
#if __WINDOWS__
        char version[128];
        openrct2_write_full_version_info(version, sizeof(version));

        char buffer[512];
        strcpy(buffer, "An assertion failed, please report this to the OpenRCT2 developers.\r\n\r\nVersion: ");
        strcat(buffer, version);
        strcat(buffer, "\r\n");
        vsprintf((char *)strchr(buffer, 0), message, args);
        int result = MessageBox(nullptr, buffer, OPENRCT2_NAME, MB_ABORTRETRYIGNORE | MB_ICONEXCLAMATION);
        if (result == IDABORT)
        {
            // Force a crash that breakpad will handle allowing us to get a dump
            *((void**)0) = 0;
        }
#else
        assert(false);
#endif
    }

    void Fail(const char * message, ...)
    {
        va_list args;
        va_start(args, message);
        Fail_VA(message, args);
        va_end(args);
    }

    void Fail_VA(const char * message, va_list args)
    {
        if (message != nullptr)
        {
            Console::Error::WriteLine_VA(message, args);
        }

#if DEBUG
        Debug::Break();
#endif
        assert(false);
    }
}
