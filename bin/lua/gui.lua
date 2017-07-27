function draw()
	gui.drawLine(20, 20, 100, 400, "green")
	gui.text(50, 50, "This is a test", "magenta", "blue", 12, "roman", "slant", "bold")
	gui.box(200, 200, 250, 250, 255, 125, 125, 255, "yellow")
	gui.pixel(225, 225, "green")
end

emu.registerbefore(draw)