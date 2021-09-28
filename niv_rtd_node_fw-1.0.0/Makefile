
all: compile upload

compile:
	arduino-cli compile --fqbn esp32:esp32:esp32 

upload:
	arduino-cli upload -p /dev/ttyUSB0 --fqbn esp32:esp32:esp32

serial:
	minicom -D /dev/ttyUSB0
