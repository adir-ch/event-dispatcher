
#include <unistd.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>
#include <fcntl.h>

#include "event_dispatcher.h"
#include "ui.h"
#include "log.h" 

/* 
   Description:
   The program is an event dispatcher which resolves host names.
   The input is a hostname, and the output is the host name IP address.
 
   The dispatcher will consist of:
   1. Thread pool (for resolvers)
   2. Request queue - will hold resolving requests.
   3. Worker threads - will do the actual resolve.
   4. Events queue - will hold resolving requests + answers.
   5. UI - will read resolving requests + prints resolving answers.
 
   Flow:
   The UI will read resolving request from the input and will push a new resolving
   events to the events queue (a Request event).
   The main dispatching loop will send a request to the threads manager which will allocate
   a worker thread to handle the resolve request (if all threads are busy, the request will wait
   in an internal requests queue, inside the thread manager).
   Upon thread allocation, the hostname IP address will be resolved (if exist), and the worker thread
   will push a resolve event with the relevant details to the event queue.
   The main loop will send the resolve answer to the UI. The UI will display the result to the
   user.
 
   Input:
   A string or a set of strings seperated by space.
 
   Output:
   A string (the hostname) with the corresponding IP address or error message. 
 
   TODO:
        1. Replace the input handler to boost ASIO instead of the traditional sockets.
 
*/

// Globals 
EventDispatcher ed; 
UI ui;

void init() 
{ 
    ui.init(ed.getDispatcherCallback()); 
    ed.init(&ui); 
}

int main(int argc, char **argv)
{
    LOG_DEBUG("Starting run...");
    // init the main data structures 
    init(); 
    ed.run(); 
    return 0;
} // main end

