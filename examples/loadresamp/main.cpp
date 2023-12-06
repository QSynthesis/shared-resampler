#include <iostream>

#include <LoadExecutable.h>

#include <Windows.h>

/*!
      Returns the wide string converted from UTF-8 multi-byte string.
  */
std::string wideToUtf8(const std::wstring &utf16Str) {
    if (utf16Str.empty()) {
        return {};
    }
    const auto utf16Length = static_cast<int>(utf16Str.size());
    if (utf16Length >= (std::numeric_limits<int>::max)()) {
        return {};
    }
    const int utf8Length = ::WideCharToMultiByte(CP_UTF8, WC_ERR_INVALID_CHARS, utf16Str.data(),
                                                 utf16Length, nullptr, 0, nullptr, nullptr);
    if (utf8Length <= 0) {
        return {};
    }
    std::string utf8Str;
    utf8Str.resize(utf8Length);
    ::WideCharToMultiByte(CP_UTF8, WC_ERR_INVALID_CHARS, utf16Str.data(), utf16Length,
                          utf8Str.data(), utf8Length, nullptr, nullptr);
    return utf8Str;
}

/*!
    Returns the UTF-8 multi-byte string converted from wide string.
*/
std::wstring utf8ToWide(const std::string &utf8Str) {
    if (utf8Str.empty()) {
        return {};
    }
    const auto utf8Length = static_cast<int>(utf8Str.size());
    if (utf8Length >= (std::numeric_limits<int>::max)()) {
        return {};
    }
    const int utf16Length = ::MultiByteToWideChar(CP_UTF8, MB_ERR_INVALID_CHARS, utf8Str.data(),
                                                  utf8Length, nullptr, 0);
    if (utf16Length <= 0) {
        return {};
    }
    std::wstring utf16Str;
    utf16Str.resize(utf16Length);
    ::MultiByteToWideChar(CP_UTF8, MB_ERR_INVALID_CHARS, utf8Str.data(), utf8Length,
                          utf16Str.data(), utf16Length);
    return utf16Str;
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        std::cout << "Usage: loadresamp [exe]" << std::endl;
        return 0;
    }

    LoadExe::INIT_LoadExecutable(
        reinterpret_cast<void *>(::LoadLibraryA), reinterpret_cast<void *>(::GetCurrentProcess),
        reinterpret_cast<void *>(::VirtualProtect), reinterpret_cast<void *>(::GetProcAddress));

    auto hModule = LoadExe::LoadExecutable(argv[1]);
    if (!hModule) {
        std::cout << "Failed to load executable." << std::endl;
        return 0;
    }

    using Entry = int (*)(int, wchar_t **);

    auto hEntry = (Entry) LoadExe::GetEntryFunction(hModule);
    if (!hEntry) {
        std::cout << "Failed to get entry." << std::endl;
        return 0;
    }

    std::wstring exeName = utf8ToWide(argv[1]);
    wchar_t *data = exeName.data();
    int code = hEntry(1, &data);
    return code;
}
