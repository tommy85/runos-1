#pragma once

#include "Common.hh"

/**
 * Do OXM match procedure on one field.
 * @param tlv - OXM match structure.
 * @param value - Holds value to match.
 * @returns true if value->value() matches tlv->value() and tlv->mask()
 */
bool oxm_match(const of13::OXMTLV* tlv, const of13::OXMTLV* value);

/** Bitwise and */
EthAddress operator&(const EthAddress& a, const EthAddress& b);

/** Bitwise and */
IPAddress operator&(const IPAddress& a, const IPAddress& b);

/// Implementation details
template<class oxm_type, bool has_mask = true>
struct oxm_match_impl {
    static bool match(const oxm_type& op, const decltype(oxm_type().value())& value)
    {
        if (op.has_mask())
            return value == (op.oxm_type::value() & op.oxm_type::mask());
        else
            return value == op.oxm_type::value();
    }
};

template<class oxm_type>
struct oxm_match_impl<oxm_type, false> {
    static bool match(const oxm_type& op, const decltype(oxm_type().value())& value)
    {
        return value == op.oxm_type::value();
    }
};

// Some magic that checks if oxm_type::mask() exists
template<class oxm_type>
constexpr auto oxm_type_has_mask(oxm_type*) -> decltype(oxm_type().mask(), bool())
{ return true; }

constexpr bool oxm_type_has_mask(...) { return false; }
///

/**
 * Templated version of oxm_match that avoids dynamic field comparasion and
 * indirect virtual method calls.
 */
template<class oxm_type, bool has_mask = oxm_type_has_mask((oxm_type*)nullptr)>
bool oxm_match(const oxm_type& op, const decltype(oxm_type().value())& value)
{
    return oxm_match_impl<oxm_type, has_mask>::match(op, value);
}

