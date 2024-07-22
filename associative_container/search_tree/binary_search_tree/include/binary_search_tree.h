#ifndef MATH_PRACTICE_AND_OPERATING_SYSTEMS_BINARY_SEARCH_TREE_H
#define MATH_PRACTICE_AND_OPERATING_SYSTEMS_BINARY_SEARCH_TREE_H

#include <list>
#include <stack>
#include <vector>
#include <logger.h>
#include <logger_guardant.h>
#include <allocator.h>
#include <allocator_guardant.h>
#include <not_implemented.h>
#include <search_tree.h>

template<
    typename tkey,
    typename tvalue>
class binary_search_tree:
    public search_tree<tkey, tvalue>
{

protected:

    struct node
    {

    public:

        tkey key;

        tvalue value;

        node *left_subtree;

        node *right_subtree;

    public:

        explicit node(
            tkey const &key,
            tvalue const &value);

        explicit node(
            tkey const &key,
            tvalue &&value);

        virtual ~node() noexcept = default;

    };

public:
    explicit binary_search_tree<tkey, tvalue>(
            std::function<int(tkey const &, tvalue const &)> keys_comparer = std::less<tkey>(),
            logger *logger = nullptr,
            allocator *allocator = nullptr)
            : search_tree<tkey, tvalue>(keys_comparer, logger, allocator)
    {

    }

public:

    // region iterators definition

    class prefix_iterator;
    class prefix_const_iterator;
    class prefix_reverse_iterator;
    class prefix_reverse_const_iterator;
    class infix_iterator;
    class infix_const_iterator;
    class infix_reverse_iterator;
    class infix_reverse_const_iterator;
    class postfix_iterator;
    class postfix_const_iterator;
    class postfix_reverse_iterator;
    class postfix_reverse_const_iterator;

    struct iterator_data
    {

        friend class prefix_iterator;
        friend class prefix_const_iterator;
        friend class prefix_reverse_iterator;
        friend class prefix_reverse_const_iterator;
        friend class infix_iterator;
        friend class infix_const_iterator;
        friend class infix_reverse_iterator;
        friend class infix_reverse_const_iterator;
        friend class postfix_iterator;
        friend class postfix_const_iterator;
        friend class postfix_reverse_iterator;
        friend class postfix_reverse_const_iterator;

    public:

        unsigned int depth;

    protected:

        tkey *_key;

        tvalue *_value;

        bool _is_initialized;

    public:

        bool is_initialized() const noexcept;

        explicit iterator_data(
            unsigned int depth,
            tkey const &key,
            tvalue const &value);

        iterator_data();

        inline const tkey &get_key() const noexcept;

        inline const tvalue &get_value() const noexcept;

        /*iterator_data()
        : _key(reinterpret_cast<tkey *>(::operator new(sizeof(tkey)))),
        _value(reinterpret_cast<tvalue *>(::operator new(sizeof(tvalue)))),
        _is_initialized(false),
        depth(0)
        {}

        iterator_data(const iterator_data &other)
        : _key(reinterpret_cast<tkey *>(::operator new(sizeof(tkey)))),
        _value(reinterpret_cast<tvalue *>(::operator new(sizeof(tvalue)))),
        _is_initialized(other.is_initialized()),
        depth(other.depth)
        {
            allocator::construct(_key, *(other._key));
            allocator::construct(_value, *(other._value));
        }

        iterator_data(const iterator_data &&other) noexcept
        : _key(other._key),
        _value(other._value),
        _is_initialized(other._is_initialized),
        depth(other.depth)
        {
            other._key = nullptr;
            other._value = nullptr;
            other._is_initialized = false;
            other.depth = 0;
        }

        iterator_data& operator=(const iterator_data &other)
        {
            if(&other != this)
            {
                this->_key = other._key;
                this->_value = other._value;
                this->_is_initialized = other._is_initialized;
                this->depth = other.depth;
            }
            return *this;
        }

        iterator_data& operator=(iterator_data&& other) noexcept
        {
            if(&other != this)
            {
                this->key = other._key;
                this->_value = other._value;
                this->depth = other.depth;
                this->_is_initialized = other._is_initialized;

                other._key = nullptr;
                other._value = nullptr;
                other.depth = 0;
                other._is_initialized = false;
            }
            return *this;
        }

        virtual ~iterator_data() noexcept
        {
            if(_is_initialized)
            {
                allocator::destruct(_key);
                allocator::destruct(_value);
            }

            ::operator delete(_key);
            ::operator delete(_value);
            _key = nullptr;
            _value = nullptr;

            _is_initialized = false;
        }
         */
    };

    class prefix_iterator final
    {

        friend class binary_search_tree<tkey, tvalue>;

    private:

        iterator_data* _data;

        std::stack<node *> _path;

        binary_search_tree<tkey, tvalue> *_tree;

        void assign_data();

    public:

        explicit prefix_iterator(
            typename binary_search_tree<tkey, tvalue>::node *subtree_root,
            binary_search_tree<tkey, tvalue> *tree,
            iterator_data *data);

    public:

        bool operator==(
            prefix_iterator const &other) const noexcept;

        bool operator!=(
            prefix_iterator const &other) const noexcept;

        prefix_iterator &operator++();

        prefix_iterator const operator++(
            int not_used);

        iterator_data *operator*() const;

    };

    class prefix_const_iterator final
    {

        friend class binary_search_tree<tkey, tvalue>;

    private:

        iterator_data* _data;

        std::stack<node *> _path;

        binary_search_tree<tkey, tvalue> *_tree;

        void assign_data();

    public:

        explicit prefix_const_iterator(
            typename binary_search_tree<tkey, tvalue>::node *subtree_root,
            binary_search_tree<tkey, tvalue> *tree,
            iterator_data *data);

    public:

        bool operator==(
            prefix_const_iterator const &other) const noexcept;

        bool operator!=(
            prefix_const_iterator const &other) const noexcept;

        prefix_const_iterator &operator++();

        prefix_const_iterator const operator++(
            int not_used);

        iterator_data const *operator*() const;

    };

    class prefix_reverse_iterator final
    {

        friend class binary_search_tree<tkey, tvalue>;

    private:

        iterator_data* _data;

        std::stack<node *> _path;

        binary_search_tree<tkey, tvalue> *_tree;

        void assign_data();

    public:

        explicit prefix_reverse_iterator(
            typename binary_search_tree<tkey, tvalue>::node *subtree_root,
            binary_search_tree<tkey, tvalue> *tree,
            iterator_data *data);

    public:

        bool operator==(
            prefix_reverse_iterator const &other) const noexcept;

        bool operator!=(
            prefix_reverse_iterator const &other) const noexcept;

        prefix_reverse_iterator &operator++();

        prefix_reverse_iterator const operator++(
            int not_used);

        iterator_data *operator*() const;

    };

    class prefix_const_reverse_iterator final
    {

        friend class binary_search_tree<tkey, tvalue>;

    private:

        iterator_data* _data;

        std::stack<node *> _path;

        binary_search_tree<tkey, tvalue> *_tree;

        void assign_data();

    public:

        explicit prefix_const_reverse_iterator(
            typename binary_search_tree<tkey, tvalue>::node *subtree_root,
            binary_search_tree<tkey, tvalue> *tree,
            iterator_data *data);

    public:

        bool operator==(
            prefix_const_reverse_iterator const &other) const noexcept;

        bool operator!=(
            prefix_const_reverse_iterator const &other) const noexcept;

        prefix_const_reverse_iterator &operator++();

        prefix_const_reverse_iterator const operator++(
            int not_used);

        iterator_data const *operator*() const;

    };

    class infix_iterator final
    {

        friend class binary_search_tree<tkey, tvalue>;

    private:

        iterator_data* _data;

        std::stack<node *> _path;

        binary_search_tree<tkey, tvalue> *_tree;

        void assign_data();

    public:

        explicit infix_iterator(
            typename binary_search_tree<tkey, tvalue>::node *subtree_root,
            binary_search_tree<tkey, tvalue> *tree,
            iterator_data *data);

        explicit infix_iterator(
                std::stack<node *> path,
                binary_search_tree<tkey, tvalue> *tree,
                iterator_data * data)
                : _data(data), _path(path), _tree(tree)
        {
            if(_path.empty()) return;

            allocator::construct(_data->_key, _path.top()->key);
            allocator::construct(_data->_value, _path.top()->value);
            _data->_is_initialized = true;
            _data->depth = _path.size() - 1;
        }

    public:

        bool operator==(
            infix_iterator const &other) const noexcept;

        bool operator!=(
            infix_iterator const &other) const noexcept;

        infix_iterator &operator++();

        infix_iterator const operator++(
            int not_used);

        iterator_data *operator*() const;

    };

    class infix_const_iterator final
    {

        friend class binary_search_tree<tkey, tvalue>;

    private:

        iterator_data* _data;

        std::stack<node *> _path;

        binary_search_tree<tkey, tvalue> *_tree;

        void assign_data();

    public:

        explicit infix_const_iterator(
            typename binary_search_tree<tkey, tvalue>::node *subtree_root,
            binary_search_tree<tkey, tvalue> *tree,
            iterator_data *data);

    public:

        bool operator==(
            infix_const_iterator const &other) const noexcept;

        bool operator!=(
            infix_const_iterator const &other) const noexcept;

        infix_const_iterator &operator++();

        infix_const_iterator const operator++(
            int not_used);

        iterator_data const *operator*() const;

    };

    class infix_reverse_iterator final
    {

        friend class binary_search_tree<tkey, tvalue>;

    private:

        iterator_data* _data;

        std::stack<node *> _path;

        binary_search_tree<tkey, tvalue> *_tree;

        void assign_data();

    public:

        explicit infix_reverse_iterator(
            typename binary_search_tree<tkey, tvalue>::node *subtree_root,
            binary_search_tree<tkey, tvalue> *tree,
            iterator_data *data);

    public:

        bool operator==(
            infix_reverse_iterator const &other) const noexcept;

        bool operator!=(
            infix_reverse_iterator const &other) const noexcept;

        infix_reverse_iterator &operator++();

        infix_reverse_iterator const operator++(
            int not_used);

        iterator_data *operator*() const;

    };

    class infix_const_reverse_iterator final
    {

        friend class binary_search_tree<tkey, tvalue>;

    private:

        iterator_data* _data;

        std::stack<node *> _path;

        binary_search_tree<tkey, tvalue> *_tree;

        void assign_data();

    public:

        explicit infix_const_reverse_iterator(
            typename binary_search_tree<tkey, tvalue>::node *subtree_root,
            binary_search_tree<tkey, tvalue> *tree,
            iterator_data *data);

    public:

        bool operator==(
            infix_const_reverse_iterator const &other) const noexcept;

        bool operator!=(
            infix_const_reverse_iterator const &other) const noexcept;

        infix_const_reverse_iterator &operator++();

        infix_const_reverse_iterator const operator++(
            int not_used);

        iterator_data const *operator*() const;

    };

    class postfix_iterator final
    {

        friend class binary_search_tree<tkey, tvalue>;

    private:

        iterator_data* _data;

        std::stack<node *> _path;

        binary_search_tree<tkey, tvalue> *_tree;

        void assign_data();

    public:

        explicit postfix_iterator(
            typename binary_search_tree<tkey, tvalue>::node *subtree_root,
            binary_search_tree<tkey, tvalue> *tree,
            iterator_data *data);

    public:

        bool operator==(
            postfix_iterator const &other) const noexcept;

        bool operator!=(
            postfix_iterator const &other) const noexcept;

        postfix_iterator &operator++();

        postfix_iterator const operator++(
            int not_used);

        iterator_data *operator*() const;

    };

    class postfix_const_iterator final
    {

        friend class binary_search_tree<tkey, tvalue>;

    private:

        iterator_data* _data;

        std::stack<node *> _path;

        binary_search_tree<tkey, tvalue> *_tree;

        void assign_data();

    public:

        explicit postfix_const_iterator(
            typename binary_search_tree<tkey, tvalue>::node *subtree_root,
            binary_search_tree<tkey, tvalue> *tree,
            iterator_data *data);

    public:

        bool operator==(
            postfix_const_iterator const &other) const noexcept;

        bool operator!=(
            postfix_const_iterator const &other) const noexcept;

        postfix_const_iterator &operator++();

        postfix_const_iterator const operator++(
            int not_used);

        iterator_data const *operator*() const;

    };

    class postfix_reverse_iterator final
    {

        friend class binary_search_tree<tkey, tvalue>;

    private:

        iterator_data* _data;

        std::stack<node *> _path;

        binary_search_tree<tkey, tvalue> *_tree;

        void assign_data();

    public:

        explicit postfix_reverse_iterator(
            typename binary_search_tree<tkey, tvalue>::node *subtree_root,
            binary_search_tree<tkey, tvalue> *tree,
            iterator_data *data);

    public:

        bool operator==(
            postfix_reverse_iterator const &other) const noexcept;

        bool operator!=(
            postfix_reverse_iterator const &other) const noexcept;

        postfix_reverse_iterator &operator++();

        postfix_reverse_iterator const operator++(
            int not_used);

        iterator_data *operator*() const;

    };

    class postfix_const_reverse_iterator final
    {

        friend class binary_search_tree<tkey, tvalue>;

    private:

        iterator_data* _data;

        std::stack<node *> _path;

        binary_search_tree<tkey, tvalue> *_tree;

        void assign_data();

    public:

        explicit postfix_const_reverse_iterator(
            typename binary_search_tree<tkey, tvalue>::node *subtree_root,
            binary_search_tree<tkey, tvalue> *tree,
            iterator_data *data);

    public:

        bool operator==(
            postfix_const_reverse_iterator const &other) const noexcept;

        bool operator!=(
            postfix_const_reverse_iterator const &other) const noexcept;

        postfix_const_reverse_iterator &operator++();

        postfix_const_reverse_iterator const operator++(
            int not_used);

        iterator_data const *operator*() const;

    };

    // endregion iterators definition

protected:

    // region target operations strategies definition

    enum class insertion_of_existent_key_attempt_strategy
    {
        update_value,
        throw_an_exception
    };

    enum class disposal_of_nonexistent_key_attempt_strategy
    {
        do_nothing,
        throw_an_exception
    };

    // endregion target operations strategies definition

    // region target operations associated exception types

    class insertion_of_existent_key_attempt_exception final:
        public std::logic_error
    {

    private:

        tkey _key;

    public:

        explicit insertion_of_existent_key_attempt_exception(
            tkey const &key);

    public:

        tkey const &get_key() const noexcept;

    };

    class obtaining_of_nonexistent_key_attempt_exception final:
        public std::logic_error
    {

    private:

        tkey _key;

    public:

        explicit obtaining_of_nonexistent_key_attempt_exception(
            tkey const &key);

    public:

        tkey const &get_key() const noexcept;

    };

    class disposal_of_nonexistent_key_attempt_exception final:
        public std::logic_error
    {

    private:

        tkey _key;

    public:

        explicit disposal_of_nonexistent_key_attempt_exception(
            tkey const &key);

    public:

        tkey const &get_key() const noexcept;

    };

    // endregion target operations associated exception types

    // region template methods definition

    class template_method_basics:
        public logger_guardant
    {

    protected:

        binary_search_tree<tkey, tvalue> *_tree;

    public:

        explicit template_method_basics(
            binary_search_tree<tkey, tvalue> *tree);

    protected:

        std::stack<node **> find_path(
                const tkey &key) const
        {
            std::stack<node **> result_path;

            node **path_finder = &(_tree->_root);
            auto const &comparer = _tree->_keys_comparer;

            while(true) {
                result_path.push(path_finder);

                if (*path_finder == nullptr) {
                    break;
                }

                auto comparison_result = comparer(key, (*path_finder)->key);
                if (comparison_result == 0) {
                    break;
                }

                path_finder = comparison_result < 0
                              ? &((*path_finder)->left_subtree)
                              : &((*path_finder)->right_subtree);
            }
            return result_path;
        }

        virtual void balance(
                std::stack<node **> &path)
        {

        }
    protected:

        [[nodiscard]] logger *get_logger() const noexcept final;

    };

    class insertion_template_method:
        public template_method_basics,
        public allocator_guardant
    {

    private:

        binary_search_tree<tkey, tvalue>::insertion_of_existent_key_attempt_strategy _insertion_strategy;
        allocator *_allocator;
        logger *logger;
        //TODO: WOW
        //where in the fuck is '_tree' supposed to lead originally??
        //binary_search_tree<tkey, tvalue> *_tree;

    public:

        explicit insertion_template_method(
            binary_search_tree<tkey, tvalue> *tree,
            typename binary_search_tree<tkey, tvalue>::insertion_of_existent_key_attempt_strategy insertion_strategy);

    public:

        void insert(
            tkey const &key,
            tvalue const &value);

        void insert(
            tkey const &key,
            tvalue &&value);

        void set_insertion_strategy(
            typename binary_search_tree<tkey, tvalue>::insertion_of_existent_key_attempt_strategy insertion_strategy) noexcept;

    private:

        [[nodiscard]] allocator *get_allocator() const noexcept final;

    };

    class obtaining_template_method:
        public template_method_basics
    {

    private:
        //TODO: WOW again
        //binary_search_tree<tkey, tvalue> *_tree;

    public:

        explicit obtaining_template_method(
            binary_search_tree<tkey, tvalue> *tree);

    public:

        tvalue const &obtain(
            tkey const &key);

        std::vector<typename associative_container<tkey, tvalue>::key_value_pair> obtain_between(
                const tkey &lower_bound,
                const tkey &upper_bound,
                bool lower_bound_inclusive,
                bool upper_bound_inclusive)
        {
            std::vector<typename associative_container<tkey, tvalue>::key_value_pair> range;

            std::stack<node *> path;
            auto th = this;
            auto thtr = th->_tree;
            auto thtrr = thtr->_root;
            node *current = this->_tree->_root;
            while(true)
            {
                if(current == nullptr)
                {
                    break;
                }

                auto comparison_result = this->_tree->_keys_comparer(lower_bound, current->key);
                path.push(current);

                if (comparison_result == 0)
                {
                    if(lower_bound_inclusive)
                    {
                        break;
                    }

                    current = current->right_subtree;
                }
                else if (comparison_result < 0)
                {
                    current = current->left_subtree;
                }
                else
                {
                    current = current->right_subtree;
                }

                if(current == nullptr && this->_tree->_keys_comparer(path.top()->key, lower_bound) < (lower_bound_inclusive ? 0 : 1))
                {
                    path = std::move(std::stack<node *>());
                }
            }

            auto it = infix_iterator(path, this->_tree, this->_tree->create_iterator_data());
            while((it != this->_tree->end_infix()) && (this->_tree->_keys_comparer(upper_bound,(*it)->get_key()) > (upper_bound_inclusive ? -1 : 0)))
            {
                range.push_back(std::move(typename associative_container<tkey, tvalue>::key_value_pair((*it)->get_key(), (*it)->get_value())));
                ++it;
            }

            return range;
        }

    };

    class disposal_template_method:
        public template_method_basics,
        public allocator_guardant
    {

    protected:
        binary_search_tree<tkey, tvalue>::disposal_of_nonexistent_key_attempt_strategy _disposal_strategy;

    public:

        explicit disposal_template_method(
            binary_search_tree<tkey, tvalue> *tree,
            typename binary_search_tree<tkey, tvalue>::disposal_of_nonexistent_key_attempt_strategy disposal_strategy);

    public:

        tvalue dispose(
            tkey const &key);

        void set_disposal_strategy(
            typename binary_search_tree<tkey, tvalue>::disposal_of_nonexistent_key_attempt_strategy disposal_strategy) noexcept;

    protected:

        template<typename T>
        inline void swap(
                T &&a,
                T &&b)
        {
            T tmp = std::move(a);
            a = std::move(b);
            b = std::move(tmp);
        }

    private:

        [[nodiscard]] allocator *get_allocator() const noexcept final;

    };

    // endregion template methods definition

private:

    node *_root;

    insertion_template_method *_insertion_template;

    obtaining_template_method *_obtaining_template;

    disposal_template_method *_disposal_template;

protected:

    explicit binary_search_tree(
        typename binary_search_tree<tkey, tvalue>::insertion_template_method *insertion_template,
        typename binary_search_tree<tkey, tvalue>::obtaining_template_method *obtaining_template,
        typename binary_search_tree<tkey, tvalue>::disposal_template_method *disposal_template,
        std::function<int(tkey const &, tkey const &)>,
        allocator *allocator,
        logger *logger);

public:

    explicit binary_search_tree(
        std::function<int(tkey const &, tkey const &)> comparer = std::less<tkey>(),
        allocator *allocator = nullptr,
        logger *logger = nullptr,
        typename binary_search_tree<tkey, tvalue>::insertion_of_existent_key_attempt_strategy insertion_strategy = binary_search_tree<tkey, tvalue>::insertion_of_existent_key_attempt_strategy::throw_an_exception,
        typename binary_search_tree<tkey, tvalue>::disposal_of_nonexistent_key_attempt_strategy disposal_strategy = binary_search_tree<tkey, tvalue>::disposal_of_nonexistent_key_attempt_strategy::throw_an_exception);

public:

    binary_search_tree(
        binary_search_tree<tkey, tvalue> const &other);

    binary_search_tree(
        binary_search_tree<tkey, tvalue> &&other) noexcept;

    binary_search_tree<tkey, tvalue> &operator=(
        binary_search_tree<tkey, tvalue> const &other);

    binary_search_tree<tkey, tvalue> &operator=(
        binary_search_tree<tkey, tvalue> &&other) noexcept;

    ~binary_search_tree() override;

protected:

    void clear(
            node *&subtree_root)
    {
        if(!subtree_root) return;

        clear(subtree_root->left_subtree);
        clear(subtree_root->right_subtree);
        subtree_root->~node();
        this->deallocate_with_guard(subtree_root);

        subtree_root = nullptr;
    }

    node *copy(
            const node *subtree_root)
    {
        if(!subtree_root) return nullptr;

        node *cpy = reinterpret_cast<node *>(this->allocate_with_guard(get_node_size(), 1));
        call_node_constructor(cpy, subtree_root->key, subtree_root->value);
        cpy->left_subtree = copy(subtree_root->left_subtree);
        cpy->right_subtree = cpy(subtree_root->right_subtree);

        return cpy;
    }

    virtual size_t get_node_size() const noexcept
    {
        return sizeof(typename binary_search_tree<tkey, tvalue>::node);
    }

    virtual void call_node_constructor(
            node *raw_space,
            const tkey &key,
            const tvalue &value)
    {
        allocator::construct(raw_space, key, value);
    }

    virtual void call_node_constructor(
            node *raw_space,
            const tkey &key,
            const tvalue &&value)
    {
        allocator::construct(raw_space, key, std::move(value));
    }

    void insert(
        tkey const &key,
        tvalue const &value) final;

    void insert(
        tkey const &key,
        tvalue &&value) final;

    tvalue const &obtain(
        tkey const &key) final;

    std::vector<typename associative_container<tkey, tvalue>::key_value_pair> obtain_between(
        tkey const &lower_bound,
        tkey const &upper_bound,
        bool lower_bound_inclusive,
        bool upper_bound_inclusive) final;

    tvalue dispose(
        tkey const &key) final;

public:

    void set_insertion_strategy(
        typename binary_search_tree<tkey, tvalue>::insertion_of_existent_key_attempt_strategy insertion_strategy) noexcept;

    void set_removal_strategy(
        typename binary_search_tree<tkey, tvalue>::disposal_of_nonexistent_key_attempt_strategy disposal_strategy) noexcept;

    virtual iterator_data *create_iterator_data() const
    {
        return new iterator_data();
    }
public:

    // region iterators requests definition

    prefix_iterator begin_prefix() const noexcept;

    prefix_iterator end_prefix() const noexcept;

    prefix_const_iterator cbegin_prefix() const noexcept;

    prefix_const_iterator cend_prefix() const noexcept;

    prefix_reverse_iterator rbegin_prefix() const noexcept;

    prefix_reverse_iterator rend_prefix() const noexcept;

    prefix_const_reverse_iterator crbegin_prefix() const noexcept;

    prefix_const_reverse_iterator crend_prefix() const noexcept;

    infix_iterator begin_infix() const noexcept;

    infix_iterator end_infix() const noexcept;

    infix_const_iterator cbegin_infix() const noexcept;

    infix_const_iterator cend_infix() const noexcept;

    infix_reverse_iterator rbegin_infix() const noexcept;

    infix_reverse_iterator rend_infix() const noexcept;

    infix_const_reverse_iterator crbegin_infix() const noexcept;

    infix_const_reverse_iterator crend_infix() const noexcept;

    postfix_iterator begin_postfix() const noexcept;

    postfix_iterator end_postfix() const noexcept;

    postfix_const_iterator cbegin_postfix() const noexcept;

    postfix_const_iterator cend_postfix() const noexcept;

    postfix_reverse_iterator rbegin_postfix() const noexcept;

    postfix_reverse_iterator rend_postfix() const noexcept;

    postfix_const_reverse_iterator crbegin_postfix() const noexcept;

    postfix_const_reverse_iterator crend_postfix() const noexcept;

    // endregion iterators requests definition

protected:

    // region subtree rotations definition

    void small_left_rotation(
        typename binary_search_tree<tkey, tvalue>::node *&subtree_root,
        bool validate = true) const;

    void small_right_rotation(
        typename binary_search_tree<tkey, tvalue>::node *&subtree_root,
        bool validate = true) const;

    void big_left_rotation(
        typename binary_search_tree<tkey, tvalue>::node *&subtree_root,
        bool validate = true) const;

    void big_right_rotation(
        typename binary_search_tree<tkey, tvalue>::node *&subtree_root,
        bool validate = true) const;

    void double_left_rotation(
        typename binary_search_tree<tkey, tvalue>::node *&subtree_root,
        bool at_grandparent_first,
        bool validate = true) const;

    void double_right_rotation(
        typename binary_search_tree<tkey, tvalue>::node *&subtree_root,
        bool at_grandparent_first,
        bool validate = true) const;

    // endregion subtree rotations definition

};

