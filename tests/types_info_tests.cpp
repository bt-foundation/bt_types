// =============================================================================
// TypeInfo — Exhaustive GTest Suite (EnTT-style)
//
// Tests a lightweight RTTI-free type identity module, mirroring EnTT's
// entt::type_info semantics:
//   - a per-type constant id, stable across calls and equal for the same type
//   - cv-qualifiers and references STRIPPED; pointers are DISTINCT types
//   - name() returning the unqualified type name
//   - ordering (<), equality, hashing, unordered-container usability
//   - works without RTTI (no typeid/dynamic_cast anywhere)
//
//   namespace bt::ecs {
//     using id_type = uint64_t;
//
//     class TypeInfo {
//      public:
//       id_type        hash() const;           // stable, hash-derived, per-type
//       uint32_t       index() const;          // unique atomic index
//       StringView     name() const;           // unqualified type name
// 
//       friend bool    operator==(TypeInfo, TypeInfo);
//       friend bool    operator<(TypeInfo, TypeInfo);   // total order (by index)
//     };
//
//     template <typename T> TypeInfo type_id();       // strips cv && ref, NOT *
//     template <typename T> TypeInfo type_id(const T&);    // convenience: deduce +
//                                                      // strip cv && ref
//
//     template <typename T> struct TypeIndex {
//       static constexpr id_type value();              // == type_id<T>().index()
//     };
// 
//     template <typename T> struct TypeName {
//       static constexpr id_type value();              // == type_id<T>().name()
//     };
//   }
//
//   namespace std { template <> struct hash<TypeInfo>; }
// =============================================================================

#include <gtest/gtest.h>

#include "types.hpp"

using namespace bt::types;

namespace {

// Varied test types -----------------------------------------------------------------

struct Empty {};

struct Position {
    float x, y, z;
};

struct Velocity {
    float x, y, z;
}; // same layout as Position — must still be a DIFFERENT type

struct Tagged {}; // uniquely named; name assertions use it

using PositionAlias = Position; // using alias — same type
typedef Position PositionTypedef; // typedef — same type

enum LegacyEnum { LE_A, LE_B };
enum class ScopedEnum : ::uint8_t { A, B };

template<typename T>
struct Wrapper {
    T value;
};

template<int N>
struct Generated {
    int payload = N;
};

// Private nested type: type identity must not require member access.
class Outer {
  private:
    struct Hidden {
        int x;
    };

  public:
    static TypeInfo info() {
        return type_id<Hidden>();
    }
};

} // namespace

// ===========================================================================
// Basics: identity, equality, distinctness
// ===========================================================================

TEST(TypeInfoBasics, SameTypeYieldsEqualInfoEveryTime) {
    auto a = type_id<Position>();
    auto b = type_id<Position>();
    EXPECT_EQ(a, b);
    EXPECT_EQ(a.hash(), b.hash());
    EXPECT_EQ(a.index(), b.index());
    EXPECT_EQ(a.name(), b.name());
}

TEST(TypeInfoBasics, DistinctTypesAreNeverEqual) {
    EXPECT_NE(
        type_id<Position>(),
        type_id<Velocity>()
    ); // same layout!
    EXPECT_NE(type_id<int>(), type_id<float>());
    EXPECT_NE(type_id<int>(), type_id<long>());
    EXPECT_NE(type_id<LegacyEnum>(), type_id<ScopedEnum>());
    EXPECT_NE(type_id<Wrapper<int>>(), type_id<Wrapper<float>>());
    EXPECT_NE(
        type_id<Wrapper<Wrapper<int>>>(),
        type_id<Wrapper<int>>()
    );
}

TEST(TypeInfoBasics, HashDerivedIdsAreNonZeroAndFitIdType) {
    for (const auto& info :
         {type_id<int>(),
          type_id<float>(),
          type_id<Position>(),
          type_id<Empty>(),
          type_id<void>()}) {
        EXPECT_NE(info.hash(), ID {0});
    }
}

// ===========================================================================
// cv-qualifier and reference stripping (core EnTT semantics)
// ===========================================================================

