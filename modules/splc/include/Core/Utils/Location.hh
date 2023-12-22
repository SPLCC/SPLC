#ifndef __SPLC_CORE_UTILS_LOCATION_HH__
#define __SPLC_CORE_UTILS_LOCATION_HH__ 1

// A Bison parser, made by GNU Bison 3.8.2.

// Locations for Bison parsers in C++

// Copyright (C) 2002-2015, 2018-2021 Free Software Foundation, Inc.

// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.

// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.

// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <https://www.gnu.org/licenses/>.

// As a special exception, you may create a larger work that contains
// part or all of the Bison parser skeleton and distribute that work
// under terms of your choice, so long as that work isn't itself a
// parser generator using the skeleton or a modified version thereof
// as a parser skeleton.  Alternatively, if you modify or redistribute
// the parser skeleton itself, you may (at your option) remove this
// special exception, which will cause the skeleton and the resulting
// Bison output files to be licensed under the GNU General Public
// License without this special exception.

// This special exception was added by the Free Software Foundation in
// version 2.2 of Bison.

#include <iostream>
#include <string>

#include <Core/Base.hh>

namespace splc::utils {

class Position;
class Location;

/// A point in a source file.
class Position {
  public:
    /// Type for file name.
    typedef const std::string ContextNameType;
    /// Type for ContextID
    typedef int ContextIDType;
    /// Type for line and column numbers.
    typedef int CounterType;

    using ContextKeyType = std::tuple<ContextNameType *, ContextIDType, const Location *>;

    /// Invalid context ID
    static const ContextIDType invalidContextID = -1;

    /// Construct a position.
    explicit Position(ContextNameType *f = nullptr,
                      ContextIDType cid = invalidContextID, CounterType l = 1,
                      CounterType c = 1)
        : contextName(f), contextID(cid), line(l), column(c)
    {
    }

    /// Initialization.
    void initialize(ContextNameType *fn = nullptr,
                    ContextIDType cid = invalidContextID, CounterType l = 1,
                    CounterType c = 1)
    {
        contextName = fn;
        contextID = cid;
        line = l;
        column = c;
    }

    /// Assign context information.
    void switchToContext(ContextKeyType keyType)
    {
        contextName = std::get<0>(keyType);
        contextID = std::get<1>(keyType);
    }

    /// Convert to bool to check if this position is a valid one.
    /// \return true If this position is valid.
    operator bool() const
    {
        return contextName != nullptr && contextID != invalidContextID;
    }

    /** \name Line and Column related manipulators
     ** \{ */
    /// (line related) Advance to the COUNT next lines.
    void lines(CounterType count = 1)
    {
        if (count) {
            column = 1;
            line = add_(line, count, 1);
        }
    }

    /// (column related) Advance to the COUNT next columns.
    void columns(CounterType count = 1) { column = add_(column, count, 1); }
    /** \} */

    /// File name to which this position refers.
    ContextNameType *contextName;
    /// Context ID
    ContextIDType contextID;
    /// Current line number.
    CounterType line;
    /// Current column number.
    CounterType column;

  private:
    /// Compute max (min, lhs+rhs).
    static CounterType add_(CounterType lhs, CounterType rhs, CounterType min)
    {
        return lhs + rhs < min ? min : lhs + rhs;
    }
};

/// \brief Represents the key of context, namely `ContextName` and `ContextID`.

/// Add \a width columns, in place.
inline Position &operator+=(Position &res, Position::CounterType width)
{
    res.columns(width);
    return res;
}

/// Add \a width columns.
inline Position operator+(Position res, Position::CounterType width)
{
    return res += width;
}

/// Subtract \a width columns, in place.
inline Position &operator-=(Position &res, Position::CounterType width)
{
    return res += -width;
}

/// Subtract \a width columns.
inline Position operator-(Position res, Position::CounterType width)
{
    return res -= width;
}

/** \brief Intercept output stream redirection.
 ** \param ostr the destination output stream
 ** \param pos a reference to the position to redirect
 */
inline std::ostream &operator<<(std::ostream &ostr, const Position &pos)
{
    if (pos.contextName)
        ostr << *pos.contextName << ':';
    return ostr << pos.line << '.' << pos.column;
}

/// Two points in a source file.
class Location {
  public:
    /// Type for file name.
    typedef Position::ContextNameType ContextNameType;
    /// Type for ContextID
    typedef Position::ContextIDType ContextIDType;
    /// Type for line and column numbers.
    typedef Position::CounterType CounterType;
    /// `ContextKeyType`, consisting of `ContextNameType` and `ContextIDType`.
    /// Used for quick assigning of contexts
    using ContextKeyType = Position::ContextKeyType;