// region binary_search_tree<tkey, tvalue>::node methods implementation

template<
    typename tkey,
    typename tvalue>
binary_search_tree<tkey, tvalue>::node::node(
    tkey const &key,
    tvalue const &value)
    : key(key),
    value(value),
    left_subtree(nullptr),
    right_subtree(nullptr)
{
}

template<
    typename tkey,
    typename tvalue>
binary_search_tree<tkey, tvalue>::node::node(
    tkey const &key,
    tvalue &&value)
    : key(key),
    value(std::move(value)),
    left_subtree(nullptr),
    right_subtree(nullptr)
{
}

// endregion binary_search_tree<tkey, tvalue>::node methods implementation

// region iterators implementation

// region iterator data implementation

template<
        typename tkey,
        typename tvalue>
bool binary_search_tree<tkey, tvalue>::iterator_data::is_initialized() const noexcept
{
    return _is_initialized;
}

template<
        typename tkey,
        typename tvalue>
inline const tkey & binary_search_tree<tkey, tvalue>::iterator_data::get_key() const noexcept
{
    if(is_initialized()) return *_key;
    throw std::logic_error("uninitialized");
}


template<
        typename tkey,
        typename tvalue>
inline const tvalue & binary_search_tree<tkey, tvalue>::iterator_data::get_value() const noexcept
{
    if(is_initialized()) return *_value;
    throw std::logic_error("uninitialized");
}

