// sequence

template <typename... tags_t> struct arg_predecessors : tags<tags_t...>
{
};
template <typename... tags_t> struct other_predecessors : tags<tags_t...>
{
};
template <typename... tags_t> struct successors : tags<tags_t...>
{
};

template <typename... tags_t> constexpr bool is_arg_predecessors_type(arg_predecessors<tags_t...>)
{
    return true;
}
template <typename... tags_t> constexpr bool is_other_predecessors_type(other_predecessors<tags_t...>)
{
    return true;
}
template <typename... tags_t> constexpr bool is_successors_type(successors<tags_t...>)
{
    return true;
}