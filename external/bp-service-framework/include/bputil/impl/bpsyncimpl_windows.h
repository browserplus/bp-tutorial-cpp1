/**
 * ***** BEGIN LICENSE BLOCK *****
 * The contents of this file are subject to the Mozilla Public License
 * Version 1.1 (the "License"); you may not use this file except in
 * compliance with the License. You may obtain a copy of the License at
 * http://www.mozilla.org/MPL/
 * 
 * Software distributed under the License is distributed on an "AS IS"
 * basis, WITHOUT WARRANTY OF ANY KIND, either express or implied. See the
 * License for the specific language governing rights and limitations
 * under the License.
 * 
 * The Original Code is BrowserPlus (tm).
 * 
 * The Initial Developer of the Original Code is Yahoo!.
 * Portions created by Yahoo! are Copyright (C) 2006-2009 Yahoo!.
 * All Rights Reserved.
 * 
 * Contributor(s): 
 * ***** END LICENSE BLOCK *****
 */
/*
 *  bpsyncimpl_windows.h
 *
 *  Inline implementation file for bpsync.h (windows version)
 *
 *  Note: This file is included by bpsync.h.
 *        It is not intended for direct inclusion by client code.
 *       
 *  Created by Gordon Durand on 7/20/07.
 *  Copyright 2007 Yahoo! Inc. All rights reserved.
 *
 */
#ifndef BPSYNCIMPLWINDOWS_H_
#define BPSYNCIMPLWINDOWS_H_

#include <windows.h>
#include <stdlib.h>


inline bplus::sync::Mutex::Mutex()
{
    CRITICAL_SECTION * cs =
        (CRITICAL_SECTION *) calloc(1, sizeof(CRITICAL_SECTION));
    InitializeCriticalSection(cs);
    m_osDep = (void *) cs;
}

inline bplus::sync::Mutex::~Mutex()
{
    DeleteCriticalSection((CRITICAL_SECTION *) m_osDep);
}

inline void
bplus::sync::Mutex::lock()
{
    EnterCriticalSection((CRITICAL_SECTION *) m_osDep);
}

inline void
bplus::sync::Mutex::unlock()
{
    LeaveCriticalSection((CRITICAL_SECTION *) m_osDep);
}

struct Win32Condition {
    /* This Win32 condition solution is detailed on:
     *   http://www.cs.wustl.edu/~schmidt/win32-cv-1.html */
    HANDLE e[2]; /* 0 is signal, 1 is broadcast */
    unsigned int i;
    CRITICAL_SECTION lock;
};

inline bplus::sync::Condition::Condition()
{
    Win32Condition * c =
        (Win32Condition *) malloc(sizeof(struct Win32Condition));
    InitializeCriticalSection(&(c->lock));
    c->i = 0;
    c->e[0] = CreateEvent(NULL, false, false, NULL);
    c->e[1] = CreateEvent(NULL, true, false, NULL);
    m_osDep = (void *) c;
}

inline bplus::sync::Condition::~Condition()
{
    Win32Condition * c = (Win32Condition *) m_osDep;
    CloseHandle(c->e[0]);
    CloseHandle(c->e[1]);
    DeleteCriticalSection(&c->lock);
    free(c);
}

inline void
bplus::sync::Condition::broadcast()
{
    Win32Condition * c = (Win32Condition *) m_osDep;

    int haveWait;
    EnterCriticalSection(&(c->lock));
    haveWait = (c->i > 0);
    LeaveCriticalSection(&(c->lock));
    if (haveWait) SetEvent(c->e[1]);
}

inline void
bplus::sync::Condition::signal()
{
    Win32Condition * c = (Win32Condition *) m_osDep;

    int haveWait;
    EnterCriticalSection(&(c->lock));
    haveWait = (c->i > 0);
    LeaveCriticalSection(&(c->lock));
    if (haveWait) SetEvent(c->e[0]);
}

inline void
bplus::sync::Condition::wait(bplus::sync::Mutex * m)
{
    timeWait(m, 0);
}

inline bool
bplus::sync::Condition::timeWait(bplus::sync::Mutex * m, unsigned int msecs)
{
    Win32Condition * c = (Win32Condition *) m_osDep;

    int result, lastWaiter;

    /* increment the number of waiters */
    EnterCriticalSection(&(c->lock));
    c->i++;
    LeaveCriticalSection(&(c->lock));
    m->unlock();
    result = WaitForMultipleObjects(2, c->e, false, msecs ? msecs : INFINITE);

    EnterCriticalSection(&(c->lock));
    (c->i)--;
    lastWaiter = ((result == (WAIT_OBJECT_0 + 1)) && ((c->i) == 0)); 
    LeaveCriticalSection(&(c->lock));

    if (lastWaiter) ResetEvent(c->e[1]);

    m->lock();

    return false;
}


#endif // BPSYNCIMPLWINDOWS_H_
