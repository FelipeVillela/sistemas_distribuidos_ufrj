# the compiler
CC = g++

# the build target executable:
TARGET  = producer_consumer

all: $(TARGET) $(TARGET2)

$(TARGET): $(TARGET).cpp
	$(CC) -pthread -o $(TARGET).o $(TARGET).cpp

