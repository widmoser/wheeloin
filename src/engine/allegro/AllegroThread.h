//
//  AllegroThread.h
//  steeringwheel
//
//  Created by Hannes Widmoser on 10/22/13.
//  Copyright (c) 2013 Hannes Widmoser. All rights reserved.
//

#ifndef __steeringwheel__AllegroThread__
#define __steeringwheel__AllegroThread__

#include <iostream>
#include <engine/Thread.h>
#include <allegro5/allegro.h>

class AllegroThread : public Thread {
public:
    AllegroThread(Runnable& runnable);
    ~AllegroThread();
    void start();
    void cancel();
    void join();
    bool isCancelled() const;
    
    static void* proc(ALLEGRO_THREAD* thread, void* data);
private:
    ALLEGRO_THREAD *thread;
};


#endif /* defined(__steeringwheel__AllegroThread__) */
