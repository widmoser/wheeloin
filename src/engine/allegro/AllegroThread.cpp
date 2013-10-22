//
//  AllegroThread.cpp
//  steeringwheel
//
//  Created by Hannes Widmoser on 10/22/13.
//  Copyright (c) 2013 Hannes Widmoser. All rights reserved.
//

#include "AllegroThread.h"


void* AllegroThread::proc(ALLEGRO_THREAD* thread, void* data) {
    AllegroThread* t = static_cast<AllegroThread*>(data);
    t->runnable.run(*t);
    return 0;
}

AllegroThread::AllegroThread(Runnable& runnable) : Thread(runnable) {
    thread = al_create_thread(proc, this);
}

AllegroThread::~AllegroThread() {
    al_destroy_thread(thread);
}

void AllegroThread::start() {
    al_start_thread(thread);
}

void AllegroThread::cancel() {
    al_set_thread_should_stop(thread);
}

void AllegroThread::join() {
    void** ret;
    al_join_thread(thread, ret);
}

bool AllegroThread::isCancelled() const {
    return al_get_thread_should_stop(thread);
}

