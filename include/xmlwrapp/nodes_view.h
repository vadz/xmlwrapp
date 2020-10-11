/*
 * Copyright (C) 2009 Vaclav Slavik <vslavik@gmail.com>
 * All Rights Reserved
 * 
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 * 3. Neither the name of the Author nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 * 
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR AND CONTRIBUTORS ``AS IS''
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED
 * TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
 * PARTICULAR PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR
 * OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF
 * USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 * AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT
 * OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */

/**
    @file

    This file contains the definition of the xml::nodes_view and
    xml::const_nodes_view classes.
 */

#ifndef _xmlwrapp_nodes_view_h_
#define _xmlwrapp_nodes_view_h_

// xmlwrapp includes
#include "xmlwrapp/init.h"
#include "xmlwrapp/export.h"

// standard includes
#include <iterator>

namespace xml
{

class node;
class const_nodes_view;

namespace impl
{

struct nipimpl;
class iter_advance_functor;
struct xpath_context_impl;

} // namespace impl

/**
    This class implements a view of XML nodes. A @em view is a container-like
    class that only allows access to a subset of xml::node's child nodes. The
    exact content depends on how the view was obtained; typical uses are
    e.g. a view of all element children or all elements with a given name.

    The nodes_view class implements the same container interface that
    xml::node does: it has begin() and end() methods.

    @since  0.6.0

    @see xml::node::elements(), xml::node::elements(const char *)
 */
class XMLWRAPP_API nodes_view
{
public:
    /// Size type.
    typedef std::size_t size_type;

    nodes_view() : data_begin_(0), advance_func_(0) {}
    nodes_view(const nodes_view& other);
    ~nodes_view();

    nodes_view& operator=(const nodes_view& other);

    class const_iterator;

    /**
        The iterator provides a way to access nodes in the view
        similar to a standard C++ container.

        As xml::node::iterator itself, this is only a forward iterator.
     */
    class XMLWRAPP_API iterator
    {
    public:
        typedef node value_type;
        typedef int difference_type;
        typedef value_type* pointer;
        typedef value_type& reference;
        typedef std::forward_iterator_tag iterator_category;

        iterator() : pimpl_(0), advance_func_(0) {}
        iterator(const iterator& other);
        iterator& operator=(const iterator& other);
        ~iterator();

        reference operator*() const;
        pointer   operator->() const;

        iterator& operator++();
        iterator  operator++(int);

    private:
        explicit iterator(void *data, impl::iter_advance_functor *advance_func);
        void* get_raw_node() const;
        void swap(iterator& other);

        impl::nipimpl *pimpl_;
        // function for advancing the iterator (note that it is "owned" by the
        // parent view object, so we don't have to care about its reference
        // count here)
        impl::iter_advance_functor *advance_func_;

        friend class nodes_view;
        friend class const_iterator;
        friend bool XMLWRAPP_API operator==(const iterator& lhs, const iterator& rhs);
    };

    /**
        The const_iterator provides a way to access nodes in the view
        similar to a standard C++ container. The nodes that are pointed to by
        the iterator cannot be changed.

        As xml::node::const_iterator itself, this is only a forward iterator.
     */
    class XMLWRAPP_API const_iterator
    {
    public:
        typedef const node value_type;
        typedef int difference_type;
        typedef value_type* pointer;
        typedef value_type& reference;
        typedef std::forward_iterator_tag iterator_category;

        const_iterator() : pimpl_(0), advance_func_(0) {}
        const_iterator(const const_iterator& other);
        const_iterator(const iterator& other);
        const_iterator& operator=(const const_iterator& other);
        const_iterator& operator=(const iterator& other);
        ~const_iterator();

        reference operator*() const;
        pointer   operator->() const;

        const_iterator& operator++();
        const_iterator  operator++(int);

    private:
        explicit const_iterator(void *data, impl::iter_advance_functor *advance_func);
        void* get_raw_node() const;
        void swap(const_iterator& other);

        impl::nipimpl *pimpl_;
        // function for advancing the iterator (note that it is "owned" by the
        // parent view object, so we don't have to care about its reference
        // count here)
        impl::iter_advance_functor *advance_func_;

        friend class const_nodes_view;
        friend class nodes_view;
        friend bool XMLWRAPP_API operator==(const const_iterator& lhs, const const_iterator& rhs);
    };

