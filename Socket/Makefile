all: Server ChatClient

Server: Main.cc
	g++ -o $@ $^ -std=c++17

ChatClient: ChatClient.cc
	g++ -o $@ $^ -std=c++17

.PHONY: clean
clean:
	rm -f Server ChatClient