TEST(TypeInfoStripping, ConstVolatileAndReferencesAllCollapse) {
    EXPECT_EQ(type_id<int>(), type_id<const int>());
    EXPECT_EQ(type_id<int>(), type_id<volatile int>());
    EXPECT_EQ(type_id<int>(), type_id<const volatile int>());
    EXPECT_EQ(type_id<int>(), type_id<int&>());
    EXPECT_EQ(type_id<int>(), type_id<const int&>());
    EXPECT_EQ(type_id<int>(), type_id<volatile int&>());
    EXPECT_EQ(type_id<int>(), type_id<int&&>());
    EXPECT_EQ(type_id<int>(), type_id<const int&&>());
    // Same for a class type.
    EXPECT_EQ(type_id<Position>(), type_id<const Position&>());
    EXPECT_EQ(type_id<Position>(), type_id<Position&&>());
}

TEST(TypeInfoStripping, TypeIdAlsoAcceptsInstances) {
    // Convenience overload: deduce from a value/lvalue (cv + ref stripped).
    const Position p {1.f, 2.f, 3.f};
    Position& lvalue = const_cast<Position&>(p);
    EXPECT_EQ(type_id(p), type_id<Position>());
    EXPECT_EQ(type_id(lvalue), type_id<Position>());
    EXPECT_EQ(type_id(as_const(lvalue)), type_id<Position>());
}

// ===========================================================================
// Pointers are DISTINCT types (they get their own id)
// ===========================================================================

TEST(TypeInfoPointers, PointersAreDistinctFromPointee) {
    EXPECT_NE(type_id<int>(), type_id<int*>());
    EXPECT_NE(type_id<Position>(), type_id<Position*>());
    EXPECT_NE(type_id<int>(), type_id<int**>());
    EXPECT_NE(type_id<int*>(), type_id<int**>());
    // cv of the pointee matters for pointer types (int* vs const int* differ),
    // but top-level cv on the pointer itself is stripped.
    EXPECT_NE(type_id<int*>(), type_id<const int*>());
    EXPECT_EQ(type_id<int*>(), type_id<int* const>());
    EXPECT_EQ(type_id<int*>(), type_id<int* const&>());
}

// ===========================================================================
// Fundamental types & void
// ===========================================================================

TEST(TypeInfoFundamentals, AllFundamentalsAreDistinct) {
    const Vec<TypeInfo> all = {
        type_id<void>(),
        type_id<bool>(),
        type_id<char>(),
        type_id<signed char>(),
        type_id<unsigned char>(),
        type_id<short>(),
        type_id<int>(),
        type_id<long>(),
        type_id<long long>(),
        type_id<unsigned int>(),
        type_id<float>(),
        type_id<double>(),
        type_id<long double>()
    };
    for (size_t i = 0; i < all.size(); ++i)
        for (size_t j = i + 1; j < all.size(); ++j)
            EXPECT_NE(all[i], all[j]) << "collision at i=" << i << " j=" << j;
}

TEST(TypeInfoFundamentals, VoidIsSupported) {
    auto v = type_id<void>();
    EXPECT_EQ(v, type_id<const void>()); // cv stripped
    EXPECT_FALSE(v.name().empty());
}

TEST(TypeInfoFundamentals, BoolCharIntSignednessVariants) {
    // char, signed char, unsigned char are three distinct types in C++.
    EXPECT_NE(type_id<char>(), type_id<signed char>());
    EXPECT_NE(type_id<char>(), type_id<unsigned char>());
    EXPECT_NE(type_id<signed char>(), type_id<unsigned char>());
}

// ===========================================================================
// Aliases, templates, enums, composite types
// ===========================================================================

TEST(TypeInfoAliases, UsingAndTypedefAliasesAreTheSameType) {
    EXPECT_EQ(type_id<Position>(), type_id<PositionAlias>());
    EXPECT_EQ(type_id<Position>(), type_id<PositionTypedef>());
}

TEST(TypeInfoTemplates, SpecializationsAreDistinctPerArgument) {
    EXPECT_NE(type_id<Wrapper<int>>(), type_id<Wrapper<float>>());
    EXPECT_NE(
        type_id<Wrapper<int>>(),
        type_id<Wrapper<Wrapper<int>>>()
    );
    // But alias arguments collapse before instantiation: both are Wrapper<Position>.
    EXPECT_EQ(
        type_id<Wrapper<Position>>(),
        type_id<Wrapper<PositionAlias>>()
    );
}

