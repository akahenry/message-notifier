CLIENT_PATH := client
CLIENT_SRC_PATH := $(CLIENT_PATH)/src
CLIENT_EXECUTABLE := client
CLIENT_EXECUTABLE_DEBUG := client.debug

SERVER_PATH := server
SERVER_SRC_PATH := $(SERVER_PATH)/src
SERVER_EXECUTABLE := server
SERVER_EXECUTABLE_DEBUG := server.debug

LOADBALANCER_PATH := loadbalancer
LOADBALANCER_SRC_PATH := $(LOADBALANCER_PATH)/src
LOADBALANCER_EXECUTABLE := loadbalancer
LOADBALANCER_EXECUTABLE_DEBUG := loadbalancer.debug

COMMON_PATH := common

BIN := bin


all: $(BIN)/$(CLIENT_EXECUTABLE) $(BIN)/$(SERVER_EXECUTABLE) $(BIN)/$(LOADBALANCER_EXECUTABLE)

debug-client: $(BIN)/$(CLIENT_EXECUTABLE_DEBUG)

debug-server: $(BIN)/$(SERVER_EXECUTABLE_DEBUG)

debug-loadbalancer: $(BIN)/$(LOADBALANCER_EXECUTABLE)

$(BIN)/$(CLIENT_EXECUTABLE): $(CLIENT_SRC_PATH)/*.cpp $(COMMON_PATH)/*
	$(MAKE) -C $(CLIENT_PATH) all

$(BIN)/$(CLIENT_EXECUTABLE_DEBUG): $(CLIENT_SRC_PATH)/*.cpp $(COMMON_PATH)/*
	$(MAKE) -C $(CLIENT_PATH) debug

$(BIN)/$(SERVER_EXECUTABLE): $(SERVER_SRC_PATH)/*.cpp $(COMMON_PATH)/*
	$(MAKE) -C $(SERVER_PATH) all

$(BIN)/$(SERVER_EXECUTABLE_DEBUG): $(SERVER_SRC_PATH)/*.cpp $(COMMON_PATH)/*
	$(MAKE) -C $(SERVER_PATH) debug

$(BIN)/$(LOADBALANCER_EXECUTABLE): $(LOADBALANCER_SRC_PATH)/*.cpp $(COMMON_PATH)/*
	$(MAKE) -C $(LOADBALANCER_PATH) all

$(BIN)/$(LOADBALANCER_EXECUTABLE_DEBUG): $(LOADBALANCER_SRC_PATH)/*.cpp $(COMMON_PATH)/*
	$(MAKE) -C $(LOADBALANCER_PATH) debug

run-client: $(BIN)/$(CLIENT_EXECUTABLE)
	$(BIN)/$(CLIENT_EXECUTABLE)

run-server: $(BIN)/$(SERVER_EXECUTABLE)
	$(BIN)/$(SERVER_EXECUTABLE)

run-loadbalancer: $(BIN)/$(LOADBALANCER_EXECUTABLE)
	$(BIN)/$(LOADBALANCER_EXECUTABLE)

clean: 
	-rm $(BIN)/$(CLIENT_EXECUTABLE) $(BIN)/$(CLIENT_EXECUTABLE_DEBUG) $(BIN)/$(SERVER_EXECUTABLE) $(BIN)/$(SERVER_EXECUTABLE_DEBUG) $(BIN)/$(LOADBALANCER_EXECUTABLE) $(BIN)/$(LOADBALANCER_EXECUTABLE_DEBUG)
	-rm -rf $(SERVER_PATH)/data