template<
        typename tkey,
        typename tvalue>
binary_search_tree<tkey, tvalue>::iterator_data::iterator_data()
    : _key(reinterpret_cast<tkey *>(::operator new(sizeof(tkey)))),
      _value(reinterpret_cast<tvalue *>(::operator new(sizeof(tvalue)))),
      _is_initialized(false),
      depth(0)
{

}

template<
    typename tkey,
    typename tvalue>
binary_search_tree<tkey, tvalue>::iterator_data::iterator_data(
    unsigned int depth,
    tkey const &key,
    tvalue const &value)
    : depth(depth),
    _key(reinterpret_cast<tkey *>(::operator new(sizeof(tkey)))),
    _value(reinterpret_cast<tvalue *>(::operator new(sizeof(tvalue)))),
    _is_initialized(true)
{
    allocator::construct( _key, key);
    allocator::construct(_value, value);
}

// endregion iterator data implementation

// region prefix_iterator implementation

template<
        typename tkey,
        typename tvalue>
void binary_search_tree<tkey, tvalue>::prefix_iterator::assign_data()
{
    if(_data->is_initialized())
    {
        *(_data->_key) = _path.top()->key;
        *(_data->_value) = _path.top()->value;
    }
    else
    {
        allocator::construct(_data->_key, _path.top()->key);
        allocator::construct(_data->_value, _path.top()->value);
        _data->_is_initialized = true;
    }

    _data->depth = _path.size() - 1;
    //TODO: virtual void bst::inject_additional_data()...
}

template<
        typename tkey,
        typename tvalue>
binary_search_tree<tkey, tvalue>::prefix_iterator::prefix_iterator(
    typename binary_search_tree<tkey, tvalue>::node *subtree_root,
    binary_search_tree<tkey, tvalue> *tree,
    iterator_data *data)
    : _tree(tree), _data(data)
{
    if(subtree_root)
    {
        _path.push(subtree_root);
        allocator::construct(_data->_key, _path.top()->key);
        allocator::construct(_data->_value, _path.top()->value);
        //TODO: inject data or smth
        _data->depth = 0;
        _data->_is_initialized = true;
    }
}

template<
    typename tkey,
    typename tvalue>
bool binary_search_tree<tkey, tvalue>::prefix_iterator::operator==(
    typename binary_search_tree<tkey, tvalue>::prefix_iterator const &other) const noexcept
{
    if(this->_path.empty() && other._path.empty()) return true;

    if(this->_path.empty() ^ other._path.empty()) return false;

    return (this->_path.top() == other._path.top());
}

template<
    typename tkey,
    typename tvalue>
bool binary_search_tree<tkey, tvalue>::prefix_iterator::operator!=(
    typename binary_search_tree<tkey, tvalue>::prefix_iterator const &other) const noexcept
{
    return !(*this == other);
}

template<
    typename tkey,
    typename tvalue>
typename binary_search_tree<tkey, tvalue>::prefix_iterator &binary_search_tree<tkey, tvalue>::prefix_iterator::operator++()
{
    if(_path.empty())
    {
        throw std::logic_error("incrementing empty iterator");
    }

    node *current = _path.top();
    _path.pop();

    if(current->right_subtree)
    {
        _path.push(current->right_subtree);
    }

    if(current->left_subtree)
    {
        _path.push(current->left_subtree);
    }

    if(current->right_subtree || current->left_subtree) _data->depth++;

    if(!_path.empty()) assign_data();

    return *this;
}

template<
    typename tkey,
    typename tvalue>
typename binary_search_tree<tkey, tvalue>::prefix_iterator const binary_search_tree<tkey, tvalue>::prefix_iterator::operator++(
    int not_used)
{
    auto it = *this;
    ++(*this);
    return it;
}

template<
    typename tkey,
    typename tvalue>
typename binary_search_tree<tkey, tvalue>::iterator_data *binary_search_tree<tkey, tvalue>::prefix_iterator::operator*() const
{
    if(_path.empty()) throw std::logic_error("dereferencing uninitialized iterator");
    return _data;
}

// endregion prefix_iterator implementation

// region prefix_const_iterator implementation
template<
        typename tkey,
        typename tvalue>
void binary_search_tree<tkey, tvalue>::prefix_const_iterator::assign_data()
{
    if(_data->is_initialized())
    {
        *(_data->_key) = _path.top()->key;
        *(_data->_value) = _path.top()->value;
    }
    else
    {
        allocator::construct(_data->_key, _path.top()->key);
        allocator::construct(_data->_value, _path.top()->value);
        _data->_is_initialized = true;
    }

    //TODO: virtual void bst::inject_additional_data()...
}

template<
    typename tkey,
    typename tvalue>
binary_search_tree<tkey, tvalue>::prefix_const_iterator::prefix_const_iterator(
    typename binary_search_tree<tkey, tvalue>::node *subtree_root,
    binary_search_tree<tkey, tvalue> *tree,
    iterator_data *data)
    : _tree(tree), _data(data)
{
        if(subtree_root)
        {
            _path.push(subtree_root);
            allocator::construct(_data->_key, _path.top()->key);
            allocator::construct(_data->_value, _path.top()->value);
            //TODO: inject data or smth
            _data->depth = 0;
            _data->_is_initialized = true;
        }
}

template<
    typename tkey,
    typename tvalue>
bool binary_search_tree<tkey, tvalue>::prefix_const_iterator::operator==(
    typename binary_search_tree<tkey, tvalue>::prefix_const_iterator const &other) const noexcept
{
    if(this->_path.empty() && other._path.empty()) return true;

    if(this->_path.empty() ^ other._path.empty()) return false;

    return (this->_path.top() == other._path.top());
}

template<
    typename tkey,
    typename tvalue>
bool binary_search_tree<tkey, tvalue>::prefix_const_iterator::operator!=(
    typename binary_search_tree<tkey, tvalue>::prefix_const_iterator const &other) const noexcept
{
    return !(*this == other);
}

template<
    typename tkey,
    typename tvalue>
typename binary_search_tree<tkey, tvalue>::prefix_const_iterator &binary_search_tree<tkey, tvalue>::prefix_const_iterator::operator++()
{
    if(_path.empty())
    {
        throw std::logic_error("incrementing empty iterator");
    }

    node *current = _path.top();
    _path.pop();

    if(current->right_subtree)
    {
        _path.push(current->right_subtree);
    }

    if(current->left_subtree)
    {
        _path.push(current->left_subtree);
    }


    if(current->right_subtree || current->left_subtree)
    {
        _data->depth++;
    }

    if(!_path.empty()) assign_data();

    return *this;
}

template<
    typename tkey,
    typename tvalue>
typename binary_search_tree<tkey, tvalue>::prefix_const_iterator const binary_search_tree<tkey, tvalue>::prefix_const_iterator::operator++(
    int not_used)
{
    auto it = *this;
    ++(*this);
    return it;
}

template<
    typename tkey,
    typename tvalue>
typename binary_search_tree<tkey, tvalue>::iterator_data const *binary_search_tree<tkey, tvalue>::prefix_const_iterator::operator*() const
{
    if(_path.empty()) throw std::logic_error("dereferencing uninitialized iterator");
    return _data;
}

// endregion prefix_const_iterator implementation

// region prefix_reverse_iterator implementation

template<
        typename tkey,
        typename tvalue>
void binary_search_tree<tkey, tvalue>::prefix_reverse_iterator::assign_data()
{
    if(_data->is_initialized())
    {
        *(_data->_key) = _path.top()->key;
        *(_data->_value) = _path.top()->value;
    }
    else
    {
        allocator::construct(_data->_key, _path.top()->key);
        allocator::construct(_data->_value, _path.top()->value);
        _data->_is_initialized = true;
    }

    _data->depth = _path.size() - 1;
    //TODO: virtual void bst::inject_additional_data()...
}

template<
    typename tkey,
    typename tvalue>
binary_search_tree<tkey, tvalue>::prefix_reverse_iterator::prefix_reverse_iterator(
    typename binary_search_tree<tkey, tvalue>::node *subtree_root,
    binary_search_tree<tkey, tvalue> *tree,
    iterator_data *data)
    : _tree(tree), _data(data)
{
    if(subtree_root)
    {
        _path.push(subtree_root);
        allocator::construct(_data->_key, _path.top()->key);
        allocator::construct(_data->_value, _path.top()->value);
        //TODO: inject data or smth
        _data->depth = 0;
        _data->_is_initialized = true;
    }
}


template<
    typename tkey,
    typename tvalue>
bool binary_search_tree<tkey, tvalue>::prefix_reverse_iterator::operator==(
    typename binary_search_tree<tkey, tvalue>::prefix_reverse_iterator const &other) const noexcept
{
    if(this->_path.empty() && other._path.empty()) return true;

    if(this->_path.empty() ^ other._path.empty()) return false;

    return (this->_path.top() == other._path.top());
}

template<
    typename tkey,
    typename tvalue>
