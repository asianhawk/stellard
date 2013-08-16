//------------------------------------------------------------------------------
/*
    This file is part of Beast: https://github.com/vinniefalco/Beast
    Copyright 2013, Vinnie Falco <vinnie.falco@gmail.com>

    Permission to use, copy, modify, and/or distribute this software for any
    purpose  with  or without fee is hereby granted, provided that the above
    copyright notice and this permission notice appear in all copies.

    THE  SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
    WITH  REGARD  TO  THIS  SOFTWARE  INCLUDING  ALL  IMPLIED  WARRANTIES  OF
    MERCHANTABILITY  AND  FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
    ANY  SPECIAL ,  DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
    WHATSOEVER  RESULTING  FROM  LOSS  OF USE, DATA OR PROFITS, WHETHER IN AN
    ACTION  OF  CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
    OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
*/
//==============================================================================

#ifndef BEAST_ERRORCALL_H_INCLUDED
#define BEAST_ERRORCALL_H_INCLUDED

//  Meets these requirements:
//
//      AcceptHandler
//      http://www.boost.org/doc/libs/1_54_0/doc/html/boost_asio/reference/AcceptHandler.html
//
//      ConnectHandler
//      http://www.boost.org/doc/libs/1_54_0/doc/html/boost_asio/reference/ConnectHandler.html
//
//      ShutdownHandler
//      http://www.boost.org/doc/libs/1_54_0/doc/html/boost_asio/reference/ShutdownHandler.html
//
//      HandshakeHandler
//      http://www.boost.org/doc/libs/1_54_0/doc/html/boost_asio/reference/HandshakeHandler.html
//
class ErrorCall
{
public:
    typedef void result_type;

    ErrorCall () noexcept
    {
    }

    template <class Handler>
    ErrorCall (BOOST_ASIO_MOVE_ARG(Handler) handler)
        : m_call (new CallType <Handler> (BOOST_ASIO_MOVE_CAST(Handler)(handler)))
    {
    }

    ErrorCall (ErrorCall const& other)
        : m_call (other.m_call)
    { 
    }

    bool isNull () const noexcept
    {
        return m_call == nullptr;
    }

    void operator() (boost::system::error_code const& ec)
    {
        (*m_call) (ec);
    }

private:
    struct Call : SharedObject, LeakChecked <Call>
    {
        virtual void operator() (boost::system::error_code const&) = 0;
    };

    template <class Handler>
    struct CallType : Call
    {
        CallType (BOOST_ASIO_MOVE_ARG(Handler) handler)
            : m_handler (handler)
        {
        }

        void operator() (boost::system::error_code const& ec)
        {
            m_handler (ec);
        }

        Handler m_handler;
    };

private:
    SharedObjectPtr <Call> m_call;
};

#endif