    static const ContextIDType invalidContextID = Position::invalidContextID;

    /// Construct a location from \a b to \a e.
    Location(const Position &b, const Position &e) : begin(b), end(e), parent(nullptr)  {}

    /// Construct a 0-width location in \a p.
    explicit Location(const Position &p = Position()) : begin(p), end(p), parent(nullptr) {}

    /// Construct a 0-width location in \a f, \a l, \a c.
    explicit Location(ContextNameType *f, ContextIDType cid = invalidContextID,
                      CounterType l = 1, CounterType c = 1, const Location *parent_ = nullptr)
        : begin(f, cid, l, c), end(f, cid, l, c), parent(parent_)
    {
    }

    /// Initialization.
    void initialize(ContextNameType *f = nullptr,
                    ContextIDType cid = invalidContextID, CounterType l = 1,
                    CounterType c = 1, const Location *parent_ = nullptr)
    {
        begin.initialize(f, cid, l, c);
        end = begin;
        parent = parent_;
    }

    /// Assign context information.
    void switchToContext(ContextKeyType keyType)
    {
        begin.switchToContext(keyType);
        end.switchToContext(keyType);
    }

    void setParent(const Location *parent_)
    {
        parent = parent_;
    }

    /// Convert to bool to check if this location is a valid one.
    /// \return true If this location is valid.
    operator bool() const { return begin && end; }

    /** \name Line and Column related manipulators
     ** \{ */
  public:
    /// Reset initial location to final location.
    void step() { begin = end; }

    /// Extend the current location to the COUNT next columns.
    void columns(CounterType count = 1) { end += count; }

    /// Extend the current location to the COUNT next lines.
    void lines(CounterType count = 1) { end.lines(count); }
    /** \} */

  public:
    /// Beginning of the located region.
    Position begin;
    /// End of the located region.
    Position end;
    const Location *parent;
};

/// Join two locations, in place.
inline Location &operator+=(Location &res, const Location &end)
{
    res.end = end.end;
    return res;
}

/// Join two locations.
inline Location operator+(Location res, const Location &end)
{
    return res += end;
}

/// Add \a width columns to the end position, in place.
inline Location &operator+=(Location &res, Location::CounterType width)
{
    res.columns(width);
    return res;
}

/// Add \a width columns to the end position.
inline Location operator+(Location res, Location::CounterType width)
{
    return res += width;
}

/// Subtract \a width columns to the end position, in place.
inline Location &operator-=(Location &res, Location::CounterType width)
{
    return res += -width;
}

/// Subtract \a width columns to the end position.
inline Location operator-(Location res, Location::CounterType width)
{
    return res -= width;
}

/** \brief Intercept output stream redirection.
 ** \param ostr the destination output stream
 ** \param loc a reference to the location to redirect
 **
 ** Avoid duplicate information.
 */
inline std::ostream &operator<<(std::ostream &ostr, const Location &loc)
{
    // Location::CounterType end_col = 0 < loc.end.column ? loc.end.column - 1 :
    // 0; // Exact location, deprecate
    Location::CounterType end_col = loc.end.column;
    if (loc.end.contextName) {
        if (!loc.begin.contextName ||
            loc.begin.contextID == loc.end.contextID) {
            ostr << loc.begin;
            ostr << '-' << loc.end.line << '.' << end_col;
        }
        else {
            // loc.begin.contextID != loc.end.contextID
            ostr << *loc.begin.contextName << " [" << loc.begin.contextID << "]"
                 << ":" << loc.begin.line << "." << loc.begin.column;

            ostr << '-' << *loc.end.contextName << " [" << loc.end.contextID
                 << "]" << ':' << loc.end.line << '.' << end_col;
        }
    }
    else {
        ostr << loc.begin;
        if (loc.begin.line < loc.end.line)
            ostr << '-' << loc.end.line << '.' << end_col;
        else if (loc.begin.column < end_col)
            ostr << '-' << end_col;
    }
    return ostr;
}

} // namespace splc::utils

#endif // __SPLC_CORE_UTILS_LOCATION_HH__