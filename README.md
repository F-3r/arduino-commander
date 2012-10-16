Commander
=========

A prototype for a ruby library for controlling and communicating with Arduino...

exmaples:

### blink ###
```ruby
require 'commmander.rb'

Commander.new(port: '/dev/ttyUSB0').run do
  digital_write(13, 1)
  sleep(1)
  digital_write(13, 0)
  sleep(1)
end
```

### led dimmer ###

```ruby
require 'commmander.rb'

Commander.new(port: '/dev/ttyUSB0').run do
  value = analog_read(5)
  puts value              #dump to console
  analog_write(11, value)
  sleep 0.01
end
```