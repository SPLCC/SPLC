#ifndef __SPLC_TRANSLATION_TRANSLATIONLOGHELPER_HH__
#define __SPLC_TRANSLATION_TRANSLATIONLOGHELPER_HH__ 1

//===-- splc/src/Translation/TranslationLogHelper.hh - Macros for Parser/Lexer --*- C++ -*-===//
//
// Part of the CS323-SPLC Project.
//
//===--------------------------------------------------------------------------------------===//
///
/// \file
/// This file contains macros that will be used in parser/lexer implementations which have
/// `translationManager` as their member variables for easy operation.
/// Method `translationManager.getTranslationUnit()` is required for this to work.
///
//===--------------------------------------------------------------------------------------===//

#include "Translation/TranslationBase.hh"

#include "Translation/TranslationLogger.hh"

namespace splc {

} // namespace splc

// *For non-pointers

#define SPLC_TRLH_EMPTY(loc) \
    splc::TranslationLogger{*translationManager.getTranslationUnit(), false, &loc, splc::utils::logging::Level::Empty}

#define SPLC_TRLH_DEBUG(loc) \
    splc::TranslationLogger{*translationManager.getTranslationUnit(), false, &loc, splc::utils::logging::Level::Debug}

#define SPLC_TRLH_INFO(loc) \
    splc::TranslationLogger{*translationManager.getTranslationUnit(), false, &loc, splc::utils::logging::Level::Info}

#define SPLC_TRLH_NOTE(loc) \
    splc::TranslationLogger{*translationManager.getTranslationUnit(), false, &loc, splc::utils::logging::Level::Note}

#define SPLC_TRLH_WARNING(loc) \
    splc::TranslationLogger{*translationManager.getTranslationUnit(), true, &loc, splc::utils::logging::Level::Warning}

#define SPLC_TRLH_ERROR(loc) \
    splc::TranslationLogger{*translationManager.getTranslationUnit(), true, &loc, splc::utils::logging::Level::Error}

#define SPLC_TRLH_SYNTAX_ERROR(loc) \
    splc::TranslationLogger{*translationManager.getTranslationUnit(), true, &loc, splc::utils::logging::Level::SyntaxError}

#define SPLC_TRLH_SEMANTIC_ERROR(loc) \
    splc::TranslationLogger{*translationManager.getTranslationUnit(), true, &loc, splc::utils::logging::Level::SemanticError}

#define SPLC_TRLH_FATAL_ERROR(loc) \
    splc::TranslationLogger{*translationManager.getTranslationUnit(), true, &loc, splc::utils::logging::Level::FatalError}

// *For pointers

#define SPLC_TRLHP_EMPTY(locPtr) \
    splc::TranslationLogger{*translationManager.getTranslationUnit(), false, locPtr, splc::utils::logging::Level::Empty}

#define SPLC_TRLHP_DEBUG(locPtr) \
    splc::TranslationLogger{*translationManager.getTranslationUnit(), false, locPtr, splc::utils::logging::Level::Debug}

#define SPLC_TRLHP_INFO(locPtr) \
    splc::TranslationLogger{*translationManager.getTranslationUnit(), false, locPtr, splc::utils::logging::Level::Info}

#define SPLC_TRLHP_NOTE(locPtr) \
    splc::TranslationLogger{*translationManager.getTranslationUnit(), false, locPtr, splc::utils::logging::Level::Note}

#define SPLC_TRLHP_WARNING(locPtr) \
    splc::TranslationLogger{*translationManager.getTranslationUnit(), true, locPtr, splc::utils::logging::Level::Warning}

#define SPLC_TRLHP_ERROR(locPtr) \
    splc::TranslationLogger{*translationManager.getTranslationUnit(), true, locPtr, splc::utils::logging::Level::Error}

#define SPLC_TRLHP_SYNTAX_ERROR(locPtr) \
    splc::TranslationLogger{*translationManager.getTranslationUnit(), true, locPtr, splc::utils::logging::Level::SyntaxError}

#define SPLC_TRLHP_SEMANTIC_ERROR(locPtr) \
    splc::TranslationLogger{*translationManager.getTranslationUnit(), true, locPtr, splc::utils::logging::Level::SemanticError}

#define SPLC_TRLHP_FATAL_ERROR(locPtr) \
    splc::TranslationLogger{*translationManager.getTranslationUnit(), true, locPtr, splc::utils::logging::Level::FatalError}

#endif // __SPLC_TRANSLATION_TRANSLATIONLOGHELPER_HH__