TEST(TypeInfoComposites, ArraysAndCompositeSignatures) {
    EXPECT_NE(type_id<Position[2]>(), type_id<Position[3]>());
    EXPECT_NE(type_id<int[4]>(), type_id<float[4]>());
    EXPECT_NE(type_id<int[4]>(), type_id<int[5]>());
}

TEST(TypeInfoNested, PrivateNestedTypeIsIdentifiable) {
    auto hidden = Outer::info();
    EXPECT_NE(hidden, type_id<Position>());
    EXPECT_EQ(hidden, Outer::info()); // stable across translation of the call
    EXPECT_FALSE(hidden.name().empty());
}

TEST(TypeInfoLocal, LocalClassesWork) {
    struct VeryLocal {
        int x;
    };

    auto a = type_id<VeryLocal>();
    auto b = type_id<VeryLocal>();
    EXPECT_EQ(a, b);
    EXPECT_NE(a, type_id<Position>());

    // A second, differently-named local type in the same scope is distinct.
    struct Other {
        int x;
    };

    EXPECT_NE(a, type_id<Other>());
}

// ===========================================================================
// seq: per-process creation order
// ===========================================================================

TEST(TypeInfoSequence, SeqIsUniquePerDistinctType) {
    const Vec<TypeInfo> all = {
        type_id<Position>(),
        type_id<Velocity>(),
        type_id<Empty>(),
        type_id<Tagged>(),
        type_id<int>(),
        type_id<double>(),
        type_id<LegacyEnum>(),
        type_id<ScopedEnum>(),
        type_id<Wrapper<int>>(),
        type_id<Wrapper<float>>(),
        type_id<Position[2]>(),
        type_id<void>()
    };
    HashSet<uint32_t> seqs;
    for (auto& info : all) {
        auto [it, inserted] = seqs.insert(info.index());
        EXPECT_TRUE(inserted) << "duplicate seq among distinct types";
    }
    EXPECT_EQ(seqs.size(), all.size());
}

TEST(TypeInfoSequence, SeqIsQuasiMonotonicAcrossFirstQueries) {
    // Query 16 distinct fresh types once each, in order; seq must never regress.
    using Factory = TypeInfo (*)();
    static constexpr Factory factories[] = {
        [] { return type_id<Generated<100>>(); },
        [] { return type_id<Generated<101>>(); },
        [] { return type_id<Generated<102>>(); },
        [] { return type_id<Generated<103>>(); },
        [] { return type_id<Generated<104>>(); },
        [] { return type_id<Generated<105>>(); },
        [] { return type_id<Generated<106>>(); },
        [] { return type_id<Generated<107>>(); },
        [] { return type_id<Generated<108>>(); },
        [] { return type_id<Generated<109>>(); },
        [] { return type_id<Generated<110>>(); },
        [] { return type_id<Generated<111>>(); },
        [] { return type_id<Generated<112>>(); },
        [] { return type_id<Generated<113>>(); },
        [] { return type_id<Generated<114>>(); },
        [] { return type_id<Generated<115>>(); },
    };
    Vec<uint32_t> seqs;
    for (auto* make : factories)
        seqs.push_back(make().index());
    for (size_t i = 1; i < seqs.size(); ++i)
        EXPECT_GT(seqs[i], seqs[i - 1]) << "seq regressed at " << i;
}

// ===========================================================================
// index & ordering
// ===========================================================================

TEST(TypeInfoOrdering, LessThanIsATotalOrder) {
    const Vec<TypeInfo> all = {
        type_id<int>(),
        type_id<float>(),
        type_id<Position>(),
        type_id<Velocity>(),
        type_id<Empty>(),
        type_id<Wrapper<int>>()
    };
    for (size_t i = 0; i < all.size(); ++i) {
        for (size_t j = 0; j < all.size(); ++j) {
            if (i == j) {
                EXPECT_FALSE(all[i] < all[j]);
                EXPECT_FALSE(all[j] < all[i]);
            } else {
                EXPECT_TRUE(all[i] < all[j] || all[j] < all[i])
                    << "incomparable pair " << i << "," << j;
                EXPECT_NE(all[i] < all[j], all[j] < all[i])
                    << "antisymmetry violated " << i << "," << j;
            }
        }
    }
}

