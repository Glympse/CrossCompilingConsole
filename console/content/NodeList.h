//------------------------------------------------------------------------------
//
// Copyright (c) 2013 Glympse Inc.  All rights reserved.
//
//------------------------------------------------------------------------------

#ifndef NODELIST_H__GLYMPSE__
#define NODELIST_H__GLYMPSE__

namespace Glympse
{

template< class T > class NodeList : public Common< INodeList<T> > /*S*where T : class**/
{
    private: int32 _size;
    
    private: typename GListNode<T>::ptr _first;
    
    private: typename GListNode<T>::ptr _last;
    
    public: NodeList()
    {
        _size = 0;
    }
    
    /**
     * INodeList section
     */
    
    public: virtual typename GListNode<T>::ptr addFirstNode(const T& o)
    {
        return linkFirst(o);
    }
    
    public: virtual typename GListNode<T>::ptr addLastNode(const T& o)
    {
        return linkLast(o);
    }
    
    public: virtual typename GListNode<T>::ptr firstNode()
    {
        return _first;
    }
    
    public: virtual typename GListNode<T>::ptr lastNode()
    {
        return _last;
    }
    
    public: virtual void remove(const typename GListNode<T>::ptr& node)
    {
        unlink(node);
    }
    
    public: virtual void removeFirst()
    {
        if ( _first != NULL )
        {
            unlinkFirst(_first);
        }
    }
    
    public: virtual void removeLast()
    {
        if ( _last != NULL )
        {
            unlinkLast(_last);
        }
    }
    
    public: virtual bool remove(const T& o)
    {
        if ( o == NULL )
        {
            for ( typename GListNode<T>::ptr x = _first ; x != NULL ; x = x->getNext() )
            {
                if ( x->getItem() == NULL )
                {
                    unlink(x);
                    return true;
                }
            }
        }
        else
        {
            for ( typename GListNode<T>::ptr x = _first ; x != NULL ; x = x->getNext() )
            {
                if ( o->equals(x->getItem()) )
                {
                    unlink(x);
                    return true;
                }
            }
        }
        return false;
    }
    
    public: virtual void removeAll()
    {
        for ( typename GListNode<T>::ptr x = _first ; x != NULL ; )
        {
            typename GListNode<T>::ptr next = x->getNext();
            x->setItem(NULL);
            x->setNext(NULL);
            x->setPrev(NULL);
            x = next;
        }
        _first = NULL;
        _last = NULL;
        _size = 0;        
    }
    
    public: virtual int32 size()
    {
        return _size;
    }
    
    /**
     * Private tools
     */
    
    private: inline typename GListNode<T>::ptr linkFirst(const T& o)
    {
        typename GListNode<T>::ptr f = _first;
        typename GListNode<T>::ptr newNode = new ListNode<T>(NULL, o, f);
        _first = newNode;
        if ( f == NULL )
            _last = newNode;
        else
            f->setPrev(newNode);
        ++_size;
        return newNode;
    }
    
    private: inline typename GListNode<T>::ptr linkLast(const T& o)
    {
        typename GListNode<T>::ptr l = _last;
        typename GListNode<T>::ptr newNode = new ListNode<T>(l, o, NULL);
        _last = newNode;
        if ( l == NULL )
            _first = newNode;
        else
            l->setNext(newNode);
        ++_size;
        return newNode;
    }
    
    private: inline T unlinkFirst(const typename GListNode<T>::ptr& f)
    {
        T element = f->getItem();
        typename GListNode<T>::ptr next = f->getNext();
        f->setItem(NULL);
        f->setNext(NULL);
        _first = next;
        if ( next == NULL )
            _last = NULL;
        else
            next->setPrev(NULL);
        --_size;
        return element;
    }
    
    private: inline T unlinkLast(const typename GListNode<T>::ptr& l)
    {
        T element = l->getItem();
        typename GListNode<T>::ptr prev = l->getPrev();
        l->setItem(NULL);
        l->setPrev(NULL);
        _last = prev;
        if ( prev == NULL )
            _first = NULL;
        else
            prev->setNext(NULL);
        --_size;
        return element;
    }
    
    private: inline T unlink(const typename GListNode<T>::ptr& o)
    {
        T element = o->getItem();
        typename GListNode<T>::ptr next = o->getNext();
        typename GListNode<T>::ptr prev = o->getPrev();
        
        if ( prev == NULL )
        {
            _first = next;
        }
        else
        {
            prev->setNext(next);
            o->setPrev(NULL);
        }
        
        if ( next == NULL )
        {
            _last = prev;
        }
        else
        {
            next->setPrev(prev);
            o->setNext(NULL);
        }

        o->setItem(NULL);
        --_size;
        return element;
    }
        
    /**
     * LinkedListNode section
     */
    
    private: /*J*static**/ template< class Y > class ListNode : public Common< IListNode<Y> > /*S*where Y : class**/
    {
        private: Y _item;
        private: typename GListNode<Y>::ptr _next;
        private: typename GListNode<Y>::ptr _prev;
        
        public: ListNode(const typename GListNode<Y>::ptr& prev, const Y& item, const typename GListNode<Y>::ptr& next)
        {
            _item = item;
            _next = next;
            _prev = prev;
        }
        
        public: Y getItem()
        {
            return _item;
        }
        
        public: typename GListNode<Y>::ptr getNext()
        {
            return _next;
        }
        
        public: typename GListNode<Y>::ptr getPrev()
        {
            return _prev;
        }
        
        public: void setItem(const Y& item)
        {
            _item = item;
        }
        
        public: void setNext(const typename GListNode<Y>::ptr& next)
        {
            _next = next;
        }
        
        public: void setPrev(const typename GListNode<Y>::ptr& prev)
        {
            _prev = prev;
        }
    };
};

}

#endif // !NODELIST_H__GLYMPSE__