    /// Get an iterator that points to the beginning of this view's nodes.
    iterator begin() { return iterator(data_begin_, advance_func_); }

    /// Get an iterator that points to the beginning of this view's nodes.
    const_iterator begin() const { return const_iterator(data_begin_, advance_func_); }

    /// Get an iterator that points one past the last child for this view.
    iterator end() { return iterator(); }

    /// Get an iterator that points one past the last child for this view.
    const_iterator end() const { return const_iterator(); }

    /// Returns the number of nodes in this view.
    size_type size() const;

    /// Is the view empty?
    bool empty() const { return !data_begin_; }

    /**
        Erase the node that is pointed to by the given iterator.

        The node and all its children will be removed from its parent node.
        This will invalidate any iterators that point to the node to be erased,
        or any pointers or references to that node (but not any other iterators).

        @param to_erase An iterator that points to the node to be erased.
        @return An iterator that points to the next node in this view
                after the one being erased or end() if there are none.

        @since 0.8.0
     */
    iterator erase(const iterator& to_erase);

    /**
        Erase all nodes in the given range, from first to last.

        This will invalidate any iterators that point to the nodes to be
        erased, or any pointers or references to those nodes.

        @param first The first node in the range to be removed.
        @param last An iterator that points one past the last node to erase. Think xml::node::end().

        @return An iterator that points to the next node in this view
                after the ones being erased or end() if there are none.

        @since 0.8.0
     */
    iterator erase(iterator first, const iterator& last);

private:
    explicit nodes_view(void *data_begin, impl::iter_advance_functor *advance_func)
        : data_begin_(data_begin), advance_func_(advance_func) {}

    // begin iterator
    void *data_begin_;
    // function for advancing the iterator (owned by the view object)
    impl::iter_advance_functor *advance_func_;

    friend class node;
    friend struct impl::xpath_context_impl;
    friend class const_nodes_view;
};


/**
    This class implements a @em read-only view of XML nodes. The only
    difference from xml::nodes_view is that it doesn't allow modifications of
    the nodes, it is otherwise identical.

    @see nodes_view

    @since  0.6.0
 */
class XMLWRAPP_API const_nodes_view
{
public:
    /// Size type.
    typedef std::size_t size_type;

    const_nodes_view() : data_begin_(0), advance_func_(0) {}
    const_nodes_view(const const_nodes_view& other);
    const_nodes_view(const nodes_view& other);
    ~const_nodes_view();

    const_nodes_view& operator=(const const_nodes_view& other);
    const_nodes_view& operator=(const nodes_view& other);

    typedef nodes_view::const_iterator iterator;
    typedef nodes_view::const_iterator const_iterator;

    /// Get an iterator that points to the beginning of this view's nodes.
    const_iterator begin() const
        { return const_iterator(data_begin_, advance_func_); }

    /// Get an iterator that points one past the last child for this view.
    const_iterator end() const { return const_iterator(); }

    /// Returns the number of nodes in this view.
    size_type size() const;

    /// Is the view empty?
    bool empty() const { return !data_begin_; }

private:
    explicit const_nodes_view(void *data_begin, impl::iter_advance_functor *advance_func)
        : data_begin_(data_begin), advance_func_(advance_func) {}

    // begin iterator
    void *data_begin_;
    // function for advancing the iterator (owned by the view object)
    impl::iter_advance_functor *advance_func_;

    friend class node;
    friend struct impl::xpath_context_impl;
};

// Comparison operators for xml::[const_]nodes_view iterators

inline bool operator==(const nodes_view::iterator& lhs, const nodes_view::iterator& rhs)
    { return lhs.get_raw_node() == rhs.get_raw_node(); }
inline bool operator!=(const nodes_view::iterator& lhs, const nodes_view::iterator& rhs)
    { return !(lhs == rhs); }

inline bool operator==(const nodes_view::const_iterator& lhs, const nodes_view::const_iterator& rhs)
    { return lhs.get_raw_node() == rhs.get_raw_node(); }
inline bool operator!=(const nodes_view::const_iterator& lhs, const nodes_view::const_iterator& rhs)
    { return !(lhs == rhs); }

} // end xml namespace

#endif // _xmlwrapp_nodes_view_h_
