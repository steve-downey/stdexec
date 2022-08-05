/*
 * Copyright (c) NVIDIA
 *
 * Licensed under the Apache License Version 2.0 with LLVM Exceptions
 * (the "License"); you may not use this file except in compliance with
 * the License. You may obtain a copy of the License at
 *
 *   https://llvm.org/LICENSE.txt
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#pragma once

#include <type_traits>
#include <utility>

namespace std {
  struct __ {};

  struct __ignore {
    __ignore() = default;
    __ignore(auto&&) noexcept {}
  };

  struct __none_such {};

  struct __immovable {
    __immovable() = default;
    __immovable(__immovable&&) = delete;
  };

  template <class _T>
    using __t = typename _T::__t;

  // For hiding a template type parameter from ADL
  template <class _T>
    struct __x_ {
      using __t = struct __t_ {
        using __t = _T;
      };
    };
  template <class _T>
    using __x = __t<__x_<_T>>;

  template <class...>
    struct __undef;

  template <bool _B>
    using __bool = bool_constant<_B>;

  template <size_t _N>
    using __msize_t = char(*)[_N+1];

  // Some utilities for manipulating lists of types at compile time
  template <class...>
    struct __types
#if defined(__GNUC__) && !defined(__clang__)
  {}  // BUGBUG: GCC does not like this "incomplete type"
#endif
  ;

  template <class _T>
    using __id = _T;

  template <class _T>
    inline constexpr auto __v = _T::value;

  template <class _T, class _U>
    inline constexpr bool __v<is_same<_T, _U>> = false;

  template <class _T>
    inline constexpr bool __v<is_same<_T, _T>> = true;

  template <class _T, _T _I>
    inline constexpr _T __v<integral_constant<_T, _I>> = _I;

  // To support __msize_t:
  template <class _T, size_t _N>
    inline constexpr size_t __v<_T(*)[_N]> = _N - 1;

  template <template <class...> class _Fn>
    struct __q {
      template <class... _Args>
        using __f = _Fn<_Args...>;
    };

  template <template <class> class _Fn>
    struct __q1 {
      template <class _Arg>
        using __f = _Fn<_Arg>;
    };

  template <template <class, class> class _Fn>
    struct __q2 {
      template <class _First, class _Second>
        using __f = _Fn<_First, _Second>;
    };

  template <template<class...> class _Fn, class... _Front>
    struct __mbind_front_q {
      template <class... _Args>
        using __f = _Fn<_Front..., _Args...>;
    };

  template <template<class...> class _Fn, class... _Front>
    struct __mbind_front_q1 {
      template <class _A>
        using __f = _Fn<_Front..., _A>;
    };

  template <template<class...> class _Fn, class... _Front>
    struct __mbind_front_q2 {
      template <class _A, class _B>
        using __f = _Fn<_Front..., _A, _B>;
    };

  template <template<class...> class _Fn, class... _Front>
    struct __mbind_front_q3 {
      template <class _A, class _B, class _C>
        using __f = _Fn<_Front..., _A, _B, _C>;
    };

  template <class _Fn, class... _Front>
    using __mbind_front = __mbind_front_q<_Fn::template __f, _Front...>;

  template <class _Fn, class... _Front>
    using __mbind_front1 = __mbind_front_q1<_Fn::template __f, _Front...>;

  template <class _Fn, class... _Front>
    using __mbind_front2 = __mbind_front_q2<_Fn::template __f, _Front...>;

  template <class _Fn, class... _Front>
    using __mbind_front3 = __mbind_front_q3<_Fn::template __f, _Front...>;

  template <template<class...> class _Fn, class... _Back>
    struct __mbind_back_q {
      template <class... _Args>
        using __f = _Fn<_Args..., _Back...>;
    };

  template <template<class...> class _Fn, class... _Back>
    struct __mbind_back_q1 {
      template <class _A>
        using __f = _Fn<_A, _Back...>;
    };

  template <template<class...> class _Fn, class... _Back>
    struct __mbind_back_q2 {
      template <class _A, class _B>
        using __f = _Fn<_A, _B, _Back...>;
    };

  template <template<class...> class _Fn, class... _Back>
    struct __mbind_back_q3 {
      template <class _A, class _B, class _C>
        using __f = _Fn<_A, _B, _C, _Back...>;
    };

  template <class _Fn, class... _Back>
    using __mbind_back = __mbind_back_q<_Fn::template __f, _Back...>;

  template <class _Fn, class... _Back>
    using __mbind_back1 = __mbind_back_q1<_Fn::template __f, _Back...>;

  template <class _Fn, class... _Back>
    using __mbind_back2 = __mbind_back_q2<_Fn::template __f, _Back...>;

  template <class _Fn, class... _Back>
    using __mbind_back3 = __mbind_back_q3<_Fn::template __f, _Back...>;

  template <template <class, class, class> class _Fn>
    struct __q3 {
      template <class _First, class _Second, class _Third>
        using __f = _Fn<_First, _Second, _Third>;
    };

  template <class _Fn, class... _Args>
    using __minvoke = typename _Fn::template __f<_Args...>;

  template <class _Fn, class _First>
    using __minvoke1 = typename _Fn::template __f<_First>;

  template <class _Fn, class _First, class _Second>
    using __minvoke2 = typename _Fn::template __f<_First, _Second>;

  template <class _Fn, class _First, class _Second, class _Third>
    using __minvoke3 = typename _Fn::template __f<_First, _Second, _Third>;

  template <template <class...> class _T, class... _Args>
    concept __valid = requires { typename _T<_Args...>; };

  template <template <class> class _T, class _First>
    concept __valid1 = requires { typename _T<_First>; };

  template <template <class, class> class _T, class _First, class _Second>
    concept __valid2 = requires { typename _T<_First, _Second>; };

  template <template <class, class, class> class _T, class _First, class _Second, class _Third>
    concept __valid3 = requires { typename _T<_First, _Second, _Third>; };

  template <class _Fn, class... _Args>
    concept __minvocable = __valid<_Fn::template __f, _Args...>;

  template <class _Fn, class _First>
    concept __minvocable1 = __valid1<_Fn::template __f, _First>;

  template <class _Fn, class _First, class _Second>
    concept __minvocable2 = __valid2<_Fn::template __f, _First, _Second>;

  template <class _Fn, class _First, class _Second, class _Third>
    concept __minvocable3 = __valid3<_Fn::template __f, _First, _Second, _Third>;

  template <class _Fn, class _Default>
    struct __with_default {
      template <class... _Args>
        struct __f_ { using __t = _Default; };
      template <class... _Args>
          requires __minvocable<_Fn, _Args...>
        struct __f_<_Args...> { using __t = __minvoke<_Fn, _Args...>; };
      template <class... _Args>
        using __f = __t<__f_<_Args...>>;
    };

  template <template <class...> class _T>
    struct __defer {
      template <class... _Args>
          requires __valid<_T, _Args...>
        struct __f_ { using __t = _T<_Args...>; };
      template <class _A>
          requires requires { typename _T<_A>; }
        struct __f_<_A> { using __t = _T<_A>; };
      template <class _A, class _B>
          requires requires { typename _T<_A, _B>; }
        struct __f_<_A, _B> { using __t = _T<_A, _B>; };
      template <class _A, class _B, class _C>
          requires requires { typename _T<_A, _B, _C>; }
        struct __f_<_A, _B, _C> { using __t = _T<_A, _B, _C>; };
      template <class _A, class _B, class _C, class _D>
          requires requires { typename _T<_A, _B, _C, _D>; }
        struct __f_<_A, _B, _C, _D> { using __t = _T<_A, _B, _C, _D>; };
      template <class _A, class _B, class _C, class _D, class _E>
          requires requires { typename _T<_A, _B, _C, _D, _E>; }
        struct __f_<_A, _B, _C, _D, _E> { using __t = _T<_A, _B, _C, _D, _E>; };
      template <class... _Args>
        using __f = __t<__f_<_Args...>>;
    };

  template <class _T>
    struct __mconst {
      template <class...>
        using __f = _T;
    };

  template <class _Fn, class _Continuation = __q<__types>>
    struct __transform {
      template <class... _Args>
        using __f = __minvoke<_Continuation, __minvoke1<_Fn, _Args>...>;
    };

  template <class _Init, class _Fn>
    struct __fold_right {
      template <class...>
        struct __f_ {};
      template <class _State, class _Head, class... _Tail>
          requires __minvocable2<_Fn, _State, _Head>
        struct __f_<_State, _Head, _Tail...>
          : __f_<__minvoke2<_Fn, _State, _Head>, _Tail...>
        {};
      template <class _State>
        struct __f_<_State> {
          using __t = _State;
        };
      template <class... _Args>
        using __f = __t<__f_<_Init, _Args...>>;
    };

  template <class _Continuation = __q<__types>>
    struct __concat {
      template <class...>
        struct __f_ {};
      template <class... _As>
          requires (sizeof...(_As) == 0) &&
            __minvocable<_Continuation, _As...>
        struct __f_<_As...> {
          using __t = __minvoke<_Continuation>;
        };
      template <template <class...> class _A, class... _As>
          requires __minvocable<_Continuation, _As...>
        struct __f_<_A<_As...>> {
          using __t = __minvoke<_Continuation, _As...>;
        };
      template <template <class...> class _A, class... _As,
                template <class...> class _B, class... _Bs,
                class... _Tail>
        struct __f_<_A<_As...>, _B<_Bs...>, _Tail...>
          : __f_<__types<_As..., _Bs...>, _Tail...> {};
      template <template <class...> class _A, class... _As,
                template <class...> class _B, class... _Bs,
                template <class...> class _C, class... _Cs,
                class... _Tail>
        struct __f_<_A<_As...>, _B<_Bs...>, _C<_Cs...>, _Tail...>
          : __f_<__types<_As..., _Bs..., _Cs...>, _Tail...> {};
      template <template <class...> class _A, class... _As,
                template <class...> class _B, class... _Bs,
                template <class...> class _C, class... _Cs,
                template <class...> class _D, class... _Ds,
                class... _Tail>
        struct __f_<_A<_As...>, _B<_Bs...>, _C<_Cs...>, _D<_Ds...>, _Tail...>
          : __f_<__types<_As..., _Bs..., _Cs..., _Ds...>, _Tail...> {};
      template <class... _Args>
        using __f = __t<__f_<_Args...>>;
    };

  template <bool>
    struct __if_ {
      template <class _True, class>
        using __f = _True;
    };
  template <>
    struct __if_<false> {
      template <class, class _False>
        using __f = _False;
    };
  template <class _Pred, class _True, class _False>
    using __if = __minvoke2<__if_<__v<_Pred>>, _True, _False>;
  template <bool _Pred, class _True, class _False>
    using __if_c = __minvoke2<__if_<_Pred>, _True, _False>;

  template <class _Fn>
    struct __curry {
      template <class... _Ts>
        using __f = __minvoke<_Fn, _Ts...>;
    };

  template <class _Fn>
    struct __uncurry : __concat<_Fn> {};

  template <class _Fn, class _List>
    using __mapply =
      __minvoke<__uncurry<_Fn>, _List>;

  struct __mcount {
    template <class... _Ts>
      using __f = integral_constant<size_t, sizeof...(_Ts)>;
  };

  template <class _Fn>
    struct __mcount_if {
      template <class... _Ts>
        using __f =
          integral_constant<size_t, (bool(__minvoke1<_Fn, _Ts>::value) + ...)>;
    };

  template <class _T>
    struct __contains {
      template <class... _Args>
        using __f = __bool<(__v<is_same<_T, _Args>> ||...)>;
    };

  template <class _Continuation = __q<__types>>
    struct __push_back {
      template <class _List, class _Item>
        using __f =
          __mapply<__mbind_back<_Continuation, _Item>, _List>;
    };

  template <class _Continuation = __q<__types>>
    struct __push_back_unique {
      template <class _List, class _Item>
        using __f =
          __mapply<
            __if<
              __mapply<__contains<_Item>, _List>,
              _Continuation,
              __mbind_back<_Continuation, _Item>>,
            _List>;
    };

  template <class _Continuation = __q<__types>>
    struct __munique {
      template <class... _Ts>
        using __f =
          __mapply<
            _Continuation,
            __minvoke<__fold_right<__types<>, __push_back_unique<>>, _Ts...>>;
    };

  template <class...>
    struct __mcompose {};

  template <class _First>
    struct __mcompose<_First> : _First {};

  template <class _Second, class _First>
    struct __mcompose<_Second, _First> {
      template <class... _Args>
        using __f = __minvoke1<_Second, __minvoke<_First, _Args...>>;
    };

  template <class _Last, class _Penultimate, class... _Rest>
    struct __mcompose<_Last, _Penultimate, _Rest...> {
      template <class... _Args>
        using __f =
          __minvoke1<_Last, __minvoke<__mcompose<_Penultimate, _Rest...>, _Args...>>;
    };

  template <class _Old, class _New, class _Continuation = __q<__types>>
    struct __replace {
      template <class... _Args>
        using __f =
          __minvoke<_Continuation, __if<is_same<_Args, _Old>, _New, _Args>...>;
    };

  template <class _Old, class _Continuation = __q<__types>>
    struct __remove {
      template <class... _Args>
        using __f =
          __minvoke<
            __concat<_Continuation>,
            __if<is_same<_Args, _Old>, __types<>, __types<_Args>>...>;
    };

  template <class _Return>
    struct __qf {
      template <class... _Args>
        using __f = _Return(_Args...);
    };

  template <class _T>
    _T&& __declval() noexcept requires true;

  template <class>
    void __declval() noexcept;

  template <class _T>
    _T __declval_ex() noexcept requires true;

  template <class>
    void __declval_ex() noexcept;

  // For copying cvref from one type to another:
  template <class _Member, class _Self>
    _Member _Self::*__memptr(const _Self&);

  template <typename _Self, typename _Member>
    using __member_t = decltype(
      (__declval<_Self>() .* __memptr<_Member>(__declval<_Self>())));

  template <class... _As>
      requires (sizeof...(_As) != 0)
    struct __front;
  template <class _A, class... _As>
    struct __front<_A, _As...> {
      using __t = _A;
    };
  template <class... _As>
      requires (sizeof...(_As) == 1)
    using __single_t = __t<__front<_As...>>;
  template <class _Ty>
    struct __single_or {
      template <class... _As>
          requires (sizeof...(_As) <= 1)
        using __f = __t<__front<_As..., _Ty>>;
    };

  template <class _Fun, class... _As>
    concept __callable =
      requires (_Fun&& __fun, _As&&... __as) {
        ((_Fun&&) __fun)((_As&&) __as...);
      };
  template <class _Fun, class... _As>
    concept __nothrow_callable =
      __callable<_Fun, _As...> &&
      requires (_Fun&& __fun, _As&&... __as) {
        { ((_Fun&&) __fun)((_As&&) __as...) } noexcept;
      };
  template <class _Fun, class... _As>
    using __call_result_t = decltype(__declval<_Fun>()(__declval<_As>()...));

  // For working around clang's lack of support for CWG#2369:
  // http://www.open-std.org/jtc1/sc22/wg21/docs/cwg_defects.html#2369
  struct __qcall_result {
    template <class _Fun, class... _As>
      using __f = __call_result_t<_Fun, _As...>;
  };
  template <bool _Enable, class _Fun, class... _As>
    using __call_result_if_t =
      typename __if<__bool<_Enable>, __qcall_result, __>
        ::template __f<_Fun, _As...>;

  // For emplacing non-movable types into optionals:
  template <class _Fn>
      requires is_nothrow_move_constructible_v<_Fn>
    struct __conv {
      _Fn __fn_;
      using __t = __call_result_t<_Fn>;
      operator __t() && noexcept(__nothrow_callable<_Fn>) {
        return ((_Fn&&) __fn_)();
      }
      __t operator()() && noexcept(__nothrow_callable<_Fn>) {
        return ((_Fn&&) __fn_)();
      }
    };
  template <class _Fn>
    __conv(_Fn) -> __conv<_Fn>;

  template <class _T>
    using __cref_t = const remove_reference_t<_T>&;

  template <class _Fn, class _Continuation = __q<__types>>
    struct __mzip_with2 {
      template <class, class>
        struct __f_;
      template <template <class...> class _C, class... _Cs,
                template <class...> class _D, class... _Ds>
          requires requires {
            typename __minvoke<_Continuation, __minvoke2<_Fn, _Cs, _Ds>...>;
          }
        struct __f_<_C<_Cs...>, _D<_Ds...>> {
          using __t = __minvoke<_Continuation, __minvoke2<_Fn, _Cs, _Ds>...>;
        };
      template <class _C, class _D>
        using __f = __t<__f_<_C, _D>>;
    };

  template <size_t... _Indices>
    auto __mconvert_indices(index_sequence<_Indices...>*)
      -> __types<__msize_t<_Indices>...>;
  template <size_t _N>
    using __mmake_index_sequence =
      decltype(__mconvert_indices((make_index_sequence<_N>*) nullptr));
  template <class... _Ts>
    using __mindex_sequence_for =
      __mmake_index_sequence<sizeof...(_Ts)>;

  template <class... _Bools>
    using __mand = __bool<(__v<_Bools> &&...)>;
  template <class... _Bools>
    using __mor = __bool<(__v<_Bools> ||...)>;

  template <class _N, class _T, class _Continuation = __q<__types>>
    using __mfill_n =
      __mapply<
        __transform<
          __mconst<_T>,
          _Continuation>,
        __mmake_index_sequence<__v<_N>>>;

  struct __m_any {
    template <class _T>
      operator _T&& () const;
  };

  template <size_t _N>
    struct __mpriority_
      : __mpriority_<_N - 1> {
      using value_type = size_t;
      using type = __mpriority_<_N>;
      static constexpr size_t value = _N;
    };

  template <size_t _N>
    inline constexpr size_t __mpriority_<_N>::value;

  template <>
    struct __mpriority_<0u> {
      using value_type = size_t;
      using type = __mpriority_<0u>;
      static constexpr size_t value = 0;
    };

  inline constexpr size_t __mpriority_<0u>::value;

  template <size_t _N>
    using __mpriority = __mpriority_<_N>*;

  template <size_t _N>
    inline constexpr size_t __v<__mpriority<_N>> = _N;

  template <class _T, class... _Ts>
    concept __aggregate_initializable_with =
      is_aggregate_v<_T> &&
      requires (_Ts(*... __ts)() noexcept) {
        _T{__ts()...};
      };

  template <class _T, class... _Ts>
    using __cmembers_ = __msize_t<__aggregate_initializable_with<_T, _Ts...> ? sizeof...(_Ts) : 0>;

  inline constexpr size_t __max_aggregate_members = 16;

  template <class _A, class _B>
    using __mmax_ = __if_c<(__v<_A> < __v<_B>), _B, _A>;

  template <class _BinPred = __q2<__mmax_>, class _Init = __msize_t<0>>
    using __mmax = __fold_right<_Init, _BinPred>;

  template <class _T>
    using __aggregate_member_count =
      __mapply<
        __transform<
          __mbind_back1<__q3<__mfill_n>, __m_any, __mbind_front_q<__cmembers_, _T>>,
          __mmax<>>,
        __mmake_index_sequence<__max_aggregate_members>>;

  #define _FWD(_X) ((decltype(_X)&&) _X)

  template <class _Fn, class _T>
    static constexpr decltype(auto) __call_with_members_of(_Fn __fun, _T&& __t) {
      constexpr size_t __cmembers = __v<__aggregate_member_count<_T>>;
      if constexpr (__cmembers == 0) {
        return ((_Fn&&) __fun)();
      } else if constexpr (__cmembers == 1) {
        auto &&[_0] = (_T&&) __t;
        return ((_Fn&&) __fun)(_FWD(_0));
      } else if constexpr (__cmembers == 2) {
        auto &&[_0, _1] = (_T&&) __t;
        return ((_Fn&&) __fun)(_FWD(_0), _FWD(_1));
      } else if constexpr (__cmembers == 3) {
        auto &&[_0, _1, _2] = (_T&&) __t;
        return ((_Fn&&) __fun)(_FWD(_0), _FWD(_1), _FWD(_2));
      } else if constexpr (__cmembers == 4) {
        auto &&[_0, _1, _2, _3] = (_T&&) __t;
        return ((_Fn&&) __fun)(_FWD(_0), _FWD(_1), _FWD(_2), _FWD(_3));
      } else if constexpr (__cmembers == 5) {
        auto &&[_0, _1, _2, _3, _4] = (_T&&) __t;
        return ((_Fn&&) __fun)(_FWD(_0), _FWD(_1), _FWD(_2), _FWD(_3), _FWD(_4));
      } else if constexpr (__cmembers == 6) {
        auto &&[_0, _1, _2, _3, _4, _5] = (_T&&) __t;
        return ((_Fn&&) __fun)(_FWD(_0), _FWD(_1), _FWD(_2), _FWD(_3), _FWD(_4), _FWD(_5));
      } else if constexpr (__cmembers == 7) {
        auto &&[_0, _1, _2, _3, _4, _5, _6] = (_T&&) __t;
        return ((_Fn&&) __fun)(_FWD(_0), _FWD(_1), _FWD(_2), _FWD(_3), _FWD(_4), _FWD(_5),
                                _FWD(_6));
      } else {
        static_assert(__cmembers < 8, "NOT IMPLEMENTED YET"); // TODO
        return;
      }
    }

  struct __call_with_members_of_impl {
    template <class... _Ts>
      auto operator()(_Ts&&...) const noexcept {
        return __mbind_back_q1<invoke_result_t, _Ts...>{};
      }
  };

  template <class _T>
    using __callable_with_members_of_t =
      decltype((__call_with_members_of)(
        __call_with_members_of_impl{},
        __declval<_T>()));

  template <class _Fn, class _T>
    concept __callable_with_members_of =
      is_aggregate_v<_T> &&
      (__v<__aggregate_member_count<_T>> != 0) &&
      __valid2<__minvoke1, __callable_with_members_of_t<_T>, _Fn>;

  template <class _Fn, class _T>
    using __callable_with_members_of_result_t =
      __minvoke1<__callable_with_members_of_t<_T>, _Fn>;

  inline constexpr struct __magic_apply_t {
    template <class _T, __callable_with_members_of<_T> _Fn>
      constexpr auto operator()(_Fn __fun, _T&& __t) const
        -> __callable_with_members_of_result_t<_Fn, _T> {
        return (__call_with_members_of)((_Fn&&) __fun, (_T&&) __t);
      }
  } __magic_apply {};

  template <class _Fn, class _T>
    concept __magic_applicable =
      requires (_Fn&& __fun, _T&& __t) {
        __magic_apply((_Fn&&) __fun, (_T&&) __t);
      };

  template <class _Fn, class _T>
    using __magic_apply_result_t =
      decltype(__magic_apply(__declval<_Fn>(), __declval<_T>()));

  #undef _FWD
}