bool binary_search_tree<tkey, tvalue>::prefix_reverse_iterator::operator!=(
    typename binary_search_tree<tkey, tvalue>::prefix_reverse_iterator const &other) const noexcept
{
    return !(*this == other);
}

template<
    typename tkey,
    typename tvalue>
typename binary_search_tree<tkey, tvalue>::prefix_reverse_iterator &binary_search_tree<tkey, tvalue>::prefix_reverse_iterator::operator++()
{
    if(_path.empty())
    {
        throw std::logic_error("incrementing empty iterator");
    }

    node *current = _path.top();
    _path.pop();

    if(current->left_subtree)
    {
        _path.push(current->left_subtree);
    }

    if(current->right_subtree)
    {
        _path.push(current->right_subtree);
    }

    if(current->right_subtree || current->left_subtree) _data->depth++;

    if(!_path.empty()) assign_data();

    return *this;
}

template<
    typename tkey,
    typename tvalue>
typename binary_search_tree<tkey, tvalue>::prefix_reverse_iterator const binary_search_tree<tkey, tvalue>::prefix_reverse_iterator::operator++(
    int not_used)
{
    auto it = *this;
    ++(*this);
    return it;
}

template<
    typename tkey,
    typename tvalue>
typename binary_search_tree<tkey, tvalue>::iterator_data *binary_search_tree<tkey, tvalue>::prefix_reverse_iterator::operator*() const
{
    if(_path.empty()) throw std::logic_error("dereferencing empty iterator");
    return _data;
}

// endregion prefix_reverse_iterator implementation

// region prefix_const_reverse_iterator implementation

template<
        typename tkey,
        typename tvalue>
void binary_search_tree<tkey, tvalue>::prefix_const_reverse_iterator::assign_data()
{
    if(_data->is_initialized())
    {
        *(_data->_key) = _path.top()->key;
        *(_data->_value) = _path.top()->value;
    }
    else
    {
        allocator::construct(_data->_key, _path.top()->key);
        allocator::construct(_data->_value, _path.top()->value);
        _data->_is_initialized = true;
    }

    _data->depth = _path.size() - 1;
    //TODO: virtual void bst::inject_additional_data()...
}

template<
    typename tkey,
    typename tvalue>
binary_search_tree<tkey, tvalue>::prefix_const_reverse_iterator::prefix_const_reverse_iterator(
    typename binary_search_tree<tkey, tvalue>::node *subtree_root,
    binary_search_tree<tkey, tvalue> *tree,
    iterator_data *data)
        : _tree(tree), _data(data)
{
    if(subtree_root)
    {
        _path.push(subtree_root);
        allocator::construct(_data->_key, _path.top()->key);
        allocator::construct(_data->_value, _path.top()->value);
        //TODO: inject data or smth
        _data->depth = 0;
        _data->_is_initialized = true;
    }
}

template<
    typename tkey,
    typename tvalue>
bool binary_search_tree<tkey, tvalue>::prefix_const_reverse_iterator::operator==(
    typename binary_search_tree<tkey, tvalue>::prefix_const_reverse_iterator const &other) const noexcept
{
    if(this->_path.empty() && other._path.empty()) return true;

    if(this->_path.empty() ^ other._path.empty()) return false;

    return (this->_path.top() == other._path.top());
}

template<
    typename tkey,
    typename tvalue>
bool binary_search_tree<tkey, tvalue>::prefix_const_reverse_iterator::operator!=(
    typename binary_search_tree<tkey, tvalue>::prefix_const_reverse_iterator const &other) const noexcept
{
    return !(*this == other);
}

template<
    typename tkey,
    typename tvalue>
typename binary_search_tree<tkey, tvalue>::prefix_const_reverse_iterator &binary_search_tree<tkey, tvalue>::prefix_const_reverse_iterator::operator++()
{
    if(_path.empty())
    {
        throw std::logic_error("incrementing empty iterator");
    }

    node *current = _path.top();
    _path.pop();

    if(current->left_subtree)
    {
        _path.push(current->left_subtree);
    }

    if(current->right_subtree)
    {
        _path.push(current->right_subtree);
    }

    if(current->right_subtree || current->left_subtree) _data->depth++;

    if(!_path.empty()) assign_data();

    return *this;
}

template<
    typename tkey,
    typename tvalue>
typename binary_search_tree<tkey, tvalue>::prefix_const_reverse_iterator const binary_search_tree<tkey, tvalue>::prefix_const_reverse_iterator::operator++(
    int not_used)
{
    auto it = *this;
    ++(*this);
    return it;
}

template<
    typename tkey,
    typename tvalue>
typename binary_search_tree<tkey, tvalue>::iterator_data const *binary_search_tree<tkey, tvalue>::prefix_const_reverse_iterator::operator*() const
{
    if(_path.empty()) throw std::logic_error("dereferencing uninitialized iterator");
    return _data;
}

// endregion prefix_const_reverse_iterator implementation

// region infix_iterator implementation

template<
        typename tkey,
        typename tvalue>
void binary_search_tree<tkey, tvalue>::infix_iterator::assign_data()
{
    if(_data->is_initialized())
    {
        *(_data->_key) = _path.top()->key;
        *(_data->_value) = _path.top()->value;
    }
    else
    {
        allocator::construct(_data->_key, _path.top()->key);
        allocator::construct(_data->_value, _path.top()->value);
        _data->_is_initialized = true;
    }

    _data->depth = _path.size() - 1;
    //TODO: virtual void bst::inject_additional_data()...
}

template<
    typename tkey,
    typename tvalue>
binary_search_tree<tkey, tvalue>::infix_iterator::infix_iterator(
    typename binary_search_tree<tkey, tvalue>::node *subtree_root,
    binary_search_tree<tkey, tvalue> *tree,
    iterator_data *data)
    : _tree(tree), _data(data)
{
    while(subtree_root)
    {
        _path.push(subtree_root);
        subtree_root = subtree_root->left_subtree;
    }

    if(_path.empty())
    {
        return;
    }

    allocator::construct(_data->_key, _path.top()->key);
    allocator::construct(_data->_value, _path.top()->value);
    //TODO: inject additional
    _data->depth = _path.size() - 1;
    _data->_is_initialized = true;
}

template<
    typename tkey,
    typename tvalue>
bool binary_search_tree<tkey, tvalue>::infix_iterator::operator==(
    typename binary_search_tree<tkey, tvalue>::infix_iterator const &other) const noexcept
{
    if(this->_path.empty() && other._path.empty()) return true;

    if(this->_path.empty() ^ other._path.empty()) return false;

    return (this->_path.top() == other._path.top());
}

template<
    typename tkey,
    typename tvalue>
bool binary_search_tree<tkey, tvalue>::infix_iterator::operator!=(
    typename binary_search_tree<tkey, tvalue>::infix_iterator const &other) const noexcept
{
    return !(*this == other);
}

template<
    typename tkey,
    typename tvalue>
typename binary_search_tree<tkey, tvalue>::infix_iterator &binary_search_tree<tkey, tvalue>::infix_iterator::operator++()
{
    if(_path.empty())
    {
        throw std::logic_error("incrementing empty iterator");
    }

    if(_path.top()->right_subtree)
    {
        _path.push(_path.top()->right_subtree);

        while(_path.top()->left_subtree)
        {
            _path.push(_path.top()->left_subtree);
        }

        assign_data();
    }
    else
    {
        node *current;
        do
        {
            current = _path.top();
            _path.pop();

            if(_path.empty())
            {
                return *this;
            }

            if(_path.top()->left_subtree == current)
            {
                assign_data();
                return *this;
            }
        }
        while(true);
    }
    return *this;
}

template<
    typename tkey,
    typename tvalue>
typename binary_search_tree<tkey, tvalue>::infix_iterator const binary_search_tree<tkey, tvalue>::infix_iterator::operator++(
    int not_used)
{
    auto it = *this;
    ++(*this);
    return it;
}

template<
    typename tkey,
    typename tvalue>
typename binary_search_tree<tkey, tvalue>::iterator_data *binary_search_tree<tkey, tvalue>::infix_iterator::operator*() const
{
    if(_path.empty()) throw std::logic_error("dereferencing uninitialized iterator");
    return _data;
}

// endregion infix_iterator implementation

// region infix_const_iterator implementation

template<
        typename tkey,
        typename tvalue>
void binary_search_tree<tkey, tvalue>::infix_const_iterator::assign_data()
{
    if(_data->is_initialized())
    {
        *(_data->_key) = _path.top()->key;
        *(_data->_value) = _path.top()->value;
    }
    else
    {
        allocator::construct(_data->_key, _path.top()->key);
        allocator::construct(_data->_value, _path.top()->value);
        _data->_is_initialized = true;
    }

    _data->depth = _path.size() - 1;
    //TODO: virtual void bst::inject_additional_data()...
}
template<
    typename tkey,
    typename tvalue>
binary_search_tree<tkey, tvalue>::infix_const_iterator::infix_const_iterator(
    typename binary_search_tree<tkey, tvalue>::node *subtree_root,
    binary_search_tree<tkey, tvalue> *tree,
    iterator_data *data)
        : _tree(tree), _data(data)
{
    while(subtree_root)
    {
        _path.push(subtree_root);
        subtree_root = subtree_root->left_subtree;
    }

    if(_path.empty())
    {
        return;
    }

    allocator::construct(_data->_key, _path.top()->key);
    allocator::construct(_data->_value, _path.top()->value);
    //TODO: inject additional
    _data->depth = _path.size() - 1;
    _data->_is_initialized = true;
}

template<
    typename tkey,
    typename tvalue>
bool binary_search_tree<tkey, tvalue>::infix_const_iterator::operator==(
    typename binary_search_tree<tkey, tvalue>::infix_const_iterator const &other) const noexcept
{
    if(this->_path.empty() && other._path.empty()) return true;

    if(this->_path.empty() ^ other._path.empty()) return false;

    return (this->_path.top() == other._path.top());
}

template<
    typename tkey,
    typename tvalue>
bool binary_search_tree<tkey, tvalue>::infix_const_iterator::operator!=(
    typename binary_search_tree<tkey, tvalue>::infix_const_iterator const &other) const noexcept
{
    return !(*this == other);
}

template<
    typename tkey,
    typename tvalue>
typename binary_search_tree<tkey, tvalue>::infix_const_iterator &binary_search_tree<tkey, tvalue>::infix_const_iterator::operator++()
{
    if(_path.empty())
    {
        throw std::logic_error("incrementing empty iterator");
    }

    if(_path.top()->right_subtree)
    {
        _path.push(_path.top()->right_subtree);

        while(_path.top()->left_subtree)
        {
            _path.push(_path.top()->left_subtree);
        }

        assign_data();
    }
    else
    {
        node *current;
        do
        {
            current = _path.top();
            _path.pop();

            if(_path.empty())
            {
                return *this;
            }

            if(_path.top()->left_subtree == current)
            {
                assign_data();
                return *this;
            }
        }
        while(true);
    }
    return *this;
}

template<
    typename tkey,
    typename tvalue>
typename binary_search_tree<tkey, tvalue>::infix_const_iterator const binary_search_tree<tkey, tvalue>::infix_const_iterator::operator++(
    int not_used)
{
    auto it = *this;
    ++(*this);
    return it;
}

template<
    typename tkey,
    typename tvalue>
typename binary_search_tree<tkey, tvalue>::iterator_data const *binary_search_tree<tkey, tvalue>::infix_const_iterator::operator*() const
{
    if(_path.empty()) throw std::logic_error("dereferencing uninitialized iterator");
    return _data;
}

// endregion infix_const_iterator implementation

// region infix_reverse_iterator implementation

template<
        typename tkey,
        typename tvalue>
void binary_search_tree<tkey, tvalue>::infix_reverse_iterator::assign_data()
{
    if(_data->is_initialized())
    {
        *(_data->_key) = _path.top()->key;
        *(_data->_value) = _path.top()->value;
    }
    else
    {
        allocator::construct(_data->_key, _path.top()->key);
        allocator::construct(_data->_value, _path.top()->value);
        _data->_is_initialized = true;
    }

    _data->depth = _path.size() - 1;
    //TODO: virtual void bst::inject_additional_data()...
}

