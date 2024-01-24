install:
	@sudo apt install libasound2-dev libx11-dev libxrandr-dev libxi-dev libgl1-mesa-dev libglu1-mesa-dev libxcursor-dev libxinerama-dev
run:
	@cc src/main.c -lraylib -lGL -lm -lpthread -ldl -lrt -lX11
	@./a.out