BUILD_FOLDER := build/


all: server client tmp

server: src/server.c
	@mkdir -p $(BUILD_FOLDER)
	gcc $< -o $(BUILD_FOLDER)$@

client: src/client.c
	@mkdir -p $(BUILD_FOLDER)
	gcc $< -o $(BUILD_FOLDER)$@

tmp: src/tmp.c
	@mkdir -p $(BUILD_FOLDER)
	gcc $< -o $(BUILD_FOLDER)$@

run-client: client
	$(BUILD_FOLDER)client

run-server: server
	$(BUILD_FOLDER)server

run-tmp: tmp 
	$(BUILD_FOLDER)tmp