template<
    typename tkey,
    typename tvalue>
binary_search_tree<tkey, tvalue>::infix_reverse_iterator::infix_reverse_iterator(
    typename binary_search_tree<tkey, tvalue>::node *subtree_root,
    binary_search_tree<tkey, tvalue> *tree,
    iterator_data *data)
    : _tree(tree), _data(data)
{
    while(subtree_root)
    {
        _path.push(subtree_root);
        subtree_root = subtree_root->right_subtree;
    }

    if(_path.empty())
    {
        return;
    }

    allocator::construct(_data->_key, _path.top().key);
    allocator::construct(_data->_value, _path.top().value);
    //TODO: inject additional
    _data->depth = _path.size() - 1;
    _data->_is_initialized = true;
}

template<
    typename tkey,
    typename tvalue>
bool binary_search_tree<tkey, tvalue>::infix_reverse_iterator::operator==(
    typename binary_search_tree<tkey, tvalue>::infix_reverse_iterator const &other) const noexcept
{
    if(this->_path.empty() && other._path.empty()) return true;

    if(this->_path.empty() ^ other._path.empty()) return false;

    return (this->_path.top() == other._path.top());
}

template<
    typename tkey,
    typename tvalue>
bool binary_search_tree<tkey, tvalue>::infix_reverse_iterator::operator!=(
    typename binary_search_tree<tkey, tvalue>::infix_reverse_iterator const &other) const noexcept
{
    return !(*this == other);
}

template<
    typename tkey,
    typename tvalue>
typename binary_search_tree<tkey, tvalue>::infix_reverse_iterator &binary_search_tree<tkey, tvalue>::infix_reverse_iterator::operator++()
{
    if(_path.empty())
    {
        throw std::logic_error("incrementing empty iterator");
    }

    if(_path.top()->left_subtree)
    {
        _path.push(_path.top()->left_subtree);

        while(_path.top()->right_subtree)
        {
            _path.push(_path.top()->right_subtree);
        }

        assign_data();
    }
    else
    {
        node *current;
        do
        {
            current = _path.top();
            _path.pop();

            if(_path.empty())
            {
                return *this;
            }

            if(_path.top()->right_subtree == current)
            {
                assign_data();
                return *this;
            }
        }
        while(true);
    }
    return *this;
}

template<
    typename tkey,
    typename tvalue>
typename binary_search_tree<tkey, tvalue>::infix_reverse_iterator const binary_search_tree<tkey, tvalue>::infix_reverse_iterator::operator++(
    int not_used)
{
    auto it = *this;
    ++(*this);
    return it;
}

template<
    typename tkey,
    typename tvalue>
typename binary_search_tree<tkey, tvalue>::iterator_data *binary_search_tree<tkey, tvalue>::infix_reverse_iterator::operator*() const
{
    if(_path.empty()) throw std::logic_error("dereferencing uninitialized iterator");
    return _data;
}

// endregion infix_reverse_iterator implementation

// region infix_const_reverse_iterator implementation

template<
        typename tkey,
        typename tvalue>
void binary_search_tree<tkey, tvalue>::infix_const_reverse_iterator::assign_data()
{
    if(_data->is_initialized())
    {
        *(_data->_key) = _path.top()->key;
        *(_data->_value) = _path.top()->value;
    }
    else
    {
        allocator::construct(_data->_key, _path.top()->key);
        allocator::construct(_data->_value, _path.top()->value);
        _data->_is_initialized = true;
    }

    _data->depth = _path.size() - 1;
    //TODO: virtual void bst::inject_additional_data()...
}

template<
    typename tkey,
    typename tvalue>
binary_search_tree<tkey, tvalue>::infix_const_reverse_iterator::infix_const_reverse_iterator(
    typename binary_search_tree<tkey, tvalue>::node *subtree_root,
    binary_search_tree<tkey, tvalue> *tree,
    iterator_data *data)
    : _tree(tree), _data(data)
{
    while(subtree_root)
    {
        _path.push(subtree_root);
        subtree_root = subtree_root->right_subtree;
    }

    if(_path.empty())
    {
        return;
    }

    allocator::construct(_data->_key, _path.top()->key);
    allocator::construct(_data->_value, _path.top()->value);
    //TODO: inject additional
    _data->depth = _path.size() - 1;
    _data->_is_initialized = true;
}

template<
    typename tkey,
    typename tvalue>
bool binary_search_tree<tkey, tvalue>::infix_const_reverse_iterator::operator==(
    typename binary_search_tree<tkey, tvalue>::infix_const_reverse_iterator const &other) const noexcept
{
    if(this->_path.empty() && other._path.empty()) return true;

    if(this->_path.empty() ^ other._path.empty()) return false;

    return (this->_path.top() == other._path.top());
}

template<
    typename tkey,
    typename tvalue>
bool binary_search_tree<tkey, tvalue>::infix_const_reverse_iterator::operator!=(
    typename binary_search_tree<tkey, tvalue>::infix_const_reverse_iterator const &other) const noexcept
{
    return !(*this == other);
}

template<
    typename tkey,
    typename tvalue>
typename binary_search_tree<tkey, tvalue>::infix_const_reverse_iterator &binary_search_tree<tkey, tvalue>::infix_const_reverse_iterator::operator++()
{
    if(_path.empty())
    {
        throw std::logic_error("incrementing empty iterator");
    }

    if(_path.top()->left_subtree)
    {
        _path.push(_path.top()->left_subtree);

        while(_path.top()->right_subtree)
        {
            _path.push(_path.top()->right_subtree);
        }

        assign_data();
    }
    else
    {
        node *current;
        do
        {
            current = _path.top();
            _path.pop();

            if(_path.empty())
            {
                return *this;
            }

            if(_path.top()->right_subtree == current)
            {
                assign_data();
                return *this;
            }
        }
        while(true);
    }
    return *this;
}

template<
    typename tkey,
    typename tvalue>
typename binary_search_tree<tkey, tvalue>::infix_const_reverse_iterator const binary_search_tree<tkey, tvalue>::infix_const_reverse_iterator::operator++(
    int not_used)
{
    auto it = *this;
    ++(*this);
    return it;
}

template<
    typename tkey,
    typename tvalue>
typename binary_search_tree<tkey, tvalue>::iterator_data const *binary_search_tree<tkey, tvalue>::infix_const_reverse_iterator::operator*() const
{
    if(_path.empty()) throw std::logic_error("dereferencing uninitialized iterator");
    return _data;
}

// endregion infix_const_reverse_iterator implementation

// region postfix_iterator implementation

template<
        typename tkey,
        typename tvalue>
void binary_search_tree<tkey, tvalue>::postfix_iterator::assign_data()
{
    if(_data->is_initialized())
    {
        *(_data->_key) = _path.top()->key;
        *(_data->_value) = _path.top()->value;
    }
    else
    {
        allocator::construct(_data->_key, _path.top()->key);
        allocator::construct(_data->_value, _path.top()->value);
        _data->_is_initialized = true;
    }

    //TODO: virtual void bst::inject_additional_data()...
}

template<
    typename tkey,
    typename tvalue>
binary_search_tree<tkey, tvalue>::postfix_iterator::postfix_iterator(
    typename binary_search_tree<tkey, tvalue>::node *subtree_root,
    binary_search_tree<tkey, tvalue> *tree,
    iterator_data *data)
    : _tree(tree), _data(data)
{
    if(subtree_root)
    {
        _path.push(subtree_root);
        _data->depth = 0;
        while (true)
        {
            node *current = _path.top();
            if (!current->right_subtree && !current->left_subtree) break;

            if (current->right_subtree)
            {
                _path.push(current->right_subtree);
            }

            if (current->left_subtree)
            {
                _path.push(current->left_subtree);
            }
            ++_data->depth;
        }

        allocator::construct(_data->_key, _path.top()->key);
        allocator::construct(_data->_value, _path.top()->value);
        //TODO: inject
        _data->_is_initialized = true;

    }
    else return;
}

template<
    typename tkey,
    typename tvalue>
bool binary_search_tree<tkey, tvalue>::postfix_iterator::operator==(
    typename binary_search_tree<tkey, tvalue>::postfix_iterator const &other) const noexcept
{
    if(this->_path.empty() && other._path.empty()) return true;

    if(this->_path.empty() ^ other._path.empty()) return false;

    return (this->_path.top() == other._path.top());
}

template<
    typename tkey,
    typename tvalue>
bool binary_search_tree<tkey, tvalue>::postfix_iterator::operator!=(
    typename binary_search_tree<tkey, tvalue>::postfix_iterator const &other) const noexcept
{
    return !(*this == other);
}

template<
    typename tkey,
    typename tvalue>
typename binary_search_tree<tkey, tvalue>::postfix_iterator &binary_search_tree<tkey, tvalue>::postfix_iterator::operator++()
{
    if(_path.empty()) throw std::logic_error("incrementing empty iterator");

    while(true)
    {
        node *current = _path.top();
        _path.pop();

        if(_path.empty()) return *this;

        if(current == _path.top()->left_subtree)
        {
            node *tmp = _path.top()->right_subtree;
            --_data->depth;
            while(tmp)
            {
                _path.push(tmp);
                tmp = tmp->left_subtree;
                ++_data->depth;
            }
        }
        else if(current == _path.top()->right_subtree)
        {
            --_data->depth;
        }
        assign_data();
        return *this;
    }
}

template<
    typename tkey,
    typename tvalue>
typename binary_search_tree<tkey, tvalue>::postfix_iterator const binary_search_tree<tkey, tvalue>::postfix_iterator::operator++(
    int not_used)
{
    auto it = *this;
    ++(*this);
    return it;
}

template<
    typename tkey,
    typename tvalue>
typename binary_search_tree<tkey, tvalue>::iterator_data *binary_search_tree<tkey, tvalue>::postfix_iterator::operator*() const
{
    if(_path.empty()) throw std::logic_error("dereferencing uninitialized iterator");
    return _data;
}

// endregion postfix_iterator implementation

// region postfix_const_iterator implementation

template<
        typename tkey,
        typename tvalue>
void binary_search_tree<tkey, tvalue>::postfix_const_iterator::assign_data()
{
    if(_data->is_initialized())
    {
        *(_data->_key) = _path.top()->key;
        *(_data->_value) = _path.top()->value;
    }
    else
    {
        allocator::construct(_data->_key, _path.top()->key);
        allocator::construct(_data->_value, _path.top()->value);
        _data->_is_initialized = true;
    }

    //TODO: virtual void bst::inject_additional_data()...
}

template<
    typename tkey,
    typename tvalue>
binary_search_tree<tkey, tvalue>::postfix_const_iterator::postfix_const_iterator(
    typename binary_search_tree<tkey, tvalue>::node *subtree_root,
    binary_search_tree<tkey, tvalue> *tree,
    iterator_data *data)
    : _tree(tree), _data(data)
{
    if(subtree_root)
    {
        _path.push(subtree_root);
        _data->depth = 0;
        while (true)
        {
            node *current = _path.top();
            if (!current->right_subtree && !current->left_subtree) break;

            if (current->right_subtree)
            {
                _path.push(current->right_subtree);
            }

            if (current->left_subtree)
            {
                _path.push(current->left_subtree);
            }
            ++_data->depth;
        }

        allocator::construct(_data->_key, _path.top()->key);
        allocator::construct(_data->_value, _path.top()->value);
        //TODO: inject
        _data->_is_initialized = true;

    }
    else return;
}

template<
    typename tkey,
    typename tvalue>
bool binary_search_tree<tkey, tvalue>::postfix_const_iterator::operator==(
    typename binary_search_tree<tkey, tvalue>::postfix_const_iterator const &other) const noexcept
{
    if(this->_path.empty() && other._path.empty()) return true;

    if(this->_path.empty() ^ other._path.empty()) return false;

    return (this->_path.top() == other._path.top());
}

template<
    typename tkey,
    typename tvalue>
bool binary_search_tree<tkey, tvalue>::postfix_const_iterator::operator!=(
    typename binary_search_tree<tkey, tvalue>::postfix_const_iterator const &other) const noexcept
{
    return !(*this == other);
}

template<
    typename tkey,
    typename tvalue>
