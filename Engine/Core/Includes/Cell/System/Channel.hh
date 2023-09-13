// SPDX-FileCopyrightText: Copyright 2023 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#pragma once

#include <Cell/System/Event.hh>
#include <Cell/System/Mutex.hh>

namespace Cell::System {

// Represents a channel; this serves as a one way pipe to communicate across threads.
template <typename T> class Channel : public Object {
public:
    // Creates a new event.
    CELL_INLINE Channel(T data) : data(data) { }

    // Sends a new message.
    // If the message hasn't yet been read, this will block.
    CELL_INLINE void Send(const T data) {
        while (this->IsDataAvailable()) ;

        this->ownership.Lock();
        this->data = data;
        this->newMessage.Signal();
        this->ownership.Unlock();
    }

    // Attempts to a new message.
    CELL_INLINE void TrySend(const T data) {
        if (this->IsDataAvailable()) {
            return;
        }

        this->ownership.Lock();
        this->data = data;
        this->newMessage.Signal();
        this->ownership.Unlock();
    }

    // Returns the currently contained data, if any unread data is available.
    // Data is cleared on read.
    CELL_INLINE T Read() {
        CELL_ASSERT(this->IsDataAvailable());

        this->newMessage.Reset();
        return this->data;
    }

    // Returns whether there's unread data available.
    CELL_INLINE bool IsDataAvailable() {
        return this->newMessage.Wait(1) == EventWaitState::Signaled;
    }

private:
    Event newMessage;
    Mutex ownership;
    T data;
};

}
