/*
 * Copyright (C) 2024-2026, Kazankov Nikolay
 * <nik.kazankov.05@mail.ru>
 */

#pragma once

#include <string>
#include <array>
#include "logger.hpp"
#include "../languagesNames.hpp"


// Class, storing one text variant in different locations
class LanguagedText {
 private:
    // Text in different languages
    const std::array<const std::string, (unsigned)Language::Count> textVariants;
    // Currently selected language for whole app
    static Language currentLanguage;

 public:
    // englishVariant, russianVariant, germanVariant, bellarussianVariant
    template <typename ...Vars>
    LanguagedText(const Vars... variants);
    LanguagedText(const std::string singleVariant);
    LanguagedText(const LanguagedText& copyText) noexcept;
    LanguagedText(LanguagedText&& movedText) noexcept;
    const std::string& getString() const;
    // Options for change current language
    // Return true, if need to change language
    static bool setLanguage(Language newLanguage);
    static Language getLanguage();
};

template <typename ...Vars>
LanguagedText::LanguagedText(const Vars... _variants)
: textVariants{_variants...} {}