typename binary_search_tree<tkey, tvalue>::postfix_const_iterator &binary_search_tree<tkey, tvalue>::postfix_const_iterator::operator++()
{
    if(_path.empty()) throw std::logic_error("incrementing empty iterator");

    while(true)
    {
        node *current = _path.top();
        _path.pop();

        if(_path.empty()) return *this;

        if(current == _path.top()->left_subtree)
        {
            node *tmp = _path.top()->right_subtree;
            --_data->depth;
            while(tmp)
            {
                _path.push(tmp);
                tmp = tmp->left_subtree;
                ++_data->depth;
            }
        }
        else if(current == _path.top()->right_subtree)
        {
            --_data->depth;
        }
        assign_data();
        return *this;
    }
}

template<
    typename tkey,
    typename tvalue>
typename binary_search_tree<tkey, tvalue>::postfix_const_iterator const binary_search_tree<tkey, tvalue>::postfix_const_iterator::operator++(
    int not_used)
{
    auto it = *this;
    ++(*this);
    return it;
}

template<
    typename tkey,
    typename tvalue>
typename binary_search_tree<tkey, tvalue>::iterator_data const *binary_search_tree<tkey, tvalue>::postfix_const_iterator::operator*() const
{
    if(_path.empty()) throw std::logic_error("dereferencing uninitialized iterator");
    return _data;
}

// endregion postfix_const_iterator implementation

// region postfix_reverse_iterator implementation

template<
        typename tkey,
        typename tvalue>
void binary_search_tree<tkey, tvalue>::postfix_reverse_iterator::assign_data()
{
    if(_data->is_initialized())
    {
        *(_data->_key) = _path.top()->key;
        *(_data->_value) = _path.top()->value;
    }
    else
    {
        allocator::construct(_data->_key, _path.top()->key);
        allocator::construct(_data->_value, _path.top()->value);
        _data->_is_initialized = true;
    }

    //TODO: virtual void bst::inject_additional_data()...
}

template<
    typename tkey,
    typename tvalue>
binary_search_tree<tkey, tvalue>::postfix_reverse_iterator::postfix_reverse_iterator(
    typename binary_search_tree<tkey, tvalue>::node *subtree_root,
    binary_search_tree<tkey, tvalue> *tree,
    iterator_data *data)
    : _tree(tree), _data(data)
{
    if(subtree_root)
    {
        _path.push(subtree_root);
        _data->depth = 0;
        while (true)
        {
            node *current = _path.top();
            if (!current->right_subtree && !current->left_subtree) break;

            if (current->left_subtree)
            {
                _path.push(current->left_subtree);
            }

            if (current->right_subtree)
            {
                _path.push(current->right_subtree);
            }

            ++_data->depth;
        }

        allocator::construct(_data->_key, _path.top()->key);
        allocator::construct(_data->_value, _path.top()->value);
        //TODO: inject
        _data->_is_initialized = true;

    }
    else return;
}

template<
    typename tkey,
    typename tvalue>
bool binary_search_tree<tkey, tvalue>::postfix_reverse_iterator::operator==(
    typename binary_search_tree<tkey, tvalue>::postfix_reverse_iterator const &other) const noexcept
{
    if(this->_path.empty() && other._path.empty()) return true;

    if(this->_path.empty() ^ other._path.empty()) return false;

    return (this->_path.top() == other._path.top());
}

template<
    typename tkey,
    typename tvalue>
bool binary_search_tree<tkey, tvalue>::postfix_reverse_iterator::operator!=(
    typename binary_search_tree<tkey, tvalue>::postfix_reverse_iterator const &other) const noexcept
{
    return !(*this == other);
}

template<
    typename tkey,
    typename tvalue>
typename binary_search_tree<tkey, tvalue>::postfix_reverse_iterator &binary_search_tree<tkey, tvalue>::postfix_reverse_iterator::operator++()
{
    if(_path.empty()) throw std::logic_error("incrementing empty iterator");

    while(true)
    {
        node *current = _path.top();
        _path.pop();

        if(_path.empty()) return *this;

        if(current == _path.top()->right_subtree)
        {
            node *tmp = _path.top()->left_subtree;
            --_data->depth;
            while(tmp)
            {
                _path.push(tmp);
                tmp = tmp->right_subtree;
                ++_data->depth;
            }
        }
        else if(current == _path.top()->left_subtree)
        {
            --_data->depth;
        }
        assign_data();
        return *this;
    }
}

template<
    typename tkey,
    typename tvalue>
typename binary_search_tree<tkey, tvalue>::postfix_reverse_iterator const binary_search_tree<tkey, tvalue>::postfix_reverse_iterator::operator++(
    int not_used)
{
    auto it = *this;
    ++(*this);
    return it;
}

template<
    typename tkey,
    typename tvalue>
typename binary_search_tree<tkey, tvalue>::iterator_data *binary_search_tree<tkey, tvalue>::postfix_reverse_iterator::operator*() const
{
    if(_path.empty()) throw std::logic_error("dereferencing uninitialized iterator");
    return _data;
}

// endregion postfix_reverse_iterator implementation

// region postfix_const_reverse_iterator implementation

template<
        typename tkey,
        typename tvalue>
void binary_search_tree<tkey, tvalue>::postfix_const_reverse_iterator::assign_data()
{
    if(_data->is_initialized())
    {
        *(_data->_key) = _path.top()->key;
        *(_data->_value) = _path.top()->value;
    }
    else
    {
        allocator::construct(_data->_key, _path.top()->key);
        allocator::construct(_data->_value, _path.top()->value);
        _data->_is_initialized = true;
    }

    //TODO: virtual void bst::inject_additional_data()...
}

template<
    typename tkey,
    typename tvalue>
binary_search_tree<tkey, tvalue>::postfix_const_reverse_iterator::postfix_const_reverse_iterator(
    typename binary_search_tree<tkey, tvalue>::node *subtree_root,
    binary_search_tree<tkey, tvalue> *tree,
    iterator_data *data)
        : _tree(tree), _data(data)
{
    if(subtree_root)
    {
        _path.push(subtree_root);
        _data->depth = 0;
        while (true)
        {
            node *current = _path.top();
            if (!current->right_subtree && !current->left_subtree) break;

            if (current->left_subtree)
            {
                _path.push(current->left_subtree);
            }

            if (current->right_subtree)
            {
                _path.push(current->right_subtree);
            }

            ++_data->depth;
        }

        allocator::construct(_data->_key, _path.top()->key);
        allocator::construct(_data->_value, _path.top()->value);
        //TODO: inject
        _data->_is_initialized = true;

    }
    else return;
}

template<
    typename tkey,
    typename tvalue>
bool binary_search_tree<tkey, tvalue>::postfix_const_reverse_iterator::operator==(
    typename binary_search_tree<tkey, tvalue>::postfix_const_reverse_iterator const &other) const noexcept
{
    if(this->_path.empty() && other._path.empty()) return true;

    if(this->_path.empty() ^ other._path.empty()) return false;

    return (this->_path.top() == other._path.top());
}

template<
    typename tkey,
    typename tvalue>
bool binary_search_tree<tkey, tvalue>::postfix_const_reverse_iterator::operator!=(
    typename binary_search_tree<tkey, tvalue>::postfix_const_reverse_iterator const &other) const noexcept
{
    return !(*this == other);
}

template<
    typename tkey,
    typename tvalue>
typename binary_search_tree<tkey, tvalue>::postfix_const_reverse_iterator &binary_search_tree<tkey, tvalue>::postfix_const_reverse_iterator::operator++()
{
    if(_path.empty()) throw std::logic_error("incrementing empty iterator");

    while(true)
    {
        node *current = _path.top();
        _path.pop();

        if(_path.empty()) return *this;

        if(current == _path.top()->right_subtree)
        {
            node *tmp = _path.top()->left_subtree;
            --_data->depth;
            while(tmp)
            {
                _path.push(tmp);
                tmp = tmp->right_subtree;
                ++_data->depth;
            }
        }
        else if(current == _path.top()->left_subtree)
        {
            --_data->depth;
        }
        assign_data();
        return *this;
    }
}

template<
    typename tkey,
    typename tvalue>
typename binary_search_tree<tkey, tvalue>::postfix_const_reverse_iterator const binary_search_tree<tkey, tvalue>::postfix_const_reverse_iterator::operator++(
    int not_used)
{
    auto it = *this;
    ++(*this);
    return it;
}

template<
    typename tkey,
    typename tvalue>
typename binary_search_tree<tkey, tvalue>::iterator_data const *binary_search_tree<tkey, tvalue>::postfix_const_reverse_iterator::operator*() const
{
    if(_path.empty()) throw std::logic_error("dereferencing uninitialized iterator");
    return _data;
}

// endregion postfix_const_reverse_iterator implementation

// endregion iterators implementation

// region target operations associated exception types implementation

template<
    typename tkey,
    typename tvalue>
binary_search_tree<tkey, tvalue>::insertion_of_existent_key_attempt_exception::insertion_of_existent_key_attempt_exception(
    tkey const &key):
    std::logic_error("Attempt to insert already existing key inside the tree.")
{

}

template<
    typename tkey,
    typename tvalue>
tkey const &binary_search_tree<tkey, tvalue>::insertion_of_existent_key_attempt_exception::get_key() const noexcept
{
    return _key;
}

template<
    typename tkey,
    typename tvalue>
binary_search_tree<tkey, tvalue>::obtaining_of_nonexistent_key_attempt_exception::obtaining_of_nonexistent_key_attempt_exception(
    tkey const &key):
    std::logic_error("Attempt to obtain a value by non-existing key from the tree.")
{

}

template<
    typename tkey,
    typename tvalue>
tkey const &binary_search_tree<tkey, tvalue>::obtaining_of_nonexistent_key_attempt_exception::get_key() const noexcept
{
    return _key;
}

template<
    typename tkey,
    typename tvalue>
binary_search_tree<tkey, tvalue>::disposal_of_nonexistent_key_attempt_exception::disposal_of_nonexistent_key_attempt_exception(
    tkey const &key):
    std::logic_error("Attempt to dispose a value by non-existing key from the tree.")
{

}

template<
    typename tkey,
    typename tvalue>
tkey const &binary_search_tree<tkey, tvalue>::disposal_of_nonexistent_key_attempt_exception::get_key() const noexcept
{
    return _key;
}

// endregion target operations associated exception types implementation

// region template methods implementation

// region binary_search_tree<tkey, tvalue>::template_method_basics implementation

template<
    typename tkey,
    typename tvalue>
binary_search_tree<tkey, tvalue>::template_method_basics::template_method_basics(
    binary_search_tree<tkey, tvalue> *tree)
    : _tree(tree)
{

}

template<
    typename tkey,
    typename tvalue>
[[nodiscard]] inline logger *binary_search_tree<tkey, tvalue>::template_method_basics::get_logger() const noexcept
{
    return this->_tree->_logger;
}

// endregion binary_search_tree<tkey, tvalue>::template_method_basics implementation

// region search_tree<tkey, tvalue>::insertion_template_method implementation

template<
    typename tkey,
    typename tvalue>
binary_search_tree<tkey, tvalue>::insertion_template_method::insertion_template_method(
    binary_search_tree<tkey, tvalue> *tree,
    typename binary_search_tree<tkey, tvalue>::insertion_of_existent_key_attempt_strategy insertion_strategy):
    binary_search_tree<tkey, tvalue>::template_method_basics::template_method_basics(tree),
    _insertion_strategy(insertion_strategy)
{

}

template<
    typename tkey,
    typename tvalue>
void binary_search_tree<tkey, tvalue>::insertion_template_method::insert(
    tkey const &key,
    tvalue const &value)
{
    auto path = this->find_path(key);

    if (*(path.top()))
    {
        switch (_insertion_strategy)
        {
            case insertion_of_existent_key_attempt_strategy::throw_an_exception:
                throw insertion_of_existent_key_attempt_exception(key);
            case insertion_of_existent_key_attempt_strategy::update_value:
                (*(path.top()))->value = std::move(value);
                break;
        }

        return;
    }

    this->_tree->call_node_constructor(*(path.top()) = reinterpret_cast<node *>(allocate_with_guard(this->_tree->get_node_size(), 1)), key, value);

    this->balance(path);

}

template<
    typename tkey,
    typename tvalue>
void binary_search_tree<tkey, tvalue>::insertion_template_method::insert(
    tkey const &key,
    tvalue &&value)
{
    auto path = this->find_path(key);

    if (*(path.top()))
    {
        switch (_insertion_strategy)
        {
            case insertion_of_existent_key_attempt_strategy::throw_an_exception:
                throw insertion_of_existent_key_attempt_exception(key);
            case insertion_of_existent_key_attempt_strategy::update_value:
                (*(path.top()))->value = std::move(value);
                break;
        }

        return;
    }

    this->_tree->call_node_constructor(*(path.top()) = reinterpret_cast<node *>(allocate_with_guard(this->_tree->get_node_size(), 1)), key, std::move(value));

    this->balance(path);

}

template<
    typename tkey,
    typename tvalue>
void binary_search_tree<tkey, tvalue>::insertion_template_method::set_insertion_strategy(
    typename binary_search_tree<tkey, tvalue>::insertion_of_existent_key_attempt_strategy insertion_strategy) noexcept
{
    _insertion_strategy = insertion_strategy;
}

template<
    typename tkey,
    typename tvalue>
allocator *binary_search_tree<tkey, tvalue>::insertion_template_method::get_allocator() const noexcept
{
    return this->_tree->get_allocator();
}

// endregion search_tree<tkey, tvalue>::insertion_template_method implementation

// region search_tree<tkey, tvalue>::obtaining_template_method implementation

template<
    typename tkey,
    typename tvalue>
binary_search_tree<tkey, tvalue>::obtaining_template_method::obtaining_template_method(
    binary_search_tree<tkey, tvalue> *tree):
    binary_search_tree<tkey, tvalue>::template_method_basics::template_method_basics(tree)
{

}

template<
    typename tkey,
    typename tvalue>
tvalue const &binary_search_tree<tkey, tvalue>::obtaining_template_method::obtain(
    tkey const &key)
{
    auto path = this->find_path(key);
    if(!*(path.top()))
    {
        throw obtaining_of_nonexistent_key_attempt_exception(key);
    }

    const tvalue &value = (*(path.top()))->value;
    this->balance(path);
    return value;
}

// endregion search_tree<tkey, tvalue>::obtaining_template_method implementation

// region search_tree<tkey, tvalue>::disposal_template_method implementation

template<
    typename tkey,
    typename tvalue>
binary_search_tree<tkey, tvalue>::disposal_template_method::disposal_template_method(
    binary_search_tree<tkey, tvalue> *tree,
    typename binary_search_tree<tkey, tvalue>::disposal_of_nonexistent_key_attempt_strategy disposal_strategy):
    binary_search_tree<tkey, tvalue>::template_method_basics(tree)
{

}

template<
    typename tkey,
    typename tvalue>
tvalue binary_search_tree<tkey, tvalue>::disposal_template_method::dispose(
    tkey const &key)
{
    auto path = this->find_path(key);
    if(!*(path.top()))
    {
        switch(_disposal_strategy)
        {
            case disposal_of_nonexistent_key_attempt_strategy::throw_an_exception:
                throw disposal_of_nonexistent_key_attempt_exception(key);
            case disposal_of_nonexistent_key_attempt_strategy::do_nothing:
                return tvalue();
        }
    }

    if((*(path.top()))->left_subtree && (*(path.top()))->right_subtree)
    {
        auto *to_swap = *(path.top());
        auto **current = &((*(path.top()))->left_subtree);

        while(*current)
        {
            path.push(current);
            current = &((*current)->right_subtree);
        }

        swap(std::move(to_swap->key), std::move((*(path.top()))->key));
        swap(std::move(to_swap->value), std::move((*(path.top()))->value));
    }

    tvalue value = std::move((*(path.top()))->value);
    node *rest = (*(path.top()))->left_subtree
            ? (*(path.top()))->left_subtree
            : (*(path.top()))->right_subtree;
    allocator::destruct(*(path.top()));
    deallocate_with_guard(*(path.top()));
    *(path.top()) = rest;
    this->balance(path);
    return value;
}

template<
    typename tkey,
    typename tvalue>
void binary_search_tree<tkey, tvalue>::disposal_template_method::set_disposal_strategy(
    typename binary_search_tree<tkey, tvalue>::disposal_of_nonexistent_key_attempt_strategy disposal_strategy) noexcept
{
    _disposal_strategy = disposal_strategy;
}

template<
    typename tkey,
    typename tvalue>
[[nodiscard]] inline allocator *binary_search_tree<tkey, tvalue>::disposal_template_method::get_allocator() const noexcept
{
    return this->_tree->get_allocator();
}

// endregion search_tree<tkey, tvalue>::disposal_template_method implementation

// endregion template methods

// region construction, assignment, destruction implementation

template<
    typename tkey,
    typename tvalue>
binary_search_tree<tkey, tvalue>::binary_search_tree(
    typename binary_search_tree<tkey, tvalue>::insertion_template_method *insertion_template,
    typename binary_search_tree<tkey, tvalue>::obtaining_template_method *obtaining_template,
    typename binary_search_tree<tkey, tvalue>::disposal_template_method *disposal_template,
    std::function<int(tkey const &, tkey const &)> comparer,
    allocator *allocator,
    logger *logger):
    search_tree<tkey, tvalue>(comparer, logger, allocator),
    _root(nullptr),
    _insertion_template(insertion_template),
    _obtaining_template(obtaining_template),
    _disposal_template(disposal_template)
{

}

template<
    typename tkey,
    typename tvalue>
binary_search_tree<tkey, tvalue>::binary_search_tree(
    std::function<int(tkey const &, tkey const &)> keys_comparer,
    allocator *allocator,
    logger *logger,
    typename binary_search_tree<tkey, tvalue>::insertion_of_existent_key_attempt_strategy insertion_strategy,
    typename binary_search_tree<tkey, tvalue>::disposal_of_nonexistent_key_attempt_strategy disposal_strategy):
    binary_search_tree(
        new binary_search_tree<tkey, tvalue>::insertion_template_method(this, insertion_strategy),
        new binary_search_tree<tkey, tvalue>::obtaining_template_method(this),
        new binary_search_tree<tkey, tvalue>::disposal_template_method(this, disposal_strategy),
        keys_comparer,
        allocator,
        logger)
{

}

template<
    typename tkey,
    typename tvalue>
binary_search_tree<tkey, tvalue>::binary_search_tree(
    binary_search_tree<tkey, tvalue> const &other)
    :search_tree<tkey, tvalue>(other._keys_comparer, other.get_logger(), other.get_allocator()),
    _insertion_template(new binary_search_tree<tkey, tvalue>::insertion_template_method(this, binary_search_tree<tkey, tvalue>::insertion_of_existent_key_attempt_strategy::throw_an_exception)),
    _obtaining_template(new binary_search_tree<tkey, tvalue>::obtaining_template_method(this)),
    _disposal_template(new binary_search_tree<tkey, tvalue>::disposal_template_method(this, binary_search_tree<tkey, tvalue>::disposal_of_nonexistent_key_attempt_strategy::throw_an_exception))
{
    _root = copy(other._root);
}

template<
    typename tkey,
    typename tvalue>
binary_search_tree<tkey, tvalue>::binary_search_tree(
    binary_search_tree<tkey, tvalue> &&other) noexcept
    : search_tree<tkey, tvalue>(
            other._keys_comparer,
            other.get_logger(),
            other.get_allocator()),
    _insertion_template(other._insertion_template),
    _obtaining_template(other._obtaining_template),
    _disposal_template(other._disposal_template),
    _root(other._root)
{
    other._logger = nullptr;
    other._allocator = nullptr;
    other._insertion_template = nullptr;
    other._obtaining_template = nullptr;
    other._disposal_template = nullptr;
    other._root = nullptr;
}

template<
    typename tkey,
    typename tvalue>
binary_search_tree<tkey, tvalue> &binary_search_tree<tkey, tvalue>::operator=(
    binary_search_tree<tkey, tvalue> const &other)
{
    if(this != &other)
    {
        clear(_root);
        this->_allocator = other._allocator;
        this->_logger = other._logger;
        this->_keys_comparer = other._keys_comparer;
        _root = copy(other._root);
    }

    return *this;
}

template<
    typename tkey,
    typename tvalue>
binary_search_tree<tkey, tvalue> &binary_search_tree<tkey, tvalue>::operator=(
    binary_search_tree<tkey, tvalue> &&other) noexcept
{
    if(this != &other)
    {
        clear(_root);
        _root = other._root;
        other._root = nullptr;

        _insertion_template = other._insertion_template;
        other._insertion_template = nullptr;

        _obtaining_template = other._obtaining_template;
        other._obtaining_template = nullptr;

        _disposal_template = other._disposal_template;
        other._disposal_template = nullptr;

        this->_allocator = other._allocator;
        other._allocator = nullptr;

        this->_logger = other._logger;
        other._logger = nullptr;

        this->_keys_comparer = other._keys_comparer;
        other._keys_comparer = nullptr;
    }

    return *this;
}

template<
    typename tkey,
    typename tvalue>
binary_search_tree<tkey, tvalue>::~binary_search_tree()
{
    delete _insertion_template;
    delete _obtaining_template;
    delete _disposal_template;
    clear(_root);
}

// endregion construction, assignment, destruction implementation

// region associative_container<tkey, tvalue> contract implementation

template<
    typename tkey,
    typename tvalue>
void binary_search_tree<tkey, tvalue>::insert(
    tkey const &key,
    tvalue const &value)
{
    _insertion_template->insert(key, value);
}

template<
    typename tkey,
    typename tvalue>
void binary_search_tree<tkey, tvalue>::insert(
    tkey const &key,
    tvalue &&value)
{
    _insertion_template->insert(key, std::move(value));
}

template<
    typename tkey,
    typename tvalue>
tvalue const &binary_search_tree<tkey, tvalue>::obtain(
    tkey const &key)
{
    return _obtaining_template->obtain(key);
}

template<
    typename tkey,
    typename tvalue>
std::vector<typename associative_container<tkey, tvalue>::key_value_pair> binary_search_tree<tkey, tvalue>::obtain_between(
    tkey const &lower_bound,
    tkey const &upper_bound,
    bool lower_bound_inclusive,
    bool upper_bound_inclusive)
{
    return _obtaining_template->obtain_between(lower_bound, upper_bound, lower_bound_inclusive, upper_bound_inclusive);
}

template<
    typename tkey,
    typename tvalue>
tvalue binary_search_tree<tkey, tvalue>::dispose(
    tkey const &key)
{
    return _disposal_template->dispose(key);
}

// endregion associative_containers contract implementations

template<
    typename tkey,
    typename tvalue>
void binary_search_tree<tkey, tvalue>::set_insertion_strategy(
    typename binary_search_tree<tkey, tvalue>::insertion_of_existent_key_attempt_strategy insertion_strategy) noexcept
{
    this->_insertion_template->set_insertion_strategy(insertion_strategy);
}

template<
    typename tkey,
    typename tvalue>
void binary_search_tree<tkey, tvalue>::set_removal_strategy(
    typename binary_search_tree<tkey, tvalue>::disposal_of_nonexistent_key_attempt_strategy disposal_strategy) noexcept
{
    this->_disposal_template->set_disposal_strategy(disposal_strategy);
}

// region iterators requesting implementation

template<
    typename tkey,
    typename tvalue>
typename binary_search_tree<tkey, tvalue>::prefix_iterator binary_search_tree<tkey, tvalue>::begin_prefix() const noexcept
{
    return binary_search_tree<tkey, tvalue>::prefix_iterator(_root, const_cast<binary_search_tree<tkey, tvalue> *>(this), create_iterator_data());
}

template<
    typename tkey,
    typename tvalue>
typename binary_search_tree<tkey, tvalue>::prefix_iterator binary_search_tree<tkey, tvalue>::end_prefix() const noexcept
{
    return binary_search_tree<tkey, tvalue>::prefix_iterator(nullptr, const_cast<binary_search_tree<tkey, tvalue> *>(this), create_iterator_data());
}

template<
    typename tkey,
    typename tvalue>
