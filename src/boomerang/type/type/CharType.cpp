#pragma region License
/*
 * This file is part of the Boomerang Decompiler.
 *
 * See the file "LICENSE.TERMS" for information on usage and
 * redistribution of this file, and for a DISCLAIMER OF ALL
 * WARRANTIES.
 */
#pragma endregion License
#include "CharType.h"


#include "boomerang/type/type/ArrayType.h"
#include "boomerang/type/type/SizeType.h"


CharType::CharType()
    : Type(eChar)
{
}


CharType::~CharType()
{
}


SharedType CharType::clone() const
{
    return CharType::get();
}


size_t CharType::getSize() const
{
    return 8;
}


bool CharType::operator==(const Type& other) const
{
    return other.isChar();
}


bool CharType::operator<(const Type& other) const
{
    return id < other.getId();
}

QString CharType::getCtype(bool /*final*/) const
{
    return "char";
}


SharedType CharType::meetWith(SharedType other, bool& ch, bool bHighestPtr) const
{
    if (other->resolvesToVoid() || other->resolvesToChar()) {
        return ((CharType *)this)->shared_from_this();
    }

    // Also allow char to merge with integer
    if (other->resolvesToInteger()) {
        ch = true;
        return other->clone();
    }

    if (other->resolvesToSize() && (other->as<SizeType>()->getSize() == 8)) {
        return ((CharType *)this)->shared_from_this();
    }

    return createUnion(other, ch, bHighestPtr);
}


bool CharType::isCompatible(const Type& other, bool /*all*/) const
{
    if (other.resolvesToVoid()) {
        return true;
    }

    if (other.resolvesToChar()) {
        return true;
    }

    if (other.resolvesToInteger()) {
        return true;
    }

    if (other.resolvesToSize() && (((const SizeType&)other).getSize() == 8)) {
        return true;
    }

    if (other.resolvesToUnion()) {
        return other.isCompatibleWith(*this);
    }

    if (other.resolvesToArray()) {
        return isCompatibleWith(*((const ArrayType&)other).getBaseType());
    }

    return false;
}