TEST(TypeInfoOrdering, OrderingAgreesWithIndex) {
    const Vec<TypeInfo> all = {
        type_id<int>(),
        type_id<double>(),
        type_id<Position>(),
        type_id<Empty>(),
        type_id<Wrapper<int>>()
    };
    for (size_t i = 0; i < all.size(); ++i)
        for (size_t j = 0; j < all.size(); ++j) {
            EXPECT_EQ(all[i] < all[j], all[i].index() < all[j].index());
            if (i != j)
                EXPECT_NE(all[i].index(), all[j].index());
        }
}

TEST(TypeInfoOrdering, SortableByLessThan) {
    Vec<TypeInfo> all = {
        type_id<Wrapper<int>>(),
        type_id<int>(),
        type_id<Position>(),
        type_id<float>(),
        type_id<Empty>(),
        type_id<double>()
    };
    sort(all.begin(), all.end());
    EXPECT_TRUE(is_sorted(all.begin(), all.end()));
    for (size_t i = 1; i < all.size(); ++i)
        EXPECT_LT(all[i - 1].index(), all[i].index());
}

TEST(TypeInfoOrdering, EqualityIsConsistentWithLessThan) {
    auto a = type_id<int>();
    auto b = type_id<int>();
    auto c = type_id<float>();
    EXPECT_EQ(a == b, !(a < b) && !(b < a));
    EXPECT_EQ(a == c, !(a < c) && !(c < a));
}

TEST(TypeInfoOrdering, IndexIsDenseForFreshTypes) {
    // 16 distinct fresh types: indices must be unique and tightly clustered
    // (no gaps beyond the process's global creation).
    using Factory = TypeInfo (*)();
    static constexpr Factory factories[] = {
        [] { return type_id<Generated<200>>(); },
        [] { return type_id<Generated<201>>(); },
        [] { return type_id<Generated<202>>(); },
        [] { return type_id<Generated<203>>(); },
        [] { return type_id<Generated<204>>(); },
        [] { return type_id<Generated<205>>(); },
        [] { return type_id<Generated<206>>(); },
        [] { return type_id<Generated<207>>(); },
        [] { return type_id<Generated<208>>(); },
        [] { return type_id<Generated<209>>(); },
        [] { return type_id<Generated<210>>(); },
        [] { return type_id<Generated<211>>(); },
        [] { return type_id<Generated<212>>(); },
        [] { return type_id<Generated<213>>(); },
        [] { return type_id<Generated<214>>(); },
        [] { return type_id<Generated<215>>(); },
    };
    Vec<uint32_t> idx;
    for (auto* make : factories)
        idx.push_back(make().index());
    sort(idx.begin(), idx.end());
    EXPECT_LT(idx.front(), 64u) << "index seems non-dense";
    for (size_t i = 1; i < idx.size(); ++i)
        EXPECT_GT(idx[i], idx[i - 1]);
}

// ===========================================================================
// type_index: constant per-type value
// ===========================================================================

TEST(TypeIndex, ValueMatchesTypeId) {
    EXPECT_EQ(TypeIndex<int>::value(), type_id<int>().index());
    EXPECT_EQ(TypeIndex<Position>::value(),
        type_id<Position>().index()
    );
    EXPECT_NE(TypeIndex<int>::value(), TypeIndex<float>::value());
}

// ===========================================================================
// name()
// ===========================================================================

TEST(TypeInfoName, NonEmptyForCommonTypes) {
    for (const auto& info :
         {type_id<int>(),
          type_id<Position>(),
          type_id<Empty>(),
          type_id<Wrapper<float>>()}) {
        EXPECT_FALSE(info.name().empty());
    }
}

TEST(TypeInfoName, DistinctTypesHaveDistinctNames) {
    EXPECT_NE(type_id<Position>().name(), type_id<Velocity>().name());
    EXPECT_NE(type_id<int>().name(), type_id<float>().name());
}

