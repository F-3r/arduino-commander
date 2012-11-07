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

  At the present time, there are no constants values for representing HIGH, LOW, and such.
  Use simple integers:

  * HIGH = 1
  * LOW = 0
  * 0..255 for analog values (mapping to 0..5 volts)

### example ###

  In [example.rb](example.rb), you'll find a very basic RGB led dimmer, that sends sequence of colors to the board.