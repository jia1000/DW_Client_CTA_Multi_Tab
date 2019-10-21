/*
 *  
 *  $Id: slice_change_notify.h $
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

#pragma once
class SliceChangeObserver;

 // 通知者
class SliceChangeBaseSubject
{
public:
    virtual void Attatch(SliceChangeObserver* p) = 0;
    virtual void Detach(SliceChangeObserver* p) = 0;
    virtual void Notify() = 0;
    virtual void changeState(std::string str);

protected:
    std::string m_str;
};
// 传统观察者模式
class ConcreteSubject : public SliceChangeBaseSubject
{
public:
    ConcreteSubject();
    ~ConcreteSubject();
    virtual void Attatch(SliceChangeObserver* p);
    virtual void Detach(SliceChangeObserver* p);
    virtual void Notify();

private:
    std::vector<SliceChangeObserver* > m_vec;
};