TEST(TypeInfoName, StrippingAffectsNameToo) {
    EXPECT_EQ(
        type_id<Position>().name(),
        type_id<const Position&>().name()
    );
    EXPECT_EQ(
        type_id<int>().name(),
        type_id<volatile int&&>().name()
    );
}

TEST(TypeInfoName, NameContainsUnqualifiedTypeName) {
    // EnTT strips namespaces/nesting; assert the bare name appears as-is.
    EXPECT_NE(
        type_id<Tagged>().name().find("Tagged"),
        StringView::npos
    );
    EXPECT_NE(
        type_id<Position>().name().find("Position"),
        StringView::npos
    );
    // Template name includes the template id.
    EXPECT_NE(
        type_id<Wrapper<int>>().name().find("Wrapper"),
        StringView::npos
    );
    EXPECT_NE(
        type_id<Wrapper<int>>().name().find("int"),
        StringView::npos
    );
}

// ===========================================================================
// Hashing & unordered containers
// ===========================================================================

TEST(TypeInfoHash, UsableInUnorderedSet) {
    HashSet<TypeInfo> set;
    for (const auto& info :
         {type_id<int>(),
          type_id<float>(),
          type_id<Position>(),
          type_id<Velocity>(),
          type_id<Empty>(),
          type_id<void>()}) {
        set.insert(info);
        set.insert(info); // duplicate insert is a no-op
    }
    EXPECT_EQ(set.size(), 6u);
    EXPECT_FALSE(set.contains(type_id<double>())) << "never inserted here";
    // Re-querying in a different TU order/cv-form must find them.
    EXPECT_TRUE(set.find(type_id<Position&>()) != set.end());
}

// ===========================================================================
// Aggregated: many types, pairwise distinct, totally orderable, hashable
// ===========================================================================

namespace {

// Query 1000 distinct template instantiations in one compile-time unrolled
// expression (instantiation happens at compile time; the runtime cost of the
// fold is 1000 id queries). Range 2000+ avoids overlap with other tests.
template<int... Ns>
void insert_generated(
    HashSet<TypeInfo>& set,
    integer_sequence<int, Ns...>
) {
    (set.insert(type_id<Generated<2000 + Ns>>()), ...);
}

} // namespace

TEST(TypeInfoAggregate, ThousandGeneratedTypesAreAllDistinct) {
    HashSet<TypeInfo> set;
    insert_generated(set, make_integer_sequence<int, 1000> {});
    EXPECT_EQ(set.size(), 1000u)
        << "distinct template instantiations must not collide (id or hash)";
}

TEST(TypeInfoAggregate, MixedSetTotallyOrderedAndHashable) {
    const Vec<TypeInfo> all = {
        type_id<void>(),
        type_id<bool>(),
        type_id<int>(),
        type_id<double>(),
        type_id<LegacyEnum>(),
        type_id<ScopedEnum>(),
        type_id<Position>(),
        type_id<Velocity>(),
        type_id<Empty>(),
        type_id<Tagged>(),
        type_id<Position[3]>(),
        type_id<Wrapper<int>>(),
        type_id<Wrapper<Position>>(),
        type_id<int*>(),
        type_id<const int*>(),
        type_id<Position*>()
    };
    // Pairwise distinct
    for (size_t i = 0; i < all.size(); ++i)
        for (size_t j = i + 1; j < all.size(); ++j)
            EXPECT_NE(all[i], all[j]) << "i=" << i << " j=" << j;
    // All distinct ids too (collision freedom at this scale)
    HashSet<ID> ids;
    for (const auto& info : all) {
        auto [_, ok] = ids.insert(info.hash());
        EXPECT_TRUE(ok);
    }
    // Sortable
    Vec<TypeInfo> sorted = all;
    sort(sorted.begin(), sorted.end());
    EXPECT_TRUE(is_sorted(sorted.begin(), sorted.end()));
    // Hashable into a set
    HashSet<TypeInfo> hashed(all.begin(), all.end());
    EXPECT_EQ(hashed.size(), all.size());
}