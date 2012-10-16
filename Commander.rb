require 'serialport'

class Commander

  @@op_codes = {pin_mode: 5, digital_write: 6, analog_write: 7, digital_read: 8, analog_read: 9}

  def initialize(options)
    raise ArgumentError, "must specify :port option" if options[:port].nil?
    options = {baud_rate: 57600, data_bits: 8, stop_bits: 1, parity: SerialPort::NONE}.merge options
    @serial = SerialPort.new(options[:port], options[:baud_rate], options[:data_bits], options[:stop_bits], options[:parity])
  end
  
  def run
    puts "hit Ctrl+C when you're bored..."
    begin
      while true
        instance_eval &Proc.new
      end
    rescue Interrupt => e
      @serial.close
      puts "bye!"
    end
  end

  def analog_write (pin, value)
    send_command :analog_write, pin, value
  end

  def digital_write (pin, value)
    send_command :digital_write, pin, value
  end

  def digital_read (pin)
    send_command :digital_read, pin
    receive
  end

  def analog_read (pin)
    send_command :analog_read, pin
    receive
  end

  private
  def send_command(command, *args)
    @serial.write build_command command, args
  end

  def build_command(command, *args)
    "#{@@op_codes[command]},#{args.join ','};"
  end

  def receive
    @serial.gets
  end
end
