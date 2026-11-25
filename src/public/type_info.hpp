#pragma once

#include "atomic.hpp"
#include "config.hpp"
#include "cstdint.hpp"
#include "string_view.hpp"
#include "type_traits.hpp"
#include "utility.hpp"

namespace bt::types {
namespace internal {

    struct TypeIndex final {
        [[nodiscard]] static ID next() noexcept {
            static Atomic<uint64_t> value {};
            return value.fetch_add(1, MemOrd::memory_order_seq_cst);
        }
    };

    template<typename Type>
    [[nodiscard]] constexpr const char* pretty_function() noexcept {
#if defined BT_PRETTY_FUNCTION
        return static_cast<const char*>(BT_PRETTY_FUNCTION);
#else
        return "";
#endif
    }

    template<typename Type>
    [[nodiscard]] constexpr auto stripped_type_name() noexcept {
#if defined BT_PRETTY_FUNCTION
        const StringView full_name {pretty_function<Type>()};

        size_t first = full_name.find_first_not_of(
            ' ',
            full_name.find_first_of(BT_PRETTY_FUNCTION_PREFIX) + 1
        );
        StringView value = full_name.substr(
            first,
            full_name.find_last_of(BT_PRETTY_FUNCTION_SUFFIX) - first
        );

        return value;
#else
        return StringView {};
#endif
    }

    // TODO: consteval specialization
    template<
        typename Type,
        auto = stripped_type_name<Type>().find_first_of('_')>
    [[nodiscard]] consteval auto type_name(int) noexcept {
        constexpr auto value = stripped_type_name<Type>();
        return value;
    }

    // run-time fallback
    template<typename Type>
    [[nodiscard]] auto type_name(char) noexcept -> StringView {
        constexpr StringView value = stripped_type_name<Type>();
        return value;
    }

    // TODO: consteval specialization of type_hash (requires constexpr hash function)

    // run-time fallback
    template<typename Type>
    [[nodiscard]] auto type_hash(char) noexcept -> ID {
        constexpr auto stripped = stripped_type_name<Type>();
        auto value = Hash<StringView> {}(stripped);
        return value;
    }

} // namespace internal

/**
* @brief A unique identifier or index for a given type
* @tparam Type Type for which to get the unique identifier
*/
template<typename Type>
struct TypeIndex final {
    /**
    * @brief Returns a unique id for the given type
    * @return Sequentially unique identifier for the given type
    */
    [[nodiscard]] static auto value() noexcept -> ID {
        static const ID value = internal::TypeIndex::next();
        return value;
    }

    /*! @copydoc value */
    [[nodiscard]] constexpr operator ID() const noexcept {
        return value();
    }
};

/**
* @brief Generates a hash from the stripped name of a Type (if a pretty function is present), otherwise generates a TypeIndex
* @tparam Type Type for which to get the hash
*/
template<typename Type>
struct TypeHash final {
    /**
    * @brief Returns a unique id based on the name of the given type
    * @return ID
    */
    [[nodiscard]] static auto value() noexcept -> ID {
#if defined BT_PRETTY_FUNCTION
        return internal::type_hash<Type>(0);
#else
        return TypeIndex<Type>::value();
#endif
    }

    /*! @copydoc value */
    [[nodiscard]] constexpr operator ID() const noexcept {
        return value();
    }
};

/**
* @brief Generates a name for a given type
* @tparam Type Type for which to get the name
*/
template<typename Type>
struct TypeName final {
    /**
    * @brief Returns the string repersentation of name of the given type
    * @return StringView
    */
    [[nodiscard]] static auto value() noexcept -> StringView {
        return internal::type_name<Type>(0);
    }

    /*! @copydoc value */
    [[nodiscard]] constexpr operator StringView() const noexcept {
        return value();
    }
};

/*! @brief Implementation specific information about a type. */
struct TypeInfo final {
    template<typename Type>
    // NOLINTBEGIN(modernize-use-transparent-functors)
    constexpr TypeInfo(in_place_type_t<Type>) noexcept :
        m_Sequence {TypeIndex<remove_cvref_t<Type>>::value()},
        m_Identifier {TypeHash<remove_cvref_t<Type>>::value()},
        m_Alias {TypeName<remove_cvref_t<Type>>::value()} {}

    // NOLINTEND(modernize-use-transparent-functors)

    [[nodiscard]] constexpr auto index() const noexcept -> ID {
        return m_Sequence;
    }

    [[nodiscard]] constexpr auto hash() const noexcept -> ID {
        return m_Identifier;
    }

    [[nodiscard]] constexpr auto name() const noexcept -> StringView {
        return m_Alias;
    }

    [[nodiscard]] constexpr auto
    operator==(const TypeInfo& other) const noexcept {
        return m_Identifier == other.m_Identifier;
    }

    [[nodiscard]] constexpr auto
    operator!=(const TypeInfo& other) const noexcept {
        return m_Identifier != other.m_Identifier;
    }

    [[nodiscard]] constexpr auto
    operator<=>(const TypeInfo& other) const noexcept {
        return m_Sequence <=> other.m_Sequence;
    }

  private:
    ID m_Sequence;
    ID m_Identifier;
    StringView m_Alias;
};

template<typename Type>
[[nodiscard]] auto type_id() noexcept -> const TypeInfo& {
    if constexpr (is_same_v<Type, remove_cvref_t<Type>>) {
        static const TypeInfo instance {in_place_type<Type>};
        return instance;
    } else {
        return type_id<remove_cvref_t<Type>>();
    }
}

template<typename Type>
[[nodiscard]] auto type_id(const Type&) noexcept -> const TypeInfo& {
    return type_id<remove_cvref_t<Type>>();
}

} // namespace bt::types

template<>
struct std::hash<bt::types::TypeInfo> {
    std::size_t operator()(const bt::types::TypeInfo& info) const noexcept {
        return info.hash();
    }
};