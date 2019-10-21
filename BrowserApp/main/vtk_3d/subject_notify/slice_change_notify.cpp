/*
 *  
 *  $Id: slice_change_notify.cpp $
 *  DeepWise 
 *
 *  Copyright 2017-2030 DeepWise All rights reserved.
 *  http://www.deepwise.com
 *
 *  This file is licensed under LGPL v3 license.
 *  See License.txt for details
 *
 *  Code based in VTK
 */

#include "stdafx.h"
#include "global_include.h"

#include "slice_change_notify.h"
#include "slice_change_observer.h"

 // 通知者
void SliceChangeBaseSubject::Attatch(SliceChangeObserver* p)
{

}
void SliceChangeBaseSubject::Detach(SliceChangeObserver* p)
{

}
void SliceChangeBaseSubject::Notify()
{

}
void SliceChangeBaseSubject::changeState(std::string str)
{
    m_str = str;
    Notify();
}

///////////////////////////////////
// 传统观察者模式

ConcreteSubject::ConcreteSubject()
{
    ;
}
ConcreteSubject::~ConcreteSubject()
{
    m_vec.clear();
}
void ConcreteSubject::Attatch(SliceChangeObserver* p)
{
    m_vec.push_back(p);
}
void ConcreteSubject::Detach(SliceChangeObserver* p)
{
    auto it = find(m_vec.begin(), m_vec.end(), p);
    if (m_vec.end() != it)
    {
        m_vec.erase(it);
    }
}
void ConcreteSubject::Notify()
{
    for (auto it = m_vec.cbegin(); it != m_vec.cend(); it++)
    {
        std::cout << m_str << " ";
        (*it)->UpdateSliceChange();
    }
}

