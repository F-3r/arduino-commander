Commander
=========
Almost a prototype for a ruby library to controll and communicate with Arduino from the computer...  
(it's a personal experiment, inspired on Arduino ruby gem, built using CommandMessenger arduino library)  


### how to use it ###

* Take the commander.ino sketch (arduino v1.0.1) and burn it on the arduino board  
 
* write a ruby class that extends Commander:
  * implement **#loop** method (this is the actual program executed by Arduino)
  * implement **#setup** method (optional. Is called once, before entering the loop)
  * implement **#finish** method (optional. Is called once, after leaving the loop and before closing the serial connection)  
  
  
* instantiate and send it **#run**  
  (arduino must be connected to the computer, and you must instantiate your class using the correct port)


### API ###

  Commander class wraps arduino basic input/output functions, 
  they are used just like if you where programing on the board.
 

The currently implemented method are:


```ruby
      digital_read(pin)
      digital_write(pin, value)
      analog_read(pin)
      analog_write(pin, value)
```

### Values ###

  At the present time, there are no constants values for representing HIGH, LOW.
  
  Use simple integers:

  * HIGH = 1
  * LOW = 0
  * 0..255 for analog values (mapping to 0..5 volts)

  Neither classes representing Buttons, Switches, Leds, etc. (If you like those abstractions, you can easily build them!)
  As far as Commander knows, these are just pins (that are just Integers!)
  
### example ###

  In [example.rb](https://github.com/F-3r/commander/blob/master/example.rb), you'll find a very basic RGB led dimmer, that sends sequence of colors to the board.


### Extending ###
  Want to add support for a new command? fine! it's quite easy.
  Lets add a command to hook the ```tone()``` function.
  
  #### 1. Add callback in ```commander.ino```
  you must add a hook to the message callback function 
  in messengerCallbacks array before the NULL (this is important, otherwise CmdMessenger won't work!):
  
  ```arduino
messengerCallbackFunction messengerCallbacks[] = 
{
  pin_mode,
  d_write,
  a_write,
  d_read,
  a_read,
  tone_handler,
  NULL
};
  ```
  
  then implement the callback (it's just an arduino function that reads the message from serial and does something):
  
  ```arduino
void tone_handler()
{
  int pin   = cmdMessenger.readInt();
  int value = cmdMessenger.readInt();
  
  tone(pin,value)
}
  ```
  
  #### 2. Add the operation in ```commander.rb```:
  
  add the command in ```@@opcodes```:
  
  ```ruby
  @@op_codes = { ... , tone: 10 }
  ```
  
  and, finally, create the message that sends the message to the board
  
  ```ruby
  def tone (pin, value)
    send_command :tone, pin, value
  end
  ```
  
  #### 3. Use it!
  
  ```ruby
  class MyCommander < Commander
    def loop
     tone(13, 100)
    end
  end
  ```
  
### TODO  
 * **TEST F-3R!!!! TEST!!** this is such a shame untested library!
 *  make it a gem lazy!
