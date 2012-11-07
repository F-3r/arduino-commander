require './commander.rb'

class RGBCommander < Commander

  def setup
    @red = 9
    @green = 10
    @blue = 11

    @colors     = {red: 0, green: 0, blue: 0} 
    @velocities = {red: 1, green: 2, blue: 3} 
  end

  def finish
    #turn off each led
    analog_write(@red, 255)
    analog_write(@green, 255)
    analog_write(@blue, 255)    
    sleep 1
  end

  def loop
    advance
    checkAndReset
    sendColor
    
    p @colors

    sleep 0.03
  end


  def checkAndReset
    @colors.each do |color, value|
      if value > 255 || value < 0
        @velocities[color] *= -1
        if value > 255
          @colors[color] = 255
        else
          @colors[color] = 0
        end
      end
    end
  end

  def advance
    @colors.each do |color, value|
      @colors[color] += @velocities[color]
    end
  end

  def sendColor
    analog_write(@red, @colors[:red])
    analog_write(@green, @colors[:green])
    analog_write(@blue, @colors[:blue])
  end

  def setColors (r,g,b)
    @colors = {red: r, green: g, blue: b}
    sendColor
  end
end

#run it!
RGBCommander.new(port: '/dev/ttyUSB0').run