typename binary_search_tree<tkey, tvalue>::prefix_const_iterator binary_search_tree<tkey, tvalue>::cbegin_prefix() const noexcept
{
    return binary_search_tree<tkey, tvalue>::prefix_const_iterator(_root, const_cast<binary_search_tree<tkey, tvalue> *>(this), create_iterator_data());
}

template<
    typename tkey,
    typename tvalue>
typename binary_search_tree<tkey, tvalue>::prefix_const_iterator binary_search_tree<tkey, tvalue>::cend_prefix() const noexcept
{
    return binary_search_tree<tkey, tvalue>::prefix_const_iterator(nullptr, const_cast<binary_search_tree<tkey, tvalue> *>(this), create_iterator_data());
}

template<
    typename tkey,
    typename tvalue>
typename binary_search_tree<tkey, tvalue>::prefix_reverse_iterator binary_search_tree<tkey, tvalue>::rbegin_prefix() const noexcept
{
    return binary_search_tree<tkey, tvalue>::prefix_reverse_iterator(_root, const_cast<binary_search_tree<tkey, tvalue> *>(this), create_iterator_data());
}

template<
    typename tkey,
    typename tvalue>
typename binary_search_tree<tkey, tvalue>::prefix_reverse_iterator binary_search_tree<tkey, tvalue>::rend_prefix() const noexcept
{
    return binary_search_tree<tkey, tvalue>::prefix_reverse_iterator(nullptr, const_cast<binary_search_tree<tkey, tvalue> *>(this), create_iterator_data());
}

template<
    typename tkey,
    typename tvalue>
typename binary_search_tree<tkey, tvalue>::prefix_const_reverse_iterator binary_search_tree<tkey, tvalue>::crbegin_prefix() const noexcept
{
    return binary_search_tree<tkey, tvalue>::prefix_const_reverse_iterator(_root, const_cast<binary_search_tree<tkey, tvalue> *>(this), create_iterator_data());
}

template<
    typename tkey,
    typename tvalue>
typename binary_search_tree<tkey, tvalue>::prefix_const_reverse_iterator binary_search_tree<tkey, tvalue>::crend_prefix() const noexcept
{
    return binary_search_tree<tkey, tvalue>::prefix_const_reverse_iterator(nullptr, const_cast<binary_search_tree<tkey, tvalue> *>(this), create_iterator_data());
}

template<
    typename tkey,
    typename tvalue>
typename binary_search_tree<tkey, tvalue>::infix_iterator binary_search_tree<tkey, tvalue>::begin_infix() const noexcept
{
    return binary_search_tree<tkey, tvalue>::infix_iterator(_root, const_cast<binary_search_tree<tkey, tvalue> *>(this), create_iterator_data());
}

template<
    typename tkey,
    typename tvalue>
typename binary_search_tree<tkey, tvalue>::infix_iterator binary_search_tree<tkey, tvalue>::end_infix() const noexcept
{
    return binary_search_tree<tkey, tvalue>::infix_iterator(nullptr, const_cast<binary_search_tree<tkey, tvalue> *>(this), create_iterator_data());
}

template<
    typename tkey,
    typename tvalue>
typename binary_search_tree<tkey, tvalue>::infix_const_iterator binary_search_tree<tkey, tvalue>::cbegin_infix() const noexcept
{
    return binary_search_tree<tkey, tvalue>::infix_const_iterator(_root, const_cast<binary_search_tree<tkey, tvalue> *>(this), create_iterator_data());
}

template<
    typename tkey,
    typename tvalue>
typename binary_search_tree<tkey, tvalue>::infix_const_iterator binary_search_tree<tkey, tvalue>::cend_infix() const noexcept
{
    return binary_search_tree<tkey, tvalue>::infix_const_iterator(nullptr, const_cast<binary_search_tree<tkey, tvalue> *>(this), create_iterator_data());
}

template<
    typename tkey,
    typename tvalue>
typename binary_search_tree<tkey, tvalue>::infix_reverse_iterator binary_search_tree<tkey, tvalue>::rbegin_infix() const noexcept
{
    return binary_search_tree<tkey, tvalue>::infix_reverse_iterator(_root, const_cast<binary_search_tree<tkey, tvalue> *>(this), create_iterator_data());
}

template<
    typename tkey,
    typename tvalue>
typename binary_search_tree<tkey, tvalue>::infix_reverse_iterator binary_search_tree<tkey, tvalue>::rend_infix() const noexcept
{
    return binary_search_tree<tkey, tvalue>::infix_reverse_iterator(nullptr, const_cast<binary_search_tree<tkey, tvalue> *>(this), create_iterator_data());
}

template<
    typename tkey,
    typename tvalue>
typename binary_search_tree<tkey, tvalue>::infix_const_reverse_iterator binary_search_tree<tkey, tvalue>::crbegin_infix() const noexcept
{
    return binary_search_tree<tkey, tvalue>::infix_const_reverse_iterator(_root, const_cast<binary_search_tree<tkey, tvalue> *>(this), create_iterator_data());
}

template<
    typename tkey,
    typename tvalue>
typename binary_search_tree<tkey, tvalue>::infix_const_reverse_iterator binary_search_tree<tkey, tvalue>::crend_infix() const noexcept
{
    return binary_search_tree<tkey, tvalue>::infix_const_reverse_iterator(nullptr, const_cast<binary_search_tree<tkey, tvalue> *>(this), create_iterator_data());
}

template<
    typename tkey,
    typename tvalue>
typename binary_search_tree<tkey, tvalue>::postfix_iterator binary_search_tree<tkey, tvalue>::begin_postfix() const noexcept
{
    return binary_search_tree<tkey, tvalue>::postfix_iterator(_root, const_cast<binary_search_tree<tkey, tvalue> *>(this), create_iterator_data());
}

template<
    typename tkey,
    typename tvalue>
typename binary_search_tree<tkey, tvalue>::postfix_iterator binary_search_tree<tkey, tvalue>::end_postfix() const noexcept
{
    return binary_search_tree<tkey, tvalue>::postfix_iterator(nullptr, const_cast<binary_search_tree<tkey, tvalue> *>(this), create_iterator_data());
}

template<
    typename tkey,
    typename tvalue>
typename binary_search_tree<tkey, tvalue>::postfix_const_iterator binary_search_tree<tkey, tvalue>::cbegin_postfix() const noexcept
{
    return binary_search_tree<tkey, tvalue>::postfix_const_iterator(_root, const_cast<binary_search_tree<tkey, tvalue> *>(this), create_iterator_data());
}

template<
    typename tkey,
    typename tvalue>
typename binary_search_tree<tkey, tvalue>::postfix_const_iterator binary_search_tree<tkey, tvalue>::cend_postfix() const noexcept
{
    return binary_search_tree<tkey, tvalue>::postfix_const_iterator(nullptr, const_cast<binary_search_tree<tkey, tvalue> *>(this), create_iterator_data());
}

template<
    typename tkey,
    typename tvalue>
typename binary_search_tree<tkey, tvalue>::postfix_reverse_iterator binary_search_tree<tkey, tvalue>::rbegin_postfix() const noexcept
{
    return binary_search_tree<tkey, tvalue>::postfix_reverse_iterator(_root, const_cast<binary_search_tree<tkey, tvalue> *>(this), create_iterator_data());
}

template<
    typename tkey,
    typename tvalue>
typename binary_search_tree<tkey, tvalue>::postfix_reverse_iterator binary_search_tree<tkey, tvalue>::rend_postfix() const noexcept
{
    return binary_search_tree<tkey, tvalue>::postfix_reverse_iterator(nullptr, const_cast<binary_search_tree<tkey, tvalue> *>(this), create_iterator_data());
}

template<
    typename tkey,
    typename tvalue>
typename binary_search_tree<tkey, tvalue>::postfix_const_reverse_iterator binary_search_tree<tkey, tvalue>::crbegin_postfix() const noexcept
{
    return binary_search_tree<tkey, tvalue>::postfix_const_reverse_iterator(_root, const_cast<binary_search_tree<tkey, tvalue> *>(this), create_iterator_data());
}

template<
    typename tkey,
    typename tvalue>
typename binary_search_tree<tkey, tvalue>::postfix_const_reverse_iterator binary_search_tree<tkey, tvalue>::crend_postfix() const noexcept
{
    return binary_search_tree<tkey, tvalue>::postfix_const_reverse_iterator(nullptr, const_cast<binary_search_tree<tkey, tvalue> *>(this), create_iterator_data());
}


// endregion iterators request implementation

// region subtree rotations implementation

template<
    typename tkey,
    typename tvalue>
void binary_search_tree<tkey, tvalue>::small_left_rotation(
    binary_search_tree<tkey, tvalue>::node *&subtree_root,
    bool validate) const
{
    if(!validate) return;

    if(!subtree_root || !subtree_root->right_subtree)
    {
        throw std::logic_error("cannot do left rotation");
    }

    node *tmp = subtree_root->right_subtree->left_subtree;
    node *child = subtree_root->right_subtree;

    child->left_subtree = subtree_root;
    subtree_root->right_subtree = tmp;

    subtree_root = child;
}

template<
    typename tkey,
    typename tvalue>
void binary_search_tree<tkey, tvalue>::small_right_rotation(
    binary_search_tree<tkey, tvalue>::node *&subtree_root,
    bool validate) const
{
    if(!validate) return;

    if(!subtree_root || !subtree_root->left_subtree)
    {
        throw std::logic_error("cannot do right rotation");
    }

    node *tmp = subtree_root->left_subtree->right_subtree;
    node *child = subtree_root->left_subtree;

    child->right_subtree = subtree_root;
    subtree_root->left_subtree = tmp;

    subtree_root = child;
}

template<
    typename tkey,
    typename tvalue>
void binary_search_tree<tkey, tvalue>::big_left_rotation(
    binary_search_tree<tkey, tvalue>::node *&subtree_root,
    bool validate) const
{
    if(!validate) return;

    if(!subtree_root || !subtree_root->right_subtree || !subtree_root->right_subtree->left_subtree)
    {
        throw std::logic_error("cannot do big left rotation");
    }

    node *child = subtree_root->right_subtree;
    node *grand_child = child->left_subtree;

    node *grand_child_data = grand_child->right_subtree;

    subtree_root->right_subtree = grand_child;
    grand_child->right_subtree = child;

    child->left_subtree = grand_child_data;

    small_left_rotation(subtree_root);
}

template<
    typename tkey,
    typename tvalue>
void binary_search_tree<tkey, tvalue>::big_right_rotation(
    binary_search_tree<tkey, tvalue>::node *&subtree_root,
    bool validate) const
{
    if(!validate) return;

    if(!subtree_root || !subtree_root->left_subtree || !subtree_root->left_subtree->right_subtree)
    {
        throw std::logic_error("cannot do big right rotation");
    }

    node *child = subtree_root->left_subtree;
    node *grand_child = child->right_subtree;

    node *grand_child_data = grand_child->left_subtree;

    subtree_root->left_subtree = grand_child;
    grand_child->left_subtree = child;

    child->left_subtree = grand_child_data;

    small_right_rotation(subtree_root);
}

template<
    typename tkey,
    typename tvalue>
void binary_search_tree<tkey, tvalue>::double_left_rotation(
    binary_search_tree<tkey, tvalue>::node *&subtree_root,
    bool at_grandparent_first,
    bool validate) const
{
    if(!validate) return;

    if(!subtree_root || !subtree_root->right_subtree || !subtree_root->right_subtree->right_subtree)
    {
        throw std::logic_error("cannot do double left rotation");
    }

    small_left_rotation(small_left_rotation(subtree_root, false), false);
}

template<
    typename tkey,
    typename tvalue>
void binary_search_tree<tkey, tvalue>::double_right_rotation(
    binary_search_tree<tkey, tvalue>::node *&subtree_root,
    bool at_grandparent_first,
    bool validate) const
{
    if(!validate) return;

    if(!subtree_root || !subtree_root->left_subtree || !subtree_root->left_subtree->left_subtree)
    {
        throw std::logic_error("cannot do double right rotation");
    }

    small_right_rotation(small_right_rotation(subtree_root, false), false);
}

// endregion subtree rotations implementation

#endif //MATH_PRACTICE_AND_OPERATING_SYSTEMS_BINARY_SEARCH_TREE_H