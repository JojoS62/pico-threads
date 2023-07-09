#include <Arduino.h>
#include <mbed.h>
#include <pico/multicore.h> 

using namespace mbed;
using namespace rtos;
using namespace events;
using namespace std::chrono_literals;
using namespace std;

Thread thread;
Thread thread_events;
EventQueue queue;
InterruptIn user_button(p15);

void thread_fn() {
  DigitalOut led(LED1);

  while(1) {
    led = !led;
    ThisThread::sleep_for(100ms);
  }
}

void setup() {
  Serial1.begin(115200);
  Serial1.println(F("Hello from Pico\n"));

  thread.start(thread_fn);
  thread_events.start(callback(&queue, &EventQueue::dispatch_forever));

  user_button.rise( []() {
    queue.call(printf, "ping\n");
    queue.call_in(2s, printf, "pong\n");
  });

}

int counter;

void loop() {
    printf("Hello world from Mbed CE! %d\n", counter);
    ThisThread::sleep_for(1s);
    counter++;